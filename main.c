#include <stdio.h>
#include "compile.h"

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        printf("Expected one argument");
        return -1;
    }

    char *regex = argv[1];
    // regex = bracketForAdjacency(regex);

    // printf("%s\n", regex);

    REcompile(regex);

    return 0;
}