// parte-2.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>

#include "grafo.h"
#include "algoritmo.h"

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Uso: " << argv[0]
                  << " <vertice-1> <vertice-2> <nombre-del-mapa> <fichero-salida>\n";
        return 1;
    }

    int start = std::stoi(argv[1]);
    int goal  = std::stoi(argv[2]);
    std::string mapBase = argv[3];
    std::string outFile = argv[4];

    Graph graph;
    if (!graph.load(mapBase)) {
        return 1;
    }

    std::cout << "# vertices: " << graph.numVertices() << "\n";
    std::cout << "# arcos : " << graph.numEdges() << "\n";

    ShortestPathSolver solver(graph);
    std::vector<int> path;
    long long totalCost = 0;
    long long expansions = 0;

    auto t0 = std::chrono::steady_clock::now();
    bool ok = solver.mejor_primero(start, goal, path, totalCost, expansions);

    auto t1 = std::chrono::steady_clock::now();

    std::chrono::duration<double> diff = t1 - t0;
    double seconds = diff.count();

    if (!ok) {
        std::cout << "No se encontró camino entre " << start
                  << " y " << goal << "\n";
        return 0;
    }

    std::cout << "Solución óptima encontrada con coste " << totalCost << "\n";
    std::cout << "Tiempo de ejecución: " << std::fixed << std::setprecision(2)
              << seconds << " segundos\n";

    double nodesPerSec = (seconds > 0.0) ? (expansions / seconds) : 0.0;
    std::cout << "# expansiones : " << expansions
              << " (" << std::fixed << std::setprecision(2)
              << nodesPerSec << " nodes/sec)\n";

    // Escribir el camino con costes de arcos en el fichero de salida
    std::ofstream out(outFile);
    if (!out) {
        std::cerr << "No se pudo abrir fichero de salida: " << outFile << "\n";
        return 1;
    }

    // Formato: <inicio> - (coste) - <v1> - (coste) - <v2> - ... - <final>
    if (path.size() >= 1) {
        out << path[0];
        for (std::size_t i = 0; i + 1 < path.size(); ++i) {
            int u = path[i];
            int v = path[i + 1];
            int cost = graph.getEdgeCost(u, v);
            out << " - (" << cost << ") - " << v;
        }
        out << "\n";
    }

    return 0;
}
