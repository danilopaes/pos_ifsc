#include <stdio.h>
#include <string.h>

#define PI 3.1415
 
int main(void) {
	int x;
	float y, z;
	char sexo;
	char nome[40];
	
	
	x = 4;
	x = x + 2;
	y = 2.5;
	sexo = 'M';
	
	strcpy(nome, "Danilo");
	
	
	printf("informe o valor para a variável x:");
	scanf("%d%*c", &x);
	printf("você informou o valor %d na variável x \n", x);
	
	return 0;
}