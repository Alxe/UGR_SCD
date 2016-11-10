import monitor.*;

class Estanco extends AbstractMonitor {
	private int ingrediente_actual = -1;
	private Condition estanquero = makeCondition();
	private Condition[] fumadores;

	public Estanco(int num_fumadores) {
		this.fumadores = new Condition[num_fumadores];

		for(int i = 0; i < fumadores.length; ++i) {
			fumadores[i] = makeCondition();
		}
	}

	public void obtenerIngrediente(int ingrediente) throws InterruptedException {
		enter();
		try {
			if(ingrediente_actual < 0) { fumadores[ingrediente].await(); }

			System.out.printf("[Fumador]\t\tIngrediente retirado: %d\n", ingrediente_actual);

			this.ingrediente_actual = -1;

			estanquero.signal();

		} finally {
			leave();
		}

	}

	public void ponerIngrediente(int ingrediente) throws InterruptedException {
		enter();
		try {
			if(ingrediente_actual >= 0) { estanquero.await(); }

			this.ingrediente_actual = ingrediente;

			System.out.printf("[Estanquero]\t\tIngrediente colocado: %d\n", ingrediente_actual);

			fumadores[ingrediente].signal();

		} finally {
			leave();
		}
	}

	public void esperarRecogidaIngrediente() throws InterruptedException {

	}
}

class Fumador implements Runnable {
	public Thread thr;

	private int ingrediente_necesario;
	private Estanco estanco;

	public Fumador(int ingrediente_necesario, Estanco estanco) {
		this.ingrediente_necesario = ingrediente_necesario;
		this.estanco = estanco;

		this.thr = new Thread(this, "Fumador " + ingrediente_necesario);
	}

	public void run() {
		try {
			while(true) {
				estanco.obtenerIngrediente(ingrediente_necesario);
				aux.dormir_max(2000);
			}
		} catch (InterruptedException ie) {}
	}
}

class Estanquero implements Runnable {
	public Thread thr;

	private Estanco estanco;

	public Estanquero(Estanco estanco) {
		this.estanco = estanco;

		this.thr = new Thread(this, "Estanquero");
	}

	public void run() {
		try {
			while(true) {
				int ingrediente = (int)(Math.random() * 3.0);
				estanco.ponerIngrediente(ingrediente);
				estanco.esperarRecogidaIngrediente();
			}
		} catch (InterruptedException ie) {}
	}
}

class ProblemaFumadores {
	public static void main(String... args) {
		final int num_fumadores = 3;

		Estanco estanco = new Estanco(num_fumadores);
		Estanquero estanquero = new Estanquero(estanco);
		Fumador fumadores[] = new Fumador[num_fumadores];
		for(int i = 0; i < num_fumadores; ++i) {
			fumadores[i] = new Fumador(i, estanco);
		}

		estanquero.thr.start();
		for(int i = 0; i < num_fumadores; ++i) {
			fumadores[i].thr.start();
		}
		try {
			estanquero.thr.join();
		} catch(InterruptedException ie) {}
	}
}