#ifndef ABIERTA_HPP
#define ABIERTA_HPP

#include <queue>
#include <vector>

// Representa un estado en la búsqueda
struct Estado {
    int id;
    double f; // f = g + h
    double g; // coste real acumulado

    // Sobrecarga para que la priority_queue ordene correctamente (menor f arriba)
    bool operator>(const Estado& otro) const {
        return f > otro.f;
    }
};

class ListaAbierta {
private:
    // Cola de prioridad interna (Min-Heap)
    std::priority_queue<Estado, std::vector<Estado>, std::greater<Estado>> cola;

public:
    ListaAbierta();
    
    // Inserta un estado en la lista abierta
    void insertar(int id, double f, double g);
    
    // Extrae el estado con menor f (el mejor candidato)
    Estado sacar_mejor();
    
    // Comprueba si la lista está vacía
    bool es_vacia() const;
    
    // Devuelve el tamaño (opcional, para estadísticas)
    size_t tamano() const;
};

#endif // ABIERTA_HPP