# cerrada.py - estructura de la lista cerrada para la búsqueda

class Cerrada:
    def __init__(self):
        """
        Lista cerrada implementada como un diccionario:
        estado -> mejor g(n) conocido para ese estado.
        """
        self.mejor_g = {}

    def contiene(self, estado):
        """
        True si el estado ya está en la cerrada.
        """
        return estado in self.mejor_g

    def get_g(self, estado):
        """
        Devuelve el mejor g(n) conocido para 'estado',
        o None si no está en la cerrada.
        """
        return self.mejor_g.get(estado, None)

    def actualizar(self, estado, g):
        """
        Guarda o actualiza el mejor g(n) para 'estado'.
        """
        self.mejor_g[estado] = g

    def __len__(self):
        """
        Permite usar len(cerrada) para saber cuántos estados hay.
        """
        return len(self.mejor_g)
