#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "tensor.h"
#include "macro.h"
/* string manipulation */
static int is_str_digit(char* str)
{
    int len = strlen(str);

    for(int i=0; i<len; i++)
    {
        if(str[i] <'0' || str[i] >'9')
            return 0;

    }
    return 1;

}
char* findUnion(char* str1, char* str2){

    int f, p = 0;
    int i, j;

    char* str3 = (char*) calloc(sizeof(char),100);
    int len1 = strlen(str1);
    int len2 = strlen(str2);

    while (str1[p] != '\0') {
        str3[p] = str1[p];
        p++;
    }

    for (i = 0; i < len2; i++) {
        f = 0;
        for (j = 0; j < len1; j++) {
            if (str2[i] == str1[j]) {
                f = 1;
            }
        }
        if (f == 0) {
            str3[p] = str2[i];
            p++;
        }
    }
    str3[p] = '\0';
    LOG printf("\n[findUnion] union(%s,%s)=%s", str1,str2,str3);
    return str3;
}
char* findIntersection(char* str1, char* str2) {

    int i, j, k = 0;
    char* str3 = (char*) calloc(sizeof(char),100);

    int len1 = strlen(str1);
    int len2 = strlen(str2);

    for (i = 0; i < len1; i++) {
        for (j = 0; j < len2; j++) {
            if (str1[i] == str2[j]) {
                str3[k] = str1[i];
                k++;
            }
        }
    }
    str3[k] = '\0';

    LOG printf("\n[findIntersection] intersection(%s,%s)=%s", str1,str2,str3);
    return str3;
}
char* findXOR(char* str1, char* str2){

    char* str3 = (char*) calloc(sizeof(char),100);
    strcpy(str3,"");
    
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    int k = 0;
    int found = 0;

    for(int i = 0; i < len1 ; i++){
        for( int j = 0; j < len2 ; j++){
           if(str1[i] == str2[j]) found = 1;
	}
	if(!found) str3[k++] = str1[i];
	found = 0 ;
    }

    str3[k]='\0';

    LOG printf("\n[findXOR] XOR(%s,%s)=%s", str1,str2,str3);
    return str3;
}
/* list_str function */
list_str* list_str_alloc(size_t elems){
  list_str* plist_str = malloc(sizeof(list_str));
  plist_str->elems    = malloc(sizeof(char*)*elems);
  plist_str->size     = elems;
  plist_str->index    = 0;
  return plist_str;
}
list_str* list_str_copy(list_str* source){
  if(!source) return NULL;
  list_str* plist_str = list_str_alloc(source->size);
  for(int i=0 ; i < source->size ; i++)
    plist_str=list_str_add(plist_str,strdup(source->elems[i]));
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
    if(!plist_str){
	    LOG printf("\n[list_str_access] NULL input pointer.");
	    return NULL;
    };
    if(elem >= plist_str->index ){
            LOG printf("\n[list_str_access] invalid access");
            return NULL;
    }
    else
      return plist_str->elems[elem];
}
list_str* list_str_update(list_str* plist_str,int id, char* elem){
    if(id<plist_str->index||(!id&&!plist_str->index)){
	    free(plist_str->elems[id]);
	    plist_str->elems[id]=strdup(elem);
    }
    else
         printf("\n[list_str_update] invalid index.");

    return plist_str;
}
size_t list_str_len(list_str* plist_str){
    return plist_str->index ; 
}
list_str* list_str_add(list_str* plist_str,char* elem){
    if(plist_str->size > list_str_len(plist_str) )
        plist_str->elems[plist_str->index++]=elem;
    else
        LOG printf("\n[list_str_add] not enough memory [size:%zu len:%zu plist_str->index:%d]\n",plist_str->size,list_str_len(plist_str),plist_str->index);
    return plist_str;
}
list_str* list_str_from_int_vector(int* vector,size_t size){

    list_str* str = list_str_alloc(size);
    char buff[256];
    for(int i=0 ; i < size ; i++){
	    if(vector[i]>=0)
            	sprintf(buff,"%d",vector[i]);
	    else
            	sprintf(buff,"%d",vector[i]);
            list_str_add(str,strdup(buff));
            
    }
    //list_str_print(str);
    return str;
}
list_str* list_str_from_double_vector(double* vector,size_t size){

    list_str* str = list_str_alloc(size);
    char buff[256];
    for(int i=0 ; i < size ; i++){
            sprintf(buff,"%g",vector[i]);
            list_str_add(str,strdup(buff));
            
    }
    //list_str_print(str);
    return str;
}

list_str* list_str_create_sequence(size_t size){

    char buff[4];
    list_str* str = list_str_alloc(size);
    for(int i=0 ; i < size ; i++){
            sprintf(buff,"%d",i);
            list_str_add(str,strdup(buff));
    }
    LOG printf("\n[list_create_sequence]:\n");
    list_str_print(str);
    LOG printf("\n");
    return str;
}
void list_str_print(list_str* plist_str)
{
    size_t len = list_str_len(plist_str);
    printf("[");
    for(int i=0 ; i < len ; i++){
             if(!plist_str->elems[i][0]=='-')
                printf("%s",plist_str->elems[i]);
             else
                printf(" %s",plist_str->elems[i]);
    }
    printf(" ]");
}
void list_str_print_column(list_str* plist_str)
{
    size_t len = list_str_len(plist_str);
    printf("[");
    for(int i=0 ; i < len ; i++)
             printf(" %s\n",plist_str->elems[i]);
    printf(" ]");
}


list_str* list_str_product(list_str* a,list_str* b)
{
    LOG printf("[list_str_product] a->size:%zu b->size:%zu",a->size,b->size);
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

    if(level==0)
        return list_str_create_sequence(sizes[0]);
    list_str* permutations = NULL;
    int       count = level ; 
    int       nperm = 1; 
    stack*    pstack   = stack_alloc(level+1);
    stack*    pgarbage = stack_alloc((level+1)*2);

    while(count >= 0)
    {
      nperm*=sizes[count];
      stack_push(pstack,list_str_create_sequence(sizes[count]));
      count--;
    }
    //stack_print(pstack);
    permutations = stack_pop(pstack);
    list_str* aux=NULL;
    while(stack_len(pstack))
    {
        stack_push(pgarbage,permutations);
        aux = stack_pop(pstack);
        permutations=list_str_product(permutations,aux);
        stack_push(pgarbage,aux);
    }
    LOG printf("\n[list_str_permutations] level:%d  nperm=%d pstack->len:%d",level,nperm,stack_len(pstack));
  
    //stack_print(pgarbage); 
    if(stack_len(pgarbage)){
      for(int i = 0 ; i < stack_len(pgarbage) ; i++){
        list_str_free(pgarbage->elems[i]);
      }
      stack_free(pgarbage);
    }
   for(int i = 0 ; i < stack_len(pstack) ; i++)
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
    list_str* plist_str = list_str_alloc(1);
    list_str_add(plist_str,strdup(str));

    return plist_str;
}
char* list_str_dot_product(list_str* a, list_str*b){

    if(!a||!b){

      LOG printf("\n[list_str_dot_product] null pointers received.");
      return NULL;      
    }

    if(a->size!=b->size){

      LOG printf("\n[list_str_dot_product] different vector sizes.");
      return NULL;
    }

    char buff[256];
    int sign = 0;

    char* dot_product = (char*)calloc(sizeof(char),(a->size+b->size)*2);
    strcpy(dot_product,"");

    for(int i=0 ; i< a->size ; i++){
 
      char* term1 = a->elems[i];
      char* term2 = b->elems[i];
      int n1 ;
      int n2 ;

      int term1_is_digit = sscanf(term1,"%d",&n1);
      int term2_is_digit = sscanf(term2,"%d",&n2);


      if(    term1_is_digit > 0 && term2_is_digit > 0){
      
         int n  = n1*n2;
         sign = n >= 0 ? 1 : -1 ;
         if(n==0)
            strcpy(buff,"");
         else
            sprintf(buff,"%d",abs(n));
    
      }
      else if(term1_is_digit == 0 && term2_is_digit > 0){

         sign = n2 >= 0 ? 1 : -1 ;
         if(n2==0)
           strcpy(buff,"");
         else if(abs(n2)==1)
           sprintf(buff,"%s",term1);
         else
           sprintf(buff,"%d%s",abs(n2),term1);
	 
      }
      else if(term1_is_digit > 0 && term2_is_digit == 0){

         sign = n1 >= 0 ? 1 : -1 ;
         if(n1==0)
           strcpy(buff,"");
         else if(abs(n1)==1)
           sprintf(buff,"%s",term2);
         else
           sprintf(buff,"%d%s",abs(n1),term2);

      }
      else if(term1_is_digit == 0 && term2_is_digit == 0){

         sign = 1 ;
         sprintf(buff,"%s%s",term1,term2);

      }
      if(strlen(buff)==0) continue;
      if(strlen(dot_product)>0){
          
          term1_is_digit = sscanf(dot_product,"%d",&n1);
          term2_is_digit = sscanf(buff,"%d",&n2);

          if(term1_is_digit > 0 && term2_is_digit > 0){
              int result = n1 + n2 * sign;
              sprintf(dot_product,"%d",result);
          }else{

    	      if(sign>=0) { strcat(dot_product," + "); strcat(dot_product,buff);}
	          else        { strcat(dot_product," - "); strcat(dot_product,buff);}
          }
      }
      else{

    	  if(sign>=0) strcpy(dot_product,buff);
	      else        { strcpy(dot_product,"-"); strcat(dot_product,buff);}

      }
      LOG printf("\n[list_str_dot_product] dot_product:%s",dot_product);  
    }
  
    return dot_product;
}
list_str* list_str_substract(list_str* a, list_str* b){

   if(!a||!b){
      LOG printf("\n[list_str_dot_product] null pointers received.");
      return NULL;      
   }

   if(a->size!=b->size){
      LOG printf("\n[list_str_dot_product] different vector sizes.");
      return NULL;
   }

   list_str* res = list_str_alloc(a->size);

   char buff[256];
   strcpy(buff,"");

   for(int i=0;i<a->size;i++){

      char* term1 = a->elems[i];
      char* term2 = b->elems[i];
      int n1 ;
      int n2 ;
      int term1_is_digit = sscanf(term1,"%d",&n1);
      int term2_is_digit = sscanf(term2,"%d",&n2);

      if(    term1_is_digit > 0 && term2_is_digit > 0){
      
	int n  = n1-n2;
	if(n==0)
	  strcpy(buff,"");
	else
	  sprintf(buff,"%d",n);
    
      }
      else if(term1_is_digit == 0 && term2_is_digit > 0){

	 if(n2==0)
	   sprintf(buff,"%s",term1);
	 else
	   sprintf(buff,"%s%+d",term1,-n2);
	 
      }
      else if(term1_is_digit > 0 && term2_is_digit == 0){

	   sprintf(buff,"%+d-%s",n1,term2);

      }
      else if(term1_is_digit == 0 && term2_is_digit == 0){

	 sprintf(buff,"%s-%s",term1,term2);

      }
     strcat(buff,"=0");
     list_str_add(res,strdup(buff));
   }
   
   return res;
}
list_str* list_str_sum(list_str* a, list_str* b){

   if(!a||!b){
      printf("\n[list_str_sum] null pointers received.");
      return NULL;      
   }

   if(a->size!=b->size){
      printf("\n[list_str_sum] different vector sizes.");
      return NULL;
   }

   list_str* res = list_str_alloc(a->size);

   char buff[256];
   strcpy(buff,"");

   for(int i=0;i<a->size;i++){

      char* term1 = a->elems[i];
      char* term2 = b->elems[i];

      int n1 = 0; sscanf(term1,"%d",&n1);
      int n2 = 0; sscanf(term2,"%d",&n2);

      
      int term1_is_digit = is_str_digit(term1);
      int term2_is_digit = is_str_digit(term2);

      LOG printf("\n[list_str_sum] term1:%s is_digit:%d term2:%s is_digit:%d n1:%d n2:%d",term1,term1_is_digit,term2,term2_is_digit,n1,n2);

      if(    term1_is_digit > 0 && term2_is_digit > 0){
      
          int n  = n1+n2;
	      if(n==0)
	          strcpy(buff,"");
	      else
	          sprintf(buff,"%d",n);
    
      }
      else if(term1_is_digit == 0 && term2_is_digit > 0){

	      if(n2==0)
	          sprintf(buff,"%s",term1);
	      else
	          sprintf(buff,"%s%+d",term1,n2);
	 
      }
      else if(term1_is_digit > 0 && term2_is_digit == 0){
         
          if(n1==0)
              sprintf(buff,"%s",term2);
          else if(strchr(term2,'-'))
              sprintf(buff,"%d%s",n1,term2);
          else 
              sprintf(buff,"%d+%s",n1,term2);

      }
      else if(term1_is_digit == 0 && term2_is_digit == 0){
            
          if(strchr(term2,'-'))
              sprintf(buff,"%s%s",term1,term2);
          else
              sprintf(buff,"%s+%s",term1,term2);

      }
  
     list_str_add(res,strdup(buff));
   }
   
   return res;
}


/* stack_ptr functions */
stack_ptr* stack_ptr_alloc(int elems){
    stack_ptr* pstack = (stack_ptr*)malloc(sizeof(stack_ptr));
    pstack->elems = (tensor**)malloc(sizeof(tensor*)*elems);
    pstack->ptr   = - 1 ;
    pstack->size  = elems;
    return pstack;
}
void stack_ptr_free(stack_ptr* pstack){
    free(pstack->elems);
    free(pstack);
}
int stack_ptr_len(stack_ptr* pstack){
    return pstack->ptr + 1;
}
tensor* stack_ptr_pop(stack_ptr* pstack){
    if(stack_ptr_len(pstack)){
        return pstack->elems[pstack->ptr--];
    }
    else{
       LOG printf("\n[stack_pop] can't pop! stack is empty.");
       return NULL;

    }
}
void stack_ptr_push(stack_ptr* pstack,tensor* elem){
    if(pstack->size >stack_ptr_len(pstack))
      pstack->elems[++pstack->ptr] = elem;
    else
      LOG printf("\n[stack_push] can´t push! not enough memory.");
}
void stack_ptr_push_tail(stack_ptr* pstack,tensor* elem){
  size_t len=stack_ptr_len(pstack);
  if(pstack->size > stack_ptr_len(pstack)){
    for(int i=len-1;i>=0;i--){
        tensor* ptensor=pstack->elems[i];
        pstack->elems[i+1]=ptensor;       
    }
    pstack->elems[0]=elem;
    pstack->ptr++;
  }
  else
    LOG printf("\n[stack_push] can´t push! not enough memory.");
  //stack_ptr_print(pstack);
} 
void stack_ptr_print(stack_ptr* pstack){
    int max_width = 0;
    for( int i = 0 ; i <= pstack->ptr ; i++)
            if(strlen(pstack->elems[i]->id) > max_width)
                    max_width = strlen(pstack->elems[i]->id);
    int white_spaces = max_width+2;

    LOG printf("\n[stack_print] size:%d len:%d stack->ptr:%d\n",pstack->size,stack_ptr_len(pstack),pstack->ptr);
    for( int i = 0 ; i < pstack->size ; i++){
            if(pstack->ptr == i){

                    LOG printf("-->[");
                    if(stack_ptr_len(pstack ) > i ){
                            LOG printf("%c%s%c",' ',pstack->elems[i]->id,' ');
                    }
                    else
                            LOG printf("%*c",white_spaces,' ');
                    LOG printf("]\n");
            }
            else{
                    LOG printf("   [");
                    if(stack_ptr_len(pstack) > i ){
                            LOG printf("%c%s%c",' ',pstack->elems[i]->id,' ');
                    }
                    else
                            LOG printf("%*c",white_spaces,' ');
                    LOG printf("]\n");
            }
    }

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
       LOG printf("\n[stack_pop] can't pop! stack is empty.");
       return NULL;

    }
}
void stack_push(stack* pstack,list_str* elem){
    if(pstack->size > stack_len(pstack) )
        pstack->elems[++pstack->ptr] = elem;
    else
        LOG printf("\n[stack_push] can´t push! not enough memory.");
}
void stack_print(stack* pstack){
    int max_width = 0;
    for( int i = 0 ; i <= pstack->ptr ; i++)
            if(pstack->elems[i]->size > max_width)
                    max_width = pstack->elems[i]->size;
    int white_spaces = max_width*2+3;

    LOG printf("\n[stack_print] size:%d len:%d stack->ptr:%d\n",pstack->size,stack_len(pstack),pstack->ptr);
    for( int i = 0 ; i < pstack->size ; i++){
            if(pstack->ptr == i){

                    LOG printf("-->[");
                    if(stack_len(pstack ) > i ){
                            list_str_print(pstack->elems[i]);
                            int extra = white_spaces - (pstack->elems[i]->size*2+3);
                            if(extra)  LOG printf("%*c",extra,' ');
                    }
                    else
                            LOG printf("%*c",white_spaces,' ');
                    LOG printf("]\n");
            }
            else{
                    LOG printf("   [");
                    if(stack_len(pstack) > i ){
                            list_str_print(pstack->elems[i]);
                            int extra = white_spaces - (pstack->elems[i]->size*2+3);
                            if(extra)  LOG printf("%*c",extra,' ');
                    }
                    else
                            LOG printf("%*c",white_spaces,' ');
                    LOG printf("]\n");
            }
    }

}
/* tensor functions */
void tensor_set_elem_double(tensor* ptensor,list_tuple* tuple,double elem)
{
    if(tensor_get_n_indeces(ptensor)){
        tensor* pfound = tensor_access_with_tuple(ptensor,tuple);
        int i;
        for(i = 0 ; i < tuple->elems ; i++){
    	if(tuple->p[i].c == pfound->index)
    	    break;
        }
        int id = tuple->p[i].n;
        
        LOG printf("\n[tensor_set_elem] leave:%s size:%zu id:%d ",pfound->id,pfound->size,id);
    
        char* val=NULL;
        if(strlen(ptensor->id)==0) id=0;
        pfound->d[id]=elem; 
    }
    else{
	    ptensor->d[0]=elem;
    }
}
void tensor_set_elem_int(tensor* ptensor,list_tuple* tuple,int elem)
{
    if(tensor_get_n_indeces(ptensor)){
        tensor* pfound = tensor_access_with_tuple(ptensor,tuple);
	int i;
	for(i = 0 ; i < tuple->elems ; i++){
            if(tuple->p[i].c == pfound->index)
                break;
	}
	int id = tuple->p[i].n;
	    
	LOG printf("\n[tensor_set_elem] leave:%s size:%zu id:%d ",pfound->id,pfound->size,id);

	char* val=NULL;
	if(strlen(ptensor->id)==0) id=0;
	pfound->i[id]=elem; 
    }
    else{
	ptensor->i[0]=elem;
    }
}

void tensor_set_elem(tensor* ptensor,list_tuple* tuple,char* elem)
{
    if(tensor_get_n_indeces(ptensor)){
        tensor* pfound = tensor_access_with_tuple(ptensor,tuple);
	int i;
	for(i = 0 ; i < tuple->elems ; i++){
            if(tuple->p[i].c == pfound->index)
                break;
        }
	int id = tuple->p[i].n;
	    
	LOG printf("\n[tensor_set_elem] leave:%s size:%zu id:%d ",pfound->id,pfound->size,id);

	char* val=NULL;
	if(strlen(ptensor->id)==0) id=0;
	    pfound->p=list_str_update(pfound->p,id,elem); 
	    free(elem);
    }
    else{
	list_str_update(ptensor->p,0,elem);
	free(elem);
    }
}

tensor* tensor_access_with_tuple(tensor* ptensor,list_tuple* tuple)
{
    if(strlen(ptensor->id)==0){return ptensor;}
    
    tensor* pfound=ptensor;
    int accesslength=tuple->elems;
    //LOG printf("\n[tensor_access_with_tuple] id:%s visitor:",ptensor->id);
    //list_tuple_print(tuple);
	//LOG printf("\n");
    
    int id;
    int visitor = 0;
    while(visitor < accesslength){
	while(pfound->index!=tuple->p[visitor].c) {visitor++;}
        id=tuple->p[visitor].n;
        if(id > pfound->size - 1){
            printf("\n[tensor_access_with_tuple] Bad request: index overflow.");
            return NULL;
        }
        if(!pfound->isleave)
            pfound = pfound->t[id];
        else
            break;
        visitor++;
    }
	//LOG printf("\n[tensor_access_with_tuple] found:%s(%d)",pfound->id,id);
    return pfound;
}
double  tensor_elem_access_with_tuple_double(tensor* ptensor,list_tuple* tuple)
{
    if(tensor_get_n_indeces(ptensor)==0){return ptensor->d[0];}
    tensor* pfound=ptensor;
    int accesslength=tuple->elems;
    
    int id;
    int visitor = 0;
    while(visitor < accesslength){
	while(pfound->index!=tuple->p[visitor].c) {visitor++;}
        id=tuple->p[visitor].n;
        if(id > pfound->size - 1){
            LOG printf("\n[tensor_access_with_tuple] Bad request: index overflow.");
            return -1;
        }
        if(!pfound->isleave)
            pfound = pfound->t[id];
        else
            break;
        visitor++;
    }
	//LOG printf("\n[tensor_access_with_tuple] found:%s(%d)",pfound->id,id);
    return pfound->d[id];
}
int  tensor_elem_access_with_tuple_int(tensor* ptensor,list_tuple* tuple)
{
    if(tensor_get_n_indeces(ptensor)==0){return ptensor->i[0];}
    tensor* pfound=ptensor;
    int accesslength=tuple->elems;
    
    int id;
    int visitor = 0;
    while(visitor < accesslength){
	while(pfound->index!=tuple->p[visitor].c) {visitor++;}
        id=tuple->p[visitor].n;
        if(id > pfound->size - 1){
            LOG printf("\n[tensor_access_with_tuple] Bad request: index overflow.");
            return -1;
        }
        if(!pfound->isleave)
            pfound = pfound->t[id];
        else
            break;
        visitor++;
    }
	//LOG printf("\n[tensor_access_with_tuple] found:%s(%d)",pfound->id,id);
    return pfound->i[id];
}
char* tensor_elem_access_with_tuple(tensor* ptensor,list_tuple* tuple)
{
    if(strlen(ptensor->id)==0){return list_str_access(ptensor->p,0);}
    tensor* pfound=ptensor;
    int accesslength=tuple->elems;
   // LOG printf("\n[tensor_access_with_tuple] id:%s visitor:",ptensor->id);
   //	list_tuple_print(tuple);
	//LOG printf("\n");
    
    int id;
    int visitor = 0;
    while(visitor < accesslength){
	while(pfound->index!=tuple->p[visitor].c) {visitor++;}
        id=tuple->p[visitor].n;
        if(id > pfound->size - 1){
            LOG printf("\n[tensor_access_with_tuple] Bad request: index overflow.");
            return NULL;
        }
        if(!pfound->isleave)
            pfound = pfound->t[id];
        else
            break;
        visitor++;
    }
	//LOG printf("\n[tensor_access_with_tuple] found:%s(%d)",pfound->id,id);
    return list_str_access(pfound->p,id);
}
char* tensor_access_elem(tensor* ptensor,char* str)
{
    tensor* pfound=ptensor;
    int accesslength=strlen(str);
    char buff[2];
    char log[32];
    sprintf(log,"visitor:%s",str);
    
    if(accesslength > strlen(ptensor->id)){
        LOG printf("\n[%s] Bad request: to many indeces",log);
        return NULL;
    }
    int id;
    int visitor = 0;
    while(visitor < accesslength){
	if(!strcmp(pfound->id,str)) break;
        sprintf(buff,"%c",str[visitor]);
        id=atoi(buff);
        if(id > pfound->size - 1){
           LOG  printf("\n[%s] Bad request:%d > %zu ",log,id,pfound->size - 1);
            return NULL;
         }
        if(!pfound->isleave)
            pfound = pfound->t[id];
        else
            break;
        visitor++;
        }
    //tensor_print_info(pfound,log);
    return list_str_access(pfound->p,id);
}

tensor* tensor_access(tensor* ptensor,char* str)
{
    tensor* pfound=ptensor;
    int accesslength=strlen(str);
    char buff[2];
    char log[32];
    sprintf(log,"visitor:%s",str);
    
    if(accesslength > strlen(ptensor->id)){
        LOG printf("\n[%s] Bad request: to many indeces",log);
        return NULL;
    }

    int visitor = 0;
    while(visitor < accesslength){
	if(!strcmp(pfound->id,str)) break;
        sprintf(buff,"%c",str[visitor]);
        int id=atoi(buff);
        if(id > pfound->size - 1){
            LOG printf("\n[%s] Bad request:%d > %zu ",log,id,pfound->size - 1);
            return NULL;
         }
        if(!pfound->isleave)
            pfound = pfound->t[id];
        else
            break;
        visitor++;
        }
    //tensor_print_info(pfound,log);
    return pfound;
}
void tensor_print_info(tensor* ptensor,char* msg)
{
    if(ptensor->size){
        printf("\n\n[%s] Index\t:%c",msg,ptensor->index);
        printf("\n[%s] IsUpper\t:%d",msg,ptensor->isupper);
        printf("\n[%s] Size\t:%zu",msg,ptensor->size);
        printf("\n[%s] Id\t:%s",msg,ptensor->id);
        printf("\n[%s] level\t:%d",msg,ptensor->level);
        printf("\n[%s] IsLeave\t:%d",msg,ptensor->isleave);
        printf("\n[%s] type\t:%d",msg,ptensor->type);
    }
}
void tensor_print(tensor* ptensor)
{
   if(ptensor){
     tensor_print_info(ptensor,"print");       
     if(ptensor->isleave)
         return;
      for(int i=0 ; i < ptensor->size ; i++){
         tensor_print(ptensor->t[i]);
      }
    }
}
void tensor_print_elems_leave(tensor* ptensor,int upper_father)
{
   int freepstr   = 0;
   list_str* pstr = NULL;

   switch(ptensor->type){
       case 0:
           pstr = ptensor->p;
       break;
       case 1:
	   pstr = list_str_from_int_vector(ptensor->i,ptensor->size);
	   freepstr = 1;
       break;
       case 2:
       	   pstr = list_str_from_double_vector(ptensor->d,ptensor->size);
	   freepstr = 1;
       break;
   }

   if(ptensor->isupper) list_str_print_column(pstr);
   else                 list_str_print(pstr);
   if(upper_father) printf("\n");

   if(freepstr) list_str_free(pstr);
}
void tensor_print_elems_recursive(tensor* ptensor,int upper_father)
{
  if(ptensor){
    if(ptensor->isleave){
        tensor_print_elems_leave(ptensor,upper_father);
        return;
     }
    else{
     if(upper_father) printf("\n{" );
     else                 printf("{");
     for(int i=0 ; i < ptensor->size ; i++)
       tensor_print_elems_recursive(ptensor->t[i],ptensor->isupper);
     printf("}");
    }
  }
}

void tensor_print_elems(tensor* ptensor)
{
    tensor_print_elems_recursive(ptensor,ptensor->isupper);
    printf("\n");
}
double*  tensor_get_leave_double(tensor* t,char* leave){
    if(t->isleave)
            return t->d;
    int indeces = strlen(leave);
    int count = 0; 
    tensor* pleave = t; 
    while(count <= indeces){
      int i =  leave[count] -'0';
      if(pleave->isleave)
        return pleave->d;
      else
        pleave=pleave->t[i];
      count++;
    }
    return NULL;
}
int*  tensor_get_leave_int(tensor* t,char* leave){
    if(t->isleave)
            return t->i;
    int indeces = strlen(leave);
    int count = 0; 
    tensor* pleave = t; 
    while(count <= indeces){
      int i =  leave[count] -'0';
      if(pleave->isleave)
        return pleave->i;
      else
        pleave=pleave->t[i];
      count++;
    }
    return NULL;
}
list_str*  tensor_get_leave(tensor* t,char* leave){
    if(t->isleave)
            return t->p;
    int indeces = strlen(leave);
    int count = 0; 
    tensor* pleave = t; 
    while(count <= indeces){
      int i =  leave[count] -'0';
      if(pleave->isleave)
        return pleave->p;
      else
        pleave=pleave->t[i];
      count++;
    }
    return NULL;
}
size_t   tensor_get_n_leaves(tensor* t){
    size_t  leaves=1;
    int     indeces=tensor_get_n_indeces(t);
    size_t* sizes=tensor_get_sizes(t);
    for(int i = 0 ; i < indeces - 1 ; i++)
            leaves*=sizes[i];
    free(sizes);
    return leaves;  
}
size_t   tensor_get_n_elems(size_t* sizes,int indeces){
    size_t elems=1;
     for(int i = 0 ; i < indeces ; i++)
            elems*=sizes[i];
    return elems;  
}
size_t tensor_get_n_nodes(size_t* sizes,int indeces){
  if(indeces<=1)
    return 1;      
  size_t nodes = 1+sizes[0]; 
  for(int i=1;i<indeces-1;i++)
          nodes+=sizes[i]*sizes[i-1];
  return nodes;
}
int tensor_get_n_indeces(tensor* ptensor){
    int cnt = 0 ; 
    for(int i=0;i<strlen(ptensor->id);i++){
        if(isdigit(ptensor->id[i])) cnt++;
    }

    return strlen(ptensor->id)-cnt;
}
size_t* tensor_get_sizes(tensor* ptensor)
{  
    size_t* sizes=NULL;
        
    if(ptensor->isleave){
      sizes=malloc(sizeof(size_t)*1);
      sizes[0] = ptensor->size;
      LOG printf("\n[tensor_get_sizes] indeces:%s size[0]=%zu",ptensor->id,sizes[0]);
      return sizes;
    } 
   
    int indeces = 0 ;
    indeces = tensor_get_n_indeces(ptensor);
    sizes=malloc(sizeof(size_t)*indeces);
    
    for(int i=0 ; i < indeces; i++){
        tensor* pvisitor = tensor_access_index(ptensor,ptensor->id[i]);
        sizes[i]=pvisitor->size;
    }

    for(int i=0;i<indeces;i++)LOG printf("\n[tensor_get_sizes] indeces:%s size[%d]=%zu",ptensor->id,i,sizes[i]);
    return sizes;
 
}
size_t** tensor_get_access(tensor* ptensor,size_t* sizes,int indeces)
{
   size_t** access = (size_t**) calloc(sizeof(size_t*),indeces);
   
   LOG printf("\n\n[tensor_get_acess]:\n");
   
   for(int i=0 ; i < indeces ; i++){
           access[i] = (size_t*) calloc(sizeof(size_t),sizes[i]);
           LOG printf("\taccess[%d]:[",i);
           for(size_t j=0 ; j < sizes[i] ; j++){
                   LOG printf(" %zu",j);
                   access[i][j]=j;
           }
           LOG printf(" ]\n");
   }
   return access;
}
tensor* tensor_create(char* indeces_names,char* upperlower,size_t* sizes,int init,int type){

   int indeces = strlen(indeces_names);

   tensor* ptensor=NULL;
   if(indeces > 0 ){ 
      create_recursive(&ptensor,0,sizes,indeces,indeces_names,upperlower,init,type);
   }
   else{
      ptensor = (tensor*)malloc(sizeof(tensor));
      ptensor->t=NULL;
      ptensor->isleave=1;
      ptensor->id=strdup("");
      ptensor->index='0';
      ptensor->level=0;
      ptensor->size=1;
      ptensor->type=type;
      ptensor->isupper=0;
      switch(type){
         case 0: // symbolic tensor (strings)
         ptensor->p=list_str_alloc(1);
         list_str_add(ptensor->p,strdup("0"));
         break;
         case 1: // integer tensor
         ptensor->i=(int*)calloc(sizeof(int),1);
         break;
         case 2: // double tensor
         ptensor->d=(double*)calloc(sizeof(double),1);
         break;   
     }
   }
   return ptensor;
}
void tensor_copy_elems(tensor* dest, tensor* source)
{
    if(source->isleave){
	switch(dest->type){
	    case 0:{
        	dest->p = list_str_copy(source->p);
            break;
	    }
	    case 1:{
		dest->i = (int*)calloc(sizeof(int),source->size);
		memcpy(dest->i,source->i,source->size);
	        break;
 	    }
	    case 2:{
		dest->d = (double*)calloc(sizeof(double),source->size);
		memcpy(dest->d,source->d,source->size);
	        break;
	    }
	}
	return;
    }
    else{
        for(int i=0 ; i < source->size ; i++)
            tensor_copy_elems(dest->t[i],source->t[i]);
    }
}        
tensor* tensor_copy(tensor* ptensor)
{
    tensor* copy = NULL;
    int nindices = tensor_get_n_indeces(ptensor);
    char* str    = (char*)calloc(sizeof(char),nindices+1);
    strcpy(str,"");

    tensor* visitor = ptensor;
    int i = 0 ; 
    for(i=0; i < nindices; i++){
        str[i]  = visitor->isupper ? '+' : '-';
        visitor = visitor->t[0];
    }    
    str[i] = '\0';
    LOG printf("\n[tensor_copy] indeces:%s upperlower:%s",ptensor->id,str);
    size_t* sizes = tensor_get_sizes(ptensor);
    copy  = tensor_create(ptensor->id,str,sizes,0,ptensor->type);
    tensor_copy_elems(copy,ptensor);
    free(str);
    free(sizes);
    return copy;
}
list_str* sum_char_ptr( char* a , char* b){


    LOG printf("\n[sum_char_ptr] a=%s b=%s",a,b);

    list_str* term1=list_str_alloc(1);
    list_str* term2=list_str_alloc(1);

    list_str_add(term1,strdup(a));
    list_str_add(term2,strdup(b));
    list_str *res= list_str_sum(term1,term2);
    list_str_free(term1);
    list_str_free(term2);

    LOG printf(" a+b=%s",res->elems[0]);

    return res;
}

void nested_loops(size_t cur,list_tuple* l,list_tuple* l_cur,tensor* t_res,tensor* t_big,tensor* t_small)
{
    for(size_t i=0 ; cur < l->elems  && i < l->p[cur].n  ; i++)
    {
        l_cur->p[cur].n=i;
	    if(cur < l->elems ){
    	    nested_loops(cur+1,l,l_cur,t_res,t_big,t_small);
    	}
	    if(cur==l->elems-1){
            LOG printf("\n[nested_loops] ");
           
            list_tuple_print(l_cur);
	        switch(t_res->type){
            case 0:{
    	        char* res   = tensor_elem_access_with_tuple(t_res,l_cur);
                char* big   = tensor_elem_access_with_tuple(t_big,l_cur);
    	        char* small = tensor_elem_access_with_tuple(t_small,l_cur);
        
                list_str* term1 = list_str_from_char_ptr(big);
    	        list_str* term2 = list_str_from_char_ptr(small);
    	        char* prod      = list_str_dot_product(term1,term2);
    	        list_str* s_res = NULL;
    	        
    	        if(strlen(prod)==0) s_res = sum_char_ptr(res,"0");
    	        else                s_res = sum_char_ptr(res,prod);
    	        LOG printf("\n[nested_loops] res:%s big:%s small:%s prod:%s sum:%s",res,big,small,prod,s_res->elems[0]);
    
    	        if(strlen(s_res->elems[0])) tensor_set_elem(t_res,l_cur,strdup(s_res->elems[0]));
    	        else		                tensor_set_elem(t_res,l_cur,strdup("0"));
    	        list_str_free(s_res);
    	        if(prod) free(prod);
    	        list_str_free(term1);
    	        list_str_free(term2);
            }
            break;
            case 1:{
	        list_tuple_print(l_cur);
    	        int res   = tensor_elem_access_with_tuple_int(t_res,l_cur);
                int big   = tensor_elem_access_with_tuple_int(t_big,l_cur);
    	        int small = tensor_elem_access_with_tuple_int(t_small,l_cur);
        
   		res+=big*small;
    	        LOG printf("\n[nested_loops] res:%d big:%d small:%d",res,big,small);

		tensor_set_elem_int(t_res,l_cur,res);
	    }
	    break;
            case 2:{
	        list_tuple_print(l_cur);
    	        double res   = tensor_elem_access_with_tuple_double(t_res,l_cur);
                double big   = tensor_elem_access_with_tuple_double(t_big,l_cur);
    	        double small = tensor_elem_access_with_tuple_double(t_small,l_cur);
        
   		res+=big*small;
    	        LOG printf("\n[nested_loops] res:%g big:%g small:%g",res,big,small);

		tensor_set_elem_double(t_res,l_cur,res);
	    }
            break;
            }
    	}
   }
}
tensor* create_recursive(tensor** ptensor,int cur, size_t* sizes, int indeces,char* names,char* upperlower,int init,int type){

    if(*ptensor==0){
        LOG printf("\nindex:%c id:%s size:%zu is node:%d\n",names[0],names,sizes[0],indeces > 1 ? 1:0);
        *ptensor=tensor_alloc(names[0],upperlower[0]=='+'?1:0,sizes[0], indeces > 1 ? 0:1,names,init,type);
        (*ptensor)->level=0;
    }
    for(int i=0 ;i < sizes[cur] && cur < indeces - 1 ; i++) {
        char str[256];
        strcpy(str,names);
        str[cur]=i+'0';
        int is_node = cur + 1 != indeces - 1 ; 
        char index = is_node ? names[cur + 1] : names[indeces-1];
        int level = cur + 1 ;
        char isupper= is_node ? upperlower[cur+1] : upperlower[indeces-1];
        LOG printf("\nindex:%c id:%s size:%zu level:%d is node:%d\n",index,str,sizes[cur+1],level,is_node);
        (*ptensor)->t[i]=tensor_alloc(index,isupper=='+'?1:0,sizes[cur+1],!is_node,str,init,type);
        (*ptensor)->t[i]->level = level; 
        if(is_node) {
            (*ptensor)->t[i]=create_recursive(&((*ptensor)->t[i]),cur+1,sizes,indeces,str,upperlower,init,type);
        }
    }
    return *ptensor;
} 
tensor* tensor_fill_int(tensor* ptensor,char* leave, int* elems){
	tensor* visitor = tensor_access(ptensor,leave);
	if(visitor->i){
	    memcpy(visitor->i,elems,visitor->size * sizeof(int));
	}
	else{
            visitor->i = (int*)calloc(sizeof(int),visitor->size);
	    memcpy(visitor->i,elems,visitor->size * sizeof(int));
	}
	return ptensor;
}
tensor* tensor_fill_double(tensor* ptensor,char* leave, double* elems){

	tensor* visitor = tensor_access(ptensor,leave);
	if(visitor->d){
	    memcpy(visitor->d,elems,visitor->size * sizeof(double));
	}
	else{
            visitor->d = (double*)calloc(sizeof(double),visitor->size);
	    memcpy(visitor->i,elems,visitor->size * sizeof(double));
	}

	return ptensor;
}
tensor* tensor_fill(tensor* ptensor,char* leave, list_str* elems){

	tensor_access(ptensor,leave)->p=list_str_copy(elems);
	return ptensor;
}
tensor* tensor_alloc(char index,int isupper,size_t size,int isleave,char* id,int init,int type)
{
    tensor* ptensor=(tensor*)malloc(sizeof(tensor));
    ptensor->size=size;
    ptensor->isleave=isleave;
    ptensor->index=index;
    ptensor->isupper=isupper;
    ptensor->id=strdup(id);
    ptensor->type=type; 
    ptensor->p = NULL;
    ptensor->i = NULL;
    ptensor->d = NULL;
    if(!isleave){
        ptensor->t=(tensor**)malloc(sizeof(tensor*)*size);
    }
    else{
       if(init){
           switch(type){
	       case 0: // symbolic tensor (strings)
	           ptensor->p=list_str_alloc(size);
	           for(int i=0;i<size;i++) list_str_add(ptensor->p,strdup("0"));
	       break;
	       case 1: // integer tensor
	           ptensor->i=(int*)calloc(sizeof(int),size);
	       break;
	       case 2: // double tensor
	           ptensor->d=(double*)calloc(sizeof(double),size);
	   break;   
           }
       }
   }
    return ptensor;
}
void tensor_fill_stack(tensor* ptensor, stack_ptr* stack,int only_leaves)
{
    if(ptensor){
      if(!only_leaves)
        stack_ptr_push(stack,ptensor);
      else
      if(ptensor->isleave)
        stack_ptr_push(stack,ptensor);

      if(ptensor->isleave) return; 

      for(int i=0 ; i < ptensor->size ; i++){
        tensor_fill_stack(ptensor->t[i],stack,only_leaves);
      }
    }
}

void tensor_free(tensor* ptensor)
{
  if(!ptensor)
          return;
  int indeces  = tensor_get_n_indeces(ptensor);

  if(indeces == 0){
          tensor_free_node(ptensor);
          return;
  }
  
  size_t* sizes = tensor_get_sizes(ptensor);
  size_t elems  = tensor_get_n_elems(sizes,indeces);
  size_t total_leaves = tensor_get_n_leaves(ptensor);
  size_t total_nodes  = tensor_get_n_nodes(sizes,indeces);
  size_t total_nodes_freed = 0; 
  size_t total_leaves_freed = 0;
  //LOG printf("\n[tensor_free] indeces:%d elems:%zu nodes:%zu leaves:%zu",indeces,elems,total_nodes,total_leaves);

  stack_ptr*  stack_nodes  = stack_ptr_alloc(total_nodes*10);

  tensor_fill_stack(ptensor,stack_nodes,0);
  
  int level = indeces - 1 ;
  LOG printf("\n[tensor_free] stack_nodes_len:%d",stack_ptr_len(stack_nodes));

  while(level >= 0){
    int free_level=1;
    for(int i=0;i<level;i++) free_level*=sizes[i];
    LOG printf("\n[tensor_free] tensor to free at this level:%d",free_level);
    int count=0;
    //stack_ptr_print(stack_nodes);
    while(count < free_level){
      tensor* t = stack_ptr_pop(stack_nodes);
      if(t->level == level){
        tensor_free_node(t);
        t == NULL;
        count++;
        total_nodes_freed++;
      }
      else{
        stack_ptr_push_tail(stack_nodes,t);
      }
    }
    LOG printf("\n[tensor_free] tensor deleted  at this level:%d",count);
    level--;
  }

  LOG printf("\n[tensor_free] %zu nodes deleted",total_nodes_freed);
  free(sizes);
  stack_ptr_free(stack_nodes);
}

void tensor_free_node(tensor* ptensor){
  free(ptensor->id);
  if(ptensor->isleave){
    if(ptensor->p){
      if(ptensor->type == 0 ){
      	  list_str_free(ptensor->p);
          ptensor->p=NULL;
      }
      else if( ptensor->type == 1){
	  free(ptensor->i);
	  ptensor->i = NULL;
      }
      else if( ptensor->type == 2){
	  free(ptensor->d); 
          ptensor->d = NULL;	  
      }
    }	    
  }
  else
    free(ptensor->t);
  free(ptensor);
}

tensor* tensor_access_index(tensor* t,char index){
  tensor* pvisitor = t;
  if(isdigit(index) && pvisitor->size)             return tensor_access_index(pvisitor->t[0],index); 
  if(pvisitor->index==index)                       return pvisitor;
  if(pvisitor->size)                               return tensor_access_index(pvisitor->t[0],index);
  if(pvisitor->isleave && pvisitor->index!=index)  return NULL;
}

static void tensor_normalize_recursive(tensor*t)
{
    if(t->id[0] >='0' || t->id[0] <= '9'){

        char buff[256];
        int k;
        int cnt=0;
        printf("\n[tensor_normalize_recursive] t->id:%s",t->id);
        for(k=0;k<strlen(t->id);k++){
            if(!isdigit(t->id[k]))
                buff[cnt++]=t->id[k];
        }
        buff[cnt]='\0';
        free(t->id);
        t->id=strdup(buff);
        printf("\n[tensor_normalize_recursive] t->id:%s",t->id);
        if(t->isleave)
            return;
        for(int i=0; i< t->size; i++)
        {
            tensor_normalize_recursive(t->t[i]);

        }
    }
}    
static tensor* tensor_normalize(tensor* t)
{
    tensor* norm = tensor_copy(t);
    if(isdigit(norm->id[0]))
        tensor_normalize_recursive(norm);
    tensor_print(norm);
    return norm;
}
int dotproduct_int(int* a, int* b, size_t size){
    int res = 0 ;
    for( size_t i = 0 ; i < size ; i++)
	    res+= a[i] * b[i] ;
}
double dotproduct_double(double* a, double* b, size_t size){
    double res = 0.f ;
    for( size_t i = 0 ; i < size ; i++)
	    res+= a[i] * b[i] ;
}
tensor* tensor_einsum(tensor* a,tensor* b)
{
  tensor* ptensor = NULL;
  tensor* t_big   = NULL;
  tensor* t_small = NULL;

  if(tensor_get_n_indeces(a) >= tensor_get_n_indeces(b)){
    t_big=a;
    t_small=b;
  }else{
    t_big=b;
    t_small=a;
  }

  char*  st_union = findUnion(t_big->id,t_small->id);
  char*  sum_c    = findIntersection(t_big->id,t_small->id);
  char*  id_c     = findXOR(st_union,sum_c);
  
  if(strlen(sum_c)==0){
   LOG printf("\n[tensor_einsum] no common indeces found");
   return NULL;
  }
  int indeces_result   = strlen(id_c); 
  size_t* sizes_big    = tensor_get_sizes(t_big);
  size_t* sizes_small  = tensor_get_sizes(t_small);
  size_t* sizes_result = (size_t*)calloc(sizeof(size_t),indeces_result);

  char* upperlower = (char*)calloc(sizeof(char),indeces_result+1);
  strcpy(upperlower,"");
  int k=0;
  for(int i=0;i<indeces_result;i++)
  {
     for(int j=0;j<strlen(t_big->id);j++){
       if(id_c[i]==t_big->id[j]){
          sizes_result[i]=sizes_big[j];
          char c;
          tensor* p=tensor_access_index(t_big,id_c[i]);
          c = p->isupper ? '+' : '-';
          upperlower[k++]=c;
       }
     }
  }
  upperlower[k]='\0';
  LOG printf("\n[tensor_einsum] upperlower: %s",upperlower);
  LOG printf("\n[tensor_einsum]");
  for(int i=0;i<indeces_result;i++) LOG printf("\nid_c[%c]=%zu",id_c[i],sizes_result[i]);
  ptensor=tensor_create(id_c,upperlower,sizes_result,1,t_big->type);
  
  /*building sizes iterations*/
  int iter_len         = strlen(st_union);
  list_tuple* iter     = list_tuple_alloc(iter_len);
  list_tuple* iter_cur = list_tuple_alloc(iter_len);
  size_t* sizes_iter=(size_t*)calloc(sizeof(size_t),iter_len);

  for(int i=0;i<iter_len;i++){
      char index = st_union[i];
      tensor* t = NULL;
      t=tensor_access_index(t_big,index);
      if(!t) t=tensor_access_index(t_small,index);
      sizes_iter[i]=t->size;
      list_tuple_add(iter,index,t->size);
      list_tuple_add(iter_cur,index,0);
      LOG printf("\nst_union[%c]=%zu",index,t->size);
  }
 
  list_tuple_print(iter);
  if(!(strlen(t_big->id)==1&&strlen(t_small->id)==1))
    nested_loops(0,iter,iter_cur,ptensor,t_big,t_small);
  else{
    switch(t_big->type){
    case 0:{	    
        char* dotproduct=list_str_dot_product(t_big->p,t_small->p);
        ptensor->p=list_str_update(ptensor->p,0,dotproduct); 
        free(dotproduct);
    }
    break;
    case 1:{
        int res = dotproduct_int(t_big->i,t_small->i,t_big->size);
	ptensor->i[0] = res ;
		 
    }
    break;
    case 2:{
        double res = dotproduct_double(t_big->d,t_small->d,t_big->size);
	ptensor->d[0] = res ;
    }
    break;
    }
  }

  list_tuple_free(iter);
  list_tuple_free(iter_cur);
  
  LOG printf("\n[tensor_einsum] tensor_result:%s tensor_big:%s tensor_small:%s iterations:%s indeces:%d",ptensor->id,t_big->id,t_small->id,st_union,iter_len);
  
 
  free(st_union);
  free(sum_c);
  free(id_c);
  free(upperlower);
  free(sizes_iter);
  free(sizes_result); 
  free(sizes_big);
  free(sizes_small);
  return ptensor;
}
tensor* tensor_create_lambda(tensor* gamma)
{
    if(!gamma) return NULL;
    tensor* pvisitor = gamma;
    while(!pvisitor->isleave) pvisitor = pvisitor->t[0];
    size_t size_leave[1];
    size_leave[0] = pvisitor->size;
    char id[16];
    sprintf(id,"%c",pvisitor->index);
    tensor* lambda = tensor_create(id,"+",size_leave,0,0);
    return lambda;
}

tensor* tensor_create_from_params(char* indeces, char* lowerupper, int dimensions, int statements, int variables, int parameters)
{
    int     grouped   = strlen(indeces);
    size_t* sizes     = NULL;
    size_t  numcoeff  = 0;
    tensor* gamma     = NULL;
    char    elem[256];  strcpy(elem,"");
    char    leave[256]; strcpy(leave,"");
    list_str* coeff   = NULL;
    sizes    = (size_t*)calloc(sizeof(size_t),grouped);

    switch(grouped){
        case 3:{
            numcoeff = 4 + 2 + parameters + 1;
            sizes[0] = dimensions;
            sizes[1] = statements;
            sizes[2] = numcoeff;

            gamma    = tensor_create(indeces,lowerupper,sizes,0,0);
            coeff    = list_str_alloc(numcoeff);

            for(int k=0; k< numcoeff;k++){
                sprintf(elem,"i%d",k);
                list_str_add(coeff,strdup(elem));
            }
            for( int i = 0 ; i < dimensions ; i++)
                for( int j = 0 ; j < statements ; j++){
                    sprintf(leave,"%d%d%c",i,j,indeces[2]);
                    tensor_fill(gamma,leave,coeff);
            }
            list_str_free(coeff);
            break;
        }
        case 2:{
            numcoeff = 4 + statements*2 + parameters + 1;
            sizes[0] = dimensions;
            sizes[1] = numcoeff;
            gamma    = tensor_create(indeces,lowerupper,sizes,0,0);
            coeff    = list_str_alloc(numcoeff);
            for(int k=0; k< numcoeff;k++){
                sprintf(elem,"i%d",k);
                list_str_add(coeff,strdup(elem));
            }
             for( int i = 0 ; i < dimensions ; i++){
                sprintf(leave,"%d%c",i,indeces[1]);
                tensor_fill(gamma,leave,coeff);
            }
            list_str_free(coeff);
            break;
        }
    }
    free(sizes);
    return gamma;
}

