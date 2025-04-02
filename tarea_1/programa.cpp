#include <iostream>
#include <cmath>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"
using namespace std;

int atenuacion() {
    float atenuacion = 0.5;
    int width, height, org_channels;
    unsigned char *img = stbi_load("Pikachu.png", &width, &height, &org_channels, 0);
    if(img == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }
    int channels = 4;
    size_t img_size = width * height * channels;

    // !!! Probar multiplicando tambien el valor de los canales por la atenuación
    
    for (unsigned char *p = img; p != img + img_size; p += channels) {
        if (org_channels == 4) {
            *(p + 3) = (*(p + 3)) * atenuacion;
        } else {
            *(p + 3) = atenuacion;
        }
    }

    stbi_write_png("Pikachu.png", width, height, channels, img, width * channels);
    stbi_image_free(img);
    return 0;
}

int rotar_90_grados() {
    int width, height, channels;
    unsigned char *p_copia_img = stbi_load("Pikachu_para_copiar.png", &width, &height, &channels, 0); 
    size_t img_size = width * height * channels;
    unsigned char *img = (unsigned char*)malloc(img_size); 
    if(img == NULL) {
        printf("Error in loading the image\n");
        stbi_image_free(img);
        exit(1);
    }
    int new_width = height;
    int new_height = width;
    // empezar por el pixel de la esquina de arriba a la deracha en "img" y de ahi ir rellenando la columna hacia abajo respectivamente con el pixel que le corresponderia en el orden de "copia_img" y cuando se acaba la columna voy a la de la izquierda cosa que cada columna va siendo equivalente a la fila de la original,
    for (int x = 0; x < width; x++) { 
        for (int y = 0; y < height; y++, p_copia_img += channels) {
            int position_in_output = channels*((width - x) + width*y);
            *(&img[position_in_output]) = *p_copia_img;
            *(&img[position_in_output] + 1) = *(p_copia_img + 1); 
            *(&img[position_in_output] + 2) = *(p_copia_img + 2);
            if (channels == 4) {
                *(&img[position_in_output] + 3) = *(p_copia_img + 3); 
            }
        }
    }

    stbi_write_png("Pikachu.png", new_width, new_height, channels, img, new_width * channels); 
    stbi_image_free(img);
    return 0;
}

int invertir_eje_x() {
    int width, height, channels;
    unsigned char *p_copia_img = stbi_load("Pikachu_para_copiar.png", &width, &height, &channels, 0); 
    size_t img_size = width * height * channels;
    unsigned char *img = (unsigned char*)malloc(img_size); 
    if(img == NULL) {
        printf("Error in loading the image\n");
        stbi_image_free(img);
        exit(1);
    }
    // empezar por el pixel de la esquina de arriba a la deracha en "img" y de ahi ir rellenando la columna hacia abajo respectivamente con el pixel que le corresponderia en el orden de "copia_img" y cuando se acaba la columna voy a la de la izquierda cosa que cada columna va siendo equivalente a la fila de la original,
    for (int y = 0; y < height; y++) { 
        for (int x = 0; x < width; x++, p_copia_img += channels) {
            int position_in_output = channels* (width - x + (y*width));
            *(&img[position_in_output]) = *p_copia_img;
            *(&img[position_in_output] + 1) = *(p_copia_img + 1); 
            *(&img[position_in_output] + 2) = *(p_copia_img + 2);
            if (channels == 4) {
                *(&img[position_in_output] + 3) = *(p_copia_img + 3); 
            }
        }
    }

    stbi_write_png("Pikachu.png", width, height, channels, img, width * channels); 
    stbi_image_free(img);
    return 0;
}

int limite_negro_y_blanco(float limite) {
    int width, height, org_channels;
    unsigned char *img = stbi_load("Pikachu.png", &width, &height, &org_channels, 0);
    if(img == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }
    int channels = 4;
    size_t img_size = width * height * channels;
    for (unsigned char *p = img; p != img + img_size; p += channels) {
        float promedio;
        float total = *p + *(p + 1) + *(p + 2);
        if (channels == 4) {     // Para considerar dentro del promedio el 4to canal si es que la imagen lo tiene
            total += *(p + 3);
            promedio = total/4;
        } else {
            promedio = total/3;
        }
        if (promedio/255 < limite) {
           *p = 0;
           *(p + 1) = 0;
           *(p + 2) = 0;
        } else {
            *p = 255;
            *(p + 1) = 255;
            *(p + 2) = 255;
        }
    }

    stbi_write_png("Pikachu.png", width, height, channels, img, width * channels);
    stbi_image_free(img);
    return 0;
}

// Ascii
void convertir_en_ascii() {
    int width, height, channels;
    unsigned char *img = stbi_load("Pikachu.png", &width, &height, &channels, 0);
    if(img == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }
    size_t img_size = width * height * channels;
 
    // Voy a sacar la luminosidad aparente de los canales de un pixel y luego lo dividire en 21.25 y redondeare hacia arriba (0.01 = 1) tal que puedan dar los
    //  valores del intervalo [1,12], y asi se le asignara a ese pixel una letra del siguiente arreglo
    
    char conversion_caracteres[12] = {'.',',','-','~',':',';','=','!','*','#','$','@'};
    fstream file;
    file.open("pikachu.txt",ios::app)
    if( !file.is_open() ){
        cout << "Error al abrir el archivo\n";
        exit(1);
    };

    for (unsigned char *p = img; p != img + img_size; p += channels) {
    
        // aca uso una formula para la luminosidad aparente

        float luminosidad_aparente = (*p) * 0.299 + *(p + 1) * 0.587 + *(p + 2) * 0.114;
        int indice_de_caracter_correspondiente = ceil(luminosidad_aparente/21.25);
    
    }

    stbi_write_png("Pikachu.png", width, height, channels, img, width * channels);
    stbi_image_free(img);
    return 0;
}

int main() {

    return 0;
}
