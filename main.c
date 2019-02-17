#include<stdio.h>
#include<math.h>
#include<malloc.h>
#include<stdlib.h>
#include "rbn.h"

#define _CRT_SECURE_NO_WARNINGS 1

unsigned  numIteraciones = 100;
unsigned  inicioAtractores;
unsigned  *historial;
RBN     *redBooleana;
ATTRACTOR *atractores;

int main()
{
  int     continuar;
  int     auxNumIteraciones;
  unsigned  numEstados, i;

  redBooleana = (RBN *) malloc( sizeof( RBN ) );

  do {
    printf("\n***************************************************\n");
    printf(  "*************** GENERADOR DE RBNs *****************\n");
    printf(  "***************************************************\n\n");
  
    /* SOLICITAMOS LOS DATOS ****************************************/
    do {
      printf("\nIntroduzca el numero de nodos de la red --> ");
      scanf("%d", &(redBooleana->numNodos));
    } while(redBooleana->numNodos<0);
  
    do {
      printf("\nIntroduzca el numero de enlances entre nodos --> ");
      scanf("%d", &(redBooleana->numEnlaces));
    } while(redBooleana->numEnlaces<0);
  
    do {
      printf("\nIntroduzca el valor de probabilidad P --> ");
      scanf("%f", &(redBooleana->probabilidad));
    } while( redBooleana->probabilidad<0 || redBooleana->probabilidad>1);


    /* GENERAMOS LAS INTERCONEXIONES *****************************/
    if ( !generarConexiones( redBooleana ) ) 
      error(__FILE__, __LINE__, "Error al generar conexiones...\n");
    imprimirConexiones( redBooleana );

    /* GENERAMOS LAS FUNCIONES BOOLEANAS*****************************/
    if( !generarFunciones( redBooleana ) ) 
      error(__FILE__, __LINE__, "Error al generar Funciones...\n");
    imprimirFunciones( redBooleana );

    /* ABRIR ARCHIVO PARA GUARDAR LOS ATRACTORES ********************/
    if( (redBooleana->estadosRBN = fopen( "estados.txt", "w" )) == NULL ) 
      error(__FILE__, __LINE__, "Error al abrir el archivo...");

    /* RESERVAMOS MEMORIA PARA ALMACENAR EL HISTORIAL **************/
    if( (historial = (unsigned *) malloc( numIteraciones * sizeof( unsigned ) ) ) == NULL )
      error(__FILE__, __LINE__, "Error al asignar memoria...\n");

    /* HACEMOS LA EVOLUCION DE LA RED ***************************/
    numEstados = pow(2, redBooleana->numNodos);
    for( i = 0; i < numEstados; i++ ){

      printf("Estado inicial -> %d - ", i);
      imprimirEstado( redBooleana, i );

      historial[0] = i;

      if( !iterar( redBooleana, numIteraciones, historial ) ) 
        error(__FILE__, __LINE__, "Error durante las iteraciones...\n");

      if( !buscarAtractores( numIteraciones, numIteraciones / 2, historial, &(redBooleana->atractores) ) ) 
        error(__FILE__, __LINE__, "Error al buscar atractores...\n");
    }
      

    /* LIBERAMOS MEMORIA ****************************************/
    fclose( redBooleana->estadosRBN );
    free( historial );
    if (!liberarRBN( redBooleana )) 
      error(__FILE__, __LINE__, "Error al liberar memoria...");

    printf("\nDesea continuar? (1=si, 0=no) --> ");
    scanf("%d", &continuar);

  } while(continuar);

  return 1;
}
