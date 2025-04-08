Integrantes:
1.- Gabriel Garces 202473555-1
2.- Gabriel Ordenes 202473521-7

Pregunta 1: ¿Por qué si los pixeles de la imagen son números enteros ∈ [0, 255], utilizamos el tipo de dato unsigned char?

Respuesta: Utilizamos el tipo de dato unsigned char debido precisamente a que puede representar valores desde el 0 hasta el 255, lo cual es justamente lo que necesitamos para los pixeles de la imagen

Pregunta 2: ¿Cuales serı́an las diferencias entre usar unsigned int versus unsigned char? Responder cuantitativamente para una imagen RGB de dimensiones (N,N,3).

Respuesta: La diferencia entre usar un unsigned int versus un unsigned char reside en la memoria que ocuparan estas variables. Un unsigned int ocupa un total de memoria de 4 bytes (32 bits), y abarca numeros en un rango de [0,4294967295], a diferencia del unsigned char que tiene un rango de [0,255] y ocupa solo 1 byte (8 bits) de memoria, por lo cual no tendria sentido implementar un unsigned int. Como sabemos que nuestra imagen sera de dimensiones (N,N,3) sabemos que en caso de usar un unsigned int, necesitaremos 4 bytes por canal de RGB , lo cual nos daria 12 bytes en total por pixel de imagen. En cambio si utilizamos un unsigned char para una imagen con las mismas dimenisones utilizariamos 1 byte de memoria por canal, por lo cual utilizariamos un total de 3 bytes por pixel, por lo cual si utilizaramos el unsigned int estariamos utilizando 4 veces mas memoria sin motivo alguno.

Pregunta 3: Usando este esquema de almacenamiento unidimensional: ¿Cuál serı́a el ı́ndice del primer pixel del canal azul?

Respuesta: En un esquema de almacenamiento unidimensional, se almacenaran los canales de los pixeles uno despues de otro, tal que los 3 primeros indices serian para los canales rojo, azul, verde (Red,Green,Blue) del primer pixel.
Por lo que el indice 3 representaria el primer canal azul de un pixel (se tendria que usar arrglo[2] para el indice 3)

fuente: https://solarianprogrammer.com/2019/06/10/c-programming-reading-writing-images-stb_image-libraries/

Pregunta 4: ¿Cómo podemos convertir una imagen RGB a escala de grises?

Respuesta : Podriamos obtener el promedio de los canales rgb y luego reescribir la imagen con un solo canal usando el promedio calculado, como se hace en el siguiente codigo de la documentacion de stb_image

"""
for(unsigned char *p = img, *pg = gray_img; p != img + img_size; p += channels, pg += gray_channels) {
         *pg = (uint8_t)((*p + *(p + 1) + *(p + 2))/3.0);
         if(channels == 4) {
             *(pg + 1) = *(p + 3);
         }
     }
"""

También se podria ocupar una formula para la luminosidad aparente del pixel, en vez del promedio.

Fuente: https://solarianprogrammer.com/2019/06/10/c-programming-reading-writing-images-stb_image-libraries/

Fuentes:


1.- Gonzalez, R. C., & Woods, R. E. (2018). Digital Image Processing, Global Edition. Pearson UK.
2.- Sierra, F. J. C. (2007). C/C++: curso de programación. Editorial Ra-Ma.
3.- Miscellaneous image transformations. (s. f.). OpenCV 2.4.13.7 Documentation. Recuperado 6 de abril de 2025, de https://docs-opencv-org.translate.goog/2.4/doc/user_guide/ug_mat.html?_x_tr_sl=en&_x_tr_tl=es&_x_tr_hl=es&_x_tr_pto=tc#basic-operations-with-images

