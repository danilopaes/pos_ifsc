numeros = []
for i in range(15):
    num = int(input(f"Digite o número {i + 1}: "))
    numeros.append(num)

print("\nListagem de números:")
for i, num in enumerate(numeros, start=1):
    if num % 2 == 0:
        print(f"{i}: {num} - Par")
    else:
        print(f"{i}: {num} - Ímpar")
