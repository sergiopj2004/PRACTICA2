// abierta.cpp
#include "abierta.h"

ListaAbierta::ListaAbierta() {}

// Insertamos un nodo en la lista abierta
void ListaAbierta::insertar(int id, double f, double g) {
    cola.push({id, f, g});
}

// Extraemos el mejor estado con el menor f
Estado ListaAbierta::sacar_mejor() {
    Estado mejor = cola.top();
    cola.pop();
    return mejor;
}

// Comprobamos si la lista abierta está vacía
bool ListaAbierta::es_vacia() const {
    return cola.empty();
}