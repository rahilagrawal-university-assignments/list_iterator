/* List Iterator
Written by : Rahil Agrawal
Date Created: 10.08.2017
Last Edited: 10.08.2017
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "listIteratorG.h"
#include "positiveIntType.h"

/*
Structure Node
Pointer to void - can be made to point to any data type
Pointer to next - Points to element on the right, if there is no element on the right, next - NULL
Pointer to prev - Points to element on the left, if there is no element on the left, prev = NULL
*/

typedef struct Node {

	void *value;
	struct Node *next;
	struct Node *prev;

} Node;

/*
Structure Representation
Contains pointer to first element and pointers to elements currently in use.
Also contains pointers to functions to be used to manipulate elements

Head - keeps track of the starting of the list
Curr - Normally, used for tracking the element on which next operation should be performed
Is also used for checking which operation was performed last
Last - Pointer to the element that the last operation was performed on.
after - flag used for operations to be performed near the head of the list
*/
typedef struct IteratorGRep {


	Node *head;
	Node *curr;
	Node *last;
	int after;

	ElmCompareFp compare_element;
	ElmNewFp new_element;
	ElmFreeFp free_element;

} IteratorGRep;
//Function to assign function pointers for Generic Use
IteratorG IteratorGNew(ElmCompareFp cmp, ElmNewFp copy, ElmFreeFp free)
{
	IteratorGRep *new_iterator;
	new_iterator = malloc(sizeof(struct IteratorGRep));
	if(new_iterator==NULL)
	{
		fprintf(stderr, "Error! Not enough memory for an Iterator Struct\n");
		return 0;
	}
	//assign head of the list as NULL
	//assign curr and last as NULL
	new_iterator->head = NULL;
	new_iterator->curr=NULL;
	new_iterator->last=NULL;
	new_iterator->after = 0;

	//assign the functions to be used to manipulate the iterator
	new_iterator->compare_element = cmp;
	new_iterator->new_element = copy;
	new_iterator->free_element = free;

	//return pointer to a new iterator that consists
	return new_iterator;
}
//Function to add an element after the element last used
int  add(IteratorG it, void *vp)
{
	assert(it!=NULL);
	struct Node * new_node = malloc(sizeof(struct Node));
	assert(new_node!=NULL);
	new_node -> value = it->new_element(vp);
	// Insert first element
	// head and last all point to first element as it is the only element in the list
	if(it->head==NULL)
	{
		it->head = new_node;
		it->head->next=NULL;
		it->head->prev=NULL;
		//it->curr = it->head;
		it->last = it->head;
		it->after = 1;
		return 1;
	}
	if(it->head == it->last && it->after == 0)
	{
		new_node->next = it->head;
		it->head->prev = new_node;
		it->head = new_node;
		it->last = it->head;
		it->head->prev = NULL;
		it->curr = it->head->next;
		it->after = 1;
		return 1;
	}
	// There is atleast one element, insert element after the last element
	// last element point to the new element
	if(it->curr == it->last->prev && it->last != it->head)
	{
		if(it->curr!=NULL)
		{
			it->curr->next=new_node;
			new_node->prev = it->curr;
		}
		new_node->next = it->last;
		it->last->prev = new_node;
		it->last = new_node;
		it->curr = it->last->next;
		return 1;
	}
	new_node->next = it->last->next;
	new_node->prev = it->last;
	it->last->next = new_node;
	//it->curr = new_node->prev;
	it->last = new_node;
	if(new_node->next==NULL)
	return 1;
	new_node->next->prev = new_node;
	return 1;
}

//Function to check if there is an element after the current element
//Returns 0 if there is no element after current element, 1 otherwise
int hasNext(IteratorG it)
{
	assert(it!=NULL);
	if(it->last->next==NULL)
	{
		if(it->last==it->head && it->curr==NULL && it->after==0)
		{
			//it->after = 1;
			return 1;
		}
		return 0;
	}
	else
	{
		return 1;
	}
}

//Function to check if there is an element before current element
//Returns 0 if there is no element before current element, 1 otherwise
int hasPrevious(IteratorG it)
{
	assert(it!=NULL);
	if(it->last->prev==NULL)
	{
		if(it->last == it->head && it->curr==NULL && it->after==1)
		{
			//it->after=0;
			return 1;
		}
		return 0;
	}
	else
	{
		return 1;
	}
}

//Function to print the element after 'cursor' and move cursor after the element
void * next(IteratorG it)
{
	assert(it!=NULL);
	//if the cursor is after last element, no more elements on the right, return NULL
	if(!hasNext(it) && it->curr==NULL)
	{
		return NULL;
	}
	//if cursor is on the left of the first element, move it between first and second element and return first element
	if(it->curr==NULL && it->last==it->head)
	{
		it->curr=it->last->next;
		if(it->after==0)
		it->after=1;
		return it->last->value;
	}
	//if cursor is beween second last and last element, return last element and move cursor to the right of last element
	if(it->curr->next==NULL)
	{
		it->last=it->curr;
		it->curr=NULL;
		return it->last->value;
	}
	//if previous() was called immediately before next(), return the same element that was returned by next
	if(it->last==it->curr->next)
	{
		it->curr=it->last->next;
		return it->last->value;
	}
	//return element on the right of the cursor and move cursor one to the right
	it->curr=it->curr->next;
	it->last = it->curr->prev;
	return it->last->value;
}

//Function to print element before cursor and move cursor before the element
void * previous(IteratorG it)
{
	assert(it!=NULL);
	//return NULL if there is no element before current element, cursor is on the left of the first element
	if(!hasPrevious(it) && it->curr==NULL)
	{
		return NULL;
	}
	//return the first element if cursor is between first and second element
	if(it->curr==it->head)
	{
		it->curr = NULL;
		it->last = it->head;
		if(it->after==1)
		it->after = 0;
		return it->last->value;
	}
	//if cursor is after the last element, print last element
	if(it->curr==NULL && it->last->next==it->curr)
	{
		it->curr=it->last->prev;
		if(it->last == it->head && it->after == 1)
		{
			it->after = 0;
		}
		return it->last->value;
	}
	//if previous() is called immediately after next(), return the value that was returned with next
	if(it->last==it->curr->prev)
	{
		it->curr=it->last->prev;
		return it->last->value;
	}
	// return element on the left of the cursor and move cursor one to the left
	it->curr = it->curr->prev;
	it->last = it->curr->next;
	return it->last->value;
}

//Function to delete the last element returned with next, previous, findnext or findprevious
int delete(IteratorG it)
{
	assert(it!=NULL);
	if(it->head==NULL)
	{
		return 0;
	}
	//if the last element used was the head, set second element to head and free the previous head
	if(it->last==it->head)
	{
		if(it->head->next == NULL && it->head->prev == NULL)
		{
			free(it->head);
			it->head=NULL;
			it->last=NULL;
			it->curr=NULL;
			it->after=0;
			return 1;
		}
		it->last = it->head->next;
		it->last->prev=NULL;
		if(it->curr != NULL)
		it->curr = it->last->next;
		free(it->head);
		it->head=it->last;
		it->after = 0;
		return 1;
	}
	//if the last element used was the last element of the list, set second last element as last element as free the previous last element
	if(it->last->next==NULL)
	{
		struct Node *temp_node = it->last;
		it->last = it->last->prev;
		it->last->next = NULL;
		if(it->curr!=NULL)
		it->curr=it->last->prev;
		if(it->last == it->head) it->after = 1;
		free(temp_node);
		return 1;
	}
	//In other cases, link the elements on the right and left of the current element and free current element.
	//Set cursor between the elements
	struct Node * new_last;
	if(it->last==it->curr->next)
	new_last = it->last->next;
	else
	new_last = it->last->prev;
	it->last->prev->next = it->last->next;
	it->last->next->prev = it->last->prev;
	free(it->last);
	it->last = new_last;
	return 1;
}

//Function to replace last element returned with next,previous,findnext or previous with vp
//Uses logic similar to delete, instead of deleting, the elements is replaced and pointers are retained at corresponding places
int set (IteratorG it, void *vp)
{
	assert(it!=NULL);
	if(it->head==NULL)
	{
		return 0;
	}
	//create new element with vp as the value
	struct Node * new_node = malloc(sizeof(struct Node));
	assert(new_node!=NULL);
	new_node -> value = it->new_element(vp);
	//If replacing the first element of the list
	if(it->last == it->head)
	{
		struct Node * temp_node = it->head;
		new_node->next = it->head->next;
		it->head->next->prev = new_node;
		it->head = new_node;
		it->last = it->head;
		it->last->prev=NULL;
		//If the last fucntion called was next, cursor should be placed between first and second element, otherwise to the left of first element
		if(it->curr != NULL)
		it->curr=it->last->next;
		free(temp_node);
		return 1;
	}
	//if the last element is being replaced
	if(it->last->next==NULL)
	{
		struct Node *temp_node = it->last;
		new_node->prev = it->last->prev;
		it->last->prev->next=new_node;
		it->last = new_node;
		it->last->next=NULL;
		if(it->curr!=NULL)
		it->curr=it->last->prev;
		free(temp_node);
		return 1;
	}
	//if its not the first or the last element, insert the new element between the prev and next elements of the element
	//returned with next,prev,findnext,findprev and free/delete that element. Set the new element as the new it->last
	new_node->next= it->last->next;
	it->last->prev->next=new_node;
	new_node->prev = it->last->prev;
	it->last->next->prev = new_node;
	if(it->curr == it->last->prev)
	{
		it->curr = new_node->prev;
	}
	else if(it->curr == it->last->next)
	{
		it->curr = new_node->next;
	}
	free(it->last);
	it->last = new_node;
	return 1;
}
//Function to find a node with the same value as the value pointed by vp, on the right of the cursor
//Return pointer to the node if there is any (and move cursor accordingly), NULL otherwise (don't change cursor position)
void * findNext(IteratorG it, void * vp)
{
	assert(it!=NULL);
	struct Node * current;
	current = it->last;
	while(current!=NULL)
	{
		if(it->compare_element(vp,current->value)==0)
		{
			it->last = current;
			it->curr = it->last->next;
			return it->last->value;
		}
		current = current ->next;
	}
	return current;
}

//Function to find a node with the same value as the value pointed by vp, on the left of the cursor
//Return pointer to the node if there is any (and move cursor accordingly), NULL otherwise (don't change cursor position)
void * findPrevious(IteratorG it, void * vp)
{
	assert(it!=NULL);
	struct Node * current;
	current = it->last;
	while(current!=NULL)
	{
		if(it->compare_element(vp,current->value)==0)
		{
			it->last = current;
			it->curr = it->last->prev;
			if (it->last==it->head) it->after = 0;
			return it->last->value;
		}
		current = current ->prev;
	}
	return current;
}

//Function to reset cursor to start of the list
void reset(IteratorG it)
{
	assert(it!=NULL);
	it->last = it->head;
	it->curr = NULL;
	it->after = 0;
}

void freeIt(IteratorG it)
{
	assert(it!=NULL);
	Node * curr = it->head;
	while(curr!=NULL)
	{
		it->free_element(curr->value);
		Node * temp = curr;
		curr= curr->next;
		free(temp);
	}
	free(it);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TESTING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//This function is used to print the list
//Comment it out once testing is done
/*
void print_list(IteratorG it)
{
struct Node *current = it->head;
printf("[ ");
while(current!=NULL)
{
if(current==it->curr && it->last->next == it->curr && it->curr !=NULL)
break;
if(current == it->last && it->last->prev == it->curr && it->curr !=NULL)
break;
if(current == it->head && it->curr==NULL && it->head==it->last && it->after == 0)
break;
if(current==it->last->next && it->curr==it->last)
break;
printf("%s ",(char *) current->value);
current=current->next;
}
printf(" ^ ");
if(current==NULL)
{
printf("]\n");
return;
}
//current = current->next;
while(current!=NULL)
{
printf("%s ",(char *) current->value);
current=current->next;
}
printf("]\n");
return;
}
*/
