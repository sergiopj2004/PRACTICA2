// cerrada.h
#pragma once

#include <vector>

class ListaCerrada {
public:
    explicit ListaCerrada(int nVertices);

    // Comprobación de si un nodo ha sido ya visitado (ya ha sido expandido y vecinos analizados)
    bool isVisited(int v) const;
    // Marcar nodo para incluirlo en cerrada
    void markVisited(int v);

    // Registra el padre de un nodo en el camino óptimo (se va actualizando)
    void setParent(int v, int parent);
    // Devuelve el padre para el nodo v (si no, -1)
    int getParent(int v) const;

private:
    std::vector<bool> visited; // si el nodo ya ha sido expandido
    std::vector<int> parent;   // backpointer para reconstruir camino final
};
