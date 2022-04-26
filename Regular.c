#include<stdio.h>
#include<string.h>
int main()
{
    char seq[100];
    printf("\nEnter the sequence of characters: ");
    gets(seq);

    int length = strlen(seq);
    int rule = 0;

    if(length == 1)
    {
        if(seq[0] == 'a')
            rule=1;
        else if(seq[0] == 'b')
            rule=2;
    }
    else if(length > 1)
    {

        int i = 0;
        for(i=0;i<length;i++)
            if(seq[i] == 'b')
                rule=2;
            else if(seq[i] != 'a')
            {
                rule = 0;
                break;
            }
        if(length == 3)
        {
            if(seq[0] == 'a' && seq[1] == 'b' && seq[2] == 'b')
                rule=3;
            
        }
    }

    if(rule == 0)
        printf("\n%s is not recognised.",seq);
    if(rule == 1)
        printf("\n%s is accepted under rule 'a'",seq);
    if(rule == 2)
        printf("\n%s is accepted under rule 'a*b+'",seq);
    if(rule == 3)
        printf("\n%s is accepted under rule 'abb'",seq);
}