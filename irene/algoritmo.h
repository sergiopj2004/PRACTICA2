#pragma once

#include <vector>
#include "grafo.h"
#include "abierta.h"
#include "cerrada.h"

class ShortestPathSolver {
public:
    explicit ShortestPathSolver(const Grafo& g);

    // Implementación del algoritmo A* (f= g + h)
    // Si encuentra solución, devuelve true y camino solución
    bool mejor_primero(int start, int goal,
                       std::vector<int>& path,
                       long long& totalCost,
                       long long& expansions);

private:
    const Grafo& graph;
    
    // Reconstruye el camino final mediante los padres almacenados en la lista cerrada
    void reconstructPath(int start, int goal,
                         const ListaCerrada& cerrada,
                         std::vector<int>& path);

    // Heurística: distancia euclídea en lon/lat
    double heuristic(int v, int goal) const;
};