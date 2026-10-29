#ifndef HEADER_H
#define HEADER_H

/* ---- Size / status constants ---- */
#define WORD_SIZE       50
#define FILE_NAME_SIZE  100
#define HASHTABLE_SIZE  26

#define SUCCESS    0
#define FAILURE    1
#define NOT_FOUND (-1)

typedef struct file_node
{
    int word_count;
    char file_name[FILE_NAME_SIZE];
    struct file_node *next_file;
} Flist;

typedef struct word_node
{
    char word[WORD_SIZE];
    int file_count;
    Flist *head_file;
    struct word_node *next_word;
} Wlist;

extern Wlist *hash_table[HASHTABLE_SIZE];

int  calculate_hash(const char *word);
void clean_word(char *dest, const char *src);
int  create_database(char *valid_files[], int valid_count);

/* display_db.c */
void display_database(void);

/* search_db.c */
int  search_database(const char *word);

/* save_db.c */
int  save_database(const char *filename);

/* validation.c */
void validate_args(int argc, char *argv[], char *valid_files[], int *valid_count);

#endif /* HEADER_H */