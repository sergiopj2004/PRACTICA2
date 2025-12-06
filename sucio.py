import matplotlib.pyplot as plt
import numpy as np

# Datos del escalado SOLO para tableros vacíos
# (Los casos con pistas no deben entrar en esta gráfica)

n = [4, 6, 8, 12]
soluciones = [90, 11222, None, None]   # None = no disponible / infinito
etiquetas = ["4×4", "6×6", "8×8 (timeout)", "12×12 (abortado)"]

# Convertimos None en np.nan para dibujar sin romper la gráfica
sol_y = [s if s is not None else np.nan for s in soluciones]

plt.figure(figsize=(8, 5))

# Línea continua solo para puntos disponibles
plt.plot(n, sol_y, marker='o', linestyle='-', label="Soluciones (vacío)")

# Marcamos explícitamente los puntos no computados
for i, s in enumerate(soluciones):
    if s is None:
        plt.scatter(n[i], 0, color="red")
        plt.text(n[i], 0, etiquetas[i], color="red", ha='center', va='bottom')

# Etiquetas y título
plt.title("Escalado del problema Binairo (tableros vacíos)\nn vs número de soluciones")
plt.xlabel("Tamaño n del tablero (n×n)")
plt.ylabel("Número de soluciones")

# Eje y en escala logarítmica (muy recomendable para estos datos)
plt.yscale("log")

plt.grid(True, which="both", linestyle="--", alpha=0.4)
plt.legend()
plt.tight_layout()
plt.show()
