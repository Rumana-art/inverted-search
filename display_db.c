#include <stdio.h>
#include "header.h"

void display_database(void)
{
    printf("%-8s %-12s %-12s %-22s %-10s\n",
           "[index]", "[word]", "file count",
           "file/s File: File name", "word count");

    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        Wlist *curr_word = hash_table[i];

        while (curr_word != NULL)
        {
            printf("[%d]\t[ %s ]\t%d file/s:",
                   i, curr_word->word, curr_word->file_count);

            Flist *curr_file = curr_word->head_file;
            while (curr_file != NULL)
            {
                printf(" File : %s %d",
                       curr_file->file_name,
                       curr_file->word_count);
                curr_file = curr_file->next_file;
            }
            printf("\n");

            curr_word = curr_word->next_word;
        }
    }
}