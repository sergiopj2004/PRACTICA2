#include "grafo.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include <algorithm>

Grafo::Grafo() : num_arcos_cargados(0), max_id_nodos(0) {}

bool Grafo::cargar(const std::string& nombre_mapa) {
    std::string fichero_gr = nombre_mapa + ".gr";
    std::string fichero_co = nombre_mapa + ".co";

    // ---------------------------------------------------------
    // 1. Lectura del fichero de GRAFO (.gr)
    // ---------------------------------------------------------
    std::ifstream file_gr(fichero_gr);
    if (!file_gr.is_open()) {
        std::cerr << "Error: No se pudo abrir el fichero de grafo: " << fichero_gr << std::endl;
        return false;
    }

    std::string linea;
    // Reservamos un tamaño inicial razonable para evitar redimensionamientos constantes al principio.
    // Los grafos DIMACS suelen ser grandes.
    lista_adyacencia.reserve(100000); 

    while (std::getline(file_gr, linea)) {
        if (linea.empty()) continue;

        // Solo nos interesan las líneas que empiezan por 'a'
        if (linea[0] == 'a') {
            std::stringstream ss(linea);
            char tipo;
            int u, v, coste;
            
            // Formato: a <id1> <id2> <coste>
            ss >> tipo >> u >> v >> coste;

            // Ajuste dinámico del tamaño del vector si encontramos un ID nuevo más alto
            int max_idx = std::max(u, v);
            if (max_idx >= lista_adyacencia.size()) {
                lista_adyacencia.resize(max_idx + 1);
            }
            if (max_idx > max_id_nodos) {
                max_id_nodos = max_idx;
            }

            // Añadir arista (Grafo Dirigido)
            lista_adyacencia[u].push_back({v, coste});
            num_arcos_cargados++;
        }
        // Ignoramos líneas 'p', 'c', etc.
    }
    file_gr.close();

    // ---------------------------------------------------------
    // 2. Lectura del fichero de COORDENADAS (.co)
    // ---------------------------------------------------------
    std::ifstream file_co(fichero_co);
    if (!file_co.is_open()) {
        std::cerr << "Error: No se pudo abrir el fichero de coordenadas: " << fichero_co << std::endl;
        return false;
    }

    // Aseguramos que el vector de coordenadas tenga tamaño suficiente
    if (max_id_nodos >= coordenadas.size()) {
        coordenadas.resize(max_id_nodos + 1);
    }

    long long nodos_coord_leidos = 0;
    while (std::getline(file_co, linea)) {
        if (linea.empty()) continue;

        // Solo nos interesan las líneas que empiezan por 'v'
        if (linea[0] == 'v') {
            std::stringstream ss(linea);
            char tipo;
            int id;
            double lon_int, lat_int; // Leemos como double para simplificar, aunque el input es entero
            
            // Formato: v <id> <longitud> <latitud>
            ss >> tipo >> id >> lon_int >> lat_int;

            // Si el ID es mayor que lo que vimos en el grafo (raro, pero posible), redimensionamos
            if (id >= coordenadas.size()) {
                coordenadas.resize(id + 1);
                // También redimensionamos la lista de adyacencia para mantener consistencia de índices
                if (id >= lista_adyacencia.size()) {
                    lista_adyacencia.resize(id + 1);
                }
                max_id_nodos = std::max(max_id_nodos, id);
            }

            // Guardar convertidos a coordenadas reales (dividir por 10^6 según enunciado)
            // NOTA: El enunciado dice longitud primero, luego latitud.
            coordenadas[id] = {lat_int / 1000000.0, lon_int / 1000000.0};
            nodos_coord_leidos++;
        }
    }
    file_co.close();

    // Mostrar resumen de carga (útil para debug y cumplir requisitos de salida por pantalla)
    std::cout << "# vertices cargados (coords): " << nodos_coord_leidos << std::endl;
    std::cout << "# arcos cargados: " << num_arcos_cargados << std::endl;

    return true;
}

const std::vector<Arista>& Grafo::get_vecinos(int id_nodo) const {
    // Verificación de seguridad básica
    if (id_nodo < 0 || id_nodo >= lista_adyacencia.size()) {
        static const std::vector<Arista> vacio;
        return vacio;
    }
    return lista_adyacencia[id_nodo];
}

Punto Grafo::get_coordenadas(int id_nodo) const {
    if (id_nodo < 0 || id_nodo >= coordenadas.size()) {
        return {0.0, 0.0}; // Retorno por defecto si no existe
    }
    return coordenadas[id_nodo];
}

int Grafo::get_num_nodos() const {
    return max_id_nodos; // O coordenadas.size(), dependiendo de la definición exacta
}

long long Grafo::get_num_arcos() const {
    return num_arcos_cargados;
}

bool Grafo::existe_nodo(int id) const {
    return id >= 0 && id < lista_adyacencia.size();
}