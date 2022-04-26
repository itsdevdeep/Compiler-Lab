#include <stdio.h>
#include <conio.h>
void main()
{
    char com[30];
    int i = 2, a = 0;
    printf("\n Enter a String:");
    gets(com);
    if (com[0] == '/')
    {
        if (com[1] == '/')
            printf("\n It is a single-line comment.\n");
        else if (com[1] == '*')
        {
            for (i = 2; i <= 30; i++)
            {
                if (com[i] == '*' && com[i + 1] == '/')
                {
                    printf("\n It is a multi-line comment.\n");
                    a = 1;
                    break;
                }
                else
                    continue;
            }
            if (a == 0)
                printf("\n It is not a comment.\n");
        }
        else
            printf("\n It is not a comment.\n");
    }
    else
        printf("\n It is not a comment.\n");
    getch();
}