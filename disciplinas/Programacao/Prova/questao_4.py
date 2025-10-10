soma = 0
contador = 0

while True:
    num = float(input("Digite um número positivo (ou menor que 1 para encerrar): "))
    if num < 1:
        break
    soma += num
    contador += 1

if contador > 0:
    media = soma / contador
    print(f"A média dos números digitados é {media:.2f}.")
else:
    print("Nenhum número válido foi digitado.")
