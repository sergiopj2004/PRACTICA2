//grafo.h
#pragma once    //para que sólo se procese una vez en compilación

#include <vector>
#include <string>

// Arista del grafo
struct Arista {
    int destino;
    int coste; //en metros
};

// Coordenadas de cada vértice del grafo
struct Coordenada {
    double longitud;
    double latitud;
};

class Grafo {
    public:
        Grafo();

        // Carga tanto el .gr como el .co basándose en el nombre del mapa
        bool load(const std::string& nombre_mapa);

        
        int numVertices() const {return nVertices;}     // NTotal de vértices del grafo
        int numAristas() const {return nAristas;}           // NTotal de aristas del grafo

        // Devolver todos los vecinos de un vértice u (lista con aristas que salen de u)
        const std::vector<Arista>& vecinos(int u) const {return adj[u];}
        // Devolver coordenadas del vértice u
        const Coordenada& coordenada(int u) const {return coords[u];}

        //Devuelve el coste del arco (u,v), o -1 si no existe
        int costeArista(int u, int v) const;

    private:
        int nVertices;  //NTotal de vértices del grafo
        int nAristas;     //NTotal de aristas leídas 

        /*Lista de adyacencia: adj[u], es un vector de Arista, donde cada arista= {destino, coste}*/
        std::vector<std::vector<Arista>> adj;
        /*Coordenadas de cada vértice: coords[u] = {longitud, latitud}*/   
        std::vector<Coordenada> coords;           

        // Función interna para cargar fichero con los arcos
        bool loadGraphFile(const std::string& grPath);
        //Función interna para cargar el fichero con las coordenadas
        bool loadCoordFile(const std::string& coPath);
};