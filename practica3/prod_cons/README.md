prod_cons.cpp
===

El problema del productor consumidor con transmision de datos por el protocolo MPI.

Los cambios realizados son una ampliación de la función `main` para añadir más procesos, y el uso de etiquetas especificas para que el Buffer sepa "sobre qué" enviar sin saber "a quién".

Uso tres constantes de preprocesador (`TAG_...`) para la comunicación por MPI.

Salida por pantalla
---

> Productor 4 produce valor 0
> Productor 1 produce valor 1
> Buffer recibe 0 de Productor 1
> Buffer recibe 0 de Productor 4
> Productor 4 produce valor 1
> Buffer envía 0 a Consumidor 7
> Buffer envía 0 a Consumidor 9
> Buffer recibe 0 de Productor 0
> Buffer recibe 0 de Productor 3
> Buffer envía 0 a Consumidor 8
> Buffer recibe 0 de Productor 2
> Productor 0 produce valor 1
> Consumidor 8 recibe valor 0 de Buffer 
> Consumidor 9 recibe valor 0 de Buffer 
> Buffer envía 0 a Consumidor 6
> Productor 3 produce valor 1
> Consumidor 6 recibe valor 0 de Buffer 
> Consumidor 7 recibe valor 0 de Buffer 
> Productor 2 produce valor 1
> Buffer recibe 1 de Productor 4
> Productor 4 produce valor 2
