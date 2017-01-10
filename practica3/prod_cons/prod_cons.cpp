#include <mpi.h>
#include <iostream>
#include <math.h>
#include <time.h>      // incluye "time"
#include <unistd.h>    // incluye "usleep"
#include <stdlib.h>    // incluye "rand" y "srand"

#define TAG_PROD         0
#define TAG_CONS_BUFF    1
#define TAG_BUFF_CONS    2

#define BUFFER_ID        5

#define TAM              5

using namespace std;

void wait() {
   usleep(1000U * (100U+(rand()%900U)));
}

void productor(int rank) {
   int value;
   
   for(int i = 0; i < 4; i++) { 
      value = i ;

      cout << "Productor " << rank << " produce valor " << value << endl << flush ;
      
      wait();
      
      MPI_Ssend(&value, 1, MPI_INT, BUFFER_ID, TAG_PROD, MPI_COMM_WORLD);
   }
}

void consumidor(int rank) {
   int         value,
               peticion = 1;
   MPI_Status  status;
 
   for(int i = 0; i < 5; i++) {
      MPI_Ssend(&peticion, 1, MPI_INT, BUFFER_ID, TAG_CONS_BUFF, MPI_COMM_WORLD); 

      MPI_Recv(&value, 1, MPI_INT, BUFFER_ID, TAG_BUFF_CONS, MPI_COMM_WORLD, &status);

      cout << "Consumidor " << rank << " recibe valor " << value << " de Buffer " << endl << flush ;
      
      wait();
   }
}


void buffer() {
   int        value[TAM] , 
              peticion , 
              pos  =  0,
              rama ;
   MPI_Status status ;
   
   for( unsigned int i=0 ; i < 40 ; i++ )
   {  
      if      (pos == 0)   rama = 0;        
      else if (pos == TAM) rama = 1;           
      else {
         MPI_Probe( MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status );
         
         if(status.MPI_TAG == TAG_PROD) rama = 0; 
         else                         rama = 1;
      }


      switch(rama) {
      case 0:
         MPI_Recv(&value[pos], 1, MPI_INT, MPI_ANY_SOURCE, TAG_PROD, MPI_COMM_WORLD, &status );
         
         cout << "Buffer recibe " << value[pos] << " de Productor " << status.MPI_SOURCE << endl << flush;  
         pos++;
         break;

      case 1:
         MPI_Recv(&peticion, 1, MPI_INT, MPI_ANY_SOURCE, TAG_CONS_BUFF, MPI_COMM_WORLD, &status);

         MPI_Ssend(&value[pos-1], 1, MPI_INT, status.MPI_SOURCE, TAG_BUFF_CONS, MPI_COMM_WORLD);
         
         cout << "Buffer envía " << value[pos-1] << " a Consumidor " << status.MPI_SOURCE << endl << flush;  
         pos--;
         break;
      }     
   }
}   

int main(int argc, char *argv[]) 
{
   int rank,size; 
   
   // inicializar MPI, leer identif. de proceso y número de procesos
   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &rank );
   MPI_Comm_size( MPI_COMM_WORLD, &size );
   
   // inicializa la semilla aleatoria:
   srand ( time(NULL) );
   
   // comprobar el número de procesos con el que el programa 
   // ha sido puesto en marcha (debe ser 10)
   if ( size != 10 ) 
   {
      cout<< "El numero de procesos debe ser 10 "<<endl;
      return 0;
   } 
   
   // verificar el identificador de proceso (rank), y ejecutar la
   // operación apropiada a dicho identificador
   if ( rank < BUFFER_ID ) 
      productor(rank);
   else if ( rank == BUFFER_ID ) 
      buffer();
   else 
      consumidor(rank);
   
   // al terminar el proceso, finalizar MPI
   MPI_Finalize( );
   return 0;
}
