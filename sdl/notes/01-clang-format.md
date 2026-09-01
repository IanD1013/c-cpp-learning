# clang-format 使用笔记

## 格式化整个仓库

在仓库根目录执行。

```sh
git ls-files -z '*.c' '*.h' '*.cpp' '*.hpp' | xargs -0 clang-format -i
```

## 只检查不修改

把 `-i` 换成 `--dry-run`，会列出所有不符合格式的位置。

```sh
git ls-files -z '*.c' '*.h' '*.cpp' '*.hpp' | xargs -0 clang-format --dry-run
```

## 格式化单个文件

```sh
clang-format -i sdl/main.cpp
```

## 命令拆解

```
git ls-files -z '*.c' '*.h' '*.cpp' '*.hpp' | xargs -0 clang-format -i
└────────────┬───────────────────────────┘   └───┬──┘ └──────┬──────┘
             │                                   │           │
   列出仓库里已跟踪的 C/C++ 源文件            逐个传给命令   就地改写文件
```

`-z` 和 `-0` 必须成对使用。
它们让文件名以 `\0` 分隔而不是换行，这样 `c/crash-course/day4 - build linked list/` 这种带空格的目录名才不会被拆成多个参数。

用 `git ls-files` 而不是 `find` 的好处是只处理已跟踪的文件，自动跳过 `build/`、`prog` 这些被 `.gitignore` 忽略的产物。

`clang-format` 会从每个文件所在目录逐级向上查找 `.clang-format`，所以不需要显式传 `--style`。

## 当前配置的两个关键项

配置文件在仓库根目录的 `.clang-format`。

```yaml
IndentAccessModifiers: true
PointerAlignment: Left
```

`IndentAccessModifiers: true` 让 `public:` / `private:` 相对 class 缩进一级，成员再缩进一级。

```cpp
struct Application
{
    public:
        Application();

    private:
        SDL_Window* mWindow;
};
```

注意开头必须写显式的 `public:`。
否则第一个访问块的成员会直接缩进到第 8 列，而第 4 列上没有修饰符，看起来像是凭空多了一级。

`PointerAlignment: Left` 让星号跟着类型走。

```cpp
SDL_Window* mWindow;    // Left
SDL_Window *mWindow;    // Right
```

引用会跟随这项设置，因为 `ReferenceAlignment` 默认值是 `Pointer`。

## 编辑器集成

`.vscode/settings.json` 里已开启保存时自动格式化。

```json
{
    "editor.formatOnSave": true
}
```

## 注意

批量格式化会产生很大的 diff。
建议在工作区干净时单独跑一次并单独提交，不要和逻辑改动混在同一个 commit 里。
