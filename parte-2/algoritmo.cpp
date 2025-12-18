// algoritmo.cpp
#include "algoritmo.h"
#include <cmath>
#include <algorithm>
#include <limits>

// Constante radio de la tierra en metros (aproximado)
#define RADIO_TIERRA  6371000

static inline double deg2rad(double deg) {
    return deg * M_PI / 180.0;
}

static inline double haversine_m(double lat1_deg, double lon1_deg,
                                 double lat2_deg, double lon2_deg) {
    const double lat1 = deg2rad(lat1_deg);
    const double lon1 = deg2rad(lon1_deg);
    const double lat2 = deg2rad(lat2_deg);
    const double lon2 = deg2rad(lon2_deg);

    const double dlat = lat2 - lat1;
    const double dlon = lon2 - lon1;

    const double s1 = std::sin(dlat * 0.5);
    const double s2 = std::sin(dlon * 0.5);

    const double a =
        s1 * s1 +
        std::cos(lat1) * std::cos(lat2) * (s2 * s2);

    const double c = 2.0 * std::asin(std::min(1.0, std::sqrt(a)));

    return RADIO_TIERRA * c;
}

SolverCaminoMinimo::SolverCaminoMinimo(const Grafo& g) : grafo(g) {}

// Heurística: Aproximación con Haversine
double SolverCaminoMinimo::heuristica(int u, int objetivo) const {
    const Coordenada c1 = grafo.coordenada(u);
    const Coordenada c2 = grafo.coordenada(objetivo);

    // latitud y longitud en grados (ya escaladas desde el .co)
    return haversine_m(
        c1.latitud, c1.longitud,
        c2.latitud, c2.longitud
    );
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
