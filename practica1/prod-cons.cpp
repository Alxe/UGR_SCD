// *****************************************************************************
//
// Prácticas de SCD. Práctica 1.
// Plantilla de código para el ejercicio del productor-consumidor con
// buffer intermedio.
//
// *****************************************************************************

#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> // necesario para {\ttbf usleep()}
#include <stdlib.h> // necesario para {\ttbf random()}, {\ttbf srandom()}
#include <time.h>   // necesario para {\ttbf time()}

using namespace std ;

// ---------------------------------------------------------------------
// constantes configurables:

const unsigned int
  num_items  = 40 ,    // numero total de items que se producen o consumen
  tam_vector = 5 ;    // tamaño del vector, debe ser menor que el número de items
  
  
unsigned int datos[tam_vector];

unsigned int
  num_primero = 0,
  num_ultimo = 0;

sem_t
	sem_lectura,
  sem_escritura,
  sem_es;

// ---------------------------------------------------------------------
// introduce un retraso aleatorio de duración comprendida entre
// 'smin' y 'smax' (dados en segundos)

void retraso_aleatorio( const float smin, const float smax )
{
  static bool primera = true ;
  if ( primera )        // si es la primera vez:
  {  srand(time(NULL)); //   inicializar la semilla del generador
     primera = false ;  //   no repetir la inicialización
  }
  // calcular un número de segundos aleatorio, entre {\ttbf smin} y {\ttbf smax}
  const float tsec = smin+(smax-smin)*((float)random()/(float)RAND_MAX);
  // dormir la hebra (los segundos se pasan a microsegundos, multiplicándos por 1 millón)
  usleep( (useconds_t) (tsec*1000000.0)  );
}


// ---------------------------------------------------------------------
// función que simula la producción de un dato

unsigned producir_dato()
{
  static int contador = 0 ;
  contador = contador + 1 ;

  retraso_aleatorio( 0.5, 1.5 );

  sem_wait(&sem_es);
  cout << "[Productor]\t\tDato producido: " << contador << endl << flush ;
  sem_post(&sem_es);
  
  return contador ;
}
// ---------------------------------------------------------------------
// función que simula la consumición de un dato

void consumir_dato( int dato )
{
   retraso_aleatorio( 0.5, 1.0 );

   sem_wait(&sem_es);
   cout << "[Consumidor]\t\tDato consumido: " << dato << endl << flush ;
   sem_post(&sem_es);
}
// ---------------------------------------------------------------------
// función que ejecuta la hebra del productor

void * funcion_productor( void * )
{
  for( unsigned i = 0 ; i < num_items ; i++ )
  {
    // Producción de datos
    int dato = producir_dato();

    sem_wait(&sem_escritura);

    datos[(num_ultimo++) % tam_vector] = dato;

    sem_post(&sem_lectura);

    sem_wait(&sem_es);
    cout << "[Productor]\t\tDato insertado: " << dato << endl << flush ;
    sem_post(&sem_es);

  }

  return NULL ;
}
// ---------------------------------------------------------------------
// función que ejecuta la hebra del consumidor

void * funcion_consumidor( void * )
{
  for( unsigned i = 0 ; i < num_items ; i++ )
  {
    int dato ;

    sem_wait(&sem_lectura);

    dato = datos[(num_primero++) % tam_vector];
    
    sem_post(&sem_escritura);

    sem_wait(&sem_es);
    cout << "[Consumidor]\t\tDato extraído : " << dato << endl << flush ;    
    sem_post(&sem_es);

    consumir_dato( dato ) ;
  }
  return NULL ;
}
//----------------------------------------------------------------------

int main()
{
  // Inicialización de semáforos
	sem_init(&sem_escritura, 0, tam_vector);
  sem_init(&sem_lectura, 0, 0);
  sem_init(&sem_es, 0, 1);

  // Inicialización de hilos
  pthread_t th_productor, th_consumidor;

  pthread_create(&th_productor, NULL, funcion_productor, NULL);
  pthread_create(&th_consumidor, NULL, funcion_consumidor, NULL);

  // Espera a los hilos
  pthread_join(th_productor, NULL);
  pthread_join(th_consumidor, NULL);

  // Liberación de semáforos
  sem_destroy(&sem_escritura);
  sem_destroy(&sem_lectura);
  sem_destroy(&sem_es);

  cout << "fin" << endl;

  return 0 ;
}
