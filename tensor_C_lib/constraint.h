#ifndef __CONSTRAINT_H
#include "tensor.h"
struct {
    int isconditional;
    int dimout;
    int dimin;
    int type;
    tensor* coeff;
    tensor* lambda;
}
typedef struct constraint;
constraint* tensor_create_constraint_dimensional(int dimin,tensor* gamma,tensor* lambda,int type);
constraint* tensor_create_constraint_conditional(int dimout,int dimin,tensor* gamma, tensor* lambda, int type);
char*       tensor_constraint_serialize(constraint* c);
void        tensor_constraint_print(constraint* c);
void        tensor_constraint_free(constraint* c);


#define __CONSTRAINT_H
