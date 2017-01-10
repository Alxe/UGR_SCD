filosofos_camarero.cpp
===

El problema de los filosofos (con un camarero) con transmision de datos por el protocolo MPI.

Se añade un nuevo proceso, el camarero, al que se manda notificaciones (o solicitudes) de que un filosofo quiere sentarse o levantarse de una mesa. Exceptuando esto, el proceso sigue el mismo patrón que `filosofos.cpp`

Salida por pantalla
---

> Filosofo 4 se sienta
> Filosofo 0 se sienta
> Filosofo 0 solicita tenedor izq ...1
> Filosofo 2 solicita tenedor izq ...3
> Filosofo 4 solicita tenedor izq ...5
> Ten. 1 recibe petic. de 0
> Ten. 7 recibe petic. de 6
> Ten. 5 recibe petic. de 4
> Filosofo 6 coge tenedor der ...5
> Filosofo 4 coge tenedor der ...3
> Filosofo 0 coge tenedor der ...9
> Filosofo 0 COMIENDO
> Ten. 9 recibe petic. de 0
> Ten. 3 recibe petic. de 2
> Filosofo 2 coge tenedor der ...1
> Filosofo 0 suelta tenedor izq ...1
> Filosofo 0 suelta tenedor der ...9
> Filosofo 0 avisa de que se levanta
> Ten. 9 recibe liberac. de 0
> Ten. 1 recibe liberac. de 0
> Ten. 1 recibe petic. de 2
> Filosofo 2 COMIENDO
> Filosofo 0 se levanta
> Filosofo 8 se sienta
> Filosofo 0 PENSANDO
> Ten. 9 recibe petic. de 8
