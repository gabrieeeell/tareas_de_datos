#include <fstream>
#include <iostream>
#include <string>
using namespace std;

struct nodo {
  int data;
  nodo *next = nullptr;
};

// Variables globales (*Revisar si acaban teniendo necesidad de ser gloabales*)

// Para la implementación del grafo se usaran listas adyacentes, ya que esta es
// más eficiente para hacer Bread-First Search (BFS) la cual sera el tipo de
// busqueda implementada en nuestro programa

// Lista de adyacencia modificada para incluir una lista con la ubicación de los
// ubers

// Notar que todas las listas enlazdas empiezan con un nodo con un dato que no
// se usa
class ListaAdyacencia {
  nodo *lista_vertices;
  int numero_vertices; // (numero de nodos)
  int *ubicaciones_uber;
  int numero_ubers;
  int numero_aristas;

public:
  // Después debo incluir una función tipo "agregar uber"
  ListaAdyacencia(int numero_vertices, int numero_aristas, int numero_ubers);
  int agregar_conexion(int nodo_origen, int nodo_destino);
  int agregar_ubicacion_uber(int ubicacion);
  int *obtener_ubicaciones_ubers();
  nodo *obtener_lista_de_vertices();
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

ListaAdyacencia leer_data(string nombre_data_txt) {
  string linea_actual;
  ifstream archivo_txt(nombre_data_txt);
  getline(archivo_txt, linea_actual);
  int numero_de_nodos = stoi(linea_actual.substr(0, linea_actual.find(" ")));
  linea_actual = linea_actual.substr(linea_actual.find(" ") + 1);
  int numero_de_aristas = stoi(linea_actual.substr(0, linea_actual.find(" ")));
  int numero_ubers = stoi(linea_actual.substr(linea_actual.find(" ") + 1));
  ListaAdyacencia lista_adyacencia{numero_de_nodos, numero_de_aristas,
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
  ListaAdyacencia lista_adyacencia = leer_data("data1.txt");
  int *ubicaciones_ubers = lista_adyacencia.obtener_ubicaciones_ubers();
  for (int i = 0; i < 4; i++) {
    cout << ubicaciones_ubers[i] << endl;
  }
  nodo *array_vertices = lista_adyacencia.obtener_lista_de_vertices();
  for (int i = 0; i < 40; i++) {
    nodo *nodo_actual = array_vertices[i].next;
    while (nodo_actual != nullptr) {
      cout << "Vecino del nodo " << i << ":" << nodo_actual->data << endl;
      nodo_actual = nodo_actual->next;
    }
  }
}
