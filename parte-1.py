#!/usr/bin/env python3

#parte-1.py: Resolución puzzle Binairo usando python-constraint.

import argparse
import sys
from pathlib import Path
from constraint import Problem

# Carpeta donde está este script (para trabajar con rutas relativas)
script_dir = Path(__file__).resolve().parent

def en_script_dir(path_arg: str) -> Path:
    """
    Si el argumento no tiene ruta (solo nombre de fichero),
    lo interpretamos al directorio del script.
    Si viene con ruta, la respetamos tal cual.
    """
    p = Path(path_arg)
    # si la ruta es solo el nombre del archivo, se asume el directorio del script
    if str(p) == p.name:
    return (script_dir / p.name) if str(p) == p.name else p

#Leemos el fichero de entrada y devuelve un tablero como lista de listas.
def leer_entrada(path: Path):

    lineas = []

    # Abrimos el fichero en modo texto
    with open(path, encoding="utf-8") as f:
        for line in f:
            s = line.strip()  # quitamos espacios y saltos de línea
            if not s:
                # Ignoramos líneas vacías
                continue
            # Convertimos la cadena en lista de caracteres
            lineas.append(list(s))

    # Si no hay líneas válidas, hay error
    if not lineas:
        print("fichero de entrada vacío.")
        sys.exit(3)

    # Comprobamos que el tablero sea cuadrado
    n = len(lineas[0])  # número de columnas de la primera fila
    for fila in lineas:
        if len(fila) != n:
            print("la rejilla debe ser cuadrada n×n (todas las filas igual de largas).")
            sys.exit(3)
        for c in fila:
            if c not in ".XO":
                print("ERROR: carácter no válido en la rejilla:", c)
                sys.exit(3)

    # No salimos por error si n es impar, pero lo avisamos
    if n % 2 != 0:
        print("n es impar; las restricciones de igual número de X y O solo tienen sentido con n par.")

    return lineas  # lista de listas de chars

def print_tabla(tablero, out):

    n = len(tablero)
    borde = "+---" * n + "+"

    print(borde, file=out)
    for i in range(n):
        fila_str = "|"
        for j in range(n):
            c = tablero[i][j]
            # Para la instancia original podemos querer mostrar los '.' como espacios
            if c == '.':
                c = ' '
            fila_str += f" {c} |"
        print(fila_str, file=out)
    print(borde, file=out)


def construir_problema(tablero):
    ''' Construcción del CSP con python-constraint '''
    n = len(tablero)
    problem = Problem()

    # 1) ----- VARIABLES Y DOMINIO -----
    for i in range(n):
        for j in range(n):
            c = tablero[i][j]
            if c == '.':
                dominio = ['X', 'O']
            else:
                # Si ya viene 'X' o 'O' en la entrada, la variable queda fija
                dominio = [c]
            problem.addVariable((i, j), dominio)

    # ----- RESTRICCIONES -----
    # 2) Restricciones de equilibrio por filas:
    limite = n//2
    #    En cada fila, debe haber n/2 celdas 'X' (y por tanto n/2 'O')
    for i in range(n):
        vars_fila = [(i, j) for j in range(n)]

        # Definimos una función que se aplicará a los valores de la fila
        def fila_equilibrada(*vals, target = limite):
            return vals.count('X') == target

        problem.addConstraint(fila_equilibrada, vars_fila)

    # 3) Restricciones de equilibrio por columnas:
    for j in range(n):
        vars_col = [(i, j) for i in range(n)]

        def col_equilibrada(*vals, target = limite):
            # Lo mismo que arriba, pero por columnas
            return vals.count('X') == target

        problem.addConstraint(col_equilibrada, vars_col)

    # 4) Secuencia de colores en filas
    for i in range(n):
        for j in range(n - 2):
            v1, v2, v3= (i, j), (i, j + 1), (i, j + 2)

            # La lambda recibe los tres valores a,b,c
            # y devuelve False si los tres son iguales.
            problem.addConstraint(lambda a, b, c: not (a == b == c),(v1, v2, v3))

    # 5) Secuencia de colores en columnas
    for i in range(n - 2):
        for j in range(n):
            v1, v2, v3 = (i, j), (i + 1, j), (i + 2, j)
            problem.addConstraint(lambda a, b, c: not (a == b == c),(v1, v2, v3))

    return problem

def main():
    # Definimos los argumentos de línea de comandos
    ap = argparse.ArgumentParser(
        description="Resuelve un puzzle Binairo con python-constraint."
    )
    ap.add_argument("in_path",  help="Fichero de entrada (.in)")
    ap.add_argument("out_path", help="Fichero de salida")
    args = ap.parse_args()

    # Resolución de rutas (por si se dan solo nombres de fichero)
    in_path  = en_script_dir(args.in_path)
    out_path = en_script_dir(args.out_path)

    # Comprobamos que el fichero de entrada exista
    if not in_path.exists():
        print("No existe el fichero de entrada:", in_path)
        sys.exit(1)

    # 1) Leemos la entrada
    tablero = leer_entrada(in_path)

    # 2) Construimos el problema de satisfacción de restricciones
    problem = construir_problema(tablero)

    # 3) Obtenemos las soluciones
    solutions = problem.getSolutions()

    # 4) Mostramos por pantalla
    print_tabla(tablero, sys.stdout)
    print(f"{len(solutions)} soluciones encontradas")

    # Si no hay ninguna solución, no podemos escribir solución en el fichero
    if not solutions:
        return

    # 5) Cogemos una de las soluciones
    sol = solutions[0]
    n = len(tablero)

    # Construimos una tabla "completa" con X/O para imprimirla igual que la entrada
    sol_tablero = [[sol[(i, j)] for j in range(n)] for i in range(n)]

    # 6) Escribimos en el fichero de salida
    with open(out_path, "w", encoding="utf-8") as f:
        print_tabla(tablero, f)
        print_tabla(sol_tablero, f)


# Ejecutar main() solo si este fichero se llama directamente
if __name__ == "__main__":
    main()
