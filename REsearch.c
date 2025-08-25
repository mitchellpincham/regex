#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BR 'B'

// the rows of the table.
char states_[256];
int next1_[256];
int next2_[256];
int regexLength = 10;
int width;
int readTable(char *filename);
void printTable_();

int REsearch(char *regexFilename, char *targetFileName)
{
    if (readTable(regexFilename))
        return 1;

    printTable_();
    return 0;
}

void printTable_()
{
    for (int i = 0; i < width; i++)
    {
        printf(" %d", i);
    }
    printf("\n");

    for (int i = 0; i < width; i++)
    {
        printf(" %c", states_[i]);
    }
    printf("\n");

    for (int i = 0; i < width; i++)
    {
        printf(" %d", next1_[i]);
    }
    printf("\n");

    for (int i = 0; i < width; i++)
    {
        printf(" %d", next2_[i]);
    }
    printf("\n");
}

int readTable(char *fileName)
{
    FILE *file = fopen(fileName, "r");

    char line[1024];

    // get the first line (the row of indices)
    fgets(line, sizeof(line), file);
    line[strcspn(line, "\n")] = '\0';
    width = line[strnlen(line, 1024) - 1] - '0'; // char to int

    // next line (states)
    fgets(line, sizeof(line), file);
    line[strcspn(line, "\n")] = '\0';

    // split by the comma
    char *token = NULL;
    token = strtok(line, ",");
    int i = 0;
    if (token != NULL)
    {
        do
        {
            if (strcmp(token, "BR") == 0)
            {
                states_[i] = BR;
            }
            else
            {
                states_[i] = token[0];
            }
            i++;
            token = strtok(NULL, ",");
        } while (token != NULL);
    }

    // Now for the next states
    fgets(line, sizeof(line), file);
    line[strcspn(line, "\n")] = '\0';
    token = NULL;
    token = strtok(line, ",");
    i = 0;
    if (token != NULL)
    {
        do
        {
            next1_[i] = atoi(token);
            i++;
            token = strtok(NULL, ",");

        } while (token != NULL);
    }

    fgets(line, sizeof(line), file);
    line[strcspn(line, "\n")] = '\0';
    token = NULL;
    token = strtok(line, ",");
    i = 0;
    if (token != NULL)
    {
        do
        {
            next2_[i] = atoi(token);
            i++;
            token = strtok(NULL, ",");

        } while (token != NULL);
    }

    fclose(file);
    return 0;
}