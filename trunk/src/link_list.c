#include "link_list.h"
#include <assert.h>
struct node *TIMER_LIST;

/*THIS FUNCTION ADDS A NODE AT THE END OF LINKED LIST */
void append(struct node *pnode)
{
  struct node *r;
  r = TIMER_LIST;

  while (r->next != NULL) {
    r = r->next;
  }
  r->next = pnode;
  r = pnode;
  r->next = NULL;
}

/* THIS FUNCTION REDUCES THE timeout_counter FIED VALUE OF EACH NODE */
void decreasetime()
{
  struct node *r;
  r = TIMER_LIST->next;

  while (r != NULL) {
    r->timeout_counter --;
    r = r->next;
  }
}

/*THIS FUNCTION REMOVES A NODE FROM THE LIST */
void delnode(int num)
{
  struct node *r, *temp;
  temp = TIMER_LIST;

  while (temp->next != NULL && temp->next->data != num) {
    temp = temp->next;
  }
  assert(temp->next != NULL);
  r = temp->next;
  temp->next = r->next;
}

/*THIS FUNCTION MOVES A NODE TO THE LIST END */
void movetoend(struct node *temp)
{
  struct node *r;

  r = temp->next;
  temp->next = r->next;
  append(r);
}
