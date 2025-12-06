// abierta.h
#pragma once

#include <queue>
#include <vector>


struct Estado {
    int id;
    double f; //f = g + h
    double g; // coste real acumulado
};

struct EstadoCompare {
    bool operator()(const Estado& a, const Estado& b) const {
        // priority_queue 
        return a.f > b.f;
    }
};

class ListaAbierta {
private:
    // Cola de prioridad interna (min-heap)
    std::priority_queue<Estado, std::vector<Estado>, std::greater<Estado>> cola;
public:
    ListaAbierta();

    // Inserta un estado en la lista abierta
    void insertar (int id, double f, double g);

    // Comprueba si la lista está vacía
    bool es_vacia() const;

    // Extrae el estado con menor f (el mejor candidato)
    Estado sacar_mejor();
};
