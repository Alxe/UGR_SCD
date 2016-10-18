// *****************************************************************************
//
// Prácticas de SCD. Práctica 1.
// Plantilla de código para el ejercicio de los fumadores
//
// *****************************************************************************

#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>      // incluye "time(....)"
#include <unistd.h>    // incluye "usleep(...)"
#include <stdlib.h>    // incluye "rand(...)" y "srand"

using namespace std ;

const char ingredientes[][9] = {"Papel", "Tabaco", "Cerillas"}; // 0, 1, 2

sem_t sem_estanquero,
  sem_fumadores[3],
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

// ----------------------------------------------------------------------------
// función que simula la acción de fumar, como un retardo aleatorio de la hebra.
// recibe como parámetro el numero de fumador
// el tiempo que tarda en fumar está entre dos y ocho décimas de segundo.

void fumar( int num_fumador )
{
  sem_wait(&sem_es);
  cout << "Fumador número " << num_fumador << " (sin " << ingredientes[num_fumador] << "): comienza a fumar." << endl << flush ;
  sem_post(&sem_es);

  retraso_aleatorio( 2.2, 2.8 );

  sem_wait(&sem_es);
  cout << "Fumador número " << num_fumador << " (sin " << ingredientes[num_fumador] << "): termina de fumar." << endl << flush ;
  sem_post(&sem_es);
}
// ----------------------------------------------------------------------------

void* f_estanquero(void *arg) {
  while(true) {
    sem_wait(&sem_estanquero);
      
    int i = ((int) random()) % 3;

    sem_wait(&sem_es);
    cout << "El estanquero pone " << ingredientes[i] << " sobre la mesa" << endl << flush;
    sem_post(&sem_es);

    sem_post(&sem_fumadores[i]);
  }
}

void* f_fumador(void *arg) {
  int i = (int) arg;

  while(true) {
    sem_wait(&sem_fumadores[i]);
    sem_post(&sem_estanquero);

    fumar(i);

  }
}

// ----------------------------------------------------------------------------

int main()
{
  srand( time(NULL) ); // inicializa semilla aleatoria para selección aleatoria de fumador
  
  // Inicialización de semáforos
  sem_init(&sem_estanquero, 0, 1);
  sem_init(&sem_es, 0, 1);
  for(int i = 0; i < 3; i++) sem_init(&sem_fumadores[i], 0 ,0);

  // Inicialización de hilos
  pthread_t t_estanquero,
    t_fumador_papel,
    t_fumador_tabaco,
    t_fumador_cerilla;

  pthread_create(&t_estanquero, NULL, f_estanquero, NULL);
  pthread_create(&t_fumador_papel, NULL, f_fumador, (void*) 0);
  pthread_create(&t_fumador_tabaco, NULL, f_fumador, (void*) 1);
  pthread_create(&t_fumador_cerilla, NULL, f_fumador, (void*) 2);

  // Espera a los hilos
  pthread_join(t_estanquero, NULL);
  pthread_join(t_fumador_papel, NULL);
  pthread_join(t_fumador_tabaco, NULL);
  pthread_join(t_fumador_cerilla, NULL);

  // Liberación de semáforos
  sem_destroy(&sem_estanquero);
  sem_destroy(&sem_es);
  for(int i = 0; i < 3; i++) sem_destroy(&sem_fumadores[i]);

  return 0 ;
}
