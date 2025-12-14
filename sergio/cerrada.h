// cerrada.h
#pragma once

#include <vector>

class ListaCerrada {
public:
    explicit ListaCerrada(int nVertices);

    // Si un nodo ha sido  visitado 
    bool isVisited(int v) const;
    // Marcamos nodo para incluirlo en cerrada
    void markVisited(int v);

    // Registramos el padre de un nodo en el camino óptimo 
    void setParent(int v, int parent);
    // Devovemos el padre para el nodo v (si no, -1)
    int getParent(int v) const;

private:
    std::vector<bool> visited; // si el nodo ya ha sido expandido
    std::vector<int> parent;   // backpointer para reconstruir camino final
};
