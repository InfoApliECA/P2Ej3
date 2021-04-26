/*
 * Ejercicio3.c
 *
 *  Created on: 13 abr. 2021
 *      Author: Hector Coscia
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

/* ###### Declaracion del tipo APint ##### */
typedef struct{
	char sign;
	unsigned char num_bytes;
	unsigned char* bytes;
} APint;

/* ###### Funcion extra para imprimir los enteros de precision arbitraria ##### */
void imprimir_APint(APint *E){
	/* Se apoya en la funcion itoa() para mostrar el contenido binario
	 * de cada byte de E. Recorre el arreglo en forma inversa para imprimir
	 * correctamente (El Byte mas significativo va a la izquierda (primero).
	 * Ademas muestra el signo y completa el String para que tenga longitud 8.
	 */

/* ###### Buffer para itoa ##### */
	char buf[9];

/* ###### Imprimo el signo ##### */
	printf("%+dx ",E->sign);

/* ###### Imprimo los bytes en orden inverso y completo longitud a 8 ##### */
	for (int i = E->num_bytes-1; i >= 0 ; i--){
		printf("%8s  ",itoa(E->bytes[i],buf,2));
	}
	printf("\n");
}

/* ###### Funcion suma para APint ##### */
APint *suma_APint(APint *i1, APint *i2){
	/* V1.0:
	 * Gracias a que se separa el signo, si ambos numeros
	 * tienen el mismo signo, voy a sumar numeros positivos.
	 *
	 * En esta version:
	 * ###############
	 *
	 * Se suman numeros con mismo signo.
	 *
	 * Se deja la responsabilidad al programador de que i1 e i2 sean del tamaño
	 * adecuado (cantidad de bytes) para que no haya OF y, ademas, sean del MISMO
	 * tamaño
	 */

/* ###### Declaro e inicializo variables locales ##### */
	unsigned short sumando1 	= 0; /* Voy a almacenar cada byte en una variable de 2 bytes para */
	unsigned short sumando2 	= 0; /* Poder considerar el acarreo y no tener problemas de OF    */
	unsigned short acarreo  	= 0; /* Excepto en el ultimo Byte (En esta version queda como 	  */
	unsigned short suma			= 0; /* Responsabilidad del programador controlarlo)              */

/* ###### Chequeo que los dos numeros esten representados con la misma cantidad de bytes ##### */
	if (i1->num_bytes != i2->num_bytes){
		printf("Error: suma_APint() no soporta enteros de distinto tamaño.\n");
		return NULL; /* Es mas facil detectar que devuelve un puntero nulo que otra cosa */
	}

/* ###### Chequeo que los dos numeros sean del mismo signo ##### */
	if (i1->sign != i2->sign){
		printf("Error: suma_APint() no soporta enteros de distinto signo todavia.\n");
		return NULL; /* Es mas facil detectar que devuelve un puntero nulo que otra cosa */
	}

/* ###### Reservo memoria para la suma ##### */
	APint *suma_ptr = (APint *) malloc(sizeof(APint));

/* ###### IMPORTANTE! Chequeo que haya podido reservar la memoria ##### */
if (suma_ptr == NULL){
	printf("Error: suma_APint() no pudo reservar memoria para suma_ptr.\n");
	return NULL;
}

/* ###### Inicializo suma ##### */
	suma_ptr->sign 		= i1->sign;
	suma_ptr->num_bytes	= i1->num_bytes;
	suma_ptr->bytes 	= (unsigned char *) calloc(suma_ptr->num_bytes,sizeof(unsigned char));

/* ###### IMPORTANTE! Chequeo que haya podido reservar la memoria ##### */
	if (suma_ptr->bytes == NULL){
		printf("Error: suma_APint() no pudo reservar memoria para los bytes de la suma.\n");
		return NULL;
	}

/* ###### Realizo la suma byte por byte (almacenando en short) ##### */
	for (int i = 0; i < i1->num_bytes; i++){
		sumando1	= (unsigned short) i1->bytes[i];
		sumando2 	= (unsigned short) i2->bytes[i];
		suma 		= acarreo + sumando1 + sumando2;
		acarreo 	= ( suma >> (CHAR_BIT) ); /* CHAR_BIT = 8 */
		suma_ptr->bytes[i] = (unsigned char)(suma);
	}

/* ###### Retorno el resultado ##### */
	return suma_ptr;
}

/* ###### Funcion extra para desplazar un APint nbits lugares a la izquierda ##### */
						/* (Necesaria para el producto) */
APint *DI_APint(APint *E, int nbits){
/* Como el desplazamiento puede ser mayor a 8, se puede dejar nbits / 8 (division entera)
 * bytes enteros en cero y desplazar, a partir del siguiente, nbits % 8 (resto de la division
 * entera) lugares (a ese desplazamiento lo llamo, arbitrariamente "Desplazamiento efectivo".
 */
	unsigned int bytes_en_cero  = nbits / 8;
	unsigned int despl_efectivo = nbits % 8;

/* Uso ademas la variable bits_sup para almacenar los bits superiores (los que se van a "perder"
 * en el desplazamiento) y la variable acarreo para pasar esos bits del byte actual al proximo.
 */
	unsigned char bits_sup   	= 0;
	unsigned char acarreo		= 0;

/* ###### Variable de retorno ##### */
	APint *resultado = (APint *) malloc (sizeof(APint));

/* ###### IMPORTANTE! Chequeo que haya podido reservar la memoria ##### */
	if (resultado == NULL){
		printf("Error: despl_Izq_APint() no pudo reservar memoria para un entero.\n");
		return NULL;
	}

/* ###### Inicializo ##### */
	resultado->sign 		= E->sign;
	resultado->num_bytes	= E->num_bytes;
	resultado->bytes		= (unsigned char *) calloc(resultado->num_bytes,sizeof(unsigned char));

/* ###### IMPORTANTE! Chequeo que haya podido reservar la memoria ##### */
	if (resultado->bytes == NULL){
		printf("Error: despl_Izq_APint() no pudo reservar memoria para los bytes del resultado.\n");
		return NULL;
	}

/* ###### Proceso el desplzamiento ##### */
	for(int i = bytes_en_cero; i < resultado->num_bytes; i++ ){
		/* Tomo el primer byte de E y lo pongo luego de los bytes en cero del resultado */
		resultado->bytes[i] = E->bytes[i-bytes_en_cero];

		/* Enmascaro los bits superiores */
		bits_sup			= (resultado->bytes[i]) & ( ~0 << (CHAR_BIT - despl_efectivo));

		/* Calculo el desplazamiento complementando con el acarreo (la primera vez es cero) */
		resultado->bytes[i] = ((resultado->bytes[i]) << despl_efectivo) | acarreo;

		/* Ajusto el acarreo para la proxima iteracion a partir de los bits superiores del actual */
		acarreo 			= bits_sup >> (CHAR_BIT - despl_efectivo);
	}

/* ###### Retorno el resultado ##### */
	return resultado;
}

/* ###### Funcion producto para APint ##### */
APint * producto_APint(APint * i1, APint * i2){
/* ###### Variable de retorno ##### */
	APint *resultado = (APint *) malloc (sizeof(APint));

/* ###### IMPORTANTE! Chequeo que haya podido reservar la memoria ##### */
	if (resultado == NULL){
		printf("Error: producto_APint() no pudo reservar memoria para un entero.\n");
		return NULL;
	}

/* ###### Inicializo ##### */
/* (inicialmente voy a dejar el signo igual
 * al de los factores para que no me devuelva
 * un error la funcion suma).
 */
	resultado->sign 		= i1->sign;
	resultado->num_bytes	= i1->num_bytes;
	resultado->bytes 	= (unsigned char *) calloc(resultado->num_bytes,sizeof(unsigned char));

/* ###### IMPORTANTE! Chequeo que haya podido reservar la memoria ##### */
	if (resultado->bytes == NULL){
		printf("Error: suma_APint() no pudo reservar memoria para los bytes de la suma.\n");
		return NULL;
	}

/* ###### Variable auxiliar para almacenar los desplazamientos de la variable ##### */
	APint *aux;

/* ###### Proceso el producto. Debo verificar cada bit de cada byte de un factor. ##### */
	for (int i = 0; i < i2->num_bytes; i++){
		for(int j = 0; j < CHAR_BIT; j++){
			if( (i2->bytes[i] & (1 << j)) ){
				aux = DI_APint(i1, j + i * 8);
				resultado = suma_APint(resultado,aux);
				free(aux);
			}
		}
	}

/* ###### Ahora si, con el contenido binario procesado, ajusto el signo. ##### */
	if (i1->sign != i2->sign){
		resultado->sign = -1;
	}else{
		resultado->sign = 1;
	}

/* ###### Retorno el resultado ##### */
	return resultado;
}

/* ###### Funcion main() #### */
int main(){
/* ###### Declaro las variables #### */
	APint *E1,*E2,*S;

/* ###### Reservo espacio en memoria para una de las variables #### */
	E1 = (APint *) malloc (sizeof(APint));

/* ###### IMPORTANTE! Chequeo que haya podido reservar la memoria ##### */
	if (E1 == NULL){
		printf("Error: main() no pudo reservar memoria para un entero.\n");
		return EXIT_FAILURE;
	}

/* ###### Inicializacion ##### */
	E1->sign 		= -1;
	E1->num_bytes 	= 2;
	E1->bytes 		= (unsigned char *) calloc(E1->num_bytes,sizeof(unsigned char));

/* ###### IMPORTANTE! Chequeo que haya podido reservar la memoria ##### */
	if (E1->bytes != NULL){
		/* E1 = 1362 */
		E1->bytes[0] = 82;
		E1->bytes[1] =  5;
	}else{
		printf("Error: main() no pudo reservar los bytes para el entero. \n");
		return EXIT_FAILURE;
	}

/* ##### Muestro E1 #### */
	printf("Variables: \n");
	printf("E1: ");
	imprimir_APint(E1);

/* ###### Reservo espacio en memoria para la otra variable #### */
	E2 = (APint *) malloc (sizeof(APint));

/* ###### IMPORTANTE! Chequeo que haya podido reservar la memoria ##### */
	if (E2 == NULL){
		printf("Error: main() no pudo reservar memoria para un entero.\n");
		return EXIT_FAILURE;
	}

/* ###### Inicializacion ##### */
	E2->sign 		= -1;
	E2->num_bytes 	= 2;
	E2->bytes 		= (unsigned char *) calloc(E2->num_bytes,sizeof(unsigned char));

/* ###### IMPORTANTE! Chequeo que haya podido reservar la memoria ##### */
	if (E2->bytes != NULL){
		E2->bytes[0] = 7;
		E2->bytes[1] = 0;
	}else{
		printf("Error: main() no pudo reservar los bytes para el entero. \n");
		return EXIT_FAILURE;
	}

/* ##### Muestro E2 #### */
	printf("E2: ");
	imprimir_APint(E2);

/* ###### Reservo espacio en memoria para la variable SUMA #### */
	S = (APint *) malloc (sizeof(APint));

/* ###### IMPORTANTE! Chequeo que haya podido reservar la memoria ##### */
	if (S == NULL){
		printf("Error: main() no pudo reservar memoria para la suma.\n");
		return EXIT_FAILURE;
	}

/* ###### Calculo la suma de E1 CON E1 #### */
	S = suma_APint(E1,E1);

/* ###### Muestro el resultado #### */
	printf("\n");
	printf("Suma de E1 con E1: \n");
	printf("E1: ");
	imprimir_APint(E1);

	printf("+\n");

	printf("E1: ");
	imprimir_APint(E1);

	printf("--------------------------\n");

	printf("S : ");
	imprimir_APint(S);

/* ##### Ejemplos de desplazamientos. Cinco lugares (menor a 8) y 10 lugares (mayor a 8 ####
 * un byte queda en cero completamente y desplazo los demas en dos lugares
 */
	APint *D5,*D10;
	D5 = DI_APint(E1,5);
	D10 = DI_APint(E1,10);

/* ###### Muestro el resultado #### */
	printf("\n");
	printf("Desplazamientos: \n");
	printf("E1: ");
	imprimir_APint(E1);

	printf("--------------------------\n");

	printf("D5: ");
	imprimir_APint(D5);

	printf("--------------------------\n");

	printf("D10 ");
	imprimir_APint(D10);

/* ###### Ahora pruebo el producto #### */
	APint *prod;

	prod = producto_APint(E1,E2);

/* ###### Muestro el resultado #### */
	printf("\n");
	printf("Producto de E1 con E2: \n");
	printf("E1: ");
	imprimir_APint(E1);
	printf("E2: ");
	imprimir_APint(E2);
	printf("--------------------------\n");
	printf("P : ");
	imprimir_APint(prod);

/* ###### Libero la memoria pedida (siempre en orden inverso a como la pedí) #### */
	free(E1->bytes);
	free(E1);
	free(S->bytes);
	free(S);
	free(D5->bytes);
	free(D5);
	free(D10->bytes);
	free(D10);
	free(prod->bytes);
	free(prod);

	return EXIT_SUCCESS;
}
