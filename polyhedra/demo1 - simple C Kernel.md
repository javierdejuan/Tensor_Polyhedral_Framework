## Tensor Polyhedra Framework  
### Welcome to the future.  
This is an educational version using symbolic library to focus on linear transformation. For a production code symbolic calculation is not needed.  
In this demo we are going to perform a Polyhedral Scheduler Transformation on the following kernel written in C:  
```
for(i=0;i<=N;i++)
    A[i]=0             // X statement
for(j=0;j<=N;j++)
    for(k=0,k<=N;k++)
        B[j] += A[k];  // Y Statement
```  
In a polyhedral transformation, we do not care of the computing statements, so the above Kernel can be rewritten as:  
```
for(i=0;i<=N;i++)
    X(i);             
for(j=0;j<=N;j++)
    for(k=0,k<=N;k++)
        Y(j,k);
```  
Let's introduce the concept of *scheduling vectors*. A *scheduling vector* captures information regarding *when* a tuple $(i,j,k)$ is executed. Hence whe can map our ```X``` and ```Y``` statements like this:  
```X(i,0,0)```-> X[i]     is executed at _date_ [i * *days*,0,0]  
```Y(N,j,k)```-> Y[N,j,k] is executed at _data_ [N * *days*, j * *minutes*, k * *seconds*]  

So, what really *is* a Polyhedral Transformation?  
Well, unfortunatly Polyhedral Theory is difficult to understand, but the underlying idea is quit straight forward. How can we rearrange X and Y execution dates to have a new Kernel which obtain the same result as source Kernel but takes less time to obtain it?  
This is done finding new X' and Y' such as  

$$X´=(t^0_{xi}.i+t^0_{xj}.j+t^0_{xk}.k+t^0_{xN}.N+t^0_{x1}.1,t^1_{xi}.i+t^1_{xj}.j+t^1_{xk}.k+t^1_{xN}.N+t^1_{x1}.1)$$ and  

$$Y´=(t^0_{yi}.i+t^0_{yj}.j+t^0_{yk}.k+t^0_{yN}.N+t^0_{y1}.1,t^0_{yi}.i+t^0_{yj}.j+t^0_{yk}.k+t^0_{yN}.N+t^0_{y1}.1)$$

where $t^d_{sr}$ coefficients are optimaly calculated to ensure that we get a code that yields the same result and execute faster relying on the possibilities of parallelization of the hardware.  
In order to solve our problem, we need to feed our polyhedral model with a set of constraints that ensures legal excution of code:  
we define:  
```
    D_X = { (i)     | 0 <= i <= N }  
    D_Y = { (j,k)   | 0 <= j <= N && 0 <= k <= N }
```
We can then define a kind of constraints called *dependencies* such as:

```  
    D_XY = { (ic,jc,kc)     | 0 <= ic <= N  && 0 <= jc <= N && 0 <= kc <= N && ic == kc } 
    D_YY = { (js,ks,jt,kt)  | 0 <= js <= N  && 0 <= ks <= N && 0 <= jt <= N  && 0 <= kt <= N && js == kt && kt > ks  }
```
Expression ```D_XY``` captures the read/write access of statements X and Y over array A: ```A[k]=A[i]```.  
In the other hand, expression ```D_YY``` capures the read/write access of statement Y over array B: ```B[j] = B[j] + A[k]```.

Last, let's define two function $f$ and $g$ such as:  

```
    f(i_c,j_c,k_t)=T_y(j_c,k_c)-T_x(i_c)-e_dxy>=0 in D_XY
```
and  
```    
    g(j_s,k_s,j_t,k_t)=T_y(j_t,k_t)-T_y(j_s,k_s)-e_dyy>=0 in D_YY  
    with   
    0 <= e_DXY <= 1  
    0 <= e_DYY <= 1  
``` 

Both functions captures the access relations and the precedences constraints. From C code we derive that B[k] must be executed after A[i]. Besides variables js,ks,jt and kt captures the B[k] = B[k] + A[j] precedence constraint.  
  
Let's import ```sympy``` necessary modules and functions


```python
import sympy as sym
from sympy.tensor.tensor import TensorIndexType,TensorIndex,TensorHead
from sympy import solve,Eq,Mul,ones
```

we can have **LaTex** format in Sympy outputs!


```python
sym.init_printing()
```

#### Defining indices of $\Gamma$ Polyhedral Tensor  
Let's create first our different kinds of index in our spaces:


```python
Dimension   = TensorIndexType('Dimension'  , dummy_names = 'D' , metric_symmetry=1)
Statement   = TensorIndexType('Statement'  , dummy_names = 'S' , metric_symmetry=1)
Coefficient = TensorIndexType('Coefficient', dummy_names = 'C' , metric_symmetry=1)
```

Then we can instanciate ```d,s,r``` indices for Dimensions, Statements and Coefficients, filling covariant or contravariant attribute when necesessary:


```python
d = TensorIndex('d', Dimension,   is_up = True)  # dimension -> vector space      -> contravariant index
s = TensorIndex('s', Statement,   is_up = False) # statement -> dual vector space -> covariant     index
r = TensorIndex('r', Coefficient, is_up = False) # rank      -> dual vector space -> covariant     index
```

Now we can create our $\Gamma^d_{sr}$ to instanciate a polyhedral tensor for the given C kernel


```python
Gamma = TensorHead('G',[Dimension,Statement,Coefficient])
```

Let's populate our $\Gamma^d_{sr}$ Tensor:


```python
t_i_x,t_j_x,t_k_x,t_N_x,t_1_x = sym.symbols(['t_i_x','t_j_x','t_k_x','t_N_x','t_1_x'])
t_i_y,t_j_y,t_k_y,t_N_y,t_1_y = sym.symbols(['t_i_y','t_j_y','t_k_y','t_N_y','t_1_y'])
e_dxy,e_dyy                   = sym.symbols(['e_dxy','e_dyy'])
```

we know, from kernel dependencies scanning, that ```t_j_x```,```t_k_x``` and ```t_j_y``` are null, but, to keep consistence we define thehm.  
Our problem contains three statements and a constant per dimension, so


```python
R_X = sym.Array([t_i_x,0,0,t_N_x,t_1_x])
R_Y = sym.Array([0,t_j_y,t_k_y,t_N_y,t_1_y])
R_1 = sym.Array([0,0,0,0,e_dxy])
```

And we have ```X``` and ```Y``` statements, we add 1 for tensor algebra compatibility:


```python
S   = sym.Array([R_X,R_Y,R_1])
S
```




$\displaystyle \left[\begin{matrix}t_{i x} & 0 & 0 & t_{N x} & t_{1 x}\\0 & t_{j y} & t_{k y} & t_{N y} & t_{1 y}\\0 & 0 & 0 & 0 & e_{dxy}\end{matrix}\right]$



Last, let's make room for 2 dimensions, even if we may only need one of them:


```python
D = sym.Array([S,S])
D
```




$\displaystyle \left[\begin{matrix}\left[\begin{matrix}t_{i x} & 0 & 0 & t_{N x} & t_{1 x}\\0 & t_{j y} & t_{k y} & t_{N y} & t_{1 y}\\0 & 0 & 0 & 0 & e_{dxy}\end{matrix}\right] & \left[\begin{matrix}t_{i x} & 0 & 0 & t_{N x} & t_{1 x}\\0 & t_{j y} & t_{k y} & t_{N y} & t_{1 y}\\0 & 0 & 0 & 0 & e_{dxy}\end{matrix}\right]\end{matrix}\right]$



Let's fill our $\Gamma^d_{sr}$ Tensor with C Kernel information:


```python
T_Gamma = Gamma(d,s,r).replace_with_arrays({Gamma(d,s,r):D})
```


```python
T_Gamma
```




$\displaystyle \left[\begin{matrix}\left[\begin{matrix}t_{i x} & 0 & 0 & t_{N x} & t_{1 x}\\0 & t_{j y} & t_{k y} & t_{N y} & t_{1 y}\\0 & 0 & 0 & 0 & e_{dxy}\end{matrix}\right] & \left[\begin{matrix}t_{i x} & 0 & 0 & t_{N x} & t_{1 x}\\0 & t_{j y} & t_{k y} & t_{N y} & t_{1 y}\\0 & 0 & 0 & 0 & e_{dxy}\end{matrix}\right]\end{matrix}\right]$



The purpose of the Polyhedral Model is to find $\Gamma^d_{sr}$ coefficients optimaly throught integer Linear Programming.   
To do so, it is time to introduce affine schedule constyraints derived from C Kernel  
```
  f(ic, jc, kc)     = T_Y_0(jc,kc) - T_X_0(ic)     - e_DXY is non negative everywhere in D_XY
  g(js, ks, jt, kt) = T_Y_0(jt,kt) - T_Y_0(js, ks) - e_DYY is non negative everywhere in D_YY
  with
  0 <= e_DXY <= 1  
  0 <= e_DYY <= 1   
```
Polyhedral theory combined with an interpretation of [Farkas Lemma](https://en.wikipedia.org/wiki/Farkas%27_lemma) states that :    

$$f(ic,jc,kc)=\lambda_iB^i$$  
where $B^i$ is $i^th$ bound restriction in $D_{xy}$ and $\lambda_i$ are the so-called Farkas *multipliers*.  
Let's inject both left hand side and right hand side equation terms in our tensor model, levering on tensor magic algebra, left hand side is given by the following expression:    

$$f_r(ic,jc,kc)=\Gamma^d_{sr}D_dF^s$$  
$$g_r(js,ks,jt,kt)=\Gamma^d_{sr}D_dG^s$$  
  
where $$D_s=\begin{bmatrix}0\\1\end{bmatrix}$$ and
$$F^s=[-1,1,-1]$$
$$G^s=[1,-1,-1]$$


```python
Dim  = TensorHead('D',[Dimension])
Stmt = TensorHead('F',[Statement])
t    = Gamma(d,s,r)*Dim(-d)*Stmt(-s)
t
```




$\displaystyle G{}^{D_{0}}{}_{S_{0}r}D{}_{D_{0}}F{}^{S_{0}}$




```python
T_t = t.replace_with_arrays({Gamma(d,s,r):D,Dim(-d):[1,0],Stmt(-s):[-1,1,-1]})
```


```python
T_t
```




$\displaystyle \left[\begin{matrix}- t_{i x} & t_{j y} & t_{k y} & - t_{N x} + t_{N y} & - e_{dxy} - t_{1 x} + t_{1 y}\end{matrix}\right]$



Which is a vector living in the vector space $e$ with basis $(i,j,k,N,1)$. Now, let's find the left hand side of our equation $f$.  
Let's define some symbols for our $\lambda$ multipliers.


```python
l0,l1,l2,l3,l4,l5,l6,l7,l8,l9,l10 = sym.symbols(['l0','l1','l2','l3','l4','l5','l6','l7','l8','l9','l10'])
```

Let's load our bounds defined in $D_{xy}$ domain:


```python
                     # i  j  k  N  1            
bounds = sym.Matrix([[ 1, 0, 0, 0, 0 ],  # l1:  i     >= 0
                     [-1, 0, 0, 1, 0 ],  # l2: -i + N >= 0
                     [ 0, 1, 0, 0, 0 ],  # l3:  j     >= 0
                     [ 0,-1, 0, 1, 0 ],  # l4: -j + N >= 0
                     [ 0, 0, 1, 0, 0 ],  # l5:  k     >= 0
                     [ 0, 0,-1, 1, 0 ],  # l6: -k + N >= 0
                     [ 1, 0,-1, 0, 0 ],  # l7:  i - k >= 0
                     [-1, 0, 1, 0, 0 ],  # l8: -i + k >= 0
                     [ 0, 0, 0, 0, 1 ]]) # l9:  e_dxy >= 0
     

```


```python
diag_f   = sym.diag(l1,l2,l3,l4,l5,l6,l7,l8,l9).as_mutable()
Farkas_f = (diag_f * bounds).T
Farkas_f
```




$\displaystyle \left[\begin{matrix}l_{1} & - l_{2} & 0 & 0 & 0 & 0 & l_{7} & - l_{8} & 0\\0 & 0 & l_{3} & - l_{4} & 0 & 0 & 0 & 0 & 0\\0 & 0 & 0 & 0 & l_{5} & - l_{6} & - l_{7} & l_{8} & 0\\0 & l_{2} & 0 & l_{4} & 0 & l_{6} & 0 & 0 & 0\\0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & l_{9}\end{matrix}\right]$



$f_r$ is a vector valued function living in the vector space $e$ with basis $(i,j,k,N,1)$, hence we can write:    
$$f_r\vec{e^r}=\Gamma^d_{sr}D_dF^s\vec{e^r}$$ and equating components we obtain:


```python
o = sym.ones(1,Farkas_f.shape[1]).as_mutable().T
f_rhs = Farkas_f * o
f_eq = (sym.Matrix(T_t)-f_rhs,0)
f_eq = [ a - b for a, b in zip(T_t,f_rhs)]
f_eq
```




$\displaystyle \left[ - l_{1} + l_{2} - l_{7} + l_{8} - t_{i x}, \  - l_{3} + l_{4} + t_{j y}, \  - l_{5} + l_{6} + l_{7} - l_{8} + t_{k y}, \  - l_{2} - l_{4} - l_{6} - t_{N x} + t_{N y}, \  - e_{dxy} - l_{9} - t_{1 x} + t_{1 y}\right]$



in matrix form yields:


```python
sym.linear_eq_to_matrix(f_eq,[l1,l2,l3,l4,l5,l6,l7,l8,l9,t_j_y,t_k_y,t_N_y,t_1_y,t_i_x,t_N_x,t_1_x,e_dxy])
```




$\displaystyle \left( \left[\begin{array}{ccccccccccccccccc}-1 & 1 & 0 & 0 & 0 & 0 & -1 & 1 & 0 & 0 & 0 & 0 & 0 & -1 & 0 & 0 & 0\\0 & 0 & -1 & 1 & 0 & 0 & 0 & 0 & 0 & 1 & 0 & 0 & 0 & 0 & 0 & 0 & 0\\0 & 0 & 0 & 0 & -1 & 1 & 1 & -1 & 0 & 0 & 1 & 0 & 0 & 0 & 0 & 0 & 0\\0 & -1 & 0 & -1 & 0 & -1 & 0 & 0 & 0 & 0 & 0 & 1 & 0 & 0 & -1 & 0 & 0\\0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & -1 & 0 & 0 & 0 & 1 & 0 & 0 & -1 & -1\end{array}\right], \  \left[\begin{matrix}0\\0\\0\\0\\0\end{matrix}\right]\right)$



Let's create the corresponding matrix for $g$ constraint. Now, in this case, our vector space $e$ has a different basis due to the fact that the $g$ function maps points from $(j_s,k_s,j_t,k_t)$. So let's create another $\Gamma^d_{sr}$ tensor to contain this new space vector:


```python
e_dyy  = sym.Symbol('e_dyy')
#                    js    jt    ks     kt     N      1
R_Y_s = sym.Array([t_j_y,  0  ,t_k_y,   0 ,  t_N_y, t_1_y])
R_Y_t = sym.Array([  0  ,t_j_y,  0  ,t_k_y,  t_N_y, t_1_y])
R_1   = sym.Array([  0  ,  0  ,  0  ,  0  ,    0,   e_dyy])
S     = sym.Array([R_Y_s,R_Y_t,R_1])
D     = sym.Array([S,S])
T_t   = t.replace_with_arrays({Gamma(d,s,r):D,Dim(-d):[1,0],Stmt(-s):[1,-1,-1]})
T_t
```




$\displaystyle \left[\begin{matrix}t_{j y} & - t_{j y} & t_{k y} & - t_{k y} & 0 & - e_{dyy}\end{matrix}\right]$




```python
l10,l11,l12,l13,l14,l15,l16,l17,l18,l19,l20,l21 = sym.symbols(['l10','l11','l12','l13','l14','l15','l16','l17','l18','l19','l20','l21'])
#                       js   jt    ks   kt   N   1
bounds = sym.Matrix([[ l10,   0,   0,   0,   0,  0 ], # l10: js          >= 0
                     [-l11,   0,   0,   0, l11,  0 ], # l11:-js + N      >= 0
                     [   0,   0, l12,   0,   0,  0 ], # l12: ks          >= 0
                     [   0,   0,-l13,   0, l13,  0 ], # l13:-ks + N      >= 0
                     [   0, l14,   0,   0,   0,  0 ], # l14: jt          >= 0
                     [   0,-l15,   0,   0, l15,  0 ], # l15:-jt + N      >= 0
                     [   0,   0,   0, l16,   0,  0 ], # l16: kt          >= 0
                     [   0,   0,   0,-l17, l17,  0 ], # l17:-kt + N      >= 0                 
                     [ l18,-l18,   0,   0,   0,  0 ], # l18: js - jt     >= 0
                     [-l19, l19,   0,   0,   0,  0 ], # l19:-js + jt     >= 0
                     [   0,   0, l20,-l20,   0, -1 ], # l20: ks - kt - 1 >= 0
                     [   0,   0,   0,   0,   0,l21 ]  # l21: e_dyy       >= 0
])
Farkas_g = bounds.as_mutable().T
Farkas_g
```




$\displaystyle \left[\begin{array}{cccccccccccc}l_{10} & - l_{11} & 0 & 0 & 0 & 0 & 0 & 0 & l_{18} & - l_{19} & 0 & 0\\0 & 0 & 0 & 0 & l_{14} & - l_{15} & 0 & 0 & - l_{18} & l_{19} & 0 & 0\\0 & 0 & l_{12} & - l_{13} & 0 & 0 & 0 & 0 & 0 & 0 & l_{20} & 0\\0 & 0 & 0 & 0 & 0 & 0 & l_{16} & - l_{17} & 0 & 0 & - l_{20} & 0\\0 & l_{11} & 0 & l_{13} & 0 & l_{15} & 0 & l_{17} & 0 & 0 & 0 & 0\\0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & -1 & l_{21}\end{array}\right]$




```python
o     = ones(1,Farkas_g.shape[1]).as_mutable().T
g_rhs = Farkas_g * o
g_eq  = (sym.Matrix(T_t) - g_rhs,0)
g_eq  = [ a - b for a,b in zip(T_t,g_rhs)]
g_eq
```




$\displaystyle \left[ - l_{10} + l_{11} - l_{18} + l_{19} + t_{j y}, \  - l_{14} + l_{15} + l_{18} - l_{19} - t_{j y}, \  - l_{12} + l_{13} - l_{20} + t_{k y}, \  - l_{16} + l_{17} + l_{20} - t_{k y}, \  - l_{11} - l_{13} - l_{15} - l_{17}, \  - e_{dyy} - l_{21} + 1\right]$




```python
sym.linear_eq_to_matrix(g_eq,[l10,l11,l12,l13,l14,l15,l16,l17,l18,l19,l20,l21,t_j_y,t_k_y,t_N_y,t_1_y,t_i_x,t_N_x,t_1_x,e_dyy])
```




$\displaystyle \left( \left[\begin{array}{cccccccccccccccccccc}-1 & 1 & 0 & 0 & 0 & 0 & 0 & 0 & -1 & 1 & 0 & 0 & 1 & 0 & 0 & 0 & 0 & 0 & 0 & 0\\0 & 0 & 0 & 0 & -1 & 1 & 0 & 0 & 1 & -1 & 0 & 0 & -1 & 0 & 0 & 0 & 0 & 0 & 0 & 0\\0 & 0 & -1 & 1 & 0 & 0 & 0 & 0 & 0 & 0 & -1 & 0 & 0 & 1 & 0 & 0 & 0 & 0 & 0 & 0\\0 & 0 & 0 & 0 & 0 & 0 & -1 & 1 & 0 & 0 & 1 & 0 & 0 & -1 & 0 & 0 & 0 & 0 & 0 & 0\\0 & -1 & 0 & -1 & 0 & -1 & 0 & -1 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0\\0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & -1 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & -1\end{array}\right], \  \left[\begin{matrix}0\\0\\0\\0\\0\\-1\end{matrix}\right]\right)$



combining $f$ and $g$ together


```python
eq = f_eq + g_eq
eq
```




$\displaystyle \left[ - l_{1} + l_{2} - l_{7} + l_{8} - t_{i x}, \  - l_{3} + l_{4} + t_{j y}, \  - l_{5} + l_{6} + l_{7} - l_{8} + t_{k y}, \  - l_{2} - l_{4} - l_{6} - t_{N x} + t_{N y}, \  - e_{dxy} - l_{9} - t_{1 x} + t_{1 y}, \  - l_{10} + l_{11} - l_{18} + l_{19} + t_{j y}, \  - l_{14} + l_{15} + l_{18} - l_{19} - t_{j y}, \  - l_{12} + l_{13} - l_{20} + t_{k y}, \  - l_{16} + l_{17} + l_{20} - t_{k y}, \  - l_{11} - l_{13} - l_{15} - l_{17}, \  - e_{dyy} - l_{21} + 1\right]$




```python
sym.linear_eq_to_matrix(eq,[l1,l2,l3,l4,l5,l6,l7,l8,l9,l10,l11,l12,l13,l14,l15,l16,l17,l18,l19,l20,l21,t_j_y,t_k_y,t_N_y,t_1_y,t_i_x,t_N_x,t_1_x,e_dxy,e_dyy])
```




$\displaystyle \left( \left[\begin{array}{cccccccccccccccccccccccccccccc}-1 & 1 & 0 & 0 & 0 & 0 & -1 & 1 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & -1 & 0 & 0 & 0 & 0\\0 & 0 & -1 & 1 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 1 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0\\0 & 0 & 0 & 0 & -1 & 1 & 1 & -1 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 1 & 0 & 0 & 0 & 0 & 0 & 0 & 0\\0 & -1 & 0 & -1 & 0 & -1 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 1 & 0 & 0 & -1 & 0 & 0 & 0\\0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & -1 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 1 & 0 & 0 & -1 & -1 & 0\\0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & -1 & 1 & 0 & 0 & 0 & 0 & 0 & 0 & -1 & 1 & 0 & 0 & 1 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0\\0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & -1 & 1 & 0 & 0 & 1 & -1 & 0 & 0 & -1 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0\\0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & -1 & 1 & 0 & 0 & 0 & 0 & 0 & 0 & -1 & 0 & 0 & 1 & 0 & 0 & 0 & 0 & 0 & 0 & 0\\0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & -1 & 1 & 0 & 0 & 1 & 0 & 0 & -1 & 0 & 0 & 0 & 0 & 0 & 0 & 0\\0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & -1 & 0 & -1 & 0 & -1 & 0 & -1 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0\\0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & -1 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & -1\end{array}\right], \  \left[\begin{matrix}0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\-1\end{matrix}\right]\right)$




```python

```
