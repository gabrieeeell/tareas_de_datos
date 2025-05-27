#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <unistd.h>
#define NUMERO_MAX_OPCIONES 10
#define NUMERO_MAX_MEJORAS_DE_COMBATE 20
using namespace std;

struct Opciones {
  string accion;
  string descripcion;
  string efecto;
};

struct Eventos {
  string nombre;
  float probabilidad;
  int numero_opciones;
  string descripcion;
  Opciones opciones[NUMERO_MAX_OPCIONES];
};

struct Habitacion {
  int numero;
  string nombre;
  string descripcion;
  string tipo;
};

struct NodoMapa {
  Habitacion habitacion;
  NodoMapa *hijo1 = nullptr;
  NodoMapa *hijo2 = nullptr;
  NodoMapa *hijo3 = nullptr;
};

struct enemigo {
  string nombre;
  int vida;
  int ataque;
  float precision;
  float probabilidad;
};

struct Jugador {
  int vida;
  int ataque;
  float precision;
  int recuperacion;
};

struct nodo_cola_enemigo {
  enemigo contenido;
  nodo_cola_enemigo *next;
};

struct nodo_cola_jugador {
  Jugador contenido;
  nodo_cola_enemigo *next;
};

/*
 Para que la cola funcionara tanto con nodos hechos para el struct Jugador como
 para nodos hechos para el struct enemigo, la cola esta hecha de una forma mas
 rebuscada, primero se asume que el jugador si es que esta, siempre estara al
 frente de la cola, entonces si este no esta en la cola, ahi recien se
 preguntara por el enemigo que este más al frente.
 En el código del combate, esta se usa tal que al principio de cada turno, se
 agregan el jugador y todos los enemigos que esten disponibles para atacar, en
 ese orden.
 Como el alcance de la cola es un turno y el máximo de
 enemigos es 4, la longitud máxima que pudiera alcanzar la cola es 5
 */

class Cola_turnos {
  nodo_cola_jugador *frente_jugador;
  nodo_cola_enemigo *frente_enemigo;
  nodo_cola_enemigo *ultimo_en_cola;
  int longitud_actual;

public:
  Cola_turnos();
  void agregar_a_la_cola(enemigo *enemigo_a_agregar = nullptr,
                         Jugador *jugador_a_agregar = nullptr);
};

Cola_turnos::Cola_turnos() {
  frente_jugador = nullptr;
  frente_enemigo = nullptr;
  ultimo_en_cola = nullptr;
  longitud_actual = 0;
}

Cola_turnos::agregar_a_la_cola(enemigo *enemigo_a_agregar = nullptr,
                               Jugador *jugador_a_agregar = nullptr) {
  if (jugador_a_agregar != nullptr) {
    nodo_cola_jugador nodo_jugador = new nodo_cola_jugador frente_jugador =
        jugador_a_agregar;
  }
}

void mostrar_generacion_texto(string texto, bool saltar_linea = true) {
  for (int i = 0; i < texto.length(); i++) {
    cout << texto[i];
    cout.flush();  // Sin esto el texto no se muestra hasta el salto de linea
    usleep(25000); // 0.025 segundos
  }
  cout << "\n";
};

void empezar_combate(Jugador *jugador, int total_enemigos,
                     enemigo *enemigos_mapa) {
  // elegir cantidad enemigos (maximo 4)
  int rng_enemigo = rand() % 101;
  int total_enemigos_este_combate;
  if (rng_enemigo <= 35) {
    total_enemigos_este_combate = 1;
  } else if (rng_enemigo <= 70) {
    total_enemigos_este_combate = 2;
  } else if (rng_enemigo <= 90) {
    total_enemigos_este_combate = 3;
  } else {
    total_enemigos_este_combate = 4;
  }
  enemigo *enemigos_este_combate = new enemigo[total_enemigos_este_combate];

  for (int i = 0; i < total_enemigos_este_combate; i++) {
    int rng_seleccion_enemigo =
        rand() % 10001; // Simula porcentajes de hasta centesimales (0.01),
                        // debido a que "10000 = 100%" todos los porcentajes se
                        // tendran que multiplicar por 100
    int porcentaje_acumulado = 0;
    for (int x = 0; x < total_enemigos; x++) {
      if (porcentaje_acumulado < rng_seleccion_enemigo &&
          rng_seleccion_enemigo < (enemigos_mapa[x].probabilidad * 100)) {
        enemigos_este_combate[i] = enemigos_mapa[x];
      }
    }
  }
  for (int i = 0; i < total_enemigos_este_combate; i++) {
    cout << enemigos_este_combate[i].nombre;
    if (i < total_enemigos_este_combate - 1) {
      mostrar_generacion_texto(", ", false);
    } else if (i == (total_enemigos_este_combate - 1)) {
      mostrar_generacion_texto(" y ", false);
    } else {
      mostrar_generacion_texto("!");
    }
  }

  Cola turnos;

  // Mientras el ultimo enemigo o el jugador muera, el bucle del combate sigue
  while (enemigos_este_combate[total_enemigos_este_combate - 1].vida > 0 &&
         jugador->vida > 0) {
    // Mostrar nombres jugador y enemigos
    mostrar_generacion_texto("Jugador", false);
    for (int i = 0; i < total_enemigos_este_combate; i++) {
      mostrar_generacion_texto(" | " + enemigos_este_combate[i].nombre, false);
    }
    cout << endl;
    // Mostrar vidas jugador y enemigos
  }
};

void texto_elegir_siguiente_sala(NodoMapa *&nodo_habitacion_actual) {
  NodoMapa *siguientes_habitaciones[3] = {
      nullptr, nullptr, nullptr}; // Ya que es un arbol ternario
  int siguiente_habitacion_index = 0;
  if (nodo_habitacion_actual->hijo1 != nullptr) {
    siguientes_habitaciones[siguiente_habitacion_index++] =
        nodo_habitacion_actual->hijo1;
  }
  if (nodo_habitacion_actual->hijo2 != nullptr) {
    siguientes_habitaciones[siguiente_habitacion_index++] =
        nodo_habitacion_actual->hijo2;
  }
  if (nodo_habitacion_actual->hijo3 != nullptr) {
    siguientes_habitaciones[siguiente_habitacion_index++] =
        nodo_habitacion_actual->hijo3;
  }
  mostrar_generacion_texto(
      "A donde quieres ir?\n1. " +
      siguientes_habitaciones[0]->habitacion.nombre +
      (siguientes_habitaciones[1] != nullptr
           ? "\n2. " + siguientes_habitaciones[1]->habitacion.nombre
           : "") +
      (siguientes_habitaciones[2] != nullptr
           ? "\n3. " + siguientes_habitaciones[2]->habitacion.nombre
           : ""));
  string opcion = "Lo que no te mate, te infecta wajajaj";

  while (opcion != "1" && opcion != "2" && opcion != "3" ||
         opcion == "3" && siguiente_habitacion_index < 3 ||
         opcion == "2" && siguiente_habitacion_index < 2) {
    if (opcion != "Lo que no te mate, te infecta wajajaj") {
      cout << "Por favor elija una opción valida" << endl;
    }
    cin >> opcion;
  }
  nodo_habitacion_actual = siguientes_habitaciones[stoi(opcion) - 1];
}

// Variables globales

int total_enemigos;
enemigo *enemigos_mapa;
string mejoras_de_combate[NUMERO_MAX_MEJORAS_DE_COMBATE];

// *& Por lo que usa una referencia al puntero, no una copia del puntero

void leer_mapa(string nombre_archivo, NodoMapa *&nodos_habitaciones) {
  int total_habitaciones;
  string linea;
  ifstream mapa(nombre_archivo);
  if (!mapa.is_open()) {
    cout << "No se pudo abrir el archivo" << endl;
    return;
  }

  while (getline(mapa, linea)) {
    if (linea == "HABITACIONES") {
      getline(mapa, linea);
      total_habitaciones = stoi(linea); // stoi transforma la linea a un int
      nodos_habitaciones = new NodoMapa[total_habitaciones];

      for (int i = 0; i < total_habitaciones; i++) {
        getline(mapa, linea);

        size_t espacio = linea.find(" ");
        nodos_habitaciones[i].habitacion.numero =
            stoi(linea.substr(0, espacio)); // substr(posicion, total de
                                            // caracteres a sustraer)

        size_t parentesis = linea.find("(");
        nodos_habitaciones[i].habitacion.nombre =
            linea.substr(espacio + 1, parentesis - espacio - 2);

        size_t parentesis_final = linea.find(")");
        nodos_habitaciones[i].habitacion.tipo =
            linea.substr(parentesis + 1, parentesis_final - parentesis - 1);

        getline(mapa, linea);
        nodos_habitaciones[i].habitacion.descripcion = linea;
        if (true) {
        }
      }
    }

    else if (linea == "ARCOS") {
      int total_arcos;
      getline(mapa, linea);
      total_arcos = stoi(linea);

      for (int a = 0; a < total_arcos; a++) {
        getline(mapa, linea);

        size_t espacio = linea.find(" ");
        int desde = stoi(linea.substr(0, espacio));
        size_t flecha = linea.find("->");
        int hacia = stoi(linea.substr(flecha + 2));
        if (nodos_habitaciones[desde].hijo1 == nullptr) {
          nodos_habitaciones[desde].hijo1 = &(nodos_habitaciones[hacia]);
        } else if (nodos_habitaciones[desde].hijo2 == nullptr) {
          nodos_habitaciones[desde].hijo2 = &(nodos_habitaciones[hacia]);
        } else {
          nodos_habitaciones[desde].hijo3 = &(nodos_habitaciones[hacia]);
        }
      }
    }

    else if (linea == "ENEMIGOS") {
      getline(mapa, linea);
      total_enemigos = stoi(linea);
      enemigos_mapa = new enemigo[total_enemigos];

      for (int b = 0; b < total_enemigos; b++) {
        getline(mapa, linea);

        size_t separador1 = linea.find("|");
        size_t separador2 = linea.find("|", separador1 + 1);
        size_t separador3 = linea.find("|", separador2 + 1);
        size_t separador4 = linea.find("|", separador3 + 1);

        enemigos_mapa[b].nombre = linea.substr(0, separador1);
        enemigos_mapa[b].vida =
            stoi(linea.substr(separador1 + 7, separador2 - separador1 - 2));
        enemigos_mapa[b].ataque =
            stoi(linea.substr(separador2 + 8, separador3 - (separador2 + 8)));
        enemigos_mapa[b].precision =
            stof(linea.substr(separador3 + 12, separador4 - (separador3 + 12)));
        enemigos_mapa[b].probabilidad = stof(linea.substr(separador4 + 14));
      }
    } else if (linea == "EVENTOS") {
      int total_eventos;
      getline(mapa, linea);
      total_eventos = stoi(linea);
      Eventos *eventos_mapa = new Eventos[total_eventos];

      getline(mapa, linea);                     // se ubica en los &
      for (int c = 0; c < total_eventos; c++) { // c++ que chistoso miau
        getline(mapa, linea);                   // nombre de los eventos
        eventos_mapa[c].nombre = linea;
        getline(mapa, linea); // probabilidad de aparicion

        size_t espacio_probabilidad = linea.find(" ");
        eventos_mapa[c].probabilidad =
            stof(linea.substr(espacio_probabilidad + 1));

        getline(mapa, linea);
        eventos_mapa[c].descripcion = linea;
        getline(mapa, linea);

        int num_opcion = 0;
        do {
          eventos_mapa[c].opciones[num_opcion].accion = linea.substr(3);
          getline(mapa, linea);
          eventos_mapa[c].opciones[num_opcion].descripcion = linea;
          getline(mapa, linea);
          eventos_mapa[c].opciones[num_opcion].efecto = linea;
          num_opcion++;
          getline(mapa, linea);
        } while (linea != "&" && linea != "MEJORAS DE COMBATE");
      }
    }
    if (linea == "MEJORAS DE COMBATE") {
      getline(mapa, linea);
      int indice_mejoras_de_combate = 0;
      while (linea != "FIN DE ARCHIVO") {
        mejoras_de_combate[indice_mejoras_de_combate] = linea;
        getline(mapa, linea);
      }
    }
  }

  // NO OLVIDAR BORRAR MEMORIA CAUSA //

  mapa.close();
}

int main() {

  srand(time(0)); // Esta linea es para generar una seed aleatoria para
                  // los numeros random

  Jugador jugador = {30, 7, 0.95, 3};
  cout << rand() << endl;
  cout << rand() << endl;
  cout << rand() << endl;
  NodoMapa *nodos_habitaciones;
  leer_mapa("ejemplo.map", nodos_habitaciones);
  NodoMapa *nodo_habitacion_actual = &nodos_habitaciones[0];

  mostrar_generacion_texto(
      to_string(nodo_habitacion_actual->habitacion.numero) + " " +
      nodo_habitacion_actual->habitacion.nombre + " (" +
      nodo_habitacion_actual->habitacion.tipo + ")");

  mostrar_generacion_texto(nodo_habitacion_actual->habitacion.descripcion);

  usleep(500000); // medio segundo

  do {
    texto_elegir_siguiente_sala(nodo_habitacion_actual);
    mostrar_generacion_texto(
        to_string(nodo_habitacion_actual->habitacion.numero) + " " +
        nodo_habitacion_actual->habitacion.nombre + " (" +
        nodo_habitacion_actual->habitacion.tipo + ")");
    mostrar_generacion_texto(nodo_habitacion_actual->habitacion.descripcion);
    if (nodo_habitacion_actual->habitacion.tipo == "COMBATE") {
    }

  } while (nodo_habitacion_actual->hijo1 != nullptr ||
           nodo_habitacion_actual->hijo2 != nullptr ||
           nodo_habitacion_actual->hijo3 != nullptr);

  return 0;
}
