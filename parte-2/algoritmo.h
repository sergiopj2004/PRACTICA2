// algoritmo.h
#pragma once

#include <vector>
#include "grafo.h"
#include "abierta.h"
#include "cerrada.h"

class SolverCaminoMinimo {
public:
    explicit SolverCaminoMinimo(const Grafo& g);

    // A* (f = g + h). Devuelve true si encuentra camino.
    bool a_estrella(int inicio, int objetivo,
                    std::vector<int>& camino,
                    long long& costeTotal,
                    long long& expansiones);

private:
    const Grafo& grafo;

    // Reconstruimos el camino final mediante los padres almacenados en la lista cerrada
    void reconstruirCamino(int inicio, int objetivo,
                           const ListaCerrada& cerrada,
                           std::vector<int>& camino);
    
    // Heurística: distancia haversine 
    double heuristica(int v, int objetivo) const;
};