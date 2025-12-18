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
        if (a.f != b.f) return a.f > b.f; // min-heap por f
        return a.g > b.g;                 // desempatamos con menor g primero
    }
};

class ListaAbierta {
private:
    // Cola de prioridad interna (min-heap de f)
    std::priority_queue<Estado, std::vector<Estado>, EstadoCompare> cola;
public:
    ListaAbierta();
    // Insertamos un estado en la lista abierta
    void insertar (int id, double f, double g);
    // Comprobamos si hay lista está vacía
    bool es_vacia() const;
    // Extraemos el estado con menor f que será el mejor candidato
    Estado sacar_mejor();
};