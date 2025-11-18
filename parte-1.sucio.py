#!/usr/bin/env python3

# parte1.py
# Desciption: Resolución del puzzle BINAIRO usando python-constraint
# -----------------------------------------------------------------------------

import sys
from pathlib import Path
from constraint import Problem
import math
import os

# Carpeta donde está este script (para trabajar con rutas relativas)
script_dir = Path(__file__).resolve().parent

# 1. Funciones de soporte
def obtener_rutas():
    ''' Controla los argumentos de la línea de comandos. Devuelve la ruta de entrada y de salida'''
    if len(sys.argv) != 3:
        print("Formato incorrecto", file = sys.stderr)
        sys.exit(1)
    
    return Path(sys.argv[1]), Path(sys.argv[2]) #input_file y output_file

def leer_entrada(path: Path):
    ''' Lee el fichero de entrada y devuelve el tablero conmo una lista de listas que representa el estado inicial del tablero. 
    '''
    lineas = []
    with open(path, encoding = "utf-8") as f:
        for line in f:
            s = line.strip()  # quitamos espacios y saltos de línea
            if not s:
                continue    # Ignoramos líneas vacías
            # Validación de caracteres
            if any(c not in ".XO" for c in s):
                print(f"ERROR: carácter no válido en la línea:{s}")
                sys.exit(3)
            lineas.append(list(s))  # Convertimos la cadena en lista de caracteres
    
    # Si no hay líneas válidas, se produce un error
    if not lineas:
        print("Error: Fichero de entrada vacío")
        sys.exit(3)
    
    # Comprobamos que el tablero sea cuadrado
    n = len(lineas[0])  # número de columnas de la primera fila
    for fila in lineas:
        if len(fila) != n:
            print("la rejilla debe ser cuadrada n × n ")
            sys.exit(3)
        
    # No salimos por error si n es impar, pero lo avisamos
    if n % 2 != 0:
        print("n es impar; las restricciones de igual número de X y O solo tienen sentido con n par.")

    return lineas  # lista de listas de chars

def print_tabla(tablero, out):
    ''' Formatea e imprime el tablero en el out. '''
    n = len(tablero)
    borde = "+---" * n + "+"

    print(borde, file=out)
    for fila in tablero:
        fila_str = "|"
        for c in fila:
            # Para la instancia original podemos querer mostrar los '.' como espacios
            display_c = c if c != '.' else ' ' 
            fila_str += f" {display_c} |"
        print(fila_str, file=out)
    print(borde, file=out)

# 2. Funciones de restricción 
def restriccion_secuencia(a, b, c):
    ''' Verifica el límite de secuencia en fila y en columna'''
    return not (a == b == c)

# 3. Construcción del problema
def construir_problema(tablero):
    """
    Construye el modelo CSP del Binairo usando el tablero de entrada.
    """
    n = len(tablero)
    problem = Problem()

    # 1) ------- VARIABLES Y DOMINIO -------
    for i in range(n):
        for j in range(n):
            c = tablero[i][j]
            if c == '.':
                dominio = ['X', 'O'] 
            else:
                # Si ya viene 'X' o 'O' en la entrada, la variable queda fija
                dominio = [c]
            problem.addVariable((i, j), dominio)

    # 2) ------- RESTRICCIONES DE EQUILIBRIO DE DISCOS -------
    if n % 2 == 0:
        limite = n // 2   # En cada fila debe haber n/2 celdas 'X' y n/2 celdas 'O'
        
        # Equilibrio de discos en filas
        def fila_equilibrada(*vals, n=n):
            return vals.count('X') == limite

        problem.addConstraint(fila_equilibrada, vars_fila)
            
        # Equilibrio de discos en columnas
        def col_equilibrada(*vals, n=n):
            # Lo mismo que arriba, pero por columnas
            return vals.count('X') == n // 2

        problem.addConstraint(col_equilibrada, vars_col)
        
    # 3) ------- RESTRICCIONES DE SECUENCIA -------:
    
    # Límite de secuencia en filas
    for i in range(n):
        for j in range(n - 2):
            # Prohíbe (X,X,X) y (O,O,O)
            problem.addConstraint(restriccion_secuencia, [(i, j), (i, j + 1), (i, j + 2)])

    # Límite de secuencia en columnas
    for i in range(n - 2):
        for j in range(n):
            problem.addConstraint(restriccion_secuencia, [(i, j), (i + 1, j), (i + 2, j)])

    return problem

# main
# -----------------------------------------------------------------------------
if __name__ == '__main__':

    # 1. Obtener rutas de los archivos
    input_path, output_path = obtener_rutas()

    # Comprobamos que el fichero de entrada existe
    if not input_path.exists():
        print(f"El fichero de entrada:'{input_path}' no existe", file=sys.stderr)
        sys.exit(1)

    # 2. Lectura de la entrada
    tablero = leer_entrada(input_path)
 
    # 3. Construimos el problema
    problem = construir_problema(tablero)

    # 4. Obtenemos las soluciones
    soluciones = problem.getSolutions()

    # 5. Mostramos por pantalla la instancia original y el número de soluciones encontradas
    print_tabla(tablero, sys.stdout)
    print(f"{len(soluciones)} soluciones encontradas") 

    # 6) Cogemos una de las soluciones
    if not soluciones:
        with open(output_path, "w", encoding="utf-8") as f:
            print_tabla(tablero, f)
        sys.exit(0)

    sol = soluciones[0]
    n = len(tablero)

    # Construimos la tabla final con X/O con los valores de la solución
    sol_tablero = [[sol[(i, j)] for j in range(n)] for i in range(n)]

   # 8) Escribimos la situación inicial y una solución en el fichero de salida
    try:
        with open(output_path, "w", encoding="utf-8") as f:
            print_tabla(tablero, f)
            print_tabla(sol_tablero, f)
    except Exception as e:
        print(f"Error al escribir en el fichero de salida '{output_path}': {e}", file=sys.stderr)
        sys.exit(1)


