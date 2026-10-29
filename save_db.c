#include <stdio.h>
#include "header.h"

int save_database(const char *filename)
{
    FILE *fptr = fopen(filename, "w");
    if (fptr == NULL)
        return FAILURE;

    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        Wlist *curr_word = hash_table[i];
        while (curr_word != NULL)
        {
            fprintf(fptr, "#:%d\n", i);

            fprintf(fptr, "%s:%d",
                    curr_word->word, curr_word->file_count);

            Flist *curr_file = curr_word->head_file;
            while (curr_file != NULL)
            {
                fprintf(fptr, ":%s:%d",
                        curr_file->file_name,
                        curr_file->word_count);
                curr_file = curr_file->next_file;
            }
            fprintf(fptr, ": #\n");
            curr_word = curr_word->next_word;
        }
    }
    fclose(fptr);
    return SUCCESS;
}