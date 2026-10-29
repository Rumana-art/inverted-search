#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "header.h"

int calculate_hash(const char *word)
{
    char first_letter = tolower((unsigned char)word[0]);

    if (first_letter >= 'a' && first_letter <= 'z')
        return first_letter - 'a';

    return 0;
}

void clean_word(char *dest, const char *src)
{
    int j = 0;

    for (int i = 0; src[i] != '\0' && j < WORD_SIZE - 1; i++)
    {
        if (isalnum((unsigned char)src[i]))
        {
            dest[j++] = tolower((unsigned char)src[i]);
        }
    }

    dest[j] = '\0';
}

int create_database(char *valid_files[], int valid_count)
{
    char raw_word[WORD_SIZE];
    char cleaned[WORD_SIZE];

    for (int i = 0; i < valid_count; i++)
    {
        FILE *fptr = fopen(valid_files[i], "r");

        if (fptr == NULL)
        {
            continue;
        }

        while (fscanf(fptr, "%49s", raw_word) == 1)
        {
            clean_word(cleaned, raw_word);

            if (cleaned[0] == '\0')
            {
                continue;
            }

            int index = calculate_hash(cleaned);

            Wlist *curr_word = hash_table[index];
            Wlist *prev_word = NULL;

            while (curr_word != NULL)
            {
                if (strcmp(curr_word->word, cleaned) == 0)
                {
                    break;
                }

                prev_word = curr_word;
                curr_word = curr_word->next_word;
            }

            if (curr_word == NULL)
            {
                Wlist *new_word = malloc(sizeof(Wlist));

                if (new_word == NULL)
                {
                    fclose(fptr);
                    return FAILURE;
                }

                strcpy(new_word->word, cleaned);
                new_word->file_count = 1;
                new_word->next_word = NULL;

                Flist *new_file = malloc(sizeof(Flist));

                if (new_file == NULL)
                {
                    free(new_word);
                    fclose(fptr);
                    return FAILURE;
                }

                strcpy(new_file->file_name, valid_files[i]);
                new_file->word_count = 1;
                new_file->next_file = NULL;

                new_word->head_file = new_file;

                if (prev_word == NULL)
                {
                    hash_table[index] = new_word;
                }
                else
                {
                    prev_word->next_word = new_word;
                }
            }
            else
            {
                Flist *curr_file = curr_word->head_file;
                Flist *prev_file = NULL;

                while (curr_file != NULL)
                {
                    if (strcmp(curr_file->file_name, valid_files[i]) == 0)
                    {
                        break;
                    }

                    prev_file = curr_file;
                    curr_file = curr_file->next_file;
                }

                if (curr_file != NULL)
                {
                    curr_file->word_count++;
                }
                else
                {
                    Flist *new_file = malloc(sizeof(Flist));

                    if (new_file == NULL)
                    {
                        fclose(fptr);
                        return FAILURE;
                    }

                    strcpy(new_file->file_name, valid_files[i]);
                    new_file->word_count = 1;
                    new_file->next_file = NULL;

                    if (prev_file == NULL)
                    {
                        curr_word->head_file = new_file;
                    }
                    else
                    {
                        prev_file->next_file = new_file;
                    }

                    curr_word->file_count++;
                }
            }
        }

        fclose(fptr);
    }

    return SUCCESS;
}