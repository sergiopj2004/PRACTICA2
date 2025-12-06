// cerrada.h
#pragma once

#include <vector>

class ClosedList {
public:
    explicit ClosedList(int nVertices);

    bool isVisited(int v) const;
    void markVisited(int v);

    void setParent(int v, int parent);
    int getParent(int v) const;

private:
    std::vector<bool> visited; // si el nodo ya ha sido expandido
    std::vector<int> parent;   // backpointer
};
