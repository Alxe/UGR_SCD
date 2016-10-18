prod-cons.cpp
===

El programa `prod-cons.cpp` resuelve un problema concurrente con 
`pthread` en C++.

El planteamiento base del problema es una cadena de producción, 
como una panadería. 
El consumidor necesita productos, producidos por el productor. 
Esta variante del problema tiene como elemento añadido una cola de 
elementos, como una bandeja donde se deposita el pan.

La resolución del problema consiste del uso de 3 semáforos: uno 
para el productor, uno para el consumidor y el rutinario de E/S.

* Consumidor: Un semáforo que controla que haya elementos 
producidos. Espera a ser llamado por el productor y avisa a este 
cuando consume algo, liberando espacio en la cola.

* Productor: Un semáforo que controla que se puedan producir 
elementos, existiendo un límite. Espera a ser avisado por el 
consumidor si no hay hueco, y llama al consumidor para avisarle 
que hay elementos disponibles.

Resumen de Semáforos
---

```cpp
/* Inicialización de semáforos */
sem_init(&sem_productor, 0, tam_vec); // Puede producir hasta que no quepa más
sem_init(&sem_consumidor, 0, 0); // Inicialmente no hay elementos
sem_init(&sem_es, 0, 1);

// ...

/* Hilo del productor */
sem_wait(&sem_productor); // Espera si la cola está llena
// Produce un elemento
sem_post(&sem_consumidor);

// ...

/* Hilo del consumidor */
sem_wait(&sem_consumidor); // Espera si la cola está vacía
// Consume un elemento
sem_post(&sem_productor);

```

----
