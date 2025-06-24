#include <fstream>
#include <iostream>
#include <string>
#define NUM_MAX_COLA                                                           \
  1000 // se asume que este numero sera mayor o como minimo igual al numero
       // total de nodos del archivo
#define INFINITO 1000000000

using namespace std;

struct nodo {
  int data;
  nodo *next = nullptr;
};

class tCola {
  int *datos;
  int inicio;
  int fin;
  int tamaño;

public:
  tCola() {
    tamaño = 0;
    inicio = 0;
    fin = 0;
    datos = new int[NUM_MAX_COLA];
  }
  ~tCola() { delete[] datos; }
  void clear() {
    tamaño = 0;
    inicio = 0;
    fin = 0;
  }
  void enqueue(int elemento) {
    if (fin < NUM_MAX_COLA) {
      datos[fin++] = elemento;
      tamaño++;
    }
  }
  bool vacia() { return tamaño == 0; }
  void dequeue() {
    if (!vacia()) {
      inicio++;
      tamaño--;
    }
  }
  int frontValue() { return datos[inicio]; }
};

// Variables globales (*Revisar si acaban teniendo necesidad de ser gloabales*)

// Para la implementación del grafo se usaran listas adyacentes, ya que esta es
// más eficiente para hacer Bread-First Search (BFS) la cual sera el tipo de
// busqueda implementada en nuestro programa

// Lista de adyacencia modificada para incluir una lista con la ubicación de los
// ubers

// Notar que todas las listas enlazdas empiezan con un nodo con un dato que no
// se usa
void bfs(nodo *lista_vertices, int numero_vertices, int nodo_inicial,
         int *distancia, int *predecesor);

class ListaAdyacencia {
  nodo *lista_vertices;
  
  int *ubicaciones_uber;
  int numero_ubers;
  int numero_aristas;

public:
  // Después debo incluir una función tipo "agregar uber"
  int numero_vertices; // (numero de nodos)
  ListaAdyacencia(int numero_vertices, int numero_aristas, int numero_ubers);
  int agregar_conexion(int nodo_origen, int nodo_destino);
  int agregar_ubicacion_uber(int ubicacion);
  int *obtener_ubicaciones_ubers();
  nodo *obtener_lista_de_vertices();
  int solicitar_uber(int nodo_inicio, int nodo_final); // por terminar
  int encontrar_uber_mas_cerca(
      int nodo_pasajero,
      int &monto_uber); // usar bfs para encontrar el uber mas cercano, deberia
                        // devolver el nodo uber
};

ListaAdyacencia::ListaAdyacencia(int num_vertices, int num_aristas,
                                 int num_ubers) {
  numero_vertices = num_vertices;
  numero_ubers = num_ubers;
  numero_aristas = num_aristas;
  lista_vertices = new nodo[numero_vertices];
  ubicaciones_uber = new int[numero_ubers]{
      0}; // Inicializa un array con todos los valores en 0
}

int ListaAdyacencia::agregar_conexion(int num_nodo_origen,
                                      int num_nodo_destino) {
  if (num_nodo_origen >= numero_vertices) {
    cout << "Vertice fuera de rango" << endl;
    return 0;
  }
  nodo *nodo_origen = &lista_vertices[num_nodo_origen];
  while (nodo_origen->next != nullptr) {
    nodo_origen = nodo_origen->next;
  }
  nodo_origen->next = new nodo{num_nodo_destino, nullptr};
  return 1;
}

int ListaAdyacencia::agregar_ubicacion_uber(int ubicacion) {
  int i;
  for (i = 0; i < numero_ubers; i++) {
    // En el pdf se da a entender que las ubicaciones de los ubers y por ende el
    // numero de los nodos es del 1 en adelante, por lo que si una ubicación
    // esta en 0, significa que no ha sido inicializada
    if (ubicaciones_uber[i] == 0) {
      ubicaciones_uber[i] = ubicacion;
      return 1;
    }
  }
  cout << "Ya se ha agregado el numero maximo de ubers" << endl;
  return 0;
}

int *ListaAdyacencia::obtener_ubicaciones_ubers() { return ubicaciones_uber; }

nodo *ListaAdyacencia::obtener_lista_de_vertices() { return lista_vertices; }

/* ****
 * itn encontrar_uber_mas_cerca
 ******
 *    La función se ayuda de bfs() para encontrar la ubicación del uber más
 *    cercano al pasajero y actualiza la variable monto_uber segun la distancia
 *    de este.
 ******
 * Input :
 *  int nodo_pasajero : Nodo desde el que se comenzara a buscar el uber más
 *  cercano int &monto_uber : Variable donde se guardara el precio del viaje del
 *  uber a la ubicación del pasajero
 ******
 * Returns :
 *    uber_mas_cercano : Nodo de la ubicación del nodo más cercano
 **** */
int ListaAdyacencia::encontrar_uber_mas_cerca(int nodo_pasajero,
                                              int &monto_uber) {
  int *distancia =
      new int[numero_vertices]; // guarda la cantidad de nodos de distancia
                                // desde el nodo de origen del bfs, hasta el que
                                // se quiera consultar
  int *predecesor =
      new int[numero_vertices]; // indica cual fue la numeracion del nodo
                                // anterior al actual ej: predecesor[B] = A
                                // indica que para llegar a B viniste de A
  int uber_mas_cercano = -1;
  int distancia_mas_corta = INFINITO;
  for (int i = 0; i < numero_ubers; i++) {
    bfs(lista_vertices, numero_vertices, ubicaciones_uber[i], distancia,
        predecesor);
    if (distancia[nodo_pasajero] < distancia_mas_corta ||
        (distancia[nodo_pasajero] == distancia_mas_corta &&
         ubicaciones_uber[i] < ubicaciones_uber[uber_mas_cercano])) {
      distancia_mas_corta = distancia[nodo_pasajero];
      uber_mas_cercano = i;
    }
  }
  monto_uber = distancia_mas_corta * 300;
  delete[] distancia;
  delete[] predecesor;
  return uber_mas_cercano; // es la posicion del conductor en el arreglo donde
                           // se almacenan los ubers
}

/* ****
 *   int solicitar_uber
 ******
 *    Se ayuda de la función encontrar_uber_mas_cerca(), el cual pasa el costo
 *    del viaje del uber hasta la ubicacion del pasajero. Luego la función bfs
 *    se encarga de construir el mejor camino al destino, que luego la función
 *usa para calcular el costo total del viaje.
 ******
 * Input :
 *   int nodo_inicio : Numero del nodo desde el que se solicita el uber
 *   int nodo_final : Numero del nodo destino del uber después de recoger al
 *                    pasajero
 ******
 * Returns :
 *   monto_total : Costo total del uber solicitado
 **** */
int ListaAdyacencia::solicitar_uber(int nodo_inicio, int nodo_final) {
  int *distancia = new int[numero_vertices];
  int *predecesor = new int[numero_vertices];
  int monto_uber = 0;  // monto de viaje del uber hasta el pasajero
  int monto_total = 0; // monto a cobrar
  int posf_uber = encontrar_uber_mas_cerca(nodo_inicio, monto_uber);
  int num_nodos_camino = 0; // contador
  int nodo_actual =
      nodo_final; // para reconstruir el camino desde el final hasta el inicio
  int camino[NUM_MAX_COLA];
  bfs(lista_vertices, numero_vertices, nodo_inicio, distancia, predecesor);
  while (nodo_actual != -1) {
    camino[num_nodos_camino] = nodo_actual;
    nodo_actual = predecesor[nodo_actual];
    num_nodos_camino++;
  }
  if (distancia[nodo_final] != INFINITO) {
    monto_total = monto_uber + (num_nodos_camino - 1) * 500;
    cout << "Ruta : { ";
    for (int i = num_nodos_camino - 1; i >= 0; i--) {
      cout << camino[i] << " ";
    }
    cout << " }" << endl << "Costo : " << monto_total << endl;
    ubicaciones_uber[posf_uber] = nodo_final;
  } else {
    cout << "Ruta : {} " << endl << "Costo: -1" << endl;
  }
  delete[] distancia;
  delete[] predecesor;
  return monto_total;
}

/* ****
 *   void bfs
 ******
 *    Algoritmo bfs, con la modificacion que guarda el camino desde el nodo
 *    inicial hacia el vertice que se quiera consultar, ademas de mantener un
 *    array con la distancia del nodo inicial a todos los vertices.
 ******
 * Input :
 *   nodo* lista_vertices : lista de adyacencia que representa el grafo donde se
 *                          realizara la busqueda
 *   int numero_vertices :  numero de nodos del grafo
 *   int nodo_inicial : nodo del que comenzara la busqueda
 *   int *distancia : array que representa la distancia de cada nodo respecto al
 *   vertice inicializa int *predecesor : Indica el numero del nodo del que se
 *   proviene al actual
 ******
 * Returns :
 *      void
 **** */
void bfs(nodo *lista_vertices, int numero_vertices, int nodo_inicial,
         int *distancia, int *predecesor) {
  tCola aux;
  bool nodos_visitados[NUM_MAX_COLA]; // arreglo para marcar los nodos
  for (int i = 0; i < numero_vertices;
       i++) { // inicializa los arreglos con las distancias entre vertices
    distancia[i] = INFINITO;
    predecesor[i] = -1;
    nodos_visitados[i] = false;
  }
  distancia[nodo_inicial] = 0;
  nodos_visitados[nodo_inicial] = true;
  aux.enqueue(nodo_inicial);
  while (!aux.vacia()) {
    int nodo_actual;
    nodo_actual = aux.frontValue();
    aux.dequeue();
    nodo *vecino;
    vecino = lista_vertices[nodo_actual].next;
    while (vecino != nullptr) {
      int nodo_aux;
      nodo_aux = vecino->data;
      if (!nodos_visitados[nodo_aux]) {
        nodos_visitados[nodo_aux] = true;
        distancia[nodo_aux] = distancia[nodo_actual] + 1;
        predecesor[nodo_aux] = nodo_actual;
        aux.enqueue(nodo_aux);
      }
      vecino = vecino->next;
    }
  }
}
/* ****
 * ListaAdyacencia leer_data
 ******
 *   Abre y lee un .txt cuyo nombre es pasado por el usuario, el cual contiene
 *información de un grafo para convertirlo en una lista de adyacencia y
 *devolverlo en el return
 ******
 * Input :
 *   string nombre_data_txt : Nombre del .txt con la información del grafo
 ******
 * Returns :
 *   lista_adyacencia, lista de adyacencia que representa el grafo del .txt
 **** */
ListaAdyacencia leer_data(string nombre_data_txt) {
  string linea_actual;
  ifstream archivo_txt(nombre_data_txt);
  getline(archivo_txt, linea_actual);
  int numero_de_nodos = stoi(linea_actual.substr(0, linea_actual.find(" ")));
  linea_actual = linea_actual.substr(linea_actual.find(" ") + 1);
  int numero_de_aristas = stoi(linea_actual.substr(0, linea_actual.find(" ")));
  int numero_ubers = stoi(linea_actual.substr(linea_actual.find(" ") + 1));
  ListaAdyacencia lista_adyacencia{numero_de_nodos + 1, numero_de_aristas,
                                   numero_ubers};
  for (int i = 0; i < numero_de_aristas; i++) {
    getline(archivo_txt, linea_actual);
    int num_nodo_origen = stoi(linea_actual.substr(0, linea_actual.find(" ")));
    int num_nodo_destino =
        stoi(linea_actual.substr(linea_actual.find(" ") + 1));

    lista_adyacencia.agregar_conexion(num_nodo_origen, num_nodo_destino);
  }
  getline(archivo_txt, linea_actual);
  for (int i = 0; i < numero_ubers; i++) {
    int ubicacion_uber = stoi(linea_actual.substr(0, linea_actual.find(" ")));
    lista_adyacencia.agregar_ubicacion_uber(ubicacion_uber);
    if (i != numero_ubers - 1) {
      linea_actual = linea_actual.substr(linea_actual.find(" ") + 1);
    }
  }
  return lista_adyacencia;
}

int main() {
  bool continuar = true;
  int desde;
  int hacia;
  ListaAdyacencia lista_adyacencia = leer_data("data2.txt");
  // int *ubicaciones_ubers = lista_adyacencia.obtener_ubicaciones_ubers();
  // for (int i = 0; i < 4; i++) {
  // cout << ubicaciones_ubers[i] << endl;
  //}
  nodo *array_vertices = lista_adyacencia.obtener_lista_de_vertices();
  for (int i = 0; i < lista_adyacencia.numero_vertices; i++) {
    nodo *nodo_actual = array_vertices[i].next;
    while (nodo_actual != nullptr) {
      // cout << "Vecino del nodo " << i << ":" << nodo_actual->data << endl;
      nodo_actual = nodo_actual->next;
    }
  }
  while (continuar) {
    cout << "Ingrese viaje (Desde/Hacia): ";
    cin >> desde;
    cin >> hacia;
    if (desde >= lista_adyacencia.numero_vertices || hacia >= lista_adyacencia.numero_vertices){
      cout << "Nodo fuera de rango" << endl;
      continue;
    }
    else if (desde == -1 && hacia == -1){
      continuar = false;
      continue;
    
    }
    lista_adyacencia.solicitar_uber(desde, hacia);
  }
  return 0;
}
