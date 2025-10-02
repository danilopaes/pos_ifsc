#include <stdio.h>
#include <string.h>

#define PI 3.1415

int main(void) {
  float x, y, z;
  char sexo;
  char nome[40];

  x = 4;
  printf("O valor de x é: %f\n",x);
  x = x + 2;
  printf("O valor de x agora é: %f\n",x);
  //x = x + 4;
  y = 2.5;
  printf("O valor de y é: %f\n",y);

  sexo = 'M';
  printf("O sexo é: %c\n",sexo);
  strcpy(nome, "maria"); 
  printf("O nome é: %s\n",nome);

	return 0;
}
