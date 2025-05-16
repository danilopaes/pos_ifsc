n1 = float(input("Informe a primeira nota: "))
n2 = float(input("Informe a segunda nota: "))
n3 = float(input("Informe a terceira nota: "))

p1 = float(input("Informe o peso da primeira nota: "))
p2 = float(input("Informe o peso da segunda nota: "))
p3 = float(input("Informe o peso da terceira nota: "))

# Calculando a média ponderada
media_ponderada = (n1 * p1 + n2 * p2 + n3 * p3) / (p1 + p2 + p3)

# Exibindo a média ponderada
print("A média ponderada das notas é: %.2f" % media_ponderada)
