import monitor.*;

class Barberia extends AbstractMonitor {
	private Condition listaEspera = makeCondition(),
		barberoSiesta = makeCondition(),
		silla = makeCondition();

	public void cortarPelo() {
		enter();

		if(!silla.isEmpty()) {
			System.out.println("[Cliente]:\tEspera a que se libere la silla");
			listaEspera.await();
		}

		barberoSiesta.signal(); // Despierta al barbero si es que está echándose la siesta
		System.out.println("[Cliente]:\tSe sienta en la silla");
		silla.await();

		leave();
	}

	public void siguienteCliente() {
		enter();

		if(silla.isEmpty() && listaEspera.isEmpty()) {
			System.out.println("[Barbero]:\tSe echa una siesta hasta que vengan clientes");
			barberoSiesta.await();
		}

		System.out.println("[Barbero]:\tAtiende a un cliente");
		listaEspera.signal();

		leave();
	}

	public void finCliente() {
		enter();

		System.out.println("[Barbero]:\tTermina con cliente");
		silla.signal();

		leave();
	}
}

class Barbero implements Runnable {
	public final Thread thr;

	private Barberia barberia;

	public Barbero(Barberia barberia) {
		this.barberia = barberia;

		this.thr = new Thread(this, "Barbero");
	}

	public void run() {
		while(true) {
			barberia.siguienteCliente();
			aux.dormir_max(2500);
			barberia.finCliente();
		}
	}
}

class Cliente implements Runnable {
	public final Thread thr;

	private Barberia barberia;

	private int id;

	public Cliente(int i, Barberia barberia) {
		this.id = id;
		this.barberia = barberia;

		this.thr = new Thread(this, "Cliente " + this.id);
	}

	public void run() {
		while(true) {
			barberia.cortarPelo();
			aux.dormir_max(2000);
		}
	}
}

public class BarberoDurmiente {
	private static final int NUM_CLIENTES = 2;

	public static void main(String[] args) {
		Barberia barberia = new Barberia();

		Barbero barbero = new Barbero(barberia);

		Cliente[] clientes = new Cliente[NUM_CLIENTES];
		for(int i = 0; i < NUM_CLIENTES; ++i) {
			clientes[i] = new Cliente(i, barberia);
		}

		barbero.thr.start();
		for(int i = 0; i < NUM_CLIENTES; ++i) {
			clientes[i].thr.start();
		}
	}
}