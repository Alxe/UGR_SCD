#include <iostream>
#include <time.h>      // incluye "time"
#include <unistd.h>    // incluye "usleep"
#include <stdlib.h>    // incluye "rand" y "srand"
#include <mpi.h>

using namespace std;

#define TAG_FORK_HOLD   1
#define TAG_FORK_FREE   2
#define TAG_WAITER_DOWN 3
#define TAG_WAITER_UP   4

#define WAITER_ID 10

void Filosofo( int id, int nprocesos);
void Tenedor ( int id, int nprocesos);
void Camarero();

// ---------------------------------------------------------------------

int main( int argc, char** argv )
{
   int rank, size;
   
   srand(time(0));
   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &rank );
   MPI_Comm_size( MPI_COMM_WORLD, &size );

   if( size!=11)
   {
      if( rank == 0) 
         cout<<"El numero de procesos debe ser 11" << endl << flush ;
      MPI_Finalize( ); 
      return 0; 
   }
   if(rank == WAITER_ID) {
      Camarero();
   } else if ((rank%2) == 0) {  
      Filosofo(rank,size); // Los pares son Filosofos 
   } else { 
      Tenedor(rank,size);  // Los impares son Tenedores 
   }

   MPI_Finalize( );
   return 0;
}  
// ---------------------------------------------------------------------

void Filosofo( int id, int nprocesos )
{
   int izq = (id+1) % (nprocesos-1);
   int der = ((id+nprocesos)-2) % (nprocesos-1);

   MPI_Status status;

   while(1)
   {
      // Solicita sentarse
      MPI_Ssend(NULL, 0, MPI_INT, WAITER_ID, TAG_WAITER_DOWN, MPI_COMM_WORLD);

      // Solicita tenedor izquierdo
      cout << "Filosofo "<<id<< " solicita tenedor izq ..." << izq << endl << flush;
      MPI_Ssend(NULL, 0, MPI_INT, izq, TAG_FORK_HOLD, MPI_COMM_WORLD);

      // Solicita tenedor derecho
      cout <<"Filosofo "<<id<< " coge tenedor der ..." << der << endl << flush;
      MPI_Ssend(NULL, 0, MPI_INT, der, TAG_FORK_HOLD, MPI_COMM_WORLD);

      cout<<"Filosofo "<<id<< " COMIENDO"<<endl<<flush;
      sleep((rand() % 3)+1);  //comiendo
      
      // Suelta el tenedor izquierdo
      cout <<"Filosofo "<<id<< " suelta tenedor izq ..." << izq << endl << flush;
      MPI_Ssend(NULL, 0, MPI_INT, izq, TAG_FORK_FREE, MPI_COMM_WORLD);
      
      // Suelta el tenedor derecho
      cout <<"Filosofo "<<id<< " suelta tenedor der ..." << der << endl << flush;
      MPI_Ssend(NULL, 0, MPI_INT, der, TAG_FORK_FREE, MPI_COMM_WORLD);

      // Avisa de que se levanta
      cout <<"Filosofo "<<id<< " avisa de que se levanta" << endl << flush;
      MPI_Ssend(NULL, 0, MPI_INT, WAITER_ID, TAG_WAITER_UP, MPI_COMM_WORLD);

      
      // Piensa (espera bloqueada aleatorio del proceso)
      cout << "Filosofo " << id << " PENSANDO" << endl << flush;
      
      // espera bloqueado durante un intervalo de tiempo aleatorio 
      // (entre una décima de segundo y un segundo)
      usleep( 1000U * (100U+(rand()%900U)) );
 }
}
// ---------------------------------------------------------------------

void Tenedor(int id, int nprocesos)
{
  int buf; 
  MPI_Status status; 
  int Filo;
  
  while( true )
  {
    // Espera un peticion desde cualquier filosofo vecino ...
    MPI_Recv(NULL, 0, MPI_INT, MPI_ANY_SOURCE, TAG_FORK_HOLD, MPI_COMM_WORLD, &status);
    
    // Recibe la peticion del filosofo ...
    Filo = status.MPI_SOURCE;
    
    cout << "Ten. " << id << " recibe petic. de " << Filo << endl << flush;
    
    // Espera a que el filosofo suelte el tenedor...
    MPI_Recv(NULL, 0, MPI_INT, Filo, TAG_FORK_FREE, MPI_COMM_WORLD, &status);
    cout << "Ten. " << id << " recibe liberac. de " << Filo << endl << flush; 
  }
}
// ---------------------------------------------------------------------

void Camarero() {
  int sitting = 0;

  MPI_Status status;

  while(true) {
    if(sitting < 4) {
      MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    } else {
      MPI_Probe(MPI_ANY_SOURCE, TAG_WAITER_UP, MPI_COMM_WORLD, &status);
    
    }

    int filo = status.MPI_SOURCE;

    if(status.MPI_TAG == TAG_WAITER_DOWN) {
      MPI_Recv(NULL, 0, MPI_INT, filo, TAG_WAITER_DOWN, MPI_COMM_WORLD, &status);
      sitting += 1;

      MPI_Send(NULL, 0, MPI_INT, filo, TAG_WAITER_DOWN, MPI_COMM_WORLD);
       cout << "Filosofo " << filo << " se sienta" << endl << flush;
    
    } else if(status.MPI_TAG == TAG_WAITER_UP) {
      MPI_Recv(NULL, 0, MPI_INT, filo, TAG_WAITER_UP, MPI_COMM_WORLD, &status);
      sitting -= 1;

      cout << "Filosofo " << filo << " se levanta" << endl << flush;

    }
  }
}