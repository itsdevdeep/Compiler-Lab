#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#define null 0

// Symbol table created
struct symbolTab
{
    char dataType[20];
    char varName[20];
    int address;
    struct symbolTab *next;
};

// First and last pointers of symbol table
struct symbolTab *first, *last;

// Initially the size is zero
int size = 0;

// Initialize the primary address to allocate the variables to memory
int addr = 1000;

// File to note the errors in the code
FILE *errorLogs, *errorLogsRead;

// Searches for a particular variable name to avoid duplication
int search(char var[])
{
    int i, flag = 0;
    struct symbolTab *p;
    p = first;
    for (i = 0; i < size; i++)
    {
        if (strcmp(p->varName, var) == 0)
        {
            flag = 1;
        }
        p = p->next;
    }
    return flag;
}

// Returns 'true' if the string is a Keyword representing a variable
bool isKeywordVar(char *str)
{
    if (!strcmp(str, "int") || !strcmp(str, "float") || !strcmp(str, "char") || !strcmp(str, "double") || !strcmp(str, "long"))
        return true;

    return false;
}

// Function to insert a variable into symbol table
void insert(char *str, char *datType)
{
    int n;
    n = search(str);

    errorLogs = fopen("errorLogs.txt", "a");

    if (n == 1)
    {
        fprintf(errorLogs, "The variable name %s already exists, hence the variable is not inserted again into symbol table.\n", str);
    }
    else
    {
        if (isKeywordVar(datType))
        {
            struct symbolTab *p;
            p = malloc(sizeof(struct symbolTab));
            strcpy(p->dataType, datType);
            strcpy(p->varName, str);
            p->address = addr++;
            p->next = null;
            if (size == 0)
            {
                first = p;
                last = p;
            }
            else
            {
                last->next = p;
                last = p;
            }
            size++;
        }
        else
        {
            fprintf(errorLogs, "No data type is mentioned for the variable %s.\n", str);
        }
    }
    fclose(errorLogs);
}

// Displays the final symbol table
void display()
{
    int i;
    struct symbolTab *p;
    p = first;
    printf("SRL NO.\t\tDATA TYPE\tLABEL\t\tADDRESS\n");
    for (i = 0; i < size; i++)
    {
        printf("%d\t\t%s\t\t%s\t\t%d\n", i + 1, p->dataType, p->varName, p->address);
        p = p->next;
    }
}

// Returns string after removing extra white spaces
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

// Returns 'true' if the character is a DELIMITER.
bool isDelimiter(char ch)
{
    if (ch == ' ' || ch == '+' || ch == '-' || ch == '*' ||
        ch == '/' || ch == ',' || ch == ';' || ch == '>' ||
        ch == '<' || ch == '=' || ch == '(' || ch == ')' ||
        ch == '[' || ch == ']' || ch == '{' || ch == '}')
        return (true);
    return (false);
}

// Returns 'true' if the character is an OPERATOR.
bool isOperator(char ch)
{
    if (ch == '+' || ch == '-' || ch == '*' ||
        ch == '/' || ch == '>' || ch == '<' ||
        ch == '=')
        return (true);
    return (false);
}

// Returns 'true' if the string is a VALID IDENTIFIER.
bool validIdentifier(char *str)
{
    strcpy(str, trim(str));
    if (str[0] == '0' || str[0] == '1' || str[0] == '2' ||
        str[0] == '3' || str[0] == '4' || str[0] == '5' ||
        str[0] == '6' || str[0] == '7' || str[0] == '8' ||
        str[0] == '9' || isDelimiter(str[0]) == true)
        return (false);

    return (true);
}

// Returns 'true' if the string is a KEYWORD.
bool isKeyword(char *str)
{
    strcpy(str, trim(str));
    if (!strcmp(str, "if") || !strcmp(str, "else") ||
        !strcmp(str, "while") || !strcmp(str, "do") ||
        !strcmp(str, "break") ||
        !strcmp(str, "continue") || !strcmp(str, "int") || !strcmp(str, "double") || !strcmp(str, "float") || !strcmp(str, "return") || !strcmp(str, "char") || !strcmp(str, "case") || !strcmp(str, "char") || !strcmp(str, "sizeof") || !strcmp(str, "long") || !strcmp(str, "short") || !strcmp(str, "typedef") || !strcmp(str, "switch") || !strcmp(str, "unsigned") || !strcmp(str, "void") || !strcmp(str, "static") || !strcmp(str, "struct") || !strcmp(str, "goto") || !strcmp(str, "#include") || !strcmp(str, "main") || !strcmp(str, "printf"))
        return (true);
    return (false);
}

// Returns 'true' if the string is an INTEGER.
bool isInteger(char *str)
{
    strcpy(str, trim(str));
    int i, len = strlen(str);

    if (len == 0)
        return (false);
    for (i = 0; i < len; i++)
    {
        if (str[i] != '0' && str[i] != '1' && str[i] != '2' && str[i] != '3' && str[i] != '4' && str[i] != '5' && str[i] != '6' && str[i] != '7' && str[i] != '8' && str[i] != '9' || (str[i] == '-' && i > 0))
            return (false);
    }
    return (true);
}

// Returns 'true' if the string is a REAL NUMBER.
bool isRealNumber(char *str)
{
    strcpy(str, trim(str));
    int i, len = strlen(str);
    bool hasDecimal = false;

    if (len == 0)
        return (false);
    for (i = 0; i < len; i++)
    {
        if (str[i] != '0' && str[i] != '1' && str[i] != '2' && str[i] != '3' && str[i] != '4' && str[i] != '5' && str[i] != '6' && str[i] != '7' && str[i] != '8' && str[i] != '9' && str[i] != '.' ||
            (str[i] == '-' && i > 0))
            return (false);
        if (str[i] == '.')
            hasDecimal = true;
    }
    return (hasDecimal);
}

// Returns 'true' if the string is a COMMENT LINE.
bool isComment(char *str)
{
    strcpy(str, trim(str));
    int length = strlen(str);
    if (length < 2)
        return false;
    else if ((str[0] == str[1]) && (str[0] == '/'))
        return true;
    else if ((str[0] == str[length - 1]) && (str[1] == str[length - 2]))
    {
        if (str[0] == '/' && str[1] == '*')
            return true;
    }
    else
        return false;
}

// Returns 'true' if the string is a HEADER FILE.
bool isHeader(char *str)
{
    strcpy(str, trim(str));
    int length = strlen(str);

    if ((str[length - 1] == 'h') && (str[length - 2] == '.'))
    {
        return true;
    }

    return false;
}

// Returns 'true' if the string contains a special charecter.
bool isSpecial(char *str)
{
    strcpy(str, trim(str));
    int length = strlen(str), f = 0;

    for (int i = 0; i < length; i++)
    {
        char ch = str[i];
        if (ch == '_')
            f = 0;

        else if (((int)ch >= 32 && (int)ch <= 47) || ((int)ch >= 58 && (int)ch <= 64) || ((int)ch >= 91 && (int)ch <= 96) || ((int)ch >= 123 && (int)ch <= 127))
        {
            f = 1;
            break;
        }
        else
        {
            f = 0;
        }
    }

    if (f == 1)
        return true;

    return false;
}

// Returns true if the string is a format specifier
bool isFormat(char *str)
{
    strcpy(str, trim(str));
    int length = strlen(str);

    if ((length <= 5) && (str[1] == '%'))
        return true;

    return false;
}

// Extracts the SUBSTRING.
char *subString(char *str, int left, int right)
{
    strcpy(str, trim(str));
    int i;
    char *subStr = (char *)malloc(
        sizeof(char) * (right - left + 2));

    for (i = left; i <= right; i++)
        subStr[i - left] = str[i];
    subStr[right - left + 1] = '\0';
    return (subStr);
}

// Parsing the input STRING.
void parse(char *str)
{
    int left = 0, right = 0, flag;
    int len = strlen(str);
    char tempStr[30];

    FILE *fcode;

    fcode = fopen("codefile.txt", "a");

    if (isComment(str))
    {
        fprintf(fcode, "'%s' IS A COMMENT LINE.\n", str);
    }
    else
    {
        while (right <= len && left <= right)
        {
            if (isDelimiter(str[right]) == false)
                right++;

            if (isDelimiter(str[right]) == true && left == right)
            {
                if (isOperator(str[right]) == true)
                    fprintf(fcode, "'%c' IS AN OPERATOR.\n", str[right]);

                right++;
                left = right;
            }
            else if (isDelimiter(str[right]) == true && left != right || (right == len && left != right))
            {
                char *subStr = subString(str, left, right - 1);

                if (strcmp(subStr, ""))
                {

                    if (isKeyword(subStr) == true)
                    {
                        fprintf(fcode, "'%s' IS A KEYWORD.\n", subStr);

                        strcpy(tempStr, subStr);
                    }

                    else if (isHeader(subStr) == true)
                        fprintf(fcode, "'%s' IS A HEADER FILE.\n", subStr);

                    else if (isInteger(subStr) == true)
                        fprintf(fcode, "'%s' IS AN INTEGER (CONSTANT).\n", subStr);

                    else if (isRealNumber(subStr) == true)
                        fprintf(fcode, "'%s' IS A REAL NUMBER.\n", subStr);

                    else if (isFormat(subStr) == true && isDelimiter(str[right - 1]) == false)
                        fprintf(fcode, "'%s' IS A FORMAT SPECIFIER.\n", subStr);

                    else if (isSpecial(subStr) == true && isDelimiter(str[right - 1]) == false)
                        fprintf(fcode, "'%s' IS NOT A VALID IDENTIFIER BECAUSE IT CONTAINS A SPECIAL CHARACTER.\n", subStr);

                    else if (validIdentifier(subStr) == true && isDelimiter(str[right - 1]) == false)
                    {
                        fprintf(fcode, "'%s' IS A VALID IDENTIFIER.\n", subStr);

                        insert(subStr, tempStr);
                    }

                    else if (validIdentifier(subStr) == false && isDelimiter(str[right - 1]) == false)
                        fprintf(fcode, "'%s' IS NOT A VALID IDENTIFIER BECAUSE IT EITHER STARTS WITH A NUMBER.\n", subStr);

                    left = right;
                }
            }
        }
    }

    fclose(fcode);

    return;
}

// DRIVER FUNCTION
int main()
{
    char s[5000];
    int temp, cnt = 0;
    FILE *fptr;
    char str, strTwo;


    printf("\nEnter your Code here :\n\n");

    while (1)
    {
        temp = scanf("%[^\n]%*c", s);

        if (temp == -1)
        {
            break;
        }
        else
        {
            parse(s);

            cnt++;
        }
    }

    fptr = fopen("codefile.txt", "r");
    printf("\n\nThe lexical analysis of the code is : \n\n");
    str = fgetc(fptr);
    while (str != EOF)
    {
        printf("%c", str);
        str = fgetc(fptr);
    }
    printf("\n");
    fclose(fptr);

    printf("Total number of lines in program : %d\n\n", cnt);

    printf("\nFinal Symbol table : \n\n");

    display();

    printf("\n");

    printf("\nError Logs : \n\n");

    errorLogsRead = fopen("errorLogs.txt", "r");
    strTwo = fgetc(errorLogsRead);
    while (strTwo != EOF)
    {
        printf("%c", strTwo);
        strTwo = fgetc(errorLogsRead);
    }
    printf("\n\n");

    fclose(errorLogsRead);

    remove("codefile.txt");
    remove("errorLogs.txt");

    return 0;
}
