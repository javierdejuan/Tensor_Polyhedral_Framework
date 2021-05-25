#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro.h"
#include "tuple.h"

list_tuple* list_tuple_alloc(size_t size)
{
	list_tuple* l= (list_tuple*)calloc(sizeof(list_tuple),1);
	l->p         = (tuple*)calloc(sizeof(tuple),size);
	l->size      = size;
	l->elems     = 0;
	return l;
}
list_tuple* list_tuple_free(list_tuple* l)
{
    if(!l) return NULL;

    free(l->p);
    free(l);

    return NULL;
}
void list_tuple_add(list_tuple* l, char c, size_t n)
{
    if(!l){                  LOG printf("\n[list_tuple_add] invalid null pointers."); return;}
    if(l->elems >= l->size){ LOG printf("\n[list_tuple_add] not enough memory");      return;}

    l->p[l->elems].c = c;
    l->p[l->elems].n = n;

    l->elems++;

}
size_t list_tuple_size(list_tuple* l)
{  return l->size; }
size_t list_tuple_len(list_tuple* l)
{  return l->elems; }
void list_tuple_edit_elem(list_tuple* l,size_t index,char c, size_t n)
{
   if(!l){                LOG printf("\n[list_tuple_add] invalid null pointer."); return;}
   if(index >= l->elems){ LOG printf("\n[list_tuple_add] invalid index elem.");   return;}

   l->p[index].c = c;
   l->p[index].n = n;
}
void list_tuple_print(list_tuple* l)
{
   if(!l) return;
   LOG printf("[");
   for(size_t i=0 ; i<l->elems; i++)
       LOG printf("%c:%zu ",l->p[i].c,l->p[i].n);
   LOG printf("]");
}

