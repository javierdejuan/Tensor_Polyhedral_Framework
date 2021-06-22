### Tensor C library
[WORK IN PROGRESS]
The underlying idea of this Tensor library is to unify all numerical data types in C into a tensor unique type.
Hence, we can write the following statements:
* int or double or float -> is a rank 0 Tensor.
* arrays                 -> are rank 2 Tensors.
* multdimensional arrays -> are rank > 2 Tensors.

Usage:

``` 
/*
test: Define one rank 2 tensor and one rank 1 tensor
define common index j
perform einsten summation over j
*/

size_t size_array[2]={5,5};
size_t size_vector[1]={5};

int    vec_0 = [0,-1,2,3,1];
int    vec_1 = [1,-0,2,0,1];
int    vec_2 = [0,-1,2,1,1];
int    vec_3 = [2,-2,0,2,1];
int    vec_4 = [1,-1,2,3,4];
int    vec   = [0,-1,-1,2,3];

tensor* array  = tensor_create("ij",¨+-",size_array,0,0);

tensor* row_0j = tensor_access(array,"0j");
tensor* row_1j = tensor_access(array,"1j");
tensor* row_2j = tensor_access(array,"2j");
tensor* row_3j = tensor_access(array,"3j");
tensor* row_4j = tensor_access(array,"4j");

tensor_fill(row_0j,vec_0);
tensor_fill(row_1j,vec_1);
tensor_fill(row_2j,vec_2);
tensor_fill(row_3j,vec_3);
tensor_fill(row_4j,vec_4);

tensor* vector = tensor_create("j","+",size_vector,0,0);
tensor_Fill(vector,vec);
tensor* mult   = tensor_einsum(array,vector);

tensor_fill(vector,vec);

tensor_free(mult);
tensor_free(vector);
tensor_Free(array);

```

#### Goals
This C library implements Tensor algebra for symbolic calculus as well as integer or double element type.
#### Compilation and execution
```./compile.sh```

```./tensor```

for debug info:

```./compile.sh -DDEBUG```
