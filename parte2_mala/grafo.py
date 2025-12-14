# grafo.py - representación del grafo y lectura de los ficheros .gr y .co

class Grafo:
    def __init__(self, nombre_mapa):
        """
        nombre_mapa: cadena base del mapa, por ejemplo 'USA-road-d.BAY'
        Leerá 'USA-road-d.BAY.co' y 'USA-road-d.BAY.gr'
        """
        self.nombre_mapa = nombre_mapa

        # Diccionario de adyacencia: id -> lista de (vecino, coste)
        self.adj = {}

        # Coordenadas: id -> (lon, lat)
        self.coords = {}

        # Contadores útiles
        self.num_vertices = 0
        self.num_arcos = 0

        # Cargamos los dos ficheros
        self._leer_co(nombre_mapa + ".co")
        self._leer_gr(nombre_mapa + ".gr")

    def _leer_co(self, filename):
        """
        Lee el fichero .co y guarda las coordenadas de cada vértice.
        Formato típico:
        v <id> <longitud> <latitud>
        """
        with open(filename, "r") as f:
            for line in f:
                line = line.strip()
                if not line:
                    continue  # línea vacía
                if not line.startswith("v"):
                    # ignoramos comentarios u otras líneas
                    continue

                partes = line.split()
                # Ejemplo: ["v", "1", "-122.3456", "37.1234"]
                _, vid, lon, lat = partes

                vid = int(vid)
                lon = float(lon)
                lat = float(lat)

                self.coords[vid] = (lon, lat)

        self.num_vertices = len(self.coords)

    def _leer_gr(self, filename):
        """
        Lee el fichero .gr y guarda los arcos con su coste.
        Formato típico:
        a <origen> <destino> <coste>
        """
        with open(filename, "r") as f:
            for line in f:
                line = line.strip()
                if not line:
                    continue
                if not line.startswith("a"):
                    # ignoramos comentarios, cabeceras, etc.
                    continue

                partes = line.split()
                # Ejemplo: ["a", "1", "2", "1988"]
                _, u, v, coste = partes

                u = int(u)
                v = int(v)
                coste = int(coste)

                # Si el nodo no está en adj, lo creamos
                if u not in self.adj:
                    self.adj[u] = []
                if v not in self.adj:
                    self.adj[v] = []  # así sabemos que existe el vértice

                # Añadimos el arco dirigido u -> v
                self.adj[u].append((v, coste))
                self.num_arcos += 1

    def vecinos(self, v):
        """
        Devuelve la lista de vecinos de v en forma [(hijo, coste), ...].
        Si el vértice no tiene salida, devuelve lista vacía.
        """
        return self.adj.get(v, [])

    def tiene_coordenadas(self, v):
        """
        True si tenemos lon/lat para el vértice v.
        """
        return v in self.coords

    def coordenadas(self, v):
        """
        Devuelve (lon, lat) del vértice v, o None si no existe.
        """
        return self.coords.get(v, None)

    def __str__(self):
        """
        Representación simple del grafo (para debug rápido).
        """
        return f"Grafo({self.nombre_mapa}): {self.num_vertices} vertices, {self.num_arcos} arcos"
