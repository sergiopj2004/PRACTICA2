#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <vector>
#include <string>
#include <iostream>

// Estructura ligera para representar una arista
struct Arista {
    int destino;
    int coste; // Distancia en metros
};

// Estructura para coordenadas (latitud/longitud)
struct Punto {
    double lat;
    double lon;
};

class Grafo {
private:
    // Lista de Adyacencia:
    // El índice del vector es el ID del nodo origen.
    // El contenido es un vector de sus aristas salientes.
    std::vector<std::vector<Arista>> lista_adyacencia;

    // Vector de Coordenadas:
    // El índice es el ID del nodo.
    std::vector<Punto> coordenadas;

    // Estadísticas
    long long num_arcos_cargados;
    int max_id_nodos; // Para llevar la cuenta del nodo más alto visto

public:
    Grafo();

    // Carga tanto el .gr como el .co basándose en el nombre base del mapa
    bool cargar(const std::string& nombre_mapa);

    // Getters eficientes (retornan referencias constantes para no copiar datos)
    const std::vector<Arista>& get_vecinos(int id_nodo) const;
    Punto get_coordenadas(int id_nodo) const;

    // Utilidades
    int get_num_nodos() const;
    long long get_num_arcos() const;
    
    // Verifica si un nodo existe (tiene coordenadas o aristas)
    bool existe_nodo(int id) const;
};

#endif // GRAFO_HPP