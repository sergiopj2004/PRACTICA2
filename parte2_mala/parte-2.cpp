#include <iostream>
#include <vector>
#include <string>
#include <chrono> // Para medir el tiempo con alta precisión
#include <fstream>
#include <iomanip> // Para formatear la salida (decimales)
#include "grafo.hpp"
#include "algoritmo.hpp"

// Función auxiliar para obtener el coste de una arista específica entre u y v
// Necesaria para imprimir el formato de salida: u - (coste) - v
int obtener_coste_arista(int u, int v, const Grafo& grafo) {
    const std::vector<Arista>& vecinos = grafo.get_vecinos(u);
    for (const auto& arista : vecinos) {
        if (arista.destino == v) {
            return arista.coste;
        }
    }
    return 0; // No debería ocurrir si el camino es válido
}

int main(int argc, char* argv[]) {
    // 1. Verificación de argumentos
    if (argc != 5) {
        std::cerr << "Uso: " << argv[0] << " <id_inicio> <id_final> <nombre_mapa> <fichero_salida>" << std::endl;
        return 1;
    }

    try {
        int id_inicio = std::stoi(argv[1]);
        int id_final = std::stoi(argv[2]);
        std::string nombre_mapa = argv[3];
        std::string fichero_salida = argv[4];

        // 2. Carga del Grafo
        Grafo grafo;
        // std::cout << "Cargando grafo..." << std::endl;
        if (!grafo.cargar(nombre_mapa)) {
            return 1;
        }

        // Mostrar estadísticas del grafo (Requerido por enunciado)
        std::cout << "# vertices: " << grafo.get_num_nodos() << std::endl;
        std::cout << "# arcos : " << grafo.get_num_arcos() << std::endl;

        // 3. Ejecución del Algoritmo (A*)
        AlgoritmoBusqueda algoritmo;

        // Iniciar cronómetro
        auto start_time = std::chrono::high_resolution_clock::now();

        // Ejecutar A* (usar_heuristica = true)
        Solucion sol = algoritmo.resolver(id_inicio, id_final, grafo, true);

        // Parar cronómetro
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end_time - start_time;
        double segundos = duration.count();

        // 4. Mostrar resultados por pantalla
        if (sol.encontrada) {
            std::cout << "Solución óptima encontrada con coste " << sol.coste_total << std::endl;
            std::cout << "Tiempo de ejecución: " << std::fixed << std::setprecision(2) << segundos << " segundos" << std::endl;
            
            double nodes_per_sec = (segundos > 0) ? sol.nodos_expandidos / segundos : 0.0;
            std::cout << "# expansiones : " << sol.nodos_expandidos 
                      << " (" << std::fixed << std::setprecision(2) << nodes_per_sec << " nodes/sec)" << std::endl;

            // 5. Escribir solución en fichero
            std::ofstream outfile(fichero_salida);
            if (outfile.is_open()) {
                // Formato: <inicio> - ... - <vertice-i> - (coste arco) - <vertice-i+1> ...
                for (size_t i = 0; i < sol.camino.size(); ++i) {
                    outfile << sol.camino[i];
                    
                    // Si no es el último nodo, imprimimos el coste del arco al siguiente
                    if (i < sol.camino.size() - 1) {
                        int u = sol.camino[i];
                        int v = sol.camino[i+1];
                        int coste = obtener_coste_arista(u, v, grafo);
                        outfile << " - (" << coste << ") - ";
                    }
                }
                outfile.close();
                // std::cout << "Solución guardada en: " << fichero_salida << std::endl;
            } else {
                std::cerr << "Error al crear el fichero de salida." << std::endl;
            }

        } else {
            std::cout << "No se encontró camino entre " << id_inicio << " y " << id_final << std::endl;
        }

        // --------------------------------------------------------------------------
        // BLOQUE PARA ANÁLISIS (PARTE 3) - Descomentar para comparar con Dijkstra
        // --------------------------------------------------------------------------
        /*
        std::cout << "\n--- ANALISIS COMPARATIVO (Fuerza Bruta / Dijkstra) ---" << std::endl;
        auto start_dijkstra = std::chrono::high_resolution_clock::now();
        
        // Ejecutar Dijkstra (usar_heuristica = false)
        Solucion sol_dijkstra = algoritmo.resolver(id_inicio, id_final, grafo, false);
        
        auto end_dijkstra = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration_d = end_dijkstra - start_dijkstra;
        
        std::cout << "Dijkstra Expansiones: " << sol_dijkstra.nodos_expandidos << std::endl;
        std::cout << "Dijkstra Tiempo: " << duration_d.count() << " s" << std::endl;
        std::cout << "A* es " << (sol_dijkstra.nodos_expandidos / (double)sol.nodos_expandidos) << " veces más eficiente en nodos." << std::endl;
        */

    } catch (const std::exception& e) {
        std::cerr << "Error de ejecución: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}