#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

void validate_args(int argc, char *argv[], char *valid_files[], int *valid_count)
{
    int count = 0;

    for (int i = 1; i < argc; i++)
    {
        int is_duplicate = 0;
        for (int j = 0; j < count; j++)
        {
            if (strcmp(argv[i], valid_files[j]) == 0)
            {
                is_duplicate = 1;
                break;
            }
        }

        if (is_duplicate)
        {
            printf("Warning: Skipped duplicate file argument: '%s'\n", argv[i]);
            continue;
        }

        FILE *fptr = fopen(argv[i], "r");
        if (fptr == NULL)
        {
            printf("Warning: Target file '%s' does not exist or cannot be opened.\n", argv[i]);
            continue;
        }

        fseek(fptr, 0, SEEK_END);
        long file_size = ftell(fptr);
        fclose(fptr);

        if (file_size == 0)
        {
            printf("Warning: File '%s' is empty. Skipping.\n", argv[i]);
            continue;
        }

        valid_files[count] = argv[i];
        count++;
    }

    *valid_count = count;
    printf("\nValidation Complete: Successfully registered %d file target(s).\n", count);
}