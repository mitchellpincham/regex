#include <stdio.h>
#include "compile.h"
#include "search.h"

int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        printf("Expected two arguments; regex & filename");
        return -1;
    }

    char *regex = argv[1];
    // regex = bracketForAdjacency(regex);

    // printf("%s\n", regex);

    // REcompile(regex);

    REsearch("output.csv", "test.txt");

    return 0;
}