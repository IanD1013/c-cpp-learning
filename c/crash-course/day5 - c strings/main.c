#include <stdio.h>
#include <stdlib.h>

size_t StringLength(const char *input)
{
    size_t length = 0;
    while (input[length] != '\0')
    {
        length++;
    }
    return length;
}

char *StringAppend(char *src1, const char *src2)
{
    size_t src1Length = StringLength(src1);
    size_t src2Length = StringLength(src2);

    char *newString = (char *)malloc(sizeof(char) * (src1Length + src2Length + 1));

    int i = 0;
    while (i < src1Length)
    {
        newString[i] = src1[i];
        i++;
    }

    int j = 0;
    while (j < src2Length)
    {
        newString[src1Length + j] = src2[j];
        j++;
    }

    newString[src1Length + src2Length] = '\0';

    return newString;
}

int main()
{
    const char *name = "Hello, World!";
    //   name[0] = 'h'; // This will cause undefined behavior since name is a
    //   string literal

    ////////////////////////////////////
    char  name[]       = { 'H', 'e', 'l', 'l', 'o', '\0' };
    char *appendedName = StringAppend(name, ", World!");
    printf("%s\n", appendedName);
    free(appendedName);

    return 0;
}

// 1. C puts in the terminal character for us
// 2. String literals are read-only, and cannot be modified

// The reason that string literals are not modifiable, is because the C compiler
// will generate assembly code (and eventually a binary from the assembly),
// which stores the "string literal" in a read-only region of the executable
// binary.
