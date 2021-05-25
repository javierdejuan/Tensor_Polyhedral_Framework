#include <stdio.h>
#include <stdlib.h>
#include "stacks.h"
/* stack_int_ptr functions */
stack_int_ptr* stack_int_ptr_alloc(int elems){
    stack_int_ptr* pstack = (stack_int_ptr*)malloc(sizeof(stack_int_ptr));
    pstack->elems = (int**)malloc(sizeof(int*)*elems);
    pstack->ptr   = - 1 ;
    pstack->size  = elems;
    return pstack;
}
void stack_int_ptr_free(stack_int_ptr* pstack){
    free(pstack->elems);
    free(pstack);
}
int stack_int_ptr_len(stack_int_ptr* pstack){
    return pstack->ptr + 1;
}
int* stack_int_ptr_pop(stack_int_ptr* pstack){
    if(stack_int_ptr_len(pstack)){
        return pstack->elems[pstack->ptr--];
    }
    else{
       printf("\n[stack_pop] can't pop! stack is empty.");
       return NULL;

    }
}
void stack_int_ptr_push(stack_int_ptr* pstack,int* elem){
    if(pstack->size >stack_int_ptr_len(pstack))
      pstack->elems[++pstack->ptr] = elem;
    else
      printf("\n[stack_push] can´t push! not enough memory.");
}
void stack_int_ptr_push_tail(stack_int_ptr* pstack,int* elem){
  size_t len=stack_int_ptr_len(pstack);
  if(pstack->size > stack_int_ptr_len(pstack)){
    for(int i=len-1;i>=0;i--){
        int* pint=pstack->elems[i];
        pstack->elems[i+1]=pint;       
    }
    pstack->elems[0]=elem;
    pstack->ptr++;
  }
  else
    printf("\n[stack_push] can´t push! not enough memory.");
  stack_int_ptr_print(pstack);
} 
void stack_int_ptr_print(stack_int_ptr* pstack){
/*
    for( int i = 0 ; i <= pstack->ptr ; i++)
            if(len_int_ptr(pstack->elems[i]->id) > max_width)
                    max_width = strlen(pstack->elems[i]->id);
    int white_spaces = max_width+2;

    printf("\n[stack_print] size:%d len:%d stack->ptr:%d\n",pstack->size,stack_ptr_len(pstack),pstack->ptr);
    for( int i = 0 ; i < pstack->size ; i++){
            if(pstack->ptr == i){

                    printf("-->[");
                    if(stack_int_ptr_len(pstack ) > i ){
                            printf("%c%s%c",' ',pstack->elems[i]->id,' ');
                    }
                    else
                            printf("%*c",white_spaces,' ');
                    printf("]\n");
            }
            else{
                    printf("   [");
                    if(stack_int_ptr_len(pstack) > i ){
                            printf("%c%s%c",' ',pstack->elems[i]->id,' ');
                    }
                    else
                            printf("%*c",white_spaces,' ');
                    printf("]\n");
            }
    }
*/    
}

/* stack_int functions */
static int len_int(int a){
  int i=0;
  a = a>=0? a: -a;
  while(a>0){a=a/10;i++; }
}
stack_int* stack_int_alloc(int elems){
    stack_int* pstack = (stack_int*)malloc(sizeof(stack_int));
    pstack->elems = (int*)malloc(sizeof(int)*elems);
    pstack->ptr   = - 1 ;
    pstack->size  = elems;
    return pstack;
}
void stack_int_free(stack_int* pstack){
    free(pstack->elems);
    free(pstack);
}
int stack_int_len(stack_int* pstack){
    return pstack->ptr + 1;
}
int stack_int_pop(stack_int* pstack){
    if(stack_int_len(pstack)){
        return pstack->elems[pstack->ptr--];
    }
    else{
       printf("\n[stack_pop] can't pop! stack is empty.");
       return 0;

    }
}
void stack_int_push(stack_int* pstack,int elem){
    if(pstack->size >stack_int_len(pstack))
      pstack->elems[++pstack->ptr] = elem;
    else
      printf("\n[stack_push] can´t push! not enough memory.");
}
void stack_int_push_tail(stack_int* pstack,int elem){
  size_t len=stack_int_len(pstack);
  if(pstack->size > stack_int_len(pstack)){
    for(int i=len-1;i>=0;i--){
        int pint=pstack->elems[i];
        pstack->elems[i+1]=pint;       
    }
    pstack->elems[0]=elem;
    pstack->ptr++;
  }
  else
    printf("\n[stack_push] can´t push! not enough memory.");
  stack_int_print(pstack);
} 
void stack_int_print(stack_int* pstack){

    int max_width = 0;

    for( int i = 0 ; i <= pstack->ptr ; i++)
            if(len_int(pstack->elems[i]) > max_width)
                    max_width = len_int(pstack->elems[i]);

    int white_spaces = max_width+2;

    printf("\n[stack_print] size:%d len:%d stack->ptr:%d\n",pstack->size,stack_int_len(pstack),pstack->ptr);
    for( int i = 0 ; i < pstack->size ; i++){
            if(pstack->ptr == i){

                    printf("-->[");
                    if(stack_int_len(pstack ) > i ){
                            printf("%c%d%c",' ',pstack->elems[i],' ');
                    }
                    else
                            printf("%*c",white_spaces,' ');
                    printf("]\n");
            }
            else{
                    printf("   [");
                    if(stack_int_len(pstack) > i ){
                            printf("%c%d%c",' ',pstack->elems[i],' ');
                    }
                    else
                            printf("%*c",white_spaces,' ');
                    printf("]\n");
            }
    }
}
