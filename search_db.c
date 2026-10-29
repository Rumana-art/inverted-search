#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "header.h"

int search_database(const char *word)
{
    char cleaned[WORD_SIZE] = {0};
    int j = 0;
    for (int i = 0; word[i] != '\0' && j < WORD_SIZE - 1; i++)
    {
        if (isalnum((unsigned char)word[i]))
        {
            cleaned[j++] = tolower((unsigned char)word[i]);
        }
    }
    cleaned[j] = '\0';

    if (cleaned[0] == '\0')
        return NOT_FOUND;

    int index = calculate_hash(cleaned);
    Wlist *curr = hash_table[index];

    while (curr != NULL)
    {
        if (strcmp(curr->word, cleaned) == 0)
        {
            printf("Word %s is present in %d file/s\n",
                   curr->word, curr->file_count);

            Flist *f = curr->head_file;
            while (f != NULL)
            {
                printf("In file: %s %d time/s\n",
                       f->file_name, f->word_count);
                f = f->next_file;
            }
            return SUCCESS;
        }
        curr = curr->next_word;
    }
    return NOT_FOUND;
}