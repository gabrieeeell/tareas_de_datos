#include <iostream>
#include <cmath>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"
using namespace std;


// En la tarea se indica que las imagenes procesadas no tendran transparencia, por lo que no cubrire el caso en que las imagenes tengan un 4to canal

// Struct que almacena la data de una imagen
struct Imagen{
    unsigned char* data;
    int width, height, channels;
};

void operacion_1(Imagen *img, const char filename[256]) {
    /*****
 	*	operacion_1
 	******
 	*	Hace una reflexion en el "eje x = width/2", tal que para cada pixel la "cordenada x" pasa a ser width - x
 	******
 	*Input:
 	*	Imagen *img : Puntero a struct que almacena los datos de la imagen (como ancho, canales, etc.)
	*	char filename[256] : Nombre del archivo de la imagen.
 	******
 	*Returns:
 	*	void;
 	*****/


    size_t img_size = img->width * img->height * img->channels;
    unsigned char *img_invertida = new unsigned char[img_size]; 

    for (int y = 0; y < img->height; y++) { 
        for (int x = 0; x < img->width; x++, img->data += img->channels) {
            int posicion_invertida = img->channels* (img->width - x + (y*img->width));
            *(&img_invertida[posicion_invertida]) = *img->data;
            *(&img_invertida[posicion_invertida] + 1) = *(img->data + 1); 
            *(&img_invertida[posicion_invertida] + 2) = *(img->data + 2);
        }
    }

    stbi_write_png("Pikachu.png", img->width, img->height, img->channels, img_invertida, img->width * img->channels); 
    delete[] img;

}

void operacion_2(Imagen *img, const char filename[256]) {
	/*****
 	*	operacion_2
 	******
 	*	Rota 90 grados, pixel por pixel de la imagen, a un nuevo espacio de memoria dinamica con el mismo tamaño de la imagen, usando la formula "nueva_posicion_del_pixel = channels*( (width - x) + width * y)", donde x,y serian 
	*	la posición del pixel originalmente en la imagen (tomando como (0,0) la esquina superior izquierda).
 	******
 	*Input:
 	*	Imagen *img : Puntero a struct que almacena los datos de la imagen (como ancho, canales, etc.)
	*	char filename[256] : Nombre del archivo de la imagen.
 	******
 	*Returns:
 	*	void;
 	*****/
	cout << img->channels << endl;
    size_t img_size = img->width * img->height * img->channels;
    unsigned char *img_rotada = new unsigned char[img_size]; 

    int new_width = img->height;
    int new_height = img->width;

    for (int x = 0; x < img->width; x++) { 
        for (int y = 0; y < img->height; y++, img->data += img->channels) {
            int posicion_rotada = img->channels*((img->width - x) + img->width*y);
            *(&img_rotada[posicion_rotada]) = *(img->data);
            *(&img_rotada[posicion_rotada] + 1) = *(img->data + 1); 
            *(&img_rotada[posicion_rotada] + 2) = *(img->data + 2);

        }
    }
    stbi_write_png(filename, new_width, new_height, img->channels, img_rotada, new_width * img->channels); 
    delete[] img;
}

void operacion_3(Imagen *img,float atenuacion, const char filename[256]) {
	/*****
 	*	operacion_3
 	******
 	*	Cicla por todos los pixeles de la imagen, multiplicando el valor de sus canales correspondientes por la atenuación dada.
 	******
 	*Input:
 	*	Imagen *img : Puntero a struct que almacena los datos de la imagen (como ancho, canales, etc.)
	*	float atenuacion : Atenuacion entre 0 y 1 que sera aplicada a la imagen.
	*	char filename[256] : Nombre del archivo de la imagen.
 	******
 	*Returns:
 	*	void;
 	*****/


    size_t img_size = img->width * img->height * img->channels;
 
    for (unsigned char *p = img->data; p != img->data + img_size; p += img->channels) {
	*p = *p * atenuacion;
	*(p+1) = *(p+1) * atenuacion;
	*(p+2) = *(p+2) * atenuacion;
    }

    stbi_write_png(filename, img->width, img->height, img->channels, img->data, img->width * img->channels);
    stbi_image_free(img);
}

void operacion_4(Imagen *img,int limite,const char filename[256]) {
	/*****
 	*	operacion_4
 	******
 	*	Por cada pixel toma el promedio de los canales y si es menor al limite dado deja todos los canales en 0 (que vendria siendo un pixel negro), si no es
    	*	menor al limite deja todos los canales en 255 que seria un pixel blanco.
 	******
 	*Input:
 	*	Imagen *img : Puntero a struct que almacena los datos de la imagen (como ancho, canales, etc.)
	*	int limite : valor entre 0 y 255 que representara el promedio que deben superar el promedio de los canales de un pixel para ser pixeles blancos, sino
    	*	seran pixeles negros.
	*	char filename[256] : Nombre del archivo de la imagen.
 	******
 	*Returns:
 	*	void;
 	*****/


    size_t img_size = img->width * img->height * img->channels;
    for (unsigned char *p = img->data; p != img->data + img_size; p += img->channels) {
        float promedio;
        float total = *p + *(p + 1) + *(p + 2);
	promedio = total/3;
	cout << "promedio: "<<promedio << " limite: "<<limite <<  (promedio < limite) << endl;
        if ((int)promedio < limite) {
           *p = 0;
           *(p + 1) = 0;
           *(p + 2) = 0;
        } else {
            *p = 255;
            *(p + 1) = 255;
            *(p + 2) = 255;
        }
    }

    stbi_write_png("Pikachu.png", img->width, img->height, img->channels, img->data, img->width * img->channels);
    stbi_image_free(img);
}

char** convertir_en_ascii(Imagen *img) {
	/*****
 	*	convertir_en_ascii
 	******
 	*	La función saca la luminosidad aparente de los canales de un pixel (que da entre 0 y 255) la cual luego se divide en 21.25 y se redondea hacia arriba, tal que pueda dar un valor 
        *	entero del intervalo [1,12], y asi dependiendo de este valor se le asigna un caracter para representar al pixel
 	******
 	*Input:
 	*	Imagen *img : Puntero a struct que almacena los datos de la imagen (como ancho, canales, etc.)
 	******
 	*Returns:
 	*	char** un arreglo bidimensional de caracteres que representan cada pixel de la imagen dada.
 	*****/

    // *Nota*
    // en vez de usar la imagen en escala de grises cargandola con 1 canal, use una formula para calcular la luminosidad aparente en a los valores de los canales rgb 

    size_t img_size = img->width * img->height * img->channels;
    
    char conversion_caracteres[12] = {'.',',','-','~',':',';','=','!','*','#','$','@'};
    
    char **arreglo_ascii = new char*[img->height];

    for (int i = 0; i < img->height; i++){
	arreglo_ascii[i] = new char[img->width + 1];
    };
    int y = 0;
    int x = 0;
    for (unsigned char *p = img->data; p != img->data + img_size; p += img->channels, x++) {
    
        // aca uso la formula para la luminosidad aparente

        float luminosidad_aparente = (*p) * 0.299 + *(p + 1) * 0.587 + *(p + 2) * 0.114;

	// Si la luminosidad_aparente es demasiado alta (el pixel es completamtene blanco) se coloca un espacio en vez de un caracter
	
	if (luminosidad_aparente == 255) {
		arreglo_ascii[y][x] = ' ';
	} else {
	        int indice_de_caracter_correspondiente = ceil(luminosidad_aparente/21.25);
		arreglo_ascii[y][x] = conversion_caracteres[indice_de_caracter_correspondiente];
	};

	//El condicional de abajo verifica si el puntero se encuentra "en el borde derecho de la imagen" osea al final de una linea de pixeles, para poner un salto de linea en el arreglo
	
	if (x == (img->width - 1)){
		x++;
		arreglo_ascii[y][x] = '\n';
		x = -1;
		y++;
	}

    }
    stbi_image_free(img);
    return arreglo_ascii;	
};

void save_ascii(char** arreglo_ascii,int height,const char filename[256]) {
	/*****
 	*	save_ascii
 	******
	*	Toma un arreglo bidimensional de caracteres, el cual representa una imagen en ascii art y lo coloca en un archivo .txt para que sea apreciable.
 	******
 	*Input:
	*	char** arreglo_ascii : arreglo generado por la funcion "convertir_en_ascii"
	*	int height : altura original de la imagen que representa el ascii art
	*	const char filename[256] : nombre del .txt que se va a generar (debe incluir la extension)
 	******
 	*Returns:
 	*	char** un arreglo bidimensional de caracteres que representan cada pixel de la imagen dada.
 	*****/
	ofstream file;
	file.open(filename,ios::app);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x >= 0; x++) {
			file << arreglo_ascii[y][x];
			// Si encuentra un salto de linea hace que la condicion del bucle x se deje de cumplir (el -17 es un numero negativo arbitrario)
			if (arreglo_ascii[y][x] == '\n') {
				x = -17;
			}
		}
	}
	for (int y = 0; y < height; y++) {
		delete[] arreglo_ascii[y];
	}
	delete[] arreglo_ascii;
	file.close();
}

// Abrir imagen desde memoria local
Imagen* load(const char* filename) {
    Imagen* img = new Imagen();
    img->data = stbi_load(filename, &img->width, &img->height, &img->channels, 3);
    img-> channels = 3;
    if(img->data == NULL) {
        printf("Error en la carga de la imagen\n");
        exit(1);
    };
    return img;
}

// Guardar imagen en memoria local
void save(Imagen* img, const char* filename) {
    stbi_write_png(filename, img->width, img->height, img->channels, img->data, img->width * img->channels);
    cout << "Imagen guardada:  " << filename << "\n";
}

int main() {
    	//Cargamos la imagen
    	Imagen* img = load("Pikachu_para_copiar.jpg");
	char** arreglo_ascii = convertir_en_ascii(img); save_ascii(arreglo_ascii,256, "pikachu.txt");
	//operacion_1(img, "Pikachu.png");
	//operacion_2(img, "Pikachu.png");
	//operacion_3(img, 0.5, "Pikachu.png");
	//operacion_4(img, 120, "Pikachu.png");
}
