Gabriel Garcés 202473555-1
Gabriel Ordenes 202473521-7

Analisis complejidad solicitar uber

Nuestro algoritmo bfs no tiene una complejidad notablemente mayor al pasado en clases ya que al implementarlo usando una lista de adyacencia, la obtención de los vecinos tiene tiempo O(1). Osea que la complejidad es la dada por el while que no acaba mientras hayan vertices (V) y la del for que recorre las aristas de cada vertice O(E).
La complejidad del bfs seria O(V + E).
Luego la funcion solicitar_uber() llama a encontrar_uber_mas_cerca() la que a su vez llama a bfs 4 veces. Después solicitar_uber() vuelve a llamar a bfs() para
encontrar el camino del inicio al destino. lo que sumaria 4 llamadas a bfs, osea una complejidad de O(5(V + E)) = O(V + E) = O(n), con n la suma de vertices y aristas del grafo





