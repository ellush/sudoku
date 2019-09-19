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


void initialize_stack(stack *stk);

void push(stack *stk, data newdata);

data pop(stack *stk);

bool isEmpty(stack *stk);

/*need to store two coordinates, simplest method is to use two stacks*/
void initialize_double_stack(stack *stk1,stack *stk2);

void double_push(stack *stk1, data d1, stack *stk2, data d2);

void double_pop(stack *stk1, data *d1, stack *stk2, data *d2);

bool double_isEmpty(stack *stk1, stack *stk2);
	