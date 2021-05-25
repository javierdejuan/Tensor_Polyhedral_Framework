#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "tensor.h"

/* list_str function */
list_str* list_str_alloc(size_t elems){
  list_str* plist_str = malloc(sizeof(list_str));
  plist_str->elems    = malloc(sizeof(char*)*elems);
  plist_str->size     = elems;
  plist_str->index    = 0;
  return plist_str;
}
list_str* list_str_free(list_str* plist_str){
  if(plist_str){
    for(int i = 0 ; i < plist_str->size ; i++){
        if(plist_str->elems[i]){
            free(plist_str->elems[i]);
            plist_str->elems[i]=NULL;
        }
    }
    if(plist_str->elems){
        free(plist_str->elems);
        plist_str->elems=NULL;
    }
    free(plist_str);
    plist_str=NULL;
  }
  return NULL;
}
char* list_str_access(list_str* plist_str,int elem){
    if(elem >= plist_str->index ){
            printf("\n[list_str_access] invalid access");
            return NULL;
    }
    else
      return plist_str->elems[elem];
}
size_t list_str_len(list_str* plist_str){
    return plist_str->index ; 
}
void list_str_add(list_str* plist_str,char* elem){
    if(plist_str->size > list_str_len(plist_str) )
        plist_str->elems[plist_str->index++]=elem;
    else
        printf("\n[list_str_add] not enough memory [size:%zu len:%zu plist_str->index:%d]\n",plist_str->size,list_str_len(plist_str),plist_str->index);
}
list_str* list_str_from_int_vector(size_t* vector,size_t size){

    list_str* str = list_str_alloc(size);
    char buff[4];
    for(int i=0 ; i < size ; i++){
            sprintf(buff,"%zu",vector[i]);
            list_str_add(str,strdup(buff));
            
    }
    list_str_print(str);
    return str;
}
list_str* list_str_create_sequence(size_t size){

    char buff[4];
    list_str* str = list_str_alloc(size);
    for(int i=0 ; i < size ; i++){
            sprintf(buff,"%d",i);
            list_str_add(str,strdup(buff));
    }
    printf("\n[list_create_sequence]:\n");
    list_str_print(str);
    printf("\n");
    return str;
}
void list_str_print(list_str* plist_str)
{
    size_t len = list_str_len(plist_str);
    printf("[");
    for(int i=0 ; i < len ; i++)
            printf(" %s",plist_str->elems[i]);
    printf(" ]");
}

list_str* list_str_product(list_str* a,list_str* b)
{
    printf("[list_str_product] a->size:%zu b->size:%zu",a->size,b->size);
    list_str* p_ab=list_str_alloc(a->size*b->size);

    for( int i = 0 ; i < a->size;i++)
      for( int j = 0 ; j < b->size; j++){
        char buff[256];
        sprintf(buff,"%s%s",list_str_access(a,i),list_str_access(b,j));
        list_str_add(p_ab,strdup(buff));
    }

    return p_ab;
}
list_str* list_str_permutations(int level, size_t* sizes){

    list_str* permutations = NULL;
    int       count = 0 ; 
    int       nperm = 1; 
    stack*    pstack   = stack_alloc(level+1);
    stack*    pgarbage = stack_alloc(level+1);

    while(count <= level)
    {
      nperm*=sizes[count];
      stack_push(pstack,list_str_create_sequence(sizes[count]));
      count++;
    }
    permutations = stack_pop(pstack);
    while(stack_len(pstack))
    {
        stack_push(pgarbage,permutations);
        permutations=list_str_product(permutations,stack_pop(pstack));
    }
    printf("\n[list_str_permutations] level:%d  nperm=%d pstack->len:%d",level,nperm,stack_len(pstack));
   
    if(stack_len(pgarbage)){
      for(int i = 0 ; i < pgarbage->size ; i++){
        list_str_free(pgarbage->elems[i]);
      }
      stack_free(pgarbage);
    }
   for(int i = 0 ; i < pstack->size ; i++)
            list_str_free(pstack->elems[i]);
    stack_free(pstack);

    return permutations;
}
char* list_str_to_char_ptr(list_str* plist_str)
{
    size_t size=0;
    
    
    for(int i=0;i<plist_str->size;i++)
            size+=strlen(plist_str->elems[i]);

    size++;
    char* str=(char*)malloc(sizeof(char)*size);

    strcpy(str,"");

    for( int i=0;i<plist_str->size;i++)
            strcat(str,plist_str->elems[i]);


    return str;
}
list_str* list_str_from_char_ptr(char* str)
{
    size_t len=strlen(str);
    list_str* plist_str = list_str_alloc(len);

    for(int i=0;i<len;i++){
            char buff[2];
            sprintf(buff,"%c",str[i]);
            list_str_add(plist_str,strdup(buff));
    }
    list_str_print(plist_str);

    return plist_str;
}
/* stack functions */
stack* stack_alloc(int elems){
    stack* pstack = (stack*)malloc(sizeof(stack));
    pstack->elems = (list_str**)malloc(sizeof(list_str*)*elems);
    pstack->ptr   = - 1 ;
    pstack->size  = elems;
    return pstack;
}
void stack_free(stack* pstack){
    free(pstack->elems);
    free(pstack);
}
int stack_len(stack* pstack){
    return pstack->ptr + 1;
}
list_str* stack_pop(stack* pstack){
    if(stack_len(pstack)){
        return pstack->elems[pstack->ptr--];
    }
    else{
       printf("\n[stack_pop] can't pop! stack is empty.");
       return NULL;

    }
}
void stack_push(stack* pstack,list_str* elem){
    if(pstack->size > stack_len(pstack) )
        pstack->elems[++pstack->ptr] = elem;
    else
        printf("\n[stack_push] can´t push! not enough memory.");
}
void stack_print(stack* pstack){
    int max_width = 0;
    for( int i = 0 ; i <= pstack->ptr ; i++)
            if(pstack->elems[i]->size > max_width)
                    max_width = pstack->elems[i]->size;
    int white_spaces = max_width*2+3;

    printf("\n[stack_print] size:%d len:%d stack->ptr:%d\n",pstack->size,stack_len(pstack),pstack->ptr);
    for( int i = 0 ; i < pstack->size ; i++){
            if(pstack->ptr == i){

                    printf("-->[");
                    if(stack_len(pstack ) > i ){
                            list_str_print(pstack->elems[i]);
                            int extra = white_spaces - (pstack->elems[i]->size*2+3);
                            if(extra)  printf("%*c",extra,' ');
                    }
                    else
                            printf("%*c",white_spaces,' ');
                    printf("]\n");
            }
            else{
                    printf("   [");
                    if(stack_len(pstack) > i ){
                            list_str_print(pstack->elems[i]);
                            int extra = white_spaces - (pstack->elems[i]->size*2+3);
                            if(extra)  printf("%*c",extra,' ');
                    }
                    else
                            printf("%*c",white_spaces,' ');
                    printf("]\n");
            }
    }

}
/* tensor functions */
tensor* tensor_access(tensor* ptensor,char* str)
{
    tensor* pfound=ptensor;
    int accesslength=strlen(str);
    char buff[2];
    char log[32];
    sprintf(log,"visitor:%s",str);
    
    if(accesslength > strlen(ptensor->id)){
        printf("\n[%s] Bad request: to many indeces",log);
        return NULL;
    }

    int visitor = 0;
    while(visitor < accesslength){
        sprintf(buff,"%c",str[visitor]);
        int id=atoi(buff);
        if(id > pfound->size - 1){
            printf("\n[%s] Bad request:%d > %zu ",log,id,pfound->size - 1);
            return NULL;
         }
        if(!pfound->isleave)
            pfound = pfound->t[id];
        else
            break;
        visitor++;
        }
    tensor_print_info(pfound,log);
    return pfound;
}
void tensor_print_info(tensor* ptensor,char* msg)
{
    if(ptensor->size){
            printf("\n\n[%s] Index\t:%s",msg,ptensor->index);
            printf("\n[%s] Size\t:%zu",msg,ptensor->size);
            printf("\n[%s] Id\t:%s",msg,ptensor->id);
            printf("\n[%s] IsLeave\t:%d",msg,ptensor->isleave);
    }
}
void tensor_print(tensor* ptensor)
{
    if(ptensor){
            tensor_print_info(ptensor,"print");       
            for(int i=0 ; i < ptensor->size ; i++){
                   if(ptensor->isleave)
                        return;
                   else
                        tensor_print(ptensor->t[i]);
            }
    }
}

tensor** tensor_get_leaves(tensor* ptensor,size_t* sizes,int indeces){
    tensor** res;
    return res;
}
tensor** tensor_get_nodes(tensor* ptensor,size_t* sizes,int indeces){
    tensor** res;
    return res; 
}
size_t   tensor_get_n_leaves(size_t* sizes,int indeces){
    size_t leaves=1;
     for(int i = 0 ; i < indeces - 1 ; i++)
            leaves*=sizes[i];
    return leaves;  
}
size_t   tensor_get_n_elems(size_t* sizes,int indeces){
    size_t elems=1;
     for(int i = 0 ; i < indeces ; i++)
            elems*=sizes[i];
    return elems;  
}
int tensor_get_n_indeces(tensor* ptensor){
    return strlen(ptensor->id);
}
size_t* tensor_get_sizes(tensor* ptensor)
{   
    int indeces=tensor_get_n_indeces(ptensor);
    size_t* sizes=malloc(sizeof(size_t)*indeces);
    int index=0;
    tensor* pvisitor=ptensor;
    while(index < indeces){
            sizes[index]=pvisitor->size;
            if(!pvisitor->isleave)
                pvisitor=pvisitor->t[0];
            index++;
    }
    for(int i=0;i<indeces;i++)printf("\n[tensor_get_sizes] size[%d]=%zu",i,sizes[i]);
    return sizes;
 
}
size_t** tensor_get_access(tensor* ptensor,size_t* sizes,int indeces)
{
   size_t** access = (size_t**) calloc(sizeof(size_t*),indeces);
   
   printf("\n\n[tensor_get_acess]:\n");
   
   for(int i=0 ; i < indeces ; i++){
           access[i] = (size_t*) calloc(sizeof(size_t),sizes[i]);
           printf("\taccess[%d]:[",i);
           for(size_t j=0 ; j < sizes[i] ; j++){
                   printf(" %zu",j);
                   access[i][j]=j;
           }
           printf(" ]\n");
   }
   return access;
}

char** tensor_get_access_leaves(size_t** access, size_t* sizes, int indeces){
 
    size_t  nleaves=tensor_get_n_leaves(sizes,indeces);
    char** access_leaves=(char**)malloc(sizeof(char*)*nleaves);

    size_t count=0;
    int    index=0;
    stack* pstack=stack_alloc(indeces-1);
    
    for(int i=0; i < indeces - 1 ; i++){
            list_str* elem=list_str_from_int_vector(access[i],sizes[i]);
            stack_push(pstack,elem);
            stack_print(pstack);
    }

    stack_free(pstack);
    return access_leaves;
}
tensor* tensor_create(list_str* indeces_names,size_t* sizes){

   int indeces = indeces_names->size;
   tensor* ptensor;
   /*
    T(i,j,k,l):
        -i   --> Nodes
        -ij  --> Nodes
        -ijk --> Leaves

        dim(perm) = 3
        dim(perm[0]) = sizes[0]
        dim(perm[1]) = sizes[0] * sizes[1]
        dim(perm[2]) = sizes[0] * sizes[1] * sizes[2]
      
     T(i,j):
        -i   --> Nodes
        -ij  --> Leaves

        dim(perm) = 2
        dim(perm[0]) = sizes[0]
        dim(perm[1]) = sizes[0] * sizes[1]
   
     T(i):
        -i   --> Leaves

        dim(perm) = 1
        dim(perm[0]) = sizes[0]
    
        list_str** perm = (list_str**) malloc(sizeof(list_str*)*indeces);

        for( int i = 0 ; i < indeces ; i++)
            perm[i] = list_str_permutations(i,sizes[i]);

        list_str* list_str_permutations(int i, size_t* sizes){
            list_str* permutations;
            return permuttaions;
        }

    */
 //  list_str** perm = (list_str**) malloc(sizeof(list_str*)*indeces);

  list_str_free(list_str_permutations(0,sizes));

/*   for( int i = 0 ; i < indeces ; i++)
     perm[i] = list_str_permutations(i,sizes);

   for( int i = 0 ; i < indeces ; i++)
   {
        list_str_free(perm[i]);
   }
   free(perm);
*/
   return ptensor;
}

void tensor_free(tensor* ptensor)
{
   int      indeces  = tensor_get_n_indeces(ptensor);
   size_t*  sizes    = tensor_get_sizes(ptensor);
   size_t** access   = tensor_get_access(ptensor,sizes,indeces);
   list_str** str_access=(list_str**)malloc(sizeof(list_str*)*indeces);
   stack*  pstack = stack_alloc(indeces);

   for(int i=0; i < indeces; i++){
           str_access[i]=list_str_from_int_vector(access[i],sizes[i]);
           stack_push(pstack,str_access[i]);
   }
   
   stack_print(pstack);
   stack_pop(pstack);
   stack_print(pstack);
   
   list_str* product=stack_pop(pstack);
   list_str* term1;
   list_str* garbage;

   while(stack_len(pstack)){
           term1=stack_pop(pstack);
           garbage=product;
           product=list_str_product(term1,product);
           list_str_print(product);
           list_str_free(garbage);
   }

   for( int i = 0 ; i < product->size ; i++){

            tensor* pfree = tensor_access(ptensor,product->elems[i]);
            tensor_free_node(pfree);
            pfree->size = 0;
            free(pfree);
            pfree=NULL;
   }

   for( int i = 0 ; i < str_access[0]->size ; i++){
           
            tensor* pfree = tensor_access(ptensor,str_access[0]->elems[i]);
            tensor_free_node(pfree);
            pfree->size = 0;
            free(pfree);
            pfree=NULL;
 
   }

   list_str_free(product);
   list_str_free(str_access[indeces-1]);
   list_str_free(str_access[0]);
   stack_free(pstack);
   for(int i=0 ; i < indeces; i++){
           free(access[i]);
   }
   free(str_access);
   free(access);
   free(sizes);

   tensor_free_node(ptensor);
   free(ptensor);
   ptensor=NULL;
}
void tensor_free_node(tensor* ptensor){
        free(ptensor->index);
        free(ptensor->id);
        if(ptensor->isleave)
                free(ptensor->p);
        else
                free(ptensor->t);
}
tensor* tensor_alloc(tensor* ptensor,char* index,size_t size,int isleave,char* id)
{
        ptensor   =(tensor*)malloc(sizeof(tensor));
        if(!isleave)
            ptensor->t=(tensor**)malloc(sizeof(tensor*)*size);
        else
           ptensor->p=(int*)calloc(sizeof(int),size);
        ptensor->size=size;
        ptensor->isleave=isleave;
        ptensor->index=strdup(index);
        ptensor->id=strdup(id);
        return ptensor;
}
/* unit tests */
static void list_str_tests(void){

  printf("\n[list_str_tests]\n");
  size_t a[10]={0,1,2,3,4,5,6,7,8,9};
  size_t b[3]={0,1,2};
  size_t c[4]={0,1,2,3};

  list_str* a_str=list_str_from_int_vector(a,10);
  list_str* b_str=list_str_from_int_vector(b,3);
  list_str* c_str=list_str_from_int_vector(c,4);

  for(int i= 0; i < 4 ;i++ ){
          printf("\n%s",list_str_access(a_str,i));
          printf("\n%s",list_str_access(b_str,i));
          printf("\n%s",list_str_access(c_str,i));
  }


  list_str* d_str=list_str_create_sequence(4);
  
  list_str_free(a_str);
  list_str_free(b_str);
  list_str_free(c_str);
  list_str_free(d_str);

  char str1[4];

  strcpy(str1,"sdr");

  list_str* e_str=list_str_from_char_ptr(str1);
  char* str2=list_str_to_char_ptr(e_str);
  printf("\nfrom list_str:%s",str2);
  list_str_free(e_str);
  free(str2);

}
static void stack_tests(void){
  printf("\n[stack_tests]");
  
  size_t a[10]={0,1,2,3,4,5,6,7,8,9};
  size_t b[3]={0,1,2};
  size_t c[4]={0,1,2,3};

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


}
static void combinations_tests(void){
  printf("\n[combinations tests]");
  size_t a[3]={0,1,2};
  size_t b[2]={0,1};
  size_t c[4]={0,1,2,3};

  list_str* a_str=list_str_from_int_vector(a,3);
  list_str* b_str=list_str_from_int_vector(b,2);
  list_str* c_str=list_str_from_int_vector(c,4);

  stack* pstack=stack_alloc(3);
  stack_print(pstack);
  
  stack_push(pstack,a_str);
  stack_print(pstack);

  stack_push(pstack,b_str);
  stack_print(pstack);

  stack_push(pstack,c_str);
  stack_print(pstack);



  list_str* term2;
  list_str* aux;
  list_str* pr_str=stack_pop(pstack);

  while(stack_len(pstack)){
      term2   = stack_pop(pstack);
      aux     = pr_str;
      pr_str  = list_str_product(pr_str,term2);
      aux     = list_str_free(aux);
      printf("\n");
      list_str_print(pr_str);

      stack_print(pstack);

  }

  stack_free(pstack);
  list_str_free(pr_str);
  list_str_free(a_str);
  list_str_free(b_str);


}
static char* create_recursive(int cur, size_t* size, int indeces,char* names){

    for(int i=0 ;i < size[cur] && cur < indeces - 1 ; i++) {
        char str[256];
        strcpy(str,names);
        str[cur]=i+'0';
        int is_node = cur + 1 != indeces - 1 ; 
        char index = is_node ? names[cur + 1] : names[indeces-1];
        printf("\nindex:%c id:%s is node:%d",index,str,is_node);
        if(is_node) {
          // allocate memory for node 
          // populate     
          create_recursive(cur+1,size,indeces,str);
        }
        else{
          // allocate memory for leaves        
        }
    }
    return names;
} 
static void tensor_tests(void){  
  char**    indeces ;
  size_t    sizes[3]     = {3,2,5};
  unsigned  rank         = 3 ;
  tensor*   ptensor      = NULL;

  indeces    = (char**) malloc(sizeof(char*)*3);
  indeces[0] = (char*)  malloc(sizeof(char )*8);
  indeces[1] = (char*)  malloc(sizeof(char )*8);
  indeces[2] = (char*)  malloc(sizeof(char )*8);

  strcpy(indeces[0],"d");
  strcpy(indeces[1],"s");
  strcpy(indeces[2],"r");

  char buff[8];
  printf("\n[tensor_tests]");

  ptensor=tensor_alloc(ptensor,indeces[0],sizes[0],0,"dsr");
  for(int i=0;i<sizes[0];i++){
          sprintf(buff,"%dsr",i);
          ptensor->t[i]=tensor_alloc(ptensor->t[i],indeces[1],sizes[1],0,buff);
          for(int j=0;j<sizes[1];j++){
                  sprintf(buff,"%d%dr",i,j);
                  ptensor->t[i]->t[j]=tensor_alloc(ptensor->t[i]->t[j],indeces[2],sizes[2],1,buff);
          }
  }
  
  tensor_print(ptensor);
  tensor* pvisitor;
  pvisitor=tensor_access(ptensor,"000");
  pvisitor=tensor_access(ptensor,"01");
  pvisitor=tensor_access(ptensor,"0400");
  pvisitor=tensor_access(ptensor,"34");
  tensor_free(ptensor);
  ptensor = NULL;
  free(indeces[0]);
  free(indeces[1]);
  free(indeces[2]);
  free(indeces);
  
  int indices = 3 ;
  char* ret; 
  printf("\n[");
  for(int i=0 ; i < indices ; i++)
          printf(" %zu",sizes[i]);
  printf(" ]");
  ret = create_recursive(0,sizes,indices,"ijk");
  printf("\nret:%s",ret);

  indices = 2 ;
  printf("\n[");
  for(int i=0 ; i < indices ; i++)
          printf(" %zu",sizes[i]);
  printf(" ]");
  ret = create_recursive(0,sizes,indices,"ij");
  printf("\n");
  printf("\nret:%s",ret);

  indices = 1 ;
  printf("\n[");
  for(int i=0 ; i < indices ; i++)
          printf(" %zu",sizes[i]);
  printf(" ]");
  ret = create_recursive(0,sizes,indices,"i");
  printf("\n");
  printf("\nret:%s",ret);


}

void main(void){

  printf("\nTensor C library.\n");

  list_str_tests();
  stack_tests();
  combinations_tests();
  tensor_tests();
  printf("\n");

}

