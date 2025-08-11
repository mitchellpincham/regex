#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool isvocab(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9');
}

char *bracketForAdjacency(const char *regex)
{
    size_t len = strlen(regex);

    // Allocate enough room for worst case: every char gets surrounded by ()
    // so length can at most triple
    size_t cap = len * 3 + 1;
    char *out = malloc(cap);
    if (!out)
        return NULL;

    int i = 0; // index in input
    int j = 0; // index in output

    while (i < len)
    {
        out[j] = regex[i];
        j++;
        i++;
        // TODO: loop through string, and put brackets around adjacencies that are next to |.
    }

    out[j] = '\0';
    return out;
}