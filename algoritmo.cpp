#include "algoritmo.hpp"
#include "abierta.hpp" 
#include "cerrada.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>

// Definición de PI para asegurar compatibilidad en distintos compiladores
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Constructor vacío
AlgoritmoBusqueda::AlgoritmoBusqueda() {}

// Función auxiliar para convertir grados a radianes
double AlgoritmoBusqueda::to_rad(double degree) const {
    return degree * M_PI / 180.0;
}

// Cálculo de la heurística (Distancia Haversine)
double AlgoritmoBusqueda::calcular_heuristica(int id_actual, int id_destino, const Grafo& grafo) const {
    Punto p1 = grafo.get_coordenadas(id_actual);
    Punto p2 = grafo.get_coordenadas(id_destino);

    double dLat = to_rad(p2.lat - p1.lat);
    double dLon = to_rad(p2.lon - p1.lon);
    double lat1 = to_rad(p1.lat);
    double lat2 = to_rad(p2.lat);

    // Fórmula de Haversine
    double a = std::sin(dLat / 2) * std::sin(dLat / 2) +
               std::sin(dLon / 2) * std::sin(dLon / 2) * std::cos(lat1) * std::cos(lat2);
    
    // c es la distancia angular en radianes
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));

    // Multiplicamos por el radio de la Tierra para obtener metros
    return RADIO_TIERRA * c;
}

// Método principal de resolución (A* o Dijkstra según el parámetro usar_heuristica)
Solucion AlgoritmoBusqueda::resolver(int inicio, int fin, const Grafo& grafo, bool usar_heuristica) {
    Solucion solucion;
    solucion.encontrada = false;
    solucion.nodos_expandidos = 0;
    solucion.coste_total = 0.0;

    // Verificación de seguridad: los nodos deben existir en el grafo
    if (!grafo.existe_nodo(inicio) || !grafo.existe_nodo(fin)) {
        return solucion;
    }

    int num_nodos = grafo.get_num_nodos();

    // 1. INSTANCIACIÓN DE ESTRUCTURAS DE DATOS (Encapsuladas)
    ListaAbierta abierta;
    ListaCerrada cerrada;

    // 2. INICIALIZACIÓN
    // Preparamos la lista cerrada (vector de mejores distancias) con valor infinito
    cerrada.redimensionar(num_nodos);
    
    // Vector para reconstruir el camino (padres). Se mantiene como vector simple.
    // Inicializado a -1.
    std::vector<int> padre(num_nodos + 1, -1);

    // Configuración del nodo inicial
    double h_inicial = usar_heuristica ? calcular_heuristica(inicio, fin, grafo) : 0.0;
    
    // Insertamos el nodo inicial en las estructuras
    // Nota: 'cerrada' actúa aquí como registro del mejor 'g' conocido.
    cerrada.intentar_mejorar(inicio, 0.0);
    abierta.insertar(inicio, h_inicial, 0.0); // f = 0 + h, g = 0

    // 3. BUCLE PRINCIPAL DE BÚSQUEDA
    while (!abierta.es_vacia()) {
        // Extraer el nodo con menor f de la lista abierta
        Estado actual = abierta.sacar_mejor();

        // [Lazy Deletion / Optimización]
        // Si el coste 'g' extraído es peor que el que ya hemos registrado en la cerrada,
        // significa que este es un estado obsoleto que quedó en la cola. Lo ignoramos.
        if (actual.g > cerrada.get_g(actual.id)) {
            continue;
        }

        // Comprobar si hemos llegado al destino
        if (actual.id == fin) {
            solucion.encontrada = true;
            solucion.coste_total = actual.g;
            break; // Terminamos la búsqueda
        }

        // Solo contamos la expansión si procesamos sus vecinos
        solucion.nodos_expandidos++;

        // Obtener vecinos del grafo
        const std::vector<Arista>& vecinos = grafo.get_vecinos(actual.id);

        for (const auto& arista : vecinos) {
            int vecino_id = arista.destino;
            double nuevo_g = actual.g + arista.coste;

            // Intentamos mejorar el coste en la lista cerrada
            // Si retorna true, significa que hemos encontrado un camino más corto a 'vecino_id'
            if (cerrada.intentar_mejorar(vecino_id, nuevo_g)) {
                // Actualizamos el padre para reconstruir el camino luego
                padre[vecino_id] = actual.id;

                // Calculamos f = g + h
                double h = usar_heuristica ? calcular_heuristica(vecino_id, fin, grafo) : 0.0;
                double nuevo_f = nuevo_g + h;

                // Insertamos el nuevo mejor camino en la lista abierta
                abierta.insertar(vecino_id, nuevo_f, nuevo_g);
            }
        }
    }

    // 4. RECONSTRUCCIÓN DEL CAMINO (Backtracking)
    if (solucion.encontrada) {
        int nodo_actual = fin;
        
        // Retrocedemos desde el final hasta el inicio usando el vector de padres
        while (nodo_actual != -1) {
            solucion.camino.push_back(nodo_actual);
            
            // Protección extra: si llegamos al inicio, paramos (aunque el padre[inicio] debería ser -1)
            if (nodo_actual == inicio) break;
            
            nodo_actual = padre[nodo_actual];
        }
        
        // El camino está al revés (Final -> Inicio), así que lo invertimos
        std::reverse(solucion.camino.begin(), solucion.camino.end());
    }

    return solucion;
}