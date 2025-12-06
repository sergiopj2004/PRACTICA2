// cerrada.cpp
#include "cerrada.h"

ClosedList::ClosedList(int nVertices)
    : visited(nVertices + 1, false),
      parent(nVertices + 1, -1) {}

bool ClosedList::isVisited(int v) const {
    if (v < 0 || v >= static_cast<int>(visited.size())) return false;
    return visited[v];
}

void ClosedList::markVisited(int v) {
    if (v < 0 || v >= static_cast<int>(visited.size())) return;
    visited[v] = true;
}

void ClosedList::setParent(int v, int p) {
    if (v < 0 || v >= static_cast<int>(parent.size())) return;
    parent[v] = p;
}

int ClosedList::getParent(int v) const {
    if (v < 0 || v >= static_cast<int>(parent.size())) return -1;
    return parent[v];
}
