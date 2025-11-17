# abierta.py - estructura de la lista abierta para la búsqueda

import heapq

class Abierta:
    def __init__(self):
        """
        Lista abierta implementada como una cola de prioridad (heap).
        Guarda tuplas (f, contador, estado, g, padre).
        """
        self.heap = []
        self.contador = 0  # para desempatar cuando f es igual

    def insertar(self, estado, f, g, padre):
        """
        Inserta un nuevo nodo en la abierta.
        estado: identificador del vértice (por ejemplo, un int)
        f: valor f(n) = g(n) + h(n)
        g: coste acumulado g(n)
        padre: estado anterior en el camino (o None para el origen)
        """
        entrada = (f, self.contador, estado, g, padre)
        heapq.heappush(self.heap, entrada)
        self.contador += 1

    def extraer_mejor(self):
        """
        Saca y devuelve el nodo con menor f.
        Devuelve una tupla (estado, f, g, padre) o None si está vacía.
        """
        if not self.heap:
            return None
        f, _, estado, g, padre = heapq.heappop(self.heap)
        return estado, f, g, padre

    def esta_vacia(self):
        """
        True si no queda ningún nodo en la lista abierta.
        """
        return len(self.heap) == 0

    def __len__(self):
        """
        Permite usar len(abierta) para saber cuántos elementos hay.
        """
        return len(self.heap)
