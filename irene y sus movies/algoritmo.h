#pragma once

#include <vector>
#include "grafo.h"
#include "abierta.h"
#include "cerrada.h"

class ShortestPathSolver {
public:
    explicit ShortestPathSolver(const Graph& g);

    // Algoritmo de mejor primero (A* con f = g + h)
    // Devuelve true si encuentra camino y rellena:
    //   - path: secuencia de vértices desde start hasta goal
    //   - totalCost: coste total (suma de arcos)
    //   - expansions: número de nodos expandidos
    bool mejor_primero(int start, int goal,
                       std::vector<int>& path,
                       long long& totalCost,
                       long long& expansions);

private:
    const Graph& graph;

    void reconstructPath(int start, int goal,
                         const ClosedList& closed,
                         std::vector<int>& path);

    // Heurística: distancia geodésica aproximada entre v y goal (metros)
    double heuristic(int v, int goal) const;
};
