// grafo.cpp
#include "grafo.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>

Grafo::Grafo()
    : nVertices(0), nAristas(0) {}

bool Grafo::load(const std::string& basePath) {
    std::string grPath = basePath + ".gr";
    std::string coPath = basePath + ".co";

    if (!loadGraphFile(grPath)) {
        std::cerr << "Error al leer fichero de grafo: " << grPath << "\n";
        return false;
    }
    if (!loadCoordFile(coPath)) {
        std::cerr << "Error al leer fichero de coordenadas: " << coPath << "\n";
        return false;
    }
    return true;
}

bool Grafo::loadGraphFile(const std::string& grPath) {
    std::ifstream in(grPath);
    if (!in) {
        std::cerr << "No se pudo abrir " << grPath << "\n";
        return false;
    }

    std::string line;
    nVertices = 0;
    nAristas = 0;

    while (std::getline(in, line)) {
        if (line.empty()) continue;
        if (line[0] == 'c') {
            continue; // comentario
        } else if (line[0] == 'p') {
            // Ejemplo: p sp 321270 800172
            std::istringstream iss(line);
            char p;
            std::string sp;
            int n, m;
            iss >> p >> sp >> n >> m;
            nVertices = n;
            nAristas = 0; // contaremos 'a'
            adj.assign(nVertices + 1, std::vector<Arista>());
        } else if (line[0] == 'a') {
            std::istringstream iss(line);
            char a;
            int u, v, coste;
            iss >> a >> u >> v >> coste;
            if (u >= 1 && u <= nVertices) {
                adj[u].push_back(Arista{v, coste});
                ++nAristas; // arco dirigido
            }
        }
    }

    if (nVertices == 0) {
        std::cerr << "No se encontró línea 'p' en " << grPath << "\n";
        return false;
    }

    return true;
}

bool Grafo::loadCoordFile(const std::string& coPath) {
    std::ifstream in(coPath);
    if (!in) {
        std::cerr << "No se pudo abrir " << coPath << "\n";
        return false;
    }

    // Si aún no hemos dimensionado coords, lo hacemos al tamaño nVertices
    if (nVertices <= 0) {
        std::cerr << "nVertices no inicializado antes de leer coords\n";
        return false;
    }

    coords.assign(nVertices + 1, Coordenada{0.0, 0.0});

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        if (line[0] == 'c') {
            continue;
        } else if (line[0] == 'p') {
            // Ejemplo: p aux 321270 321270
            // No necesitamos mucho más, pero podríamos comprobar n
            continue;
        } else if (line[0] == 'v') {
            std::istringstream iss(line);
            char vchar;
            int id;
            long long longitud_i, latitud_i;
            iss >> vchar >> id >> longitud_i >> latitud_i;
            if (id >= 1 && id <= nVertices) {
                Coordenada c;
                c.longitud = static_cast<double>(longitud_i) / 1e6; // están multiplicados por 10^6
                c.latitud = static_cast<double>(latitud_i) / 1e6;
                coords[id] = c;
            }
        }
    }

    return true;
}

int Grafo::costeArista(int u, int v) const {
    if (u < 1 || u > nVertices) return -1;
    for (const auto& e : adj[u]) {
        if (e.destino == v) return e.coste;
    }
    return -1;
}
