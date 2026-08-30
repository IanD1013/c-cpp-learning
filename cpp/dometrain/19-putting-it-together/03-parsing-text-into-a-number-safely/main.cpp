#include <cctype>
#include <cstddef>
#include <string>

struct Solution
{
    public:
        static int parseNonNegative(const std::string& text)
        {
            // 1. Empty text check
            if (text.empty())
            {
                return -1;
            }

            // 2. Length check: Must be <= 10 digits
            if (text.length() > 10)
            {
                return -1;
            }

            // 3. Leading zero check: "0" is okay, "0x" is not.
            if (text.length() > 1 && text[0] == '0')
            {
                return -1;
            }

            // 4. Character check: Every char must be a digit.
            for (char c : text)
            {
                if (!isdigit(static_cast<unsigned char>(c)))
                {
                    return -1;
                }
            }

            // 5. Value limit check for 10-digit strings
            if (text.length() == 10)
            {
                // Compare "2147483647" vs text
                const std::string limit = "2147483647";
                if (text > limit)
                {
                    return -1;
                }
            }

            // 6. Conversion
            return std::stoi(text);
        }
};
