a = int(input("informe um numero: "))
b = int(input("informe um numero: "))
c = int(input("informe um numero: "))

if (a > b ):
  aux = a
  a = b
  b = aux
if ( a > c):
  aux = a
  a = c
  c = aux
if ( b > c):
  aux = b
  b = c
  c = aux
print(a, b, c)