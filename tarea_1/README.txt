Integrantes:
1.- Gabriel Garces 202473555-1
2.- Gabriel Ordenes 202473521-7

Pregunta 1: ¿Por qué si los pixeles de la imagen son números enteros ∈ [0, 255], utilizamos el tipo de dato unsigned char?

Respuesta: Utilizamos el tipo de dato unsigned char debido precisamente a que puede representar valores desde el 0 hasta el 255, lo cual es justamente lo que necesitamos para los pixeles de la imagen

Pregunta 2: ¿Cuales serı́an las diferencias entre usar unsigned int versus unsigned char? Responder cuantitativamente para una imagen RGB de dimensiones (N,N,3).

Respuesta: La diferencia entre usar un unsigned int versus un unsigned char reside en la memoria que ocuparan estas variables. Un unsigned int ocupa un total de memoria de 4 bytes (32 bits), y abarca numeros en un rango de [0,4294967295], a diferencia del unsigned char que tiene un rango de [0,255] y ocupa solo 1 byte (8 bits) de memoria, por lo cual no tendria sentido implementar un unsigned int. Como sabemos que nuestra imagen sera de dimensiones (N,N,3) sabemos que en caso de usar un unsigned int, necesitaremos 4 bytes por canal de RGB , lo cual nos daria 12 bytes en total por pixel de imagen. En cambio si utilizamos un unsigned char para una imagen con las mismas dimenisones utilizariamos 1 byte de memoria por canal, por lo cual utilizariamos un total de 3 bytes por pixel, por lo cual si utilizaramos el unsigned int estariamos utilizando 4 veces mas memoria sin motivo alguno.

Pregunta 3: Usando este esquema de almacenamiento unidimensional: ¿Cuál serı́a el ı́ndice del primer pixel del canal azul?

Respuesta:

Pregunta 4: ¿Cómo podemos convertir una imagen RGB a escala de grises?

Respuesta: Para convertir una imagen RGB a escala de grises necesitamos cambiar cada uno de los valores de color RGB al de la siguiente formula:
  (0.299*R + 0.587+G + 0.114*B) 
Es necesario multiplicar los canales de color por esos porcentajes debido a que permiten conservar la luminosidad del RGB sin conservar los detales del color

Pregunta 6:



Fuentes:


1.- Gonzalez, R. C., & Woods, R. E. (2018). Digital Image Processing, Global Edition. Pearson UK.
2.- Sierra, F. J. C. (2007). C/C++: curso de programación. Editorial Ra-Ma.
3.- Miscellaneous image transformations. (s. f.). OpenCV 2.4.13.7 Documentation. Recuperado 6 de abril de 2025, de https://docs-opencv-org.translate.goog/2.4/doc/user_guide/ug_mat.html?_x_tr_sl=en&_x_tr_tl=es&_x_tr_hl=es&_x_tr_pto=tc#basic-operations-with-images

