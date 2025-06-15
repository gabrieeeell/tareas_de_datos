#include <iostream>
using namespace std;

int main() {
  string asd = "hola que tal";
  asd.substr(0, asd.find(" ") + 1);
  cout << asd << endl;
}
