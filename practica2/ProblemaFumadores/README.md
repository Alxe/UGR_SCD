fumadores.cpp
===

El programa `ProblemaFumadores.java` resuelve un problema concurrente 
con `java.lang.Thread` y monitos estilo Hoare en Java.

El planteamiento base del problema es un estanco con tres fumadores. 
Cada fumador necesita un elemento diferente para poder liarse un cigarrillo y fumarselo. 
El estanquero pone un elemento, ingrediente diferente y no lo cambia hasta que algún fumador lo usa.

La resolución del problema consiste del uso de 1+N condiciones (donde N 
es el número de fumadores), en un monitor estilo Hoare llamado `Estanco`.

* Fumadores: Array de condiciones. Cada una representa a un fumador. El 
estanquero notifica a uno de estos, y estos esperan al estanquero.

* Estanquero: Condición que espera ser notificada por cada fumador, y 
esta espera a que los fumadores cojan el ingrediente

Ejemplo de Salida
---
```
[Estanquero]		Ingrediente colocado: 2
[Fumador]		Ingrediente retirado: 2
[Estanquero]		Ingrediente colocado: 1
[Fumador]		Ingrediente retirado: 1
[Estanquero]		Ingrediente colocado: 0
[Fumador]		Ingrediente retirado: 0
[Estanquero]		Ingrediente colocado: 2
[Fumador]		Ingrediente retirado: 2
[Estanquero]		Ingrediente colocado: 1
[Fumador]		Ingrediente retirado: 1
[Estanquero]		Ingrediente colocado: 0
[Fumador]		Ingrediente retirado: 0
[Estanquero]		Ingrediente colocado: 2
[Fumador]		Ingrediente retirado: 2
[Estanquero]		Ingrediente colocado: 0
[Fumador]		Ingrediente retirado: 0
```
