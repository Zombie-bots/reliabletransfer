#include <stdio.h>
#include <stdlib.h>
#include "../src/link_list.h"

  main()
 {
        int i;
   p=NULL;
   while(1) /* this is an indefinite loop */
 {
    printf("1.INSERT A NUMBER AT BEGINNING;");
    printf("2.INSERT A NUMBER AT LAST:");
    printf(" 3.INSERT A NUMBER AT A PARTICULAR LOCATION INlIST:");
    printf(" 4.PRINT THE ELEMENTS IN THE LIST :");
    printf(" 5.PRINT THE NUMBER OF ELEMENTS IN THE LIST  ");
    printf(" 6.DELETE A NODE IN THE LINKED LIST:");
    printf(" 7.REVERSE A LINKED LIST :");
    printf(" 8.GET OUT OF LINKED LIST (BYEE BYEE):");
    printf(" PLEASE, ENTER THE NUMBER:");

    scanf("%d",&i); /* ENTER A VALUE FOR SWITCH  */

      switch(i)
    {
         case 1:
      {
        int num;
        printf("PLEASE ENTER THE NUMBER :");
        scanf("%d",&num);
        addbeg(num);
        break;
      }
          case 2:
       {
         int num;
         printf(" PLEASE ENTER THE NUMBER :");
         scanf("%d",&num);
         append(num);
         break;
       }

   case 3:
     {
      int num, loc,k;
      printf(" PLEASE ENTER THE NUMBER :");
      scanf("%d",&num);
      printf("PLEASE ENTER THE LOCATION NUMBER :");
      scanf("%d",&loc);
      addafter(num,loc);
      break;
    }  case 4:
      {
         struct node *n;
         printf("   THE  ELEMENTS IN THE LIST ARE : ");
         display(n);
         break;
      }

      case 5:
   {
      struct node *n;
      display(n);
      printf(" TOTAL NO OF ELEMENTS IN THE LSIT ARE %d",count());
      break;
   } case 6:
    {
            int    num;
      printf(" PLEASE ENTER A NUMBER FROM THE LIST :");
      scanf("%d",&num);
      delnode(num);
     break;
    }
   case 7:
    {
      reverse(p);
        display(p);
        break;
    }
  case 8:
 {
  exit(0);
 }
    }/* end if switch */
 }/* end of while */
}/* end of main */




