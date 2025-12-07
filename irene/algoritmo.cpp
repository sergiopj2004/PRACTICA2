// algoritmo.cpp
#include "algoritmo.h"
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace std;

// 1 grado de latitud son aprox 111.1 km
#define METROS_POR_GRADO_LAT 111132.0

// Para la longitud depende de la latitud. 
// En EEUU (lat ~38) el factor es aprox 0.78. 
// 111132 * 0.78 = 86682 metros aprox por grado de longitud.
// Uso un valor un poco menor para asegurar que sea admisible (no pasarnos).
#define METROS_POR_GRADO_LON 85000.0 

ShortestPathSolver::ShortestPathSolver(const Grafo& g) : graph(g) {}

// Heurística: Aproximación Euclídea Plana con factores fijos
// Es mucho más rápida que calcular senos y cosenos para cada nodo.
double ShortestPathSolver::heuristic(int u, int goal) const {
    Coordenada c1 = graph.coordenada(u);
    Coordenada c2 = graph.coordenada(goal);

    double dLat = fabs(c1.latitud - c2.latitud);
    double dLon = fabs(c1.longitud - c2.longitud);

    // Convertimos diferencia de grados a metros DIRECTAMENTE
    double dy = dLat * METROS_POR_GRADO_LAT;
    double dx = dLon * METROS_POR_GRADO_LON;

    // Pitágoras simple
    return sqrt(dx*dx + dy*dy);
}

// Algoritmo de mejor primero / A* (f = g + h)
bool ShortestPathSolver::mejor_primero(int start, int goal,
                                       std::vector<int>& path,
                                       long long& totalCost,
                                       long long& expansions) {
    int n = graph.numVertices();
    const long long INF = std::numeric_limits<long long>::max();

    // dist guarda el mejor coste conocido hasta cada nodo
    std::vector<long long> dist(n + 1, INF);

    ListaCerrada cerrada(n);
    ListaAbierta abierta;

    // Inicialización del nodo inicial
    dist[start] = 0;
    double h0 = heuristic(start, goal);
    // En la lista abierta guardamos (id, f, g)
    abierta.insertar(start, dist[start] + h0, dist[start]);

    expansions = 0;
    totalCost = -1;

    while (!abierta.es_vacia()) {
        Estado actual = abierta.sacar_mejor();
        int u = actual.id;

        // Si el estado en la cola no es el mejor g conocido, lo ignoramos
        if (actual.g > static_cast<double>(dist[u])) {
            continue; 
        }

        // Si ya lo hemos visitado, lo ignoramos
        if (cerrada.isVisited(u)) {
            continue;
        }

        cerrada.markVisited(u);
        ++expansions;

        // Si llegamos al objetivo, reconstruimos el camino
        if (u == goal) {
            totalCost = dist[u];
            reconstructPath(start, goal, cerrada, path);
            return true;
        }

        // Exploramos los vecinos del nodo 
        const auto& neigh = graph.vecinos(u);
        for (const auto& e : neigh) {
            int v = e.destino;
            long long newG = dist[u] + static_cast<long long>(e.coste);
            
            // En caso de encontrar un mejor camino, actualizamos
            if (newG < dist[v]) {
                dist[v] = newG;
                cerrada.setParent(v, u); // backpointer

                double h = heuristic(v, goal);
                double f = static_cast<double>(newG) + h;
                abierta.insertar(v, f, static_cast<double>(newG));
            }
        }
    }

    // Si se vacía la lista abierta sin llegar a la meta, no existe camino
    return false;
}


// Reconstrucción del camino a partir de backpointers
void ShortestPathSolver::reconstructPath(int start, int goal,
                                         const ListaCerrada& cerrada,
                                         std::vector<int>& path) {
    path.clear();
    int current = goal; // empezamos por el final
    // Retrocedemos usando los padres de cerrada hasta llegar al nodo inicial (que no tendrá padre y dará -1)
    while (current != -1) {
        path.push_back(current);
        if (current == start) break;
        current = cerrada.getParent(current);
    }
    std::reverse(path.begin(), path.end());
}
