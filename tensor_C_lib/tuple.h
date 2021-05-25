#ifndef __TUPLE_H_
#define __TUPLE_H_
typedef struct{
	char c;
	size_t n;
}
tuple;
typedef struct{
	tuple* p;
	size_t size;
	size_t elems;
//	size_t level;
}
list_tuple;
list_tuple* list_tuple_alloc(size_t size);
list_tuple* list_tuple_free(list_tuple* l);
void list_tuple_add(list_tuple* l, char c, size_t n);
size_t list_tuple_size(list_tuple* l);
size_t list_tuple_len(list_tuple* l);
void list_tuple_edit_elem(list_tuple* l,size_t index,char c, size_t n);
void list_tuple_print(list_tuple* l);
#endif
