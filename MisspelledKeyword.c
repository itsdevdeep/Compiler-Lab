#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define HASH_SIZE 2000
#define LENGTH 2000

typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

node *hashtable[HASH_SIZE];

int hash_function(const char *word);

int number_of_words = 0;

char keywords[32][10] = {"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register",
                         "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"};

char operators[4] = {'+', '-', '/', '*'};

char inbuilt_fns[50][10] = {"scanf", "printf", "isalpha", "isdigit", "isalnum", "islower", "isspace", "tolower", "toupper", "isupper", "isxdigit", "fprintf", "fscanf", "strcat", "strcmp", "strcpy",
                            "strncat", "strncmp", "strncpy", "strlen", "strrchr", "strchr", "strupr", "strlwr", "strdup", "strstr", "strrstr", "strset", "strnset", "strtok", "abs", "floor", "round",
                            "ceil", "memset", "memcpy", "memmove", "memcmp", "memchr", "memicmp", "malloc", "calloc", "realloc", "free", "include", "define", "undef", "main", "ifndef", "endif"};

FILE *inputFile, *keywordsFile, *operatorsFile, *identifiersFile, *commentsFile, *dictionaryFile;

bool check(const char *word)
{
    int word_length = strlen(word);
    char lower_word[LENGTH + 1];
    int i = 0;
    for (i = 0; i < word_length; i++)
    {
        if (isupper(word[i]))
        {
            lower_word[i] = tolower(word[i]);
        }
        else
        {
            lower_word[i] = word[i];
        }
    }
    lower_word[word_length] = '\0';
    int bucket = hash_function(lower_word);
    node *cursor = hashtable[bucket];
    while (cursor != NULL)
    {
        if (strcmp(lower_word, cursor->word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }

    return false;
}

bool load(const char *dictionaryFileName)
{
    int i = 0;
    for (i = 0; i < HASH_SIZE; i++)
    {
        hashtable[i] = NULL;
    }
    FILE *the_dictionary;
    the_dictionary = fopen(dictionaryFileName, "r");

    if (the_dictionary == NULL)
    {
        printf("Failed to load dictionary");
        return false;
    }

    char buffer[LENGTH + 1];
    while (fscanf(the_dictionary, "%s", buffer) > 0)
    {
        node *new_node = malloc(sizeof(node));
        new_node->next = NULL;
        strcpy(new_node->word, buffer);

        int bucket = hash_function(new_node->word);

        if (hashtable[bucket] == NULL)
        {
            hashtable[bucket] = new_node;
        }
        else
        {
            new_node->next = hashtable[bucket];
            hashtable[bucket] = new_node;
        }
        number_of_words++;
    }
    fclose(the_dictionary);
    return true;
}

unsigned int size(void)
{
    return number_of_words;
}

bool unload(void)
{
    int i = 0;
    for (i = 0; i < HASH_SIZE; i++)
    {
        node *cursor = hashtable[i];
        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    return true;
}

int hash_function(const char *word)
{
    int sum = 0;
    int word_length = strlen(word);
    int i = 0;
    for (i = 0; i < word_length; i++)
    {
        sum += word[i];
    }

    int bucket = sum % HASH_SIZE;
    return bucket;
}

bool makeDictionaryFile(char words[50][10])
{
    dictionaryFile = fopen("Dictionary.txt", "w");
    if (dictionaryFile == NULL)
    {
        printf("Error while writing in dictionary.\n");
        return false;
    }

    fseek(dictionaryFile, 0, SEEK_END);

    int size = sizeof(words) / sizeof(words[0]), i;
    for (i = 0; i < size; i++)
        fprintf(dictionaryFile, "%s\n", words[i]);
    fclose(dictionaryFile);
    return true;
}

int is_alpha(int c)
{
    return (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
}

int isLexeme(char word[20])
{
    int length = strlen(word), i;

    if (length < 1)
        return 0;

    for (i = 0; i < 32; i++)
        if (!strcmp(keywords[i], word))
            return 1;

    for (i = 0; i < 50; i++)
        if (!strcmp(inbuilt_fns[i], word))
            return 3;

    if (word[0] != '_' && !is_alpha(word[0]))
        return 0;

    return 2;
}

int isOperator(char op)
{
    int i;
    for (i = 0; i < 4; i++)
        if (op == operators[i])
            return 1;
    return 0;
}

int isSpecial(int c)
{
    if (c == '_')
        return 0;
    return (c >= 32 && c <= 47) || (c >= 58 && c <= 64) || (c >= 91 && c <= 96) || (c >= 123 && c <= 127);
}

char *trim(char *s)
{
    int i;

    while (isspace(*s))
        s++;
    for (i = strlen(s) - 1; (isspace(s[i])); i--)
        ;
    s[i + 1] = '\0';
    return s;
}

int isComment(char str[100])
{
    strcpy(str, trim(str));
    int length = strlen(str);
    if (length < 2)
        return 0;
    else if ((str[0] == str[1]) && (str[0] == '/'))
        return 1;
    else if ((str[0] == str[length - 1]) && (str[1] == str[length - 2]))
    {
        if (str[0] == '/' && str[1] == '*')
            return 1;
    }
    else
        return 0;
}

int main()
{
    printf("Enter filename to execute the lexical analyzer.\n");
    char filename[20];
    gets(filename);

    makeDictionaryFile(keywords);
    makeDictionaryFile(inbuilt_fns);

    inputFile = fopen(filename, "r");
    keywordsFile = fopen("Keywords.txt", "w");
    operatorsFile = fopen("Operators.txt", "w");
    commentsFile = fopen("Comments.txt", "w");
    identifiersFile = fopen("Identifiers.txt", "w");

    if (inputFile == NULL)
    {
        printf("Error while opening  the file.\n");
        return 0;
    }

    int lines = 0, length, i, lex = 0, misspelled = 0;
    char c, word[20] = "", line[1000] = "", lastChar;

    if (!load("Dictionary.txt"))
    {
        return 0;
    }

    printf("\nMisspelled functions:\n");

    while ((c = getc(inputFile)) != EOF)
    {
        if (c == '\n')
        {
            lines++;
            if (isComment(line))
            {
                fputs(line, commentsFile);
                putc('\n', commentsFile);
                strncpy(line, "", sizeof(line));
                continue;
            }
            length = strlen(line);
            for (i = 0; i < length; i++)
                if (isSpecial(line[i]))
                {
                    if (isOperator(line[i]))
                        putc(line[i], operatorsFile);

                    int lex = isLexeme(word);
                    if (lex == 1)
                    {
                        fputs(word, keywordsFile);
                        putc('\n', keywordsFile);
                    }
                    else if (lex == 2)
                    {
                        if (line[i] == '(' || line[i] == '<' && line[i] != '=' && line[i] != ';')
                            if (!check(word))
                            {
                                printf("%s\n", word);
                                misspelled++;
                            }
                            else
                            {
                                fputs(word, identifiersFile);
                                putc('\n', identifiersFile);
                            }
                    }
                    else if (lex == 3)
                    {
                        fputs(word, identifiersFile);
                        putc('\n', identifiersFile);
                    }
                    strncpy(word, "", sizeof(word));
                }
                else
                    strncat(word, line + i, 1);
            strncpy(line, "", sizeof(line));
        }
        else
            strncat(line, &c, 1);
        lastChar = c;
    }
    printf("Number of misspelled functions:%d\n", misspelled);
    printf("Number of lines in the file:%d\n", lines + 1);
}
