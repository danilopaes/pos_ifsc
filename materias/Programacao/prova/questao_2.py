while True:
    num = int(input("Digite um número entre 1000 e 9999 (ou fora desse intervalo para encerrar): "))
    if num < 1000 or num > 9999:
        break
    parte1 = num // 100
    parte2 = num % 100
    if (parte1 + parte2) ** 2 == num:
        print(f"O número {num} apresenta a característica.")
    else:
        print(f"O número {num} NÃO apresenta a característica.")
