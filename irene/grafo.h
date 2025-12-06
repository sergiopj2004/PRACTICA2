//grafo.h
#pragma once

#include <vector>
#include <string>

struct Edge {
    int to;
    int cost; //en metros
};

struct Coord {
    double lon;
    double lat;
};

class Graph {
    public:
        Graph();

        // basePath: por ejemplo "USA-road-d.BAY" o "/tmp/USA-road-d.BAY"
        bool load(const std::string& basePath);

        int numVertices() const {return nVertices;}
        int numEdges() const {return nEdges;}

        const std::vector<Edge>& neighbors(int u) const {return adj[u];}
        const Coord& coord(int u) const {return coords[u];}

        //Devuelve el coste del arco (u,v), o -1 si no existe
        int getEdgeCost(int u, int v) const;

    private:
        int nVertices;
        int nEdges;

        std::vector<std::vector<Edge>> adj;   // 1..nVertices
        std::vector<Coord> coords;            // 1..nVertices

        bool loadGraphFile(const std::string& grPath);
        bool loadCoordFile(const std::string& coPath);
};