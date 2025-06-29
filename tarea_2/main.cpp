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

struct nodo_cola {
  Jugador *jugador_nodo = nullptr;
  enemigo *enemigo_nodo = nullptr;
  nodo_cola *next = nullptr;
};

// Variables globales
bool se_murio = false;
Eventos *eventos_mapa;
int total_eventos;
int total_enemigos;
enemigo *enemigos_mapa;
string mejoras_de_combate[NUMERO_MAX_MEJORAS_DE_COMBATE];
int indice_mejoras_de_combate = 0;

class Cola_turnos {
  nodo_cola *frente;
  nodo_cola *final_cola;
  int longitud;

public:
  Cola_turnos();
  int length();
  void queue(nodo_cola *nuevo_nodo);
  nodo_cola *
  dequeue(); // dequeue sera implementado tal que devuelve el frontvalue en vez
             // de tener una función aparte para esto
};

Cola_turnos::Cola_turnos() {
  frente = final_cola = nullptr;
  longitud = 0;
};

void Cola_turnos::queue(nodo_cola *nuevo_nodo) {
  if (longitud == 0) {
    frente = final_cola = nuevo_nodo;
  } else {
    final_cola->next = nuevo_nodo;
    final_cola = nuevo_nodo;
  }
  longitud++;
}

nodo_cola *Cola_turnos::dequeue() {
  if (longitud == 0)
    return nullptr;

  nodo_cola *aux = frente;
  frente = frente->next;
  longitud--;
  return aux;
}

int Cola_turnos::length() { return longitud; }

/*****
 * void mostrar_generacion_texto
 ******
 * Muestra el texto de manera progresiva, como en los juegos rpg,
 * es una funcion meramente estetica
 ******
 * Input:
 *   string texto : texto que se desea mostrar.
 *   bool saltar_linea : indica si se debe imprimir un salto de línea al final
 *(por defecto true).
 ******
 * Returns:
 *   no retorna nada, es una funcion tipo void
 *****/

void mostrar_generacion_texto(string texto, bool saltar_linea = true) {
  for (size_t i = 0; i < texto.length(); i++) {
    cout << texto[i];
    cout.flush();  // Sin esto el texto no se muestra hasta el salto de linea
    usleep(25000); // 25000 =  0.025 segundos (25000 microsegundos)
  }
  if (saltar_linea)
    cout << "\n";
};

/* ****
*   void manejar_mejoras_de_combate
******
*
*  Primero se le muestran las opciones al jugador
******
*
Input :
*
*   Jugador *jugador : Puntero al struct del jugador
*   (notar que las mejoras de combate son una variable global por lo que no se
*   incluye como parametro)
*
******
*   Esta función no tiene returns, las mejoras se manejan solo en la función
**** */
void manejar_mejoras_de_combate(Jugador *jugador) {
  mostrar_generacion_texto("Debes decidir: ");
  for (int i = 0; i < indice_mejoras_de_combate; i++) {

    // Aca uso substr para leer la palabra que esta después del primer
    // espacio de la linea
    string atributo = mejoras_de_combate[i].substr(
        mejoras_de_combate[i].find(" ") + 1,
        mejoras_de_combate[i].length() - mejoras_de_combate[i].find(" "));
    string cantidad_atributo =
        mejoras_de_combate[i].substr(1, mejoras_de_combate[i].find(" ") - 1);

    if (atributo == "Vida") {
      mostrar_generacion_texto("\t" + to_string(i) + ". Recuperar " +
                               cantidad_atributo + " de vida.");

    } else {
      mostrar_generacion_texto("\t" + to_string(i) + ". Aumentar " +
                               (char)tolower(atributo[0]) + atributo.substr(1) +
                               " en " + cantidad_atributo + ".");
    }
  }

  string eleccion_usuario;

  cin >> eleccion_usuario;

  string atributo = mejoras_de_combate[stoi(eleccion_usuario)].substr(
      mejoras_de_combate[stoi(eleccion_usuario)].find(" ") + 1,
      mejoras_de_combate[stoi(eleccion_usuario)].length() -
          mejoras_de_combate[stoi(eleccion_usuario)].find(" "));

  string cantidad_atributo = mejoras_de_combate[stoi(eleccion_usuario)].substr(
      1, mejoras_de_combate[stoi(eleccion_usuario)].find(" ") - 1);
  if (atributo == "Precision") {
    jugador->precision += stof(cantidad_atributo);
    mostrar_generacion_texto("Tu precisión aumento en " + cantidad_atributo +
                             "!");
  } else if (atributo == "Vida") {
    jugador->vida += stoi(cantidad_atributo);
    mostrar_generacion_texto("Recuperaste " + cantidad_atributo + " de vida!");
  } else {
    if (atributo == "ataque")
      jugador->ataque += stoi(cantidad_atributo);
    if (atributo == "recuperacion")
      jugador->recuperacion += stoi(cantidad_atributo);
    mostrar_generacion_texto("Tu " + atributo + " aumento en " +
                             cantidad_atributo + "!");
  }
};
/* ****
 *   bool empezar_combate
 ******
 *   La función primero determina aleatoriamente el numero de enemigos que habra
 *   en el combate (de 1 a 4, con una menor cantidad siendo mas probable), luego
 *   se determina que enemigos seran.
 *   Después empieza un bucle que recorre una cola con el jugador y los enemigos
 *   para determinar a quien le toca atacar, el bucle acaba cuando el jugador o
 *   los enemigos mueran.
 *   Finalmente se llama a la función "manejar_mejoras_de_combate" y se libera
 *   la memoria dinamica utilizada en la función.
 ******
 *   Input:
 *       Jugador *jugador : Puntero al struct del jugador
 *       int total_enemigos : Numero total de enemigos distintos extraidos del
 *       .map enemigo *enemigos_mapa : array de structs de los disintos enemigos
 *       extraidos del .map
 ******
 *   Returns:
 *       bool resultado_combate : true si el jugador gano el combate, false de
 *       lo contrario
 **** */

bool empezar_combate(Jugador *jugador, int total_enemigos,
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
  nodo_cola *enemigos_este_combate[total_enemigos_este_combate];
  for (int i = 0; i < total_enemigos_este_combate; i++) {
    int rng_seleccion_enemigo =
        rand() % 10001; // Simula porcentajes de hasta centesimales (0.01%),
                        // tal que "10000 = 100%", tambien para que esto
                        // funcione, todos los porcentajes que representados
                        // decimalmente se tendran que multiplicar por 10000
    int porcentaje_acumulado = 0;
    for (int x = 0; x < total_enemigos; x++) {
      if (porcentaje_acumulado < rng_seleccion_enemigo &&
          rng_seleccion_enemigo < (porcentaje_acumulado +
                                   (enemigos_mapa[x].probabilidad * 10000))) {
        enemigos_este_combate[i] =
            new nodo_cola{nullptr, new enemigo{enemigos_mapa[x]}, nullptr};
        break;
      }
      porcentaje_acumulado += enemigos_mapa[x].probabilidad * 10000;
      // Al usar new, estoy creando una nueva instancia del enemigo, en vez de
      // hacer una referencia al "original"
    }
  }

  nodo_cola *nodo_jugador = new nodo_cola{jugador, nullptr, nullptr};
  // Aca uso directamente la referencia al jugador,
  // ya que no quiero hacer un clon de el
  for (int i = 0; i < total_enemigos_este_combate; i++) {
    mostrar_generacion_texto(enemigos_este_combate[i]->enemigo_nodo->nombre,
                             false);
    if (i < total_enemigos_este_combate - 2) {
      mostrar_generacion_texto(", ", false);
    } else if (i == (total_enemigos_este_combate - 2)) {
      mostrar_generacion_texto("y ", false);
    } else {
      mostrar_generacion_texto("!");
    }
  }

  // Se agregan el jugador y los enemigos a la cola, estos fueron agregados a
  // un struct de nodo aparte, para que asi todos los elementos de la cola
  // puedan tener el mismo tipo

  Cola_turnos turnos;
  turnos.queue(nodo_jugador);
  for (int i = 0; i < total_enemigos_este_combate; i++) {
    turnos.queue(enemigos_este_combate[i]);
  }

  // Ciclo principal del combate
  // La cola turnos solo alcanza longitud 1 cuando queda unicamente el jugador
  // con vida

  while (turnos.length() > 1 && jugador->vida > 0) {
    // Mostrar nombres jugador y enemigos
    mostrar_generacion_texto("Jugador", false);
    for (int i = 0; i < total_enemigos_este_combate; i++) {
      mostrar_generacion_texto(
          " | " + enemigos_este_combate[i]->enemigo_nodo->nombre, false);
    }
    cout << endl;
    // Mostrar vidas jugador y enemigos

    mostrar_generacion_texto(to_string(nodo_jugador->jugador_nodo->vida),
                             false);
    for (int i = 0; i < total_enemigos_este_combate; i++) {
      mostrar_generacion_texto(
          " | " + (enemigos_este_combate[i]->enemigo_nodo->vida > 0
                       ? to_string(enemigos_este_combate[i]->enemigo_nodo->vida)
                       : "X"),
          false);
    }
    cout << endl;

    // Ataque del combate

    nodo_cola *jugador_o_enemigo_que_ataca = turnos.dequeue();

    if (jugador_o_enemigo_que_ataca->enemigo_nodo == nullptr) {

      // Significa que esta atacando un jugador

      if (rand() % 10001 > jugador->precision * 10000) {
        mostrar_generacion_texto("Jugador falla!");
      } else {
        // Como tanto la cola como el array para los enemigos trabajan
        // con las direcciones de memoria de los mismos enemigos, puedo
        // usar la que mas me convenga. Para este caso determine que el
        // jugador ataca al primer enemigo del array que siga vivo

        for (int i = 0; i < total_enemigos_este_combate; i++) {
          if (enemigos_este_combate[i]->enemigo_nodo->vida > 0) {
            enemigos_este_combate[i]->enemigo_nodo->vida -= jugador->ataque;
            mostrar_generacion_texto(
                "Jugador golpea a " +
                enemigos_este_combate[i]->enemigo_nodo->nombre + " por " +
                to_string(jugador->ataque) + " de daño!");
            break;
          }
        }
      }
      turnos.queue(nodo_jugador);
    } else {

      // Ataca enemigo, el cual podria estar muerto y seguir en la cola
      // por lo que hay que verificar

      if (jugador_o_enemigo_que_ataca->enemigo_nodo->vida <= 0) {
        continue;
      } else {

        if (rand() % 10001 >
            jugador_o_enemigo_que_ataca->enemigo_nodo->precision * 10000) {
          mostrar_generacion_texto(
              jugador_o_enemigo_que_ataca->enemigo_nodo->nombre + " falla!\n");
        } else {
          jugador->vida -= jugador_o_enemigo_que_ataca->enemigo_nodo->ataque;
          mostrar_generacion_texto(
              jugador_o_enemigo_que_ataca->enemigo_nodo->nombre +
              " golpea a Jugador por " +
              to_string(jugador_o_enemigo_que_ataca->enemigo_nodo->ataque) +
              " de daño!");
        }
        turnos.queue(jugador_o_enemigo_que_ataca);
      }
    }
  }
  bool resultado_combate; // true si el jugador gano, false si no.
  if (jugador->vida > 0) {
    // Jugador gano
    mostrar_generacion_texto("Has sobrevivido el combate!");
    mostrar_generacion_texto("Recuperas " + to_string(jugador->recuperacion) +
                             " de vida tras el combate.");
    manejar_mejoras_de_combate(jugador);

    resultado_combate = true;

  } else {
    // jugador murio
    mostrar_generacion_texto("Jugador", false);
    for (int i = 0; i < total_enemigos_este_combate; i++) {
      mostrar_generacion_texto(
          " | " + enemigos_este_combate[i]->enemigo_nodo->nombre, false);
    }
    cout << endl;

    mostrar_generacion_texto("X", false);
    for (int i = 0; i < total_enemigos_este_combate; i++) {
      mostrar_generacion_texto(
          " | " + (enemigos_este_combate[i]->enemigo_nodo->vida > 0
                       ? to_string(enemigos_este_combate[i]->enemigo_nodo->vida)
                       : "X"),
          false);
    }
    cout << endl;

    resultado_combate = false;
  }

  // Liberar memoria usada para el combate

  for (int i = 0; i < total_enemigos_este_combate; i++) {
    delete enemigos_este_combate[i]->enemigo_nodo;
    delete enemigos_este_combate[i];
  }
  delete nodo_jugador;

  return resultado_combate;
};

/*****
 * void texto_elegir_siguiente_sala
 ******
 * Muestra al usuario las habitaciones hijas disponibles para la habitación
 *actual, solicita y valida la elección del usuario, y actualiza el puntero al
 *nodo actual con la habitación seleccionada.
 ******
 * Input:
 *   NodoMapa *&nodo_habitacion_actual : referencia al puntero de la habitación
 *actual.
 ******
 * Returns:
 *   no retorna nada, es void. Actualiza el nodo actual según la elección del
 *usuario.
 *****/

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

  while ((opcion != "1" && opcion != "2" && opcion != "3") ||
         (opcion == "3" && siguiente_habitacion_index < 3) ||
         (opcion == "2" && siguiente_habitacion_index < 2)) {
    if (opcion != "Lo que no te mate, te infecta wajajaj") {
      cout << "Por favor elija una opción valida" << endl;
    }
    cin >> opcion;
  }
  nodo_habitacion_actual = siguientes_habitaciones[stoi(opcion) - 1];
}

// *& Por lo que usa una referencia al puntero, no una copia del puntero

/*****
 * void leer_mapa
 ******
 * Lee el archivo del mapa y carga las habitaciones, arcos, enemigos, eventos y
 *mejoras en las estructuras dinámicas correspondientes. Actualiza los punteros
 *hijos de cada nodo según los arcos.
 ******
 * Input:
 *   string nombre_archivo : nombre del archivo .map a leer.
 *   NodoMapa *&nodos_habitaciones : referencia a puntero donde se almacenarán
 *las habitaciones.
 ******
 * Returns:
 *   no retorna valor, es una funcion void. En caso de error al abrir archivo
 *muestra mensaje y retorna.
 *****/

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

      getline(mapa, linea);
      total_eventos = stoi(linea);
      eventos_mapa = new Eventos[total_eventos];

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
        eventos_mapa[c].numero_opciones = num_opcion;
      }
    }
    if (linea == "MEJORAS DE COMBATE") {

      getline(mapa, linea);
      while (linea != "FIN DE ARCHIVO") {
        mejoras_de_combate[indice_mejoras_de_combate++] = linea;
        getline(mapa, linea);
      }
    }
  }

  // NO OLVIDAR BORRAR MEMORIA CAUSA //

  mapa.close();
}

/*****
 * void efecto_evento
 ******
 * Aplica el efecto de un evento sobre las estadísticas del jugador, cambiando
 *sus atributos, despues muestra un mensaje indicando el atributo que se
 *modifico.
 ******
 * Input:
 *   Jugador *jugador : puntero al jugador al que se aplicará el efecto.
 *   const string &efecto : cadena de texto con el efecto a aplicar.
 ******
 * Returns:
 *   void : no retorna valor.
 *****/

void efecto_evento(Jugador *jugador, const string &efecto) {
  char signo_efecto = efecto[0];
  if ((signo_efecto != '+') && (signo_efecto != '-')) {
    mostrar_generacion_texto("Ninguna consecuencia.");
    return;
  }
  size_t espacio = efecto.find(" ");
  string valor_precision =
      efecto.substr(1, espacio - 1); // valor a imprimir para que aparezca solo
                                     // un decimal, y no 0.10000 por ejemplo
  float valor = stof(efecto.substr(1, espacio - 1));
  string estadistica = efecto.substr(efecto.find(" ") + 1);

  if (estadistica == "Vida") {
    if (signo_efecto == '+') {
      jugador->vida += int(valor);
      mostrar_generacion_texto("Tu vida aumento en " + to_string(int(valor)));
    } else if (signo_efecto == '-') {
      jugador->vida -= int(valor);
      mostrar_generacion_texto("Tu vida disminuyo en " + to_string(int(valor)));
      if (jugador->vida <= 0) {
        se_murio = true;
      }
    }
  } else if (estadistica == "Ataque") {
    if (signo_efecto == '+') {
      jugador->ataque += int(valor);
      mostrar_generacion_texto("Tu ataque aumento en " + to_string(int(valor)));
    } else if (signo_efecto == '-') {
      jugador->ataque -= int(valor);
      mostrar_generacion_texto("Tu ataque disminuyo en " +
                               to_string(int(valor)));
    }
  } else if (estadistica == "Recuperacion") {
    if (signo_efecto == '+') {
      jugador->recuperacion += int(valor);
      mostrar_generacion_texto("Tu recuperacion aumento en " +
                               to_string(int(valor)));
    } else if (signo_efecto == '-') {
      jugador->recuperacion -= int(valor);
      mostrar_generacion_texto("Tu recuperacion disminuyo en " +
                               to_string(int(valor)));
    }
  } else if (estadistica == "Precision") {
    if (signo_efecto == '+') {
      jugador->precision += valor;
      mostrar_generacion_texto("Tu precision aumento en " + valor_precision);
    } else if (signo_efecto == '-') {
      jugador->precision -= valor;
      mostrar_generacion_texto("Tu precision disminuyo en " + valor_precision);
    }
    if (jugador->precision > 1.0) {
      jugador->precision = 1.0;
    }
    if (jugador->precision < 0.0) {
      jugador->precision = 0.0;
    }
  }
}
/*****
 * void empezar_evento
 ******
 * selecciona y ejecuta un evento aleatorio seleccionado según las
 *probabilidades definidas. Muestra la descripción del evento y sus opciones,
 *pide seleccionar una opcion y muestra las consecuencias de esta, al mismo
 *tiempo que modifica las estadisticas del jugador junto al efecto_evento()
 ******
 * Input:
 *   Jugador *jugador : puntero al jugador que será afectado por el evento.
 *   Eventos *eventos_mapa : arreglo de eventos posibles.
 *   int total_eventos : número total de eventos en el arreglo.
 ******
 * Returns:
 *   no retorna nada, es void
 *****/

void empezar_evento(Jugador *jugador, Eventos *eventos_mapa,
                    int total_eventos) {
  int rng_evento = rand() % 10001;
  int rng_acumulado = 0;
  Eventos *evento_generado = nullptr;
  for (int i = 0; i < total_eventos;
       i++) { // este ciclo selecciona aleatoriamente un evento
    rng_acumulado += eventos_mapa[i].probabilidad * 10000;
    if (rng_evento < rng_acumulado) {
      evento_generado = &eventos_mapa[i];
      break;
    }
  }
  mostrar_generacion_texto(evento_generado->descripcion);

  for (int i = 0; i < evento_generado->numero_opciones;
       i++) { // ciclo para imprimir las opciones
    char letra_opcion = 'A' + i;
    mostrar_generacion_texto(string(1, letra_opcion) + ")" +
                             evento_generado->opciones[i].accion);
  }
  string eleccion;
  bool parar_bucle = false;
  do {

    mostrar_generacion_texto("Elige una opcion: ");
    cin >> eleccion;

    if (eleccion.length() == 1) {
      char letra = toupper(eleccion[0]); // Se transforma la letra de la
                                         // eleccion del usuario a un caracter
      int indice =
          letra - 'A'; // La resta de la opcion elegida menos el caracter A
                       // en codigo ASCII, dara un numero que nos ayuda a saber
                       // si la opcion elegida es una opcion valida en el evento
      if (indice >= 0 && indice < evento_generado->numero_opciones) {
        mostrar_generacion_texto(evento_generado->opciones[indice].descripcion);
        efecto_evento(jugador, evento_generado->opciones[indice].efecto);
        parar_bucle = true;
      } else {
        mostrar_generacion_texto("Ingresa una opcion valida.");
      }
    } else {
      mostrar_generacion_texto("Ingresa una opcion valida.");
    }
  } while (!parar_bucle);
}

int main() {

  srand(time(0)); // Esta linea es para generar una seed aleatoria para
                  // los numeros random

  Jugador *jugador = new Jugador{30, 7, 0.95, 3};
  NodoMapa *nodos_habitaciones;
  leer_mapa("data.map", nodos_habitaciones);
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
      bool resultado_combate =
          empezar_combate(jugador, total_enemigos, enemigos_mapa);
      if (resultado_combate == false) {
        nodo_habitacion_actual->hijo1 = nullptr;
        nodo_habitacion_actual->hijo2 = nullptr;
        nodo_habitacion_actual->hijo3 = nullptr;
        mostrar_generacion_texto("Has muerto, FIN. (git gud)");
      }
    } else if (nodo_habitacion_actual->habitacion.tipo == "EVENTO") {
      empezar_evento(jugador, eventos_mapa, total_eventos);
      if (se_murio == true) {
        nodo_habitacion_actual->hijo1 = nullptr;
        nodo_habitacion_actual->hijo2 = nullptr;
        nodo_habitacion_actual->hijo3 = nullptr;
        mostrar_generacion_texto("Has muerto, FIN. (git gud)");
      }
    }
  } while (nodo_habitacion_actual->hijo1 != nullptr ||
           nodo_habitacion_actual->hijo2 != nullptr ||
           nodo_habitacion_actual->hijo3 != nullptr);

  delete jugador;
  delete[] nodos_habitaciones;
  delete[] enemigos_mapa;
  delete[] eventos_mapa;
  return 0;
}
