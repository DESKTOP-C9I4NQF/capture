#!/usr/bin/env   python3
import z3
# TODO: https://stackoverflow.com/questions/7008110/whats-a-simple-and-efficient-method-for-extracting-line-segments-from-a-simple
A, B, C, D, E, F, G, H, I, J = z3.Bools("A B C D E F G H I J")


solution = z3.Solver()

solution.add(z3.And(
        z3.And(z3.Not(z3.Or(A, z3.Not(B))), 
        z3.Not(z3.Or(z3.Or(z3.Not(C), D), z3.Or(E, z3.Not(F))))), 

        z3.And(z3.And(z3.Not(z3.Or(G, H)), z3.Xor(H, I)),
        z3.And(I, J))
))
solution.check()
m = solution.model()
lister = sorted ([(d, m[d]) for d in m], key = lambda x: str(x[0]))
flag = "CTF{" 
for i in lister:
    if i[1] == True:
        flag += str(i[0])
flag += "}"

print(flag)

