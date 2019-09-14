/* implemantaition of a stack using a linked list */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

typedef int data;
typedef struct node{
	data d;
	struct node *next;
} node;

typedef struct stack{
	node *top;
} stack;

void initialize_stack(stack *stk){
	stk->top = NULL;
}	

void push(stack *stk, data newdata){
	node *newNode = NULL;
	newNode = (node*)malloc(sizeof(node));
	if(newNode == NULL){
		printf("error! malloc failed, in 'push' func");
		exit(1);
	}
	newNode->d = newdata;
	newNode->next = stk->top;
	stk->top = newNode;
}

data pop(stack *stk){
	data d;
	node *head = NULL;
	
	if(stk->top == NULL){
		printf("\n trying to pop an empty stack! \n");
	}
	assert(stk->top != NULL); /*trying to pop an empty stack!*/
	
	
	
	d = stk->top->d;
	head = stk->top;
	stk->top = stk->top->next;
	free(head);
	return d;
}

data top(stack *stk){
	return (stk->top->d);
}

bool isEmpty(stack *stk){
	return (stk->top == NULL);
}

/*need to store two coordinates, simplest method is to use two stacks*/
void initialize_double_stack(stack *stk1,stack *stk2){
	initialize_stack(stk1);
	initialize_stack(stk2);
}	

void double_push(stack *stk1, data d1, stack *stk2, data d2){
	push(stk1, d1);
	push(stk2, d2);
}

void double_pop(stack *stk1, data *d1, stack *stk2, data *d2){
	*d1 = pop(stk1);
	*d2 = pop(stk2);
}	

bool double_isEmpty(stack *stk1, stack *stk2){
	return (stk1->top == NULL || stk2->top == NULL);
}	
	
	
	
	