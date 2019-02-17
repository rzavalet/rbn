#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include "rbn.h"

void error( char *str, int nro, char *err ){
  fprintf( stderr, "\n%s (%d). ERROR: %s. \n", str, nro, err);
  system("PAUSE");
  exit( -1 );
}

int generarConexiones(RBN *redBooleana){
  unsigned i, j;
  unsigned conexionesExitosas;
  int aleatorio, repetido;
  unsigned *hash;
  printf("\nGenerando Conexiones...\n");
  
  if( (redBooleana->conexiones = (unsigned char **) malloc( redBooleana->numNodos * sizeof( unsigned char * ) ) ) == NULL )
    error(__FILE__, __LINE__, "Error al asignar memoria...\n");

  for( i = 0; i < redBooleana->numNodos; i++ ){
    if( ( redBooleana->conexiones[i] = (unsigned char *) malloc( redBooleana->numEnlaces * sizeof( unsigned char ) ) ) == NULL )
      error(__FILE__, __LINE__, "Error al asignar memoria...\n");
  }

  if( (hash = (unsigned *) malloc( redBooleana->numNodos * sizeof( unsigned ) ) ) == NULL )
    error(__FILE__, __LINE__, "Error al asignar memoria...\n");

  for( i = 0; i < redBooleana->numNodos; i++ ){
    hash[i] = 0;
    for( j = 0; j < redBooleana->numEnlaces; j++ )
      redBooleana->conexiones[i][j] = -1;
  }
      
  for( i = 0; i < redBooleana->numNodos; i++ ){
        printf("Conexiones del nodo %d.................", i);
    conexionesExitosas = 0;
    for( j = 0; j < redBooleana->numNodos; j++ )
      hash[j] = 0;

    while( conexionesExitosas < redBooleana->numEnlaces ){
      aleatorio = rand() % redBooleana->numNodos;
      if( hash[aleatorio] == 0 ){
        redBooleana->conexiones[i][conexionesExitosas] = aleatorio;
        hash[aleatorio] = 1;
        conexionesExitosas ++;
      }
    }
    printf("GENERADO\n");
  }
  return 1;
}

int generarFunciones( RBN *redBooleana ){
  unsigned i,j;
  unsigned combinaciones = (unsigned) pow( 2, redBooleana->numEnlaces );

  printf("\nGenerando funciones...\n");
  printf("Tamaño de la tabla de funciones................... %u\n", combinaciones);
  if( (redBooleana->transiciones = (unsigned *) malloc( combinaciones * sizeof( unsigned ) ) ) == NULL )
    error(__FILE__, __LINE__, "Error al asignar memoria...\n");

  for( i = 0; i < combinaciones; i++ ){
         redBooleana->transiciones[i] = 0;
       for( j = 0; j < redBooleana->numNodos; j++ ){
              if( rand() % (100+1) <= (redBooleana->probabilidad * 100) )
              redBooleana->transiciones[i] += (unsigned) pow( 2, j );         
        }
        printf("Funcion %d............................. %u\n", i, redBooleana->transiciones[i]);
  }
  return 1;
}

int iterar( RBN *redBooleana, unsigned numIteraciones, unsigned *historial ){
  int iteracion, index;
  int i, j;
  int exponente;

  printf("Comenzamos las iteraciones...\n");
  printf("[%3d] %3u ", 0, historial[0]);
  imprimirGrafico(historial[0], redBooleana->numNodos);

  for( iteracion = 1; iteracion < numIteraciones; iteracion++){
    historial[iteracion] = 0;
    for ( i = 0; i < redBooleana->numNodos; i++){
      index = 0;
      for( j = redBooleana->numEnlaces; j>0; j-- ){
        exponente = redBooleana->conexiones[i][redBooleana->numEnlaces-j];
        index += ((historial[iteracion-1] & (unsigned)pow(2, exponente)) == (unsigned)pow(2, exponente) ) * pow( 2, j-1 );
      }
      historial[iteracion] +=  ((redBooleana->transiciones[index] & (unsigned)pow( 2, redBooleana->numEnlaces - i)) == (unsigned)pow( 2, redBooleana->numEnlaces - i)) * (unsigned)pow(2, redBooleana->numNodos-i-1);   
    }
    printf("[%3d] %3u ", iteracion, historial[iteracion]);
    imprimirGrafico(historial[iteracion], redBooleana->numNodos);
  }
  return 1;
}

int buscarAtractores( unsigned numIteraciones, unsigned inicioAtractores, unsigned *historial, ATTRACTOR **atractores  ){
  unsigned aux;
  unsigned i;
  char encontro = 0;

  aux = historial[inicioAtractores - 1];
  printf("Buscando atractores...\n");
  printf("Comenzamos la busqueda en t = %d\n", inicioAtractores);
  for( i = inicioAtractores; i < numIteraciones; i++ ){
    if( historial[i] == aux ){
      printf("Atractor finaliza en t = %d\n", i);
      encontro = 1;
      break;
    }
  }

  if( !encontro ){
    printf("No se encontraron atractores...\n");
    return 1;
  }

  return 1;
}

int liberarRBN( RBN *redBooleana ){
    unsigned i;

  free( redBooleana->transiciones );
  for( i = 0; i < redBooleana->numEnlaces; i++ )
    free( redBooleana->conexiones[i] );

  free( redBooleana->conexiones );

  /* FALTA ELIMINAR LA LISTA LIGADA */
  return 1;
}

void imprimirConexiones( RBN *redBooleana ){     
    unsigned i, j;
    printf("\n\n...");    
  for ( i = 0; i < redBooleana->numEnlaces; i++ ){
        printf("...%d", i);
    }
    putchar('\n');
    printf("---");
    for ( i = 0; i < redBooleana->numEnlaces; i++ ){
        printf("----");
    }
    putchar('\n');
    for ( i = 0; i < redBooleana->numNodos; i++ ){
        printf("%d.|", i);
        for ( j = 0; j < redBooleana->numEnlaces; j++ ){
            printf("...%u", redBooleana->conexiones[i][j]);    
        }
        putchar('\n');
    }
    putchar('\n');
}

void imprimirFunciones( RBN *redBooleana ){
    unsigned i, j;
    printf("\n\n");    
    for ( i = 0; i < redBooleana->numEnlaces ; i++ ){
        printf("...");
    }
    printf(".|.");
    for ( i = (redBooleana->numNodos) - 1; (signed)i >= 0; i-- ){
        printf("...%d", i);
    }
    putchar('\n');
    printf("---");
    for ( i = 0; i < redBooleana->numNodos + redBooleana->numEnlaces + 1; i++ ){
        printf("----");
    }
    putchar('\n');
    for ( i = 0; i < pow( 2, redBooleana->numEnlaces); i++ ){
        for ( j = redBooleana->numEnlaces-1; (signed)j >= 0 ; j-- ){
            printf("..%d",(i&(unsigned)pow(2,j)) == (unsigned)pow(2,j));
        }
        printf(".|.");
        for ( j = redBooleana->numNodos-1; (signed)j >= 0 ; j-- ){
            printf("...%d", (redBooleana->transiciones[i]&(unsigned)pow(2,j)) == (unsigned)pow(2,j));    
        }
        putchar('\n');
    }
    putchar('\n');
}

void imprimirEstado( RBN *redBooleana, unsigned estado ){
  int j;
  printf("(");
    for ( j = redBooleana->numNodos-1; (signed)j >= 0 ; j-- ){
    printf("%d", (estado&(unsigned)pow(2,j)) == (unsigned)pow(2,j));    
    }
  printf(")\n");
}

void imprimirGrafico(unsigned estado, unsigned numNodos){
  int j;
  printf("|");
  for( j = 0; j < numNodos; j++){
    if( estado % 2 )
      printf( "%c", 219);
    else
      printf( "%c", 32);
    estado /= 2;
  }
  printf( "|\n");
}

void imprimirAtractores(RBN *redBooleana, unsigned *historial){
  int i;
  fprintf(redBooleana->estadosRBN, "-----------------------\n");
  for( i=redBooleana->atractores->inicio; i<redBooleana->atractores->final; i++){
    fprintf(redBooleana->estadosRBN, "%d\n", historial[i]);
  }
  fprintf(redBooleana->estadosRBN, "-----------------------\n");
}
