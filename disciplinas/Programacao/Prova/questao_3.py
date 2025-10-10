num = int(input("Digite um número para verificar se é triangular: "))
is_triangular = False
i = 1
while i * (i + 1) * (i + 2) <= num:
    if i * (i + 1) * (i + 2) == num:
        is_triangular = True
        break
    i += 1

if is_triangular:
    print(f"O número {num} é triangular.")
else:
    print(f"O número {num} NÃO é triangular.")
