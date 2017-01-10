filosofos.cpp
===

El problema de los filosofos con transmision de datos por el protocolo MPI.

Se puede producir interbloqueo si todos los filosofos cogen el tenedor a su izquierda sin llegar a coger el de su derecha. Por esto, se modifica el planteamiento dado para que un filosofo (el primero) sea zurdo y coja los tenedores en el orden inverso.

Salida por pantalla
---

> Filosofo 2 solicita tenedor izq ...3
> Filosofo 4 solicita tenedor izq ...5
> Filosofo 0 solicita tenedor der ...1
> Ten. 3 recibe petic. de 2
> Filosofo 2 coge tenedor der ...1
> Ten. 5 recibe petic. de 4
> Ten. 9 recibe petic. de 0
> Filosofo 4 coge tenedor der ...3
> Filosofo 0 coge tenedor izq ...9
> Filosofo 8 solicita tenedor izq ...9
> Filosofo 2 COMIENDO
> Ten. 1 recibe petic. de 2
> Filosofo 6 solicita tenedor izq ...7
> Ten. 7 recibe petic. de 6
> Filosofo 6 coge tenedor der ...5
> Filosofo 2 suelta tenedor izq ...3
> Filosofo 2 suelta tenedor der ...1
> Ten. 3 recibe liberac. de 2
> Ten. 3 recibe petic. de 4
> Filosofo 4 COMIENDO
> Ten. 1 recibe liberac. de 2
> Filosofo 2 PENSANDO
> Ten. 1 recibe petic. de 0
> Filosofo 0 COMIENDO
> Filosofo 2 solicita tenedor izq ...3
> Filosofo 0 suelta tenedor izq ...1
> Ten. 1 recibe liberac. de 0
> Filosofo 0 suelta tenedor der ...9
> Ten. 9 recibe liberac. de 0
> Filosofo 0 PENSANDO
> Ten. 9 recibe petic. de 8
> Filosofo 8 coge tenedor der ...7
