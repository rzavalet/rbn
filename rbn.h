#ifndef __RBN__
#define __RBN__

/*****************************************************
Estructura: ATTRACTOR
Contenido:  Lista ligada que contiene información de los atractores
      encontrados
*****************************************************/
typedef struct attractor{
  unsigned          estado;
  unsigned          inicio;
  unsigned          final;
  struct attractor  *next;
}ATTRACTOR;


/*****************************************************
Estructura: RBN
Contenido:  Contiene la información referente a
            una RBN como numero de nodos, numero de 
            enlaces, etc.
*****************************************************/
typedef struct rbn{
  unsigned        numNodos;
  unsigned        numEnlaces;
  float           probabilidad;
  
  unsigned char **conexiones;
  unsigned       *transiciones;
  ATTRACTOR      *atractores;
  FILE           *estadosRBN;
}RBN;


/*****************************************************
FUNCTION: error()
IN        str: Archivo | nro: Línea | err: Cadena de
          información del error
OUT       void      
MAKE      Escribe un mensaje indicando el tipo de error
          encontrado y sale del programa
*****************************************************/
void error( char *str, int nro, char *err );


/*****************************************************
FUNCTION: generarConexiones();
IN        
OUT       
MAKE      Genera la tabla de conexiones del RBN
*****************************************************/
int generarConexiones(RBN *redBooleana);

/*****************************************************
FUNCTION: generarFunciones();
IN        
OUT       
MAKE      Genera las funciones para cada nodo
*****************************************************/
int generarFunciones( RBN *redBooleana );

/*****************************************************
FUNCTION: iterar();
IN        
OUT       
MAKE      Evoluciona la red
*****************************************************/
int iterar( RBN *redBooleana, unsigned numIteraciones, unsigned *historial );

/*****************************************************
FUNCTION: buscarAtractores();
IN        
OUT       
MAKE      Busca la existencia de atractores
*****************************************************/
int buscarAtractores( unsigned numIteraciones, unsigned inicioAtractores, unsigned *historial, ATTRACTOR **atractores  );

int liberarRBN( RBN *redBooleana );

void imprimirConexiones( RBN *redBooleana );
void imprimirFunciones( RBN *redBooleana );
void imprimirEstado( RBN *redBooleana, unsigned estado );
void imprimirGrafico(unsigned estado, unsigned numNodos);

#endif
