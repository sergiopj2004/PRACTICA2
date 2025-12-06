#ifndef CERRADA_HPP
#define CERRADA_HPP

#include <vector>

class ListaCerrada {
private:
    // Almacena el mejor 'g' conocido para cada nodo.
    // Usamos vector para acceso O(1) dado que los IDs son secuenciales.
    std::vector<double> min_g_score;

public:
    ListaCerrada();

    // Inicializa la estructura con el número de nodos del grafo
    void redimensionar(int num_nodos);

    // Comprueba si el nuevo_g es mejor que lo que ya tenemos guardado.
    // Retorna true si encontramos un camino mejor (o es la primera vez que lo vemos).
    // Y actualiza el valor internamente.
    bool intentar_mejorar(int id_nodo, double nuevo_g);

    // Obtiene el coste g almacenado para un nodo
    double get_g(int id_nodo) const;
};

#endif // CERRADA_HPP