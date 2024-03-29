

# This file was *autogenerated* from the file test.sage
from sage.all_cmdline import *   # import sage library

_sage_const_2 = Integer(2); _sage_const_15 = Integer(15); _sage_const_1 = Integer(1); _sage_const_3 = Integer(3); _sage_const_12 = Integer(12); _sage_const_4 = Integer(4)

F1 = GF(_sage_const_2  ** _sage_const_15 , modulus = x**_sage_const_15  + x + _sage_const_1 , names=('t',)); (t,) = F1._first_ngens(1);

a = t**_sage_const_3  + _sage_const_1 
b = t**_sage_const_12  + _sage_const_1 

F2 = GF(_sage_const_2  ** _sage_const_4 , names=('u',)); (u,) = F2._first_ngens(1)

for a in F2:
    print(a)

print("-----------");

for a in F2:
    print(-a);

print("-----------");

for a in F2:
    print(a**_sage_const_2 )



