#include "link_list.h"
/*THIS FUNCTION DELETES A NODE */

delnode(int num)
{
	struct node *temp, *m;
	temp=p;
	while(temp!=NULL)
	{
		if(temp->data==num)
		{
			if(temp==p)
			{
				p=temp->next;
				free(temp);
				return;
			}
			else
			{
				m->next=temp->next;
				free(temp);
				return;
			}
		}else
		{
			m=temp;
			temp= temp->next;
		}

	}
	printf("ELEMENT %d NOT FOUND ", num);
}/*THIS FUNCTION ADDS A NODE AT THE LAST OF LINKED LIST */

append( int num )
{
	struct node *temp,*r;
	/* CREATING A NODE AND ASSIGNING A VALUE TO IT */

	temp= (struct node *)malloc(sizeof(struct node));
	temp->data=num;
	r=(struct node *)p;

	if (p == NULL) /* IF LIST IS EMPTY CREATE FIRST NODE */
	{
		p=temp;
		p->next =NULL;
	}
	else
	{        /* GO TO LAST AND ADD*/

		while( r->next != NULL)
			r=r->next;
		r->next =temp;
		r=temp;
		r->next=NULL;
	}
}/* ADD A NEW NODE AT BEGINNING  */

addbeg( int num )
{
        /*  CREATING A NODE AND INSERTING VALUE TO IT  */

	struct node *temp;
        temp=(struct node *)malloc(sizeof(struct node));
        temp->data=num;

	/* IF LIST IS NULL ADD AT BEGINNING  */
        if ( p== NULL)
	{
		p=temp;
		p->next=NULL;
	}

	else
	{
		temp->next=p;
		p=temp;
	}
}

/*  ADD A NEW NODE AFTER A SPECIFIED NO OF NODES */

addafter(int num, int loc)
{
	int i;
	struct node *temp,*t,*r;
	r=p;       /* here r stores the first location */
	if(loc > count()+1 || loc <= 0)
	{
		printf("insertion is not possible :");
		return;
	}
        if (loc == 1)/* if list is null then add at beginning */
	{
		addbeg(num);
		return;
	}
	else
	{
		for(i=1;i<loc;i++)
		{
			t=r;   /* t will be holding previous value */
			r=r->next;
		}
		temp=(struct node *)malloc(sizeof(struct node));
		temp->data=num;
		t->next=temp;
		t=temp;
		t->next=r;
		return;
	}
}/* THIS FUNCTION DISPLAYS THE CONTENTS OF THE LINKED LIST */

display(struct node *r)
{
	r=p;
	if(r==NULL)
	{
		printf("NO ELEMENT IN THE LIST :");
		return;
	}
	/* traverse the entire linked list */
	while(r!=NULL)
	{
		printf(" -> %d ",r->data);
		r=r->next;
	}
	printf("\n");
}
//THIS FUNCTION COUNTS THE NUMBER OF ELEMENTS IN THE LIST
count()
{
	struct node *n;
	int c=0;
	n=p;
	while(n!=NULL)
	{
		n=n->next;
		c++;
	}
	return(c);
}
//THIS FUNCTION REVERSES A LINKED LIST
reverse(struct node *q)
{
	struct node *m, *n,*l,*s;
	m=q;
	n=NULL;
	while(m!=NULL)
	{
		s=n;
		n=m;
		m=m->next;
		n->next=s;
	}
	p=n;
}

