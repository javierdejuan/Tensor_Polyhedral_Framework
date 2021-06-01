#ifndef __CONSTRAINT_H
#define __CONSTRAINT_H
#include "tensor.h"
typedef struct constraint{
    int isconditional;
    int dimout;
    int dimin;
    int type;
    tensor* coeff;
    tensor* lambda;
}
constraint;
constraint* tensor_create_constraint_dimensional(int dimin,tensor* gamma,tensor* lambda,int type);
constraint* tensor_create_constraint_conditional(int dimout,int dimin,tensor* gamma, tensor* lambda, int type);
char*       tensor_constraint_serialize(constraint* c);
void        tensor_constraint_print(constraint* c);
void        tensor_constraint_free(constraint* c);
#endif
