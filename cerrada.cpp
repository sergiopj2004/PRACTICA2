#include "cerrada.hpp"
#include <limits>

ListaCerrada::ListaCerrada() {}

void ListaCerrada::redimensionar(int num_nodos) {
    // Inicializamos con infinito para indicar que no han sido visitados/alcanzados
    // +1 porque los nodos pueden ser 1-based (del 1 al N)
    min_g_score.assign(num_nodos + 1, std::numeric_limits<double>::infinity());
}

bool ListaCerrada::intentar_mejorar(int id_nodo, double nuevo_g) {
    // Seguridad de límites
    if (id_nodo < 0 || id_nodo >= min_g_score.size()) return false;

    if (nuevo_g < min_g_score[id_nodo]) {
        min_g_score[id_nodo] = nuevo_g;
        return true; // Se ha mejorado el camino
    }
    return false; // El camino encontrado no es mejor que el existente
}

double ListaCerrada::get_g(int id_nodo) const {
    if (id_nodo < 0 || id_nodo >= min_g_score.size()) {
        return std::numeric_limits<double>::infinity();
    }
    return min_g_score[id_nodo];
}