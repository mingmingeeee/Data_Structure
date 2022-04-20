#pragma once

#ifndef STACKADT_H
#define STACKADT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 

typedef long long Item;

struct stack_type {
	Item *contents;
	int top;
	int size;
};

typedef struct stack_type *Stack;

Stack create();
void destroy(Stack s);
void make_empty(Stack s);
bool is_empty(Stack s);
void push(Stack s, Item i);
Item pop(Stack s);
Item peek(Stack s);
void rellocate(Stack s);

#endif