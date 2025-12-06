// abierta.cpp
#include "abierta.h"

ListaAbierta::ListaAbierta() {}

void ListaAbierta::insertar(int id, double f, double g) {
    cola.push({id, f, g});
}

Estado ListaAbierta::sacar_mejor() {
    Estado mejor = cola.top();
    cola.pop();
    return mejor;
}

bool ListaAbierta::es_vacia() const {
    return cola.empty();
}
