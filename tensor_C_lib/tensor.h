#ifndef __TENSOR_H_
#define __TENSOR_H_
/* string manipulation */
#include "tuple.h"
char* findUnion(char* str1, char* str2); 
char* findXOR(char* str1, char* str2);
char* findIntersection(char* str1, char* str2);
/* list_str definitions */
typedef struct list_str{
        char** elems;
        size_t size;
        int    index;
}list_str;
list_str* list_str_alloc(size_t elems);
list_str* list_str_copy(list_str* source);
list_str* list_str_free(list_str* plist_str);
char* list_str_access(list_str* plist_str,int elem);
list_str* list_str_add(list_str* plist_str,char* elem);
list_str* list_str_update(list_str* plist_str,int id, char* elem);
size_t list_str_len(list_str* plist_str);
list_str* list_str_from_int_vector(int* vector,size_t size);
list_str* list_str_from_double_vector(double* vector,size_t size);
list_str* list_str_create_sequence(size_t size);
void list_str_print(list_str* plist_str);
void list_str_print_column(list_str* plist_str);
list_str* list_str_product(list_str*a,list_str*b);
list_str* list_str_permutations(int i, size_t* sizes);
char* list_str_to_char_ptr(list_str* plist_str);
char* list_str_dot_product(list_str* a, list_str*b);
list_str* list_str_from_char_ptr(char* str);
list_str* list_str_substract(list_str* a, list_str* b);
list_str* list_str_sum(list_str* a, list_str* b);
list_str* sum_char_ptr( char* a , char* b);
/* stack definitions */
typedef struct stack{
        list_str** elems;
        int    ptr;
        int    size;
}stack;
stack* stack_alloc(int elems);
void stack_free(stack* pstack);
int stack_len(stack* pstack);
list_str* stack_pop(stack* pstack);
void stack_push(stack* pstack,list_str* elem);
void stack_print(stack* pstack); 
/* tensor definitions */
typedef struct tensor{
       int type; // *tensor = tensor->type
       size_t size;
       char  index;
       int isleave;
       int level;
       char* id;
       int isupper;
       union{
               union{list_str* p;int* i;double *d;};
               struct tensor** t;
       };
}tensor;
tensor* tensor_create_from_params(char* indeces, char* lowerupper, int dimensions, int statements, int variables, int parameters);
tensor* tensor_create_lambda(tensor* gamma);
tensor* tensor_create(char* indeces,char* upperlower,size_t* sizes,int init,int type);
tensor* tensor_copy(tensor* ptensor);
void    tensor_copy_elems(tensor* dest,tensor* source);
tensor* tensor_fill_int(tensor* ptensor,char* leave, int* elems);
tensor* tensor_fill_double(tensor* ptensor,char* leave, double* elems);
tensor* tensor_fill(tensor* ptensor,char* leave, list_str* elems);
tensor* create_recursive(tensor** ptensor,int cur, size_t* sizes, int indeces,char* names,char* upperlower,int init,int type);
tensor* tensor_alloc(char index,int isupper,size_t size,int isleave,char* id,int init,int type);
tensor* tensor_access(tensor* t,char* str);
tensor* tensor_access_with_tuple(tensor* ptensor,list_tuple* tuple);
char*   tensor_elem_access_with_tuple(tensor* t,list_tuple* tu);
int     tensor_elem_access_with_tuple_int(tensor* t,list_tuple* tu);
double  tensor_elem_access_with_tuple_double(tensor* t,list_tuple* tu);
tensor* tensor_access_index(tensor* t,char c);
char* tensor_access_elem(tensor* t,char* str);
void tensor_set_elem(tensor* ptensor,list_tuple* tuple,char* elem);
void tensor_set_elem_int(tensor* ptensor,list_tuple* tuple,int elem);
void tensor_set_elem_double(tensor* ptensor,list_tuple* tuple,double elem);
void tensor_free(tensor* ptensor);
void nested_loops(size_t cur,list_tuple* l,list_tuple* l_cur,tensor* res,tensor* big,tensor* small);
void tensor_free_node(tensor* ptensor);
void tensor_print_info(tensor* ptensor,char* msg);
void tensor_print(tensor* ptensor);
void tensor_print_elems_recursive(tensor* ptensor,int upper_father);
void tensor_print_elems(tensor* ptensor);
void tensor_print_elems_leave(tensor* ptensor,int upper_father);
size_t* tensor_get_sizes(tensor* ptensor);
int tensor_get_n_indeces(tensor* ptensor);
size_t   tensor_get_n_leaves(tensor* t);
size_t   tensor_get_n_elems(size_t* sizes,int indeces);
size_t   tensor_get_n_nodes(size_t* sizes,int indeces);
tensor* tensor_einsum(tensor* a,tensor* b);
list_str*  tensor_get_leave(tensor* t,char* leave);
int* tensor_get_leave_int(tensor* t,char* leave);
double* tensor_get_leave_double(tensor*t, char* leave);
char* contract_indeces(char* a,char* b);
char* sum_indeces(char* a,char* b);
char* sum_acum(char* acum, char* term1 , char* term2);
int dotproduct_int(int* a, int* b, size_t size);
double dotproduct_double(double* a, double* b, size_t size);
/* stack_ptr definitions */
typedef struct stack_ptr{
        tensor** elems;
        int    ptr;
        int    size;
}stack_ptr;
stack_ptr* stack_ptr_alloc(int elems);
void stack_ptr_free(stack_ptr* pstack);
int stack_ptr_len(stack_ptr* pstack);
tensor* stack_ptr_pop(stack_ptr* pstack);
void stack_ptr_push(stack_ptr* pstack,tensor* elem);
void stack_ptr_push_tail(stack_ptr* pstack,tensor* elem); 
void tensor_fill_stack(tensor* ptensor, stack_ptr* stack,int only_leaves);
void stack_ptr_print(stack_ptr* pstack);
#endif
