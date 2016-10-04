
all: fun_tiempo.o calculopi.o
	g++ -o calculopi fun_tiempo.o calculopi.o -lrt -pthread

fun_tiempo.o: fun_tiempo.h
	gcc -c -o fun_tiempo.o fun_tiempo.c

calculopi.o: calculopi.cpp
	g++ -c -o calculopi.o calculopi.cpp -lrt -pthread
