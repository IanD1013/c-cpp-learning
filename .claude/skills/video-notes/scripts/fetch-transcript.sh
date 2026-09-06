#!/usr/bin/env bash
# Fetch a video's transcript and metadata for note-taking.
#
# Usage: fetch-transcript.sh <video-url> [output-dir] [-u]
#   -u   force re-download of the yt-dlp binary (do this when extraction breaks)
#
# Writes into output-dir (default: ./video-notes-work):
#   transcript.txt  timestamped, de-duplicated plain text
#   meta.txt        title / channel / duration / upload date / chapters / description
#   raw.info.json   full yt-dlp metadata
# Prints meta.txt and the transcript path on success.

set -euo pipefail

FORCE_UPDATE=0
ARGS=()
for a in "$@"; do
  case "$a" in
    -u|--update) FORCE_UPDATE=1 ;;
    *) ARGS+=("$a") ;;
  esac
done

URL="${ARGS[0]:-}"
OUTDIR="${ARGS[1]:-./video-notes-work}"

if [[ -z "$URL" ]]; then
  echo "usage: fetch-transcript.sh <video-url> [output-dir] [-u]" >&2
  exit 2
fi

CACHE="${XDG_CACHE_HOME:-$HOME/.cache}/claude-video-notes"
YTDLP="$CACHE/yt-dlp"
mkdir -p "$CACHE" "$OUTDIR"

# yt-dlp ships as a self-contained binary. YouTube extraction breaks periodically,
# so refresh anything older than 14 days.
if [[ $FORCE_UPDATE -eq 1 ]] || [[ ! -x "$YTDLP" ]] || [[ -n "$(find "$YTDLP" -mtime +14 2>/dev/null)" ]]; then
  echo ">> downloading yt-dlp ..." >&2
  curl -fsSL -o "$YTDLP.tmp" \
    https://github.com/yt-dlp/yt-dlp/releases/latest/download/yt-dlp
  chmod +x "$YTDLP.tmp"
  mv "$YTDLP.tmp" "$YTDLP"
fi
echo ">> yt-dlp $("$YTDLP" --version)" >&2

rm -f "$OUTDIR"/vid.*.vtt "$OUTDIR/raw.info.json" "$OUTDIR/transcript.txt"

# Step 1: metadata only. Kept separate from the subtitle download so a caption
# failure never costs us the metadata.
if ! "$YTDLP" --quiet --no-warnings --skip-download --no-write-subs \
      --write-info-json -o "$OUTDIR/vid" "$URL" 2>&1 | grep -v '^\[download\]'; then :; fi

if [[ ! -f "$OUTDIR/vid.info.json" ]]; then
  echo "!! could not fetch video metadata." >&2
  echo "!! retry with -u to refresh yt-dlp; if it still fails the video may be" >&2
  echo "!! private, region-locked, or need cookies (--cookies-from-browser BROWSER)." >&2
  exit 1
fi
mv "$OUTDIR/vid.info.json" "$OUTDIR/raw.info.json"

# Step 2: pick ONE real caption track. YouTube exposes hundreds of
# machine-translated variants; asking for a wildcard triggers HTTP 429.
LANG_PICK="$(python3 - "$OUTDIR/raw.info.json" <<'PY'
import json, sys
info = json.load(open(sys.argv[1], encoding="utf-8"))
manual = info.get("subtitles") or {}
auto = info.get("automatic_captions") or {}
orig = (info.get("language") or "en").split("-")[0]

def pick(tracks, prefs):
    for p in prefs:
        if p in tracks:
            return p
    for k in tracks:                      # any track in a preferred base language
        if k.split("-")[0] in (orig, "en", "zh"):
            return k
    return None

# A human-written track beats an auto-generated one. "<lang>-orig" is the real
# ASR track; a bare "<lang>" may already be a machine translation.
choice = pick(manual, [orig, "en", f"{orig}-orig", "zh-Hans", "zh-CN", "zh"]) \
      or pick(auto, [f"{orig}-orig", orig, "en-orig", "en"])
print(choice or "")
PY
)"

if [[ -z "$LANG_PICK" ]]; then
  echo "!! this video has no caption track at all." >&2
  echo "!! meta.txt was still written; base the notes on it or ask the user." >&2
else
  echo ">> caption track: $LANG_PICK" >&2
  for attempt in 1 2 3; do
    if "$YTDLP" --quiet --no-warnings --skip-download \
         --write-subs --write-auto-subs --sub-langs "$LANG_PICK" --sub-format "vtt/best" \
         -o "$OUTDIR/vid" "$URL" 2>&1 | grep -v '^\[download\]'; then :; fi
    compgen -G "$OUTDIR/vid.*.vtt" >/dev/null && break
    echo ">> caption download failed (attempt $attempt), backing off ..." >&2
    sleep $((attempt * 5))
  done
fi

VTT=""
for cand in "$OUTDIR"/vid.*.vtt; do
  [[ -f "$cand" ]] && { VTT="$cand"; break; }
done

python3 - "$OUTDIR" "$VTT" <<'PY'
import json, re, sys, collections
outdir, vtt = sys.argv[1], sys.argv[2]

info = json.load(open(f"{outdir}/raw.info.json", encoding="utf-8"))
meta = [
    f"title:    {info.get('title')}",
    f"channel:  {info.get('channel') or info.get('uploader')}",
    f"duration: {info.get('duration_string')}",
    f"uploaded: {info.get('upload_date')}",
    f"url:      {info.get('webpage_url')}",
    "",
]
chapters = info.get("chapters") or []
if chapters:
    meta.append("chapters:")
    for c in chapters:
        s = int(c.get("start_time") or 0)
        meta.append(f"  [{s//60:02d}:{s%60:02d}] {c.get('title')}")
else:
    meta.append("chapters: (none)")
meta += ["", "description:", (info.get("description") or "").strip()]
open(f"{outdir}/meta.txt", "w", encoding="utf-8").write("\n".join(meta) + "\n")

if not vtt:
    sys.exit(0)

# YouTube auto-captions use a rolling window: each cue repeats the tail of the
# previous one. De-duplicate against a short window of recently emitted lines
# rather than globally, so genuinely repeated phrases survive.
recent = collections.deque(maxlen=8)
lines, ts = [], "00:00"
for ln in open(vtt, encoding="utf-8", errors="replace").read().splitlines():
    m = re.match(r"^(\d\d):(\d\d):(\d\d)[.,]\d+\s+--> ", ln)
    if m:
        h, mm, ss = map(int, m.groups())
        ts = f"{h*60+mm:02d}:{ss:02d}"
        continue
    if not ln.strip() or ln.startswith(("WEBVTT", "Kind:", "Language:", "NOTE", "STYLE")):
        continue
    t = re.sub(r"<[^>]+>", "", ln).strip()
    if not t or t in recent:
        continue
    recent.append(t)
    lines.append(f"[{ts}] {t}")

open(f"{outdir}/transcript.txt", "w", encoding="utf-8").write("\n".join(lines) + "\n")
print(f">> transcript: {len(lines)} lines from {vtt.rsplit('/',1)[-1]}", file=sys.stderr)
PY

echo
cat "$OUTDIR/meta.txt"
echo
if [[ -f "$OUTDIR/transcript.txt" ]]; then
  echo ">> now read: $OUTDIR/transcript.txt"
else
  echo ">> NO TRANSCRIPT. only $OUTDIR/meta.txt is available."
fi
