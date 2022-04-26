#include <stdio.h>
#include <conio.h>
#include <ctype.h>
void main()
{
      int i=0,flag=0;
      char a[10];
      printf("\nEnter a String : ");
      gets(a);
      for(i=0;i<10;i++)      
      {
            flag=0;
    
            if((a[0]=='_')||(isalpha(a[0])!=0))
            {
                  for(i=1;a[i]!='\0';i++)
                  {
                        if((isalnum(a[i])==0)&&(a[i]!='_'))
                        {
                              flag=1;
                        }
                  }
            }
            else
            {
                  flag=1;
            }
      }
      if(flag==0)
      {
            printf("\n%s is an Identifier.",a);
      }
      else
      {
            printf("\n%s is not an Identifier.",a);
      }
      getch();
}