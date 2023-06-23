

F1.<t> = GF(2 ^ 15, modulus = x^15 + x + 1);

a = t^3 + 1
b = t^12 + 1

F2.<u> = GF(2 ^ 4)

for a in F2:
    print(a)

print("-----------");

for a in F2:
    print(a^2)


