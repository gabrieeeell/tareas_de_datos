#include <iostream>
using namespace std;

struct persona {
  int edad;
  string nombre;
};

int main() {
  persona gabriel = persona{19, "Gabriel"};
  persona copia_gabriel = persona{gabriel};
  cout << copia_gabriel.nombre << endl;
  return 0;
}
