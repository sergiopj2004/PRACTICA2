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
    // Comprobamos argumentos
    if (argc != 5) {
        std::cerr << "Uso: " << argv[0]
                  << " <vertice-inicio> <vertice-objetivo> <nombre-del-mapa> <fichero-salida>\n";
        return 1;
    }

    // Leemos parámetros
    int verticeInicio   = std::stoi(argv[1]);
    int verticeObjetivo = std::stoi(argv[2]);
    std::string mapaBase = argv[3];
    std::string ficheroSalida = argv[4];

    // Cargamos grafo y coordenadas
    Grafo grafo;
    if (!grafo.load(mapaBase)) return 1;

    std::cout << "# vertices: " << grafo.numVertices() << "\n";
    std::cout << "# arcos : " << grafo.numAristas() << "\n";

    SolverCaminoMinimo solucionador(grafo);
    std::vector<int> camino;
    long long costeTotal = 0;
    long long expansiones = 0;

    // Calculamos el tiempo de ejecución
    auto t0 = std::chrono::steady_clock::now();
    bool encontrado = solucionador.a_estrella(verticeInicio, verticeObjetivo, camino, costeTotal, expansiones);
    auto t1 = std::chrono::steady_clock::now();

    std::chrono::duration<double> diff = t1 - t0;
    double segundos = diff.count();

    if (!encontrado) {
        std::cout << "No se encontró camino entre " << verticeInicio
                  << " y " << verticeObjetivo << "\n";
        return 0;
    }

    // Mostramos coste y tiempo
    std::cout << "Solución óptima encontrada con coste " << costeTotal << "\n";
    std::cout << "Tiempo de ejecución: " << std::fixed << std::setprecision(2)
              << segundos << " segundos\n";

    // Expansiones por segundo
    double nodosPorSegundo = (segundos > 0.0) ? (expansiones / segundos) : 0.0;
    std::cout << "# expansiones : " << expansiones
              << " (" << std::fixed << std::setprecision(2)
              << nodosPorSegundo << " nodos/seg)\n";

    // Escribimos el camino con costes de arcos en el fichero de salida
    std::ofstream salida(ficheroSalida);
    if (!salida) {
        std::cerr << "No se pudo abrir fichero de salida: " << ficheroSalida << "\n";
        return 1;
    }

    if (!camino.empty()) {
        salida << camino[0];
        for (std::size_t i = 0; i + 1 < camino.size(); ++i) {
            int u = camino[i];
            int v = camino[i + 1];
            int costeArco = grafo.costeArista(u, v);
            salida << " - (" << costeArco << ") - " << v;
        }
        salida << "\n";
    }

    return 0;
}
