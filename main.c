#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

Wlist *hash_table[HASHTABLE_SIZE] = {NULL};

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Error: Missing file arguments.\n");
        printf("Usage: %s <file1.txt> <file2.txt> ...\n", argv[0]);
        return FAILURE;
    }

    int capacity = argc - 1;
    char **valid_files = malloc(capacity * sizeof(char *));
    int valid_count = 0;

    validate_args(argc, argv, valid_files, &valid_count);

    if (valid_count == 0)
    {
        printf("Error: No valid text files to parse. Exiting.\n");
        free(valid_files);
        return FAILURE;
    }

    int choice;
    char search_word[WORD_SIZE];
    char backup_file[FILE_NAME_SIZE];
    int db_created = 0;
    char cont;

    while (1)
    {
        printf("\nSelect your choice among following options:\n");
        printf("1. Create DATABASE\n");
        printf("2. Display Database\n");
        printf("3. Update DATABASE\n");
        printf("4. Search\n");
        printf("5. Save Database\n");
        printf("6. Exit\n");
        printf("Enter your choice\n");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Exiting.\n");
            break;
        }

        switch (choice)
        {
            case 1:
                if (db_created)
                {
                    printf("Database has already been built.\n");
                }
                else
                {
                    if (create_database(valid_files, valid_count) == SUCCESS)
                    {
                        printf("Successful : Creation of DATABASE for file: %s\n",
                               valid_files[valid_count - 1]);
                        db_created = 1;
                    }
                    else
                    {
                        printf("Database build phase failed.\n");
                    }
                }
                break;

            case 2:
                if (!db_created)
                {
                    printf("Build the database index map first (Option 1).\n");
                }
                else
                {
                    display_database();
                }
                break;

            case 3:
            {
                char new_file[FILE_NAME_SIZE];
                printf("Enter the file name to update data: ");
                scanf("%99s", new_file);

                int is_duplicate = 0;
                for (int j = 0; j < valid_count; j++)
                {
                    if (strcmp(new_file, valid_files[j]) == 0)
                    {
                        is_duplicate = 1;
                        break;
                    }
                }

                if (is_duplicate)
                {
                    printf("Warning: '%s' is already part of the database.\n", new_file);
                    break;
                }

                FILE *fptr = fopen(new_file, "r");
                if (fptr == NULL)
                {
                    printf("Warning: File '%s' does not exist or cannot be opened.\n", new_file);
                    break;
                }
                fseek(fptr, 0, SEEK_END);
                long file_size = ftell(fptr);
                fclose(fptr);

                if (file_size == 0)
                {
                    printf("Warning: File '%s' is empty. Skipping.\n", new_file);
                    break;
                }

                char *stored_name = malloc(strlen(new_file) + 1);
                strcpy(stored_name, new_file);
                char *new_arr[1] = { stored_name };

                if (create_database(new_arr, 1) == SUCCESS)
                {
                    if (valid_count == capacity)
                    {
                        capacity++;
                        valid_files = realloc(valid_files, capacity * sizeof(char *));
                    }
                    valid_files[valid_count++] = stored_name;
                    db_created = 1;
                    printf("Successful : Creation of DATABASE for file: %s\n", stored_name);
                }
                else
                {
                    printf("Failed to update database.\n");
                    free(stored_name);
                }
                break;
            }

            case 4:
                if (!db_created)
                {
                    printf("Database mapping state is empty.\n");
                }
                else
                {
                    printf("Enter the word you want to search: ");
                    scanf("%49s", search_word);
                    if (search_database(search_word) == NOT_FOUND)
                    {
                        printf("Keyword '%s' could not be found.\n", search_word);
                    }
                }
                break;

            case 5:
                if (!db_created)
                {
                    printf("No structured runtime states available to save.\n");
                }
                else
                {
                    printf("Enter the file name to save database: ");
                    scanf("%99s", backup_file);
                    if (save_database(backup_file) == SUCCESS)
                    {
                        printf("Database is saved\n");
                    }
                    else
                    {
                        printf("Could not save to file.\n");
                    }
                }
                break;

            case 6:
                printf("Closing engine operations. Freeing resources.\n");
                free(valid_files);
                return SUCCESS;

            default:
                printf("Invalid selection. Choose 1 to 6.\n");
        }

        printf("Do you want to continue ?\n");
        printf("Enter y/Y to continue and n/N to discontinue\n");
        scanf(" %c", &cont);
        if (cont != 'y' && cont != 'Y')
        {
            printf("Closing engine operations. Freeing resources.\n");
            free(valid_files);
            return SUCCESS;
        }
    }

    free(valid_files);
    return SUCCESS;
}