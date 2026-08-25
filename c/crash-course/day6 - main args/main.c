#include <stdio.h>
#include <stdlib.h> // atoi

// argc: the total number of space separated arguments when our program is
// invoked

// argv: a vector of strings, including the filename of the program
// being run as the first argument. The last element argv[argc] is a null
// pointer.
int main(int argc, char **argv)
{
    printf("argc: %d\n", argc);

    if (argc != 3)
    {
        printf("usage: ./add num1 num2\n");
        return 1;
    }

    int firstArgument  = atoi(argv[1]);
    int secondArgument = atoi(argv[2]);
    int result         = firstArgument + secondArgument;

    printf("%d + %d = %d\n", firstArgument, secondArgument, result);
    return 0;
}