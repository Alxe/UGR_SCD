// *********************************************************************
// SCD. Ejemplos del seminario 1.
//
// Plantilla para el ejercicio de cálculo de PI
// *********************************************************************

#include <iostream>
#include <pthread.h>
#include "fun_tiempo.h"

using namespace std ;

// ---------------------------------------------------------------------
// constante y variables globales (compartidas entre hebras)

unsigned long m = long(1024)*long(1024) ;  // número de muestras (un millón)
unsigned n      = 4 ;                      // número de hebras

double *resultado_parcial = NULL ; // tabla de sumas parciales (una por hebra)

// ---------------------------------------------------------------------
// implementa función $f$

double f( double x )       
{ 
   return 4.0/(1+x*x) ;     // $~~~~f(x)\,=\,4/(1+x^2)$
}
// ---------------------------------------------------------------------
// cálculo secuencial

double calcular_integral_secuencial( )
{  
   double suma = 0.0 ;                      // inicializar suma
   for( unsigned long i = 0 ; i < m ; i++ ) // para cada $i$ entre $0$ y $m-1$
      suma += f( (i+0.5)/m );               // $~~~~~$ añadir $f(x_i)$ a la suma actual
   return suma/m ;                          // devolver valor promedio de $f$
}
// ---------------------------------------------------------------------
// función que ejecuta cada hebra

void * funcion_hebra( void * ih_void ) 
{  
   unsigned long ih = (unsigned long) ih_void ; // número o índice de esta hebra
   double sumap = 0.0 ;
   // calcular suma parcial en "sumap"
   for(int i = ih; i < m; i += n) {
		sumap += f( (i+0.5)/m ); 
	}
   resultado_parcial[ih] = sumap ; // guardar suma parcial en vector.
   return NULL ;
}
// ---------------------------------------------------------------------
// cálculo concurrente

double calcular_integral_concurrente( )
{  
	// crear y lanzar $n$ hebras, cada una ejecuta "funcion\_concurrente"
	pthread_t *threads = new pthread_t[n];
	resultado_parcial = new double[n];
	for(int i = 0; i < n; i++) {
		pthread_create(&threads[i], NULL, funcion_hebra, (void*) i);
	}
	// esperar (join) a que termine cada hebra, sumar su resultado
	double sumat = 0.0;
	for(int i = 0; i < n; i++) {
		pthread_join(threads[i], NULL);
		sumat += resultado_parcial[i];
	}
	
	delete[] threads;
	delete[] resultado_parcial;
	
	// devolver resultado completo
	return sumat / m ; // (cambiar)
}

// ---------------------------------------------------------------------

int main(int argc, char **argv)
{

	if(argc < 3
		|| (n = (unsigned long) atol(argv[1])) <= 0 
		|| (m = (unsigned long) atol(argv[2])) <= 0) {
		cout << "Forma de uso: " << argv[0] << " <hilos> <muestras>" << endl;
		return -1;
	}

	cout << "Cálculo de PI" << endl ;
	double pi_sec = 0.0, pi_conc = 0.0 ;

	struct timespec start_seq = ahora();
	pi_sec  = calcular_integral_secuencial() ;
	struct timespec end_seq = ahora();
	
	struct timespec start_mult = ahora();
	pi_conc = calcular_integral_concurrente() ;
	struct timespec end_mult = ahora();

	cout << "valor de PI (calculado secuencialmente)  == " << pi_sec  << " (en " << duracion(&start_seq, &end_seq) << " segundos)" << endl 
		<< "valor de PI (calculado concurrentemente) == " << pi_conc  << " (en " << duracion(&start_mult, &end_mult) << " segundos)" << endl ; 

	return 0 ;
}
// ---------------------------------------------------------------------



// ----------------------------------------------------------------------------
