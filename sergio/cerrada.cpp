// cerrada.cpp
#include "cerrada.h"

ListaCerrada::ListaCerrada(int nVertices)
    : visited(nVertices + 1, false),    // Todos los nodos se inicializan como no visitados
      parent(nVertices + 1, -1) {}      // Padre desconocido para todos

//Devolvemos si el nodo v ya ha sido visitado
bool ListaCerrada::isVisited(int v) const {
    if (v < 0 || v >= static_cast<int>(visited.size())) return false;
    return visited[v];
}

//Marcamos el nodo v como visitado
void ListaCerrada::markVisited(int v) {
    if (v < 0 || v >= static_cast<int>(visited.size())) return;
    visited[v] = true;
}

//Relacionamos nodo p, padre del nodo v
void ListaCerrada::setParent(int v, int p) {
    if (v < 0 || v >= static_cast<int>(parent.size())) return;
    parent[v] = p;
}

//Devolvemos el padre del nodo v
int ListaCerrada::getParent(int v) const {
    if (v < 0 || v >= static_cast<int>(parent.size())) return -1;
    return parent[v];
}
