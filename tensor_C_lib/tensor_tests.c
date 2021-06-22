#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "tensor.h"
#include "macro.h"
/* unit tests */
static void list_str_tests(void){

  printf("\n[list_str_tests] START\n");
  int a[10]={0,1,2,3,4,5,6,7,8,9};
  int b[5]={0,1,2,-3,-1};
  int c[4]={0,1,2,3};

  list_str* a_str=list_str_from_int_vector(a,10);
  list_str* b_str=list_str_from_int_vector(b,5);
  list_str* c_str=list_str_from_int_vector(c,4);

  for(int i= 0; i < 4 ;i++ ){
       printf("\n%s",list_str_access(a_str,i));
       printf("\n%s",list_str_access(b_str,i));
       printf("\n%s",list_str_access(c_str,i));
  }


  list_str* d_str=list_str_create_sequence(4);
  
  char str1[4];

  strcpy(str1,"sdr");

  list_str* e_str=list_str_from_char_ptr(str1);
  char* str2=list_str_to_char_ptr(e_str);
  printf("\nfrom list_str:%s",str2);
  list_str_free(e_str);
  free(str2);

  list_str* f_str=list_str_alloc(5);
  list_str_add(f_str,strdup("i0"));
  list_str_add(f_str,strdup("i1"));
  list_str_add(f_str,strdup("i2"));
  list_str_add(f_str,strdup("i3"));
  list_str_add(f_str,strdup("i4"));
  char* dot_product=list_str_dot_product(f_str,b_str);
  printf("\ndot_product:%s",dot_product);
  free(dot_product);
  list_str* a_copy = list_str_copy(a_str);
  printf("\ncopy:\n");
  list_str_print(a_copy);
  list_str_free(a_copy);
  list_str_free(f_str);
  list_str_free(a_str);
  list_str_free(b_str);
  list_str_free(c_str);
  list_str_free(d_str);
  printf("\n[list_str_test] END\n");

}
static void stack_tests(void){
  printf("\n[stack_tests] START");
  
  int a[10]={0,1,2,3,4,5,6,7,8,9};
  int b[3]={0,1,2};
  int c[4]={0,1,2,3};

  list_str* a_str=list_str_from_int_vector(a,10);
  list_str* b_str=list_str_from_int_vector(b,3);
  list_str* c_str=list_str_from_int_vector(c,4);

  stack* pstack=stack_alloc(3);
  stack_print(pstack);
  
  stack_push(pstack,a_str);
  stack_print(pstack);

  stack_push(pstack,b_str);
  stack_print(pstack);

  stack_push(pstack,c_str);
  stack_print(pstack);

  stack_push(pstack,c_str);
  stack_print(pstack);

  stack_pop(pstack);
  stack_print(pstack);

  stack_pop(pstack);
  stack_print(pstack);

  stack_pop(pstack);
  stack_print(pstack);


  stack_pop(pstack);
  stack_print(pstack);


  stack_free(pstack);
  list_str_free(a_str);
  list_str_free(b_str);
  list_str_free(c_str);
  
  printf("\n[stack tests] END");

}
static void tensor_tests(void){  
  size_t       sizes[3]     = {3,2,5};
  int          rank         = 0 ;
  tensor*      pvisitor     = NULL;
  tensor*      t_ijk        = NULL;
  tensor*      t_ij         = NULL;
  tensor*      t_i          = NULL;
  tensor*      t_0          = NULL;
  int          arr[5]       = {2,1,-1,1,3};
  int*         parr         = NULL;

  printf("\n[tensor_tests] START");
  list_str* coeff = list_str_from_int_vector(arr,5);

  int v_00r[5] = { 1,-1, 1, 2, 3}; 
  int v_01r[5] = { 0,-1, 0, 2, 0}; 
  int v_10r[5] = { 1, 0, 1, 0, 3}; 
  int v_11r[5] = { 1, 1, 1, 2,-1}; 
  int v_20r[5] = { 1,-1, 0, 2, 3}; 
  int v_21r[5] = { 0,-1, 1, 0, 3}; 
  
  list_str* str_00r=list_str_from_int_vector(v_00r, 5);
  list_str* str_01r=list_str_from_int_vector(v_01r, 5);
  list_str* str_10r=list_str_from_int_vector(v_10r, 5);
  list_str* str_11r=list_str_from_int_vector(v_11r, 5);
  list_str* str_20r=list_str_from_int_vector(v_20r, 5);
  list_str* str_21r=list_str_from_int_vector(v_21r, 5);
  
  printf("\n[");
  for(int i=0 ; i < rank ; i++)
          printf(" %zu",sizes[i]);
  printf(" ]");
  t_ijk = tensor_create("ijk","++-",sizes,0,0);

  tensor_fill(t_ijk,"00r",str_00r);
  tensor_fill(t_ijk,"01r",str_01r);
  tensor_fill(t_ijk,"10r",str_10r);
  tensor_fill(t_ijk,"11r",str_11r);
  tensor_fill(t_ijk,"20r",str_20r);
  tensor_fill(t_ijk,"21r",str_21r);
 
  printf("\nijk ++-\n");
  tensor_print_elems(t_ijk);
 
  pvisitor = tensor_access(t_ijk,"00k");
  pvisitor = tensor_access(t_ijk,"0jk");
  
  rank  = 2 ;
  printf("\n[");
  for(int i=0 ; i < rank ; i++)
          printf(" %zu",sizes[i]);
  printf(" ]");
  t_ij = tensor_create("ij","+-",sizes,0,0);
  //tensor_print(t_ij);
  tensor_free(t_ij);
  printf("\n");

  rank  = 1 ;
  printf("\n[");
  for(int i=0 ; i < rank ; i++)
          printf(" %zu",sizes[i]);
  printf(" ]");
  t_i = tensor_create("i","+",sizes,0,0);
  //tensor_print(t_i);
  tensor_free(t_i);
  printf("\n");

  rank  = 0 ;
  printf("\n[");
  for(int i=0 ; i < rank ; i++)
          printf(" %zu",sizes[i]);
  printf(" ]");
  t_0 = tensor_create("","",NULL,1,0);
  //tensor_print_elems(t_0);
  tensor_free(t_0);
  printf("\n");

  printf("\ncopy tensor test:\n");
  tensor* t_ijk_copy = tensor_copy(t_ijk);
  //tensor_print(t_ijk_copy);
  tensor_print_elems(t_ijk_copy);
  tensor_free(t_ijk_copy);

  list_str_free(coeff);
  list_str_free(str_00r);
  list_str_free(str_01r);
  list_str_free(str_10r);
  list_str_free(str_11r);
  list_str_free(str_20r);
  list_str_free(str_21r);
 
  tensor_free(t_ijk);

  printf("\n[tensor_tests] END");
}

static void einsum_tests()
{
  printf("\n[einsum_tests] START");
  size_t sizes[3] = {3,2,5};
  int    arr[5]   = {2,1,-1,1,3};
  tensor* t_ijk   = tensor_create("ijk","+--",sizes,0,0);
  
  list_str* coeff = list_str_from_int_vector(arr,5);

  int v_00r[5] = { 1,-1, 1, 2, 3}; 
  //int v_00r[5] = { 1, 1, 1, 1, 1}; 
  int v_01r[5] = { 0,-1, 0, 2, 0}; 
  //int v_01r[5] = { 1, 1, 1, 1, 1}; 
  int v_10r[5] = { 1, 0, 1, 0, 3}; 
  int v_11r[5] = { 1, 1, 1, 2,-1}; 
  int v_20r[5] = { 1,-1, 0, 2, 3}; 
  int v_21r[5] = { 0,-1, 1, 0, 3}; 
  
  list_str* str_00r=list_str_from_int_vector(v_00r, 5);
  list_str* str_01r=list_str_from_int_vector(v_01r, 5);
  list_str* str_10r=list_str_from_int_vector(v_10r, 5);
  list_str* str_11r=list_str_from_int_vector(v_11r, 5);
  list_str* str_20r=list_str_from_int_vector(v_20r, 5);
  list_str* str_21r=list_str_from_int_vector(v_21r, 5);
 

  tensor_fill(t_ijk,"00r",str_00r);
  tensor_fill(t_ijk,"01r",str_01r);
  tensor_fill(t_ijk,"10r",str_10r);
  tensor_fill(t_ijk,"11r",str_11r);
  tensor_fill(t_ijk,"20r",str_20r);
  tensor_fill(t_ijk,"21r",str_21r);
  
  //tensor_print_elems(t_ijk);
  printf("\nc=t_a x v_a\n");
  size_t sizes_a[1]={5};
  tensor* t_a = tensor_create("a","+",sizes_a,0,0);
  tensor_fill(t_a,"a",str_00r);
  tensor* v_a = tensor_create("a","-",sizes_a,0,0);
  tensor_fill(v_a,"a",str_01r);
  tensor* c_0= tensor_einsum(t_a,v_a);
  tensor_print_elems(c_0);

  tensor_free(c_0);
  tensor_free(t_a);
  tensor_free(v_a);

  printf("\nc_ij = t_ijk x t_k\n");
  size_t sizes_k[1]={5}; 
  tensor* t_k   = tensor_create("k","+",sizes_k,0,0);
  tensor_fill(t_k,"k",coeff);
  tensor* c_ij=tensor_einsum(t_ijk,t_k);
  tensor_print_elems(c_ij);
  tensor_free(t_k);
  tensor_free(c_ij);
  
  printf("\nc_ik = t_ijk x t_j\n");
  size_t sizes_j[1]={2};
  tensor* t_j = tensor_create("j","+",sizes_j,0,0);
  tensor_fill(t_j,"j",coeff);
  tensor* c_ik = tensor_einsum(t_ijk,t_j);
  tensor_print_elems(c_ik);
  tensor_free(t_j);
  tensor_free(c_ik);
  
  printf("\nc_jk = t_ijk x t_i\n");
  size_t sizes_i[1]={3};
  tensor* t_i = tensor_create("i","-",sizes_i,0,0);
  tensor_fill(t_i,"i",coeff);
  tensor* c_jk = tensor_einsum(t_ijk,t_i);
  tensor_print_elems(c_jk);
  tensor_free(t_i);
  tensor_free(c_jk);

  printf("\nc_i = t_ijk x t_jk");
  size_t sizes_jk[2]={2,5};
  tensor* t_jk = tensor_create("jk","++",sizes_jk,0,0);
  tensor_fill(t_jk,"0k",coeff);
  tensor_fill(t_jk,"1k",coeff);
  tensor* c_i = tensor_einsum(t_ijk,t_jk);
  tensor_print_elems(c_i);
  tensor_free(t_jk);
  tensor_free(c_i);

  list_str_free(coeff);
  list_str_free(str_00r);
  list_str_free(str_01r);
  list_str_free(str_10r);
  list_str_free(str_11r);
  list_str_free(str_20r);
  list_str_free(str_21r);
  tensor_free(t_ijk);
  printf("\n[einsum_tests] END");
}

static void constraint_tests()
{
    printf("\n[constraint_test] START");
    int ndimensions  = 2 ;
    int nstatements  = 2 ;
    int nvariables   = 2 ; 
    int nparameters  = 0 ;
    tensor* gamma    = NULL;
    tensor* lambda   = NULL;

    gamma = tensor_create_from_params("dsr","+--",ndimensions,nstatements,nvariables,nparameters);
    tensor* visitor = tensor_access(gamma,"00r");
    size_t  numcoeff = visitor->size;
    printf("\n[constraint_test] num_coeff:%zu",numcoeff);
    tensor_print_elems(gamma);
    size_t sizes[3]={2,2,7};
    lambda = tensor_create("dsr","-++",sizes,1,0);

    visitor = tensor_access(lambda,"00r");
    list_str* elems = visitor->p;
    elems=list_str_update(elems,3,"1"); 
    tensor_print_elems(lambda);
    tensor* t       = tensor_einsum(gamma,lambda);
    tensor_print_elems(t);
    tensor_free(t);
    
    tensor_free(lambda);
    
    size_t sizes_1[4]={2,2,2,7};
    lambda = tensor_create("dskr","-+-+",sizes_1,1,0);
    elems = tensor_access(lambda,"000r")->p;
    elems=list_str_update(elems,3,"1");
    
    elems = tensor_access(lambda,"001r")->p;
    elems =list_str_update(elems,4,"2");

    tensor_print_elems(lambda);
    t  = tensor_einsum(gamma,lambda);
    tensor_print_elems(t);
    tensor_free(t);    
    tensor_free(lambda);
    tensor_free(gamma);

    printf("\n[constraint_test] END");
}
static void int_tensor_tests(){

    int    values_00k[10] = {0,1,2,3,4,5,6,7,8,9};
    int    values_01k[10] = {10,11,12,13,14,15,16,17,18,19};
    int    values_10k[10] = {20,21,22,23,24,25,26,27,28,29};
    int    values_11k[10] = {30,31,32,33,34,35,36,37,38,39};
    int    values_s[10]   = {0,0,0,0,1,0,0,0,0,0};
    
    size_t sizes_t_ijk[3]={2,2,10};

    printf("\n[int_tensor_tests] START\n");

    tensor* t_ijk = tensor_create("ijk","+--",sizes_t_ijk,1,1);
    tensor* s_ijk = tensor_create("ijk","-++",sizes_t_ijk,1,1);

    tensor_fill_int(t_ijk,"00k",values_00k);
    tensor_fill_int(t_ijk,"01k",values_01k);
    tensor_fill_int(t_ijk,"10k",values_10k);
    tensor_fill_int(t_ijk,"11k",values_11k);

    tensor_print_elems(t_ijk);
    tensor_fill_int(s_ijk,"00k",values_s);
    tensor* r  = tensor_einsum(t_ijk,s_ijk);
    tensor_print_elems(r);

    tensor_free(r);
    tensor_free(t_ijk);
    tensor_free(s_ijk);
    
    printf("\n");

    printf("\n[int_tensor_tests] END.");
}
static void double_tensor_tests(){

    double    values_00k[10] = {0.,1.,2.,3.,4.,5.,6.,7.,8.,9.};
    double    values_01k[10] = {10.,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9};
    double    values_10k[10] = {2.0,2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9};
    double    values_11k[10] = {3.0,3.1,3.2,3.3,3.4,3.5,3.6,3.7,3.8,3.9};
    double    values_s[10]   = {0,0,0,0,1,0,0,0,0,0};
    
    size_t sizes_t_ijk[3]={2,2,10};

    printf("\n[double_tensor_tests] START\n");

    tensor* t_ijk = tensor_create("ijk","+--",sizes_t_ijk,1,2);
    tensor* s_ijk = tensor_create("ijk","-++",sizes_t_ijk,1,2);

    tensor_fill_double(t_ijk,"00k",values_00k);
    tensor_fill_double(t_ijk,"01k",values_01k);
    tensor_fill_double(t_ijk,"10k",values_10k);
    tensor_fill_double(t_ijk,"11k",values_11k);

    tensor_print_elems(t_ijk);
    tensor_fill_double(s_ijk,"00k",values_s);
    tensor* r  = tensor_einsum(t_ijk,s_ijk);
    tensor_print_elems(r);

    tensor_free(r);
    tensor_free(t_ijk);
    tensor_free(s_ijk);
    
    printf("\n");

    printf("\n[double_tensor_tests] END.");
}


void main(void){

 printf("\nTensor C library tests.\n");
 list_str_tests();
 stack_tests();
 tensor_tests();
 einsum_tests();
 constraint_tests();
 int_tensor_tests();
 double_tensor_tests();
 printf("\n");
}

