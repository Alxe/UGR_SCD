fumadores.cpp
===

El programa `fumadores.cpp` resuelve un problema concurrente con 
`pthread` en C++.

El planteamiento base del problema es un estanco con tres 
fumadores. 
Cada fumador necesita un elemento diferente para poder 
liarse un cigarrillo y fumarselo. 
El estanquero pone un elemento, ingrediente diferente y no lo 
cambia hasta que algún fumador lo usa.

La resolución del problema consiste del uso de 5 semáforos: tres 
para los fumadores, uno para el estanquero, y el rutinario de E/S.

* Fumadores: Un semáforo para cada fumador, que espera antes de 
fumar (espera el ingrediente que necesita) y es avisado por el 
mismo estanquero.

* Estanquero: Un semáforo que que controla que siempre haya un 
ingrediente disponible. Espera a que los fumadores cojan el 
ingrediente.

Resumen de Semáforos
---

```cpp
/* Inicialización de semáforos */
sem_init(&sem_fumadores[0..2], 0, 1); // Uno para cada fumador, 
tres fumadores
sem_init(&sem_estanquero, 0, 1);
sem_init(&sem_es, 0, 1);

// ...

/* Hilo de cada fumador */
// Teniendo en consideración que i denota la identidad
// y que se ejecuta hasta la infinidad, a menos que se interrumpa
sem_wait(&sem_fumadores[i]);
sem_post(&sem_estanquero);
// fumar 

// ...

/* Hilo del estanquero */
sem_wait(&sem_estanquero);
// pone ingrediente, con índice i
sem_post(&sem_fumadores[i]);

```

----
