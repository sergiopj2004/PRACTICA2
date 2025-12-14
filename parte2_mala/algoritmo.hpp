#ifndef ALGORITMO_HPP
#define ALGORITMO_HPP

#include "grafo.hpp"
#include <vector>
#include <limits>

// Estructura para devolver los resultados de la búsqueda
struct Solucion {
    std::vector<int> camino;    // Secuencia de IDs de nodos
    double coste_total;         // Coste acumulado (g)
    long nodos_expandidos;      // Contador para comparar eficiencia
    bool encontrada;            // True si se llegó al destino
};

class AlgoritmoBusqueda {
private:
    // Radio de la Tierra en metros (aprox) para Haversine
    const double RADIO_TIERRA = 6371000.0;

    // Función Heurística: Distancia Haversine (Ortodrómica)
    double calcular_heuristica(int id_actual, int id_destino, const Grafo& grafo) const;

    // Conversión de grados a radianes
    double to_rad(double degree) const;

public:
    AlgoritmoBusqueda();

    /**
     * Resuelve el problema del camino más corto.
     * @param inicio ID del nodo origen
     * @param fin ID del nodo destino
     * @param grafo Referencia al objeto Grafo cargado
     * @param usar_heuristica Si es true, usa A* (Haversine). Si es false, usa Dijkstra (h=0).
     */
    Solucion resolver(int inicio, int fin, const Grafo& grafo, bool usar_heuristica);
};

#endif // ALGORITMO_HPP