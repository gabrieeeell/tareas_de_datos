#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <ctime>
#include <fstream>
#include <string>

using namespace std;

struct Opciones{
	string accion;
	string descripcion;
	string efecto;
};

struct Eventos{
	string nombre;
	float probabilidad;
	int numero_opciones;
	Opciones *opciones; // como pueden hbaer mas de 2 opciones, usamos memoria dinamica

};

struct Habitacion{
	int numero;
	string nombre;
	string descripcion;
	string tipo;
};

struct NodoMapa{
	Habitacion habitacion;
	NodoMapa *hijo1;
	NodoMapa *hijo2;
	NodoMapa *hijo3;
};

struct arco{
	int desde; 
	int hacia; 
};

struct enemigo{
	string nombre;
	int vida;
	int ataque;
	float precision;
	float probabilidad;
};

class Jugador {
private:
	int vida;
	int ataque;
	float precision;
	int recuperacion;
public:
	
};

void leer_mapa(string nombre_archivo){
	int total_habitaciones;
	string linea;
	ifstream mapa(nombre_archivo);
	if(!mapa.is_open()){
		cout << "No se pudo abrir el archivo"<< endl;
		return ;
	}
	while (getline(mapa, linea)){
		if (linea == "HABITACIONES"){
			getline(mapa, linea);
			total_habitaciones = stoi(linea); //stoi transforma la linea a un int
			Habitacion *habitaciones = new Habitacion[total_habitaciones];
			
			for (int i = 0 ; i < total_habitaciones ; i++){
				getline(mapa, linea);
				
				size_t espacio = linea.find(" ");
				habitaciones[i].numero = stoi(linea.substr(0, espacio)); //substr(posicion, total de caracteres a sustraer)
				
				size_t parentesis = linea.find ("(");
				habitaciones[i].nombre = linea.substr(espacio + 1, parentesis - espacio - 2 );
				
				size_t parentesis_final = linea.find(")");
				habitaciones[i].tipo = linea.substr(parentesis + 1, parentesis_final - parentesis - 1 );
				
				getline(mapa,linea);
				habitaciones[i].descripcion = linea ;
				}
			}
		
		else if (linea == "ARCOS"){
			int total_arcos;
			getline(mapa, linea); 
			total_arcos = stoi(linea);
			arco *arcos_mapa = new arco[total_arcos];
			
			for (int a = 0 ; a < total_arcos; a++ ){
				getline(mapa,linea);
				
				size_t espacio = linea.find(" ");
				arcos_mapa[a].desde = stoi(linea.substr(0 , espacio));
				
				size_t flecha = linea.find("->");
				
				arcos_mapa[a].hacia = stoi(linea.substr(flecha + 2));
			}
		}

		else if (linea == "ENEMIGOS"){
			int total_enemigos;
			getline(mapa,linea);
			total_enemigos = stoi(linea);
			enemigo *enemigos_mapa = new enemigo[total_enemigos];
			
			for (int b = 0 ; b < total_enemigos; b++ ){
				getline(mapa,linea);

				size_t separador1 = linea.find("|");
				size_t separador2 = linea.find("|", separador1 + 1);
				size_t separador3 = linea.find("|", separador2 + 1);
				size_t separador4 = linea.find("|", separador3 + 1);
				
				enemigos_mapa[b].nombre = linea.substr(0, separador1);
				enemigos_mapa[b].vida = stoi(linea.substr(separador1 + 7, separador2 - separador1 - 2));
				enemigos_mapa[b].ataque = stoi(linea.substr(separador2 + 8, separador3 - (separador2 + 8) ));
				enemigos_mapa[b].precision = stof(linea.substr(separador3 + 12, separador4 - (separador3 + 12)));
				enemigos_mapa[b].probabilidad = stof(linea.substr(separador4 + 14));
			
			
			}
		}
		else if (linea == "EVENTOS"){
			int total_eventos;
			getline(mapa,linea);
			total_eventos = stoi(linea);
			Eventos *eventos_mapa = new Eventos[total_eventos];

			for (int c = 0 ; c < total_eventos; c++){ //c++ que chistoso miau
				getline(mapa,linea); // se ubica en los &
				getline(mapa,linea); // nombre de los eventos
				eventos_mapa[c].nombre = linea;
				getline(mapa,linea); //probabilidad de aparicion

				size_t espacio_probabilidad = linea.find(" ");
				eventos_mapa[c].probabilidad = stof(linea.substr(espacio_probabilidad + 1));


				if (linea == "&"){
					while (getline(mapa,linea)){
						eventos_mapa[c] = linea;
						
						if (linea == Probabilidad){
							size_t espacio_probabilidad = linea.find(" ");
							eventos_mapa[c].probabilidad = stof(linea.substr(espacio_probabilidad + 1));
						}
						
						else if (linea)
					}
					

				}
			}
		}
	}
	
	// NO OLVIDAR BORRAR MEMORIA CAUSA //
	
	
	mapa.close();
}




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
	leer_mapa("ejemplo.map");
	
	
	return 0;
}
