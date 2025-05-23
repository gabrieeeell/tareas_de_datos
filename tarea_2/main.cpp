#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <ctime>
#include <fstream>
#include <string>

using namespace std;

struct habitacion{
	int numero;
	string nombre;
	string descripcion;
	string tipo;
};

struct enemigo{
	string nombre;
	int vida;
	int ataque;
	int precision;
	int probabilidad;
}


class Jugador {
	int vida;
	int ataque;
	float precision;
	int recuperacion;
};

void mostrar_generacion_texto(string texto) {
	for (int i = 0; i < texto.length(); i++) {
		cout << texto[i];
		cout.flush(); //Sin esto el texto no se muestra hasta el salto de linea
		usleep(25000); // 0.025 segundos
	}
	cout << "\n";
};

int main() {

	srand(time(0)); //Esta linea es para generar una seed aleatoria para los numeros random
	
	int random_number = rand();
	string word = "Esta linea es para generar una seed aleatoria para los numeros random";
	mostrar_generacion_texto(word);
	return 0;
}
