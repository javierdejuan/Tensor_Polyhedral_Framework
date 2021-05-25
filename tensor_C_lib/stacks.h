#ifndef __STACKS_H__
#define __STACKS_H_

/* stack_int_ptr definitions */
typedef struct stack_int_ptr{
        int** elems;
        int    ptr;
        int    size;
}stack_int_ptr;
stack_int_ptr* stack_int_ptr_alloc(int elems);
void stack_int_ptr_free(stack_int_ptr* pstack);
int stack_int_ptr_len(stack_int_ptr* pstack);
int* stack_int_ptr_pop(stack_int_ptr* pstack);
void stack_int_ptr_push(stack_int_ptr* pstack,int* elem);
void stack_int_ptr_push_tail(stack_int_ptr* pstack,int* elem); 
void stack_int_ptr_print(stack_int_ptr* pstack);
/* stack_int  definitions */
typedef struct stack_int{
        int* elems;
        int    ptr;
        int    size;
}stack_int;
stack_int* stack_int_alloc(int elems);
void stack_int_free(stack_int* pstack);
int stack_int_len(stack_int* pstack);
int stack_int_pop(stack_int* pstack);
void stack_int_push(stack_int* pstack,int elem);
void stack_int_push_tail(stack_int* pstack,int elem); 
void stack_int_print(stack_int* pstack);
#endif
