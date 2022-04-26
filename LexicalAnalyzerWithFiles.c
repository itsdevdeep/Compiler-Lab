#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

char keywords[32][10] = {"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register",
                         "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"};

char operators[4] = {'+', '-', '/', '*'};

FILE *inputFile, *keywordsFile, *operatorsFile, *identifiersFile, *commentsFile;

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
    printf("\nEnter the filename of a C program: ");
    char filename[20];
    gets(filename);

    inputFile = fopen(filename, "r");
    keywordsFile = fopen("Keywords.txt", "w");
    operatorsFile = fopen("Operators.txt", "w");
    commentsFile = fopen("Comments.txt", "w");
    identifiersFile = fopen("Identifiers.txt", "w");

    if (inputFile == NULL)
    {
        printf("Error while opening the file.\n");
        return 0;
    }

    int lines = 0, length, i, lex = 0;
    char c, word[20] = "", line[1000] = "", lastChar;
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
    printf("Number of lines: %d\n", lines);
}