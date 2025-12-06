#include "algoritmo.h"

#include <limits>
#include <algorithm>
#include <cmath>

ShortestPathSolver::ShortestPathSolver(const Graph& g)
    : graph(g) {}

// Heurística sencilla: distancia euclídea en grados de lat/lon
double ShortestPathSolver::heuristic(int v, int goal) const {
    const Coord& c1 = graph.coord(v);
    const Coord& c2 = graph.coord(goal);

    double dx = c2.lon - c1.lon;
    double dy = c2.lat - c1.lat;

    double dist_deg = std::sqrt(dx * dx + dy * dy);
    return dist_deg;  // en "unidades de grados", muy pequeño comparado con los metros reales
}

// --------------------------------------------------
// Algoritmo de mejor primero / A* (f = g + h)
// --------------------------------------------------
bool ShortestPathSolver::mejor_primero(int start, int goal,
                                       std::vector<int>& path,
                                       long long& totalCost,
                                       long long& expansions) {
    int n = graph.numVertices();
    const long long INF = std::numeric_limits<long long>::max();

    // g(n): coste real desde el origen hasta n
    std::vector<long long> dist(n + 1, INF);

    ClosedList closed(n);
    ListaAbierta abierta;

    dist[start] = 0;
    double h0 = heuristic(start, goal);
    // En la lista abierta guardamos (id, f, g)
    abierta.insertar(start, /*f=*/dist[start] + h0, /*g=*/dist[start]);

    expansions = 0;
    totalCost = -1;

    while (!abierta.es_vacia()) {
        Estado actual = abierta.sacar_mejor();
        int u = actual.id;

        // g(u) que viene del Estado puede estar desfasado si hemos mejorado dist[u],
        // así que usamos siempre el dist[u] actual como valor de referencia.
        if (actual.g > static_cast<double>(dist[u])) {
            continue; // entrada vieja en la cola
        }

        if (closed.isVisited(u)) {
            continue;
        }

        closed.markVisited(u);
        ++expansions;

        if (u == goal) {
            totalCost = dist[u];
            reconstructPath(start, goal, closed, path);
            return true;
        }

        const auto& neigh = graph.neighbors(u);
        for (const auto& e : neigh) {
            int v = e.to;
            long long newG = dist[u] + static_cast<long long>(e.cost);

            if (newG < dist[v]) {
                dist[v] = newG;
                closed.setParent(v, u); // backpointer

                double h = heuristic(v, goal);
                double f = static_cast<double>(newG) + h;
                abierta.insertar(v, f, static_cast<double>(newG));
            }
        }
    }

    // No hay camino
    return false;
}

// --------------------------------------------------
// Reconstrucción del camino a partir de backpointers
// --------------------------------------------------
void ShortestPathSolver::reconstructPath(int start, int goal,
                                         const ClosedList& closed,
                                         std::vector<int>& path) {
    path.clear();
    int current = goal;
    while (current != -1) {
        path.push_back(current);
        if (current == start) break;
        current = closed.getParent(current);
    }
    std::reverse(path.begin(), path.end());
}

