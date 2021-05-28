#include "constraint.h"

void tensor_constraint_free(constraint* c)
{
    tensor_free(coeff);
    tensor_free(lambfa);
    free(c);
}
constraint* tensor_create_constraint_dimensional(int dimin,tensor* gamma,tensor* lambda,int type)
{
    constraint* dimensional    = (constraint*) calloc(sizeof(constraint),1);
    dimensional->isconditional = 0 ;
    dimensional->dimout        = -1;
    dimensional->dimin         = dimin;
    dimensional->coeff         = tensor_copy(gamma);
    dimensional->lambda        = tensor_copy(lambda);
    dimensional->type          = type;
    return dimensional;
}
constraint* tensor_create_constraint_conditional(int dimout,int dimin,tensor* gamma, tensor* lambda,int type){
    constraint* conditional    = (constraint*) calloc(sizeof(constraint),1);
    conditional->isconditional = 1 ;
    conditional->dimout        = dimout1;
    conditional->dimin         = dimin;
    conditional->coeff         = tensor_copy(gamma);
    conditional->lambda        = tensor_copy(lambda);
    conditional->type          = type;
    return conditional;
}
char* tensor_constraint_serialize(constraint* c)
{
    char* ser=(char*)calloc(sizeof(char),256);
    strcpy(ser,"");
    return ser;
}

