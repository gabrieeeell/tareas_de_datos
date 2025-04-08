#include <iostream>
#include <cmath>
using namespace std;

struct persona {
	int edad;
	char nombre[10];
};

char* devuelve(){
	char** array[2] = {{'a','b'},{'c','d'}};
	int width = 1920;
	int height = 1080;
	return array, width, height;
}

int main() {
	char* lo_devuelto = devuelve;

    return 0;
}
