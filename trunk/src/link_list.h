#ifndef LINK_LIST_HEADER
#define LINK_LINS_HEADER
struct node
{
	int data;
	struct node *next;
}*p;

delnode(int num);
append(int num);
addbeg(int num);
addafter(int num, int loc);
display(struct node *r);
count();
reverse(struct node *q);

#define NULL 0
#endif
