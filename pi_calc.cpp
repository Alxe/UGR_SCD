#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include "fun_tiempo.h"

const int kErrMalformedArguments = 1;

const double kPi = 3.141592653589793238462643383279502884197169399375105820974;

using namespace std;

static unsigned long threads_no = 0;
static unsigned long samples_no = 0;

/*
 *	Función que genera un cálculo similar al valor de Pi.
 */

double f(double i) {
	return 4.0 / (1 + i*i);
}

/*
 *	Cálculo de pi de forma secuencial, con un solo hilo.
 */

double calc_pi_seq() {
	double sum = 0.0;

	for(unsigned long i = 0; i < samples_no; i++) {
		sum += f((i + 0.5) / samples_no);
	}

	return sum / samples_no;
}

/*
 *	Cálculo de pi de forma concurrente, con múltiples hilos.
 */


static double *partial_sums = NULL;

void* func(void *arg) {
	unsigned long argi = (unsigned long) arg;

	partial_sums[argi] = 0.0;

	for(unsigned long i = argi; i < samples_no; i += threads_no) {
		partial_sums[argi] += f((i + 0.5) / samples_no);
	}

	partial_sums[argi] /= samples_no;

	return nullptr;
}


double calc_pi_mult() {
	double sum = 0.0;

	partial_sums = new double[threads_no];

	// Creación y ejecución de los hilos.
	pthread_t thread_id[threads_no];
	for(unsigned long i = 0; i < threads_no; i++) {
		pthread_create(&thread_id[i], nullptr, func, (void*) i);
	}

	// Unión de cada hilo y suma de cada resultado.
	for(unsigned long i = 0; i < threads_no; i++) {
		pthread_join(thread_id[i], nullptr);
		sum += partial_sums[i];
	}

	delete[] partial_sums;

	return sum;
}

int main(int argc, char **argv) {
	cout << "Cálculo del valor Pi (Actividad Propuesta)" << endl;

	if(argc < 3
	   || (threads_no = (unsigned long) atol(argv[1])) <= 0
	   || (samples_no = (unsigned long) atol(argv[2])) <= 0) {
		cout << "El programa debe recibir como parámetros el número de hilos (mayor a 0) y de muestras a tomar (mayor a 0)." << endl
			<< "Funcionamiento: " << argv[0] << " <hilos> <muestras>" << endl;

		return kErrMalformedArguments;
	}

	double pi_seq = 0.0, pi_mult = 0.0;

	cout << "VALOR DE PI:\t" << kPi << endl;

	struct timespec start_mult = ahora();
	pi_mult = calc_pi_mult();
	struct timespec end_mult = ahora();
	cout << "MULTIHILO:" << endl
		<< "Resultado:\t" << pi_mult << endl
		<< "Segundos:\t" << duracion(&start_mult, &end_mult) << endl << endl;

	struct timespec start_seq = ahora();
	pi_seq = calc_pi_seq();
	struct timespec end_seq = ahora();
	cout << "SECUENCIAL:" << endl
		<< "Resultado:\t" << pi_seq << endl
		<< "Segundos:\t" << duracion(&start_seq, &end_seq) << endl << endl;

	return 0;
}

