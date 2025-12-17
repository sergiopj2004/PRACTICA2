// algoritmo.cpp
#include "algoritmo.h"
#include <cmath>
#include <algorithm>
#include <limits>
#include <iostream>

// Conversión de grados de latitud a metros
// 1 grado de latitud son aproximadamente 111132 metros
#define METROS_POR_GRADO_LAT 111132.0

// La longitud depende de la latitud
/*En el mapa de Estados Unidos la latitud es aproximadamente 38 (su coseno es aprox 0.78)
Los metros por grado de longitud consiste en multiplicar la latitud por este factor
111132 * 0.78 = 86682 metros aprox por grado de longitud.
Escogemos un valor un poco menor para asegurar que sea admisible .
*/
#define METROS_POR_GRADO_LON 85000.0

SolverCaminoMinimo::SolverCaminoMinimo(const Grafo& g) : grafo(g) {}

// Heurística: Aproximación Euclídea Plana con factores fijos
double SolverCaminoMinimo::heuristica(int u, int objetivo) const {
    return 0.0;
    Coordenada c1 = grafo.coordenada(u);
    Coordenada c2 = grafo.coordenada(objetivo);

    double dLat = std::fabs(c1.latitud - c2.latitud);
    double dLon = std::fabs(c1.longitud - c2.longitud);

    // Convertimos diferencia de grados a metros
    double dy = dLat * METROS_POR_GRADO_LAT;
    double dx = dLon * METROS_POR_GRADO_LON;

    return std::sqrt(dx*dx + dy*dy);
}

// A* (f = g + h)
bool SolverCaminoMinimo::a_estrella(int inicio, int objetivo,
                                   std::vector<int>& camino,
                                   long long& costeTotal,
                                   long long& expansiones) {
    int n = grafo.numVertices();
    const long long INF = std::numeric_limits<long long>::max();

    // dist guardamos el mejor coste conocido hasta cada nodo
    std::vector<long long> dist(n + 1, INF);
    ListaCerrada cerrada(n);
    ListaAbierta abierta;

    dist[inicio] = 0;
    double h0 = heuristica(inicio, objetivo);
    abierta.insertar(inicio, dist[inicio] + h0, dist[inicio]);

    expansiones = 0;
    costeTotal = -1;

    while (!abierta.es_vacia()) {
        Estado actual = abierta.sacar_mejor();
        int u = actual.id;

        // Si el estado en la cola no es el mejor g conocido, lo ignoramos
        if (actual.g > dist[u]) continue;

        // Si ya lo hemos visitado, lo ignoramos
        if (cerrada.isVisited(u)) continue;

        cerrada.markVisited(u);
        ++expansiones;

        // Si llegamos al objetivo, reconstruimos el camino
        if (u == objetivo) {
            costeTotal = dist[u];
            reconstruirCamino(inicio, objetivo, cerrada, camino);
            return true;
        }

        // Exploramos los vecinos del nodo 
        const auto& vecinos = grafo.vecinos(u);
        for (const auto& e : vecinos) {
            int v = e.destino;
            long long nuevoG = dist[u] + static_cast<long long>(e.coste);

            // En caso de encontrar un mejor camino, actualizamos
            if (nuevoG < dist[v]) {
                dist[v] = nuevoG;
                cerrada.setParent(v, u); // backpointer

                double h = heuristica(v, objetivo);
                double f = static_cast<double>(nuevoG) + h;
                abierta.insertar(v, f, nuevoG);
            }
        }
    }
    // Si vaciamos la lista abierta sin llegar a la meta, no hay camino
    return false;
}

// Reconstrucción del camino a partir de backpointers
void SolverCaminoMinimo::reconstruirCamino(int inicio, int objetivo,
                                          const ListaCerrada& cerrada,
                                          std::vector<int>& camino) {
    camino.clear();
    int actual = objetivo; // empezamos por el final
    // Retrocedemos usando los padres de cerrada hasta llegar al nodo inicial
    while (actual != -1) {
        camino.push_back(actual);
        if (actual == inicio) break;
        actual = cerrada.getParent(actual);
    }
    std::reverse(camino.begin(), camino.end());
}
