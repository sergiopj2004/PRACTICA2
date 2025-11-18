#!/usr/bin/env python3

# parte1.py
# Desciption: Resolución del puzzle BINAIRO usando python-constraint
# -----------------------------------------------------------------------------

import sys
from pathlib import Path
from constraint import Problem
import math
import os

# 1. Funciones de soporte
def obtener_rutas():
    ''' Controla los argumentos de la línea de comandos. Devuelve la ruta de entrada y de salida'''
    if len(sys.argv) != 3:
        print("Formato incorrecto", file = sys.stderr)
        sys.exit(1)
    
    input_path = Path(sys.argv[1])
    output_path = Path(sys.argv[2])

    return input_path, output_path

def leer_entrada(path: Path):
    ''' Lee el fichero de entrada y devuelve la dimensión (n) del tablero y 
    y la instancia conmo una lista de listas que representa el estado inicial del tablero. 
    '''
    lineas = []
    try:
        with open(path, encoding = "utf-8") as f:
            for line in f:
            s = line.strip()  # quitamos espacios y saltos de línea
            if not s:
                # Ignoramos líneas vacías
                continue
            # Convertimos la cadena en lista de caracteres
            lineas.append(list(s))
    except FileNotFoundError:
        print(f"Error: Fichero de entrada '{path}' no encontrado", file=sys.stderr)
        sys.exit(1)
    
    if not lineas:
        print("Error: Fichero de entrada vacío", file=sys.stderr)
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
    
    return n,lines

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
            display_c = c if c != '.' else ' '
            fila += f" {display_c} |"
        print(fila_str, file= out)
    print(borde, file=out)


# 2. Funciones de restricción 
def restriccion_secuencia(v1, v2, v3):
    ''' Verifica el límite de secuencia en fila y en columna'''
    suma_variables = v1 + v2 + v3
    return 1 <= suma_variables <= 2

# Construcción del problema
def construir_problema(tablero):
    """
    Construye el modelo CSP del Binairo usando el tablero de entrada.
    """
    n = len(tablero)
    problem = Problem()

    # 1) Añadir variables con sus dominios (fijos o libres)
    for i in range(n):
        for j in range(n):
            c = tablero[i][j]
            if c == '.':
                dominio = ['X', 'O'] # Variable libre
            else:
                dominio = [c]       # Variable fija
            problem.addVariable((i, j), dominio)

    # 2) Restricciones de equilibrio (Balance) por filas y columnas:
    if n % 2 == 0:
        target_count = n // 2
        
        # Balance de Filas
        for i in range(n):
            vars_fila = [(i, j) for j in range(n)]
            
            def fila_equilibrada(*vals, target=target_count):
                # Cuenta las 'X' y verifica que sean exactamente N/2
                return vals.count('X') == target
            
            problem.addConstraint(fila_equilibrada, vars_fila)

        # Balance de Columnas
        for j in range(n):
            vars_col = [(i, j) for i in range(n)]

            def col_equilibrada(*vals, target=target_count):
                return vals.count('X') == target

            problem.addConstraint(col_equilibrada, vars_col)


    # 3) Restricciones de Secuencia (No tres iguales seguidos):
    
    # No tres iguales seguidos en filas
    for i in range(n):
        for j in range(n - 2):
            v1, v2, v3 = (i, j), (i, j + 1), (i, j + 2)
            # Prohíbe (X,X,X) y (O,O,O)
            problem.addConstraint(lambda a, b, c: not (a == b == c), (v1, v2, v3))

    # No tres iguales seguidos en columnas
    for i in range(n - 2):
        for j in range(n):
            v1, v2, v3 = (i, j), (i + 1, j), (i + 2, j)
            problem.addConstraint(lambda a, b, c: not (a == b == c), (v1, v2, v3))

    return problem


# Modelado y resolución
# main
# -----------------------------------------------------------------------------

if __name__ = '__main__':

    # 1. Obtener rutas de los archivos
    input_path, output_path = obtener_rutas()

    # 2. Lectura de la instancia
    n, tablero_inicial = leer_entrada(input_path)

    # 3. Construimos el problema
    problem = construir_problema(tablero_inicial)

    # 4. Obtenemos las soluciones
    soluciones = problem.getSolutions()

    # 5. Mostramos por pantalla la instancia y el número de soluciones
    print_tabla(tablero_inicial, sys.stdout)
    print(f"{len(solutions)} soluciones encontradas")

    # 6. Escribimos en el fichero de salida
    if not solutions:
        return 

    # Aseguramos que el directorio de salida exista antes de escribir
    try:
        os.makedirs(output_path.parent, exist_ok=True)
    except Exception as e:
        print(f"Error al crear el directorio para '{output_path}': {e}", file=sys.stderr)
        sys.exit(1)

    sol = solutions[0]

    # Construimos la tabla final con los valores 'X'/'O' de la solución
    # Note: Solución es un diccionario { (i,j): 'X' | 'O' }
    sol_tablero = [[sol[(i, j)] for j in range(n)] for i in range(n)]

    # Escribimos ambos tableros
    try:
        with open(output_path, "w", encoding="utf-8") as f:
            print_tabla(tablero_inicial, f)
            print_tabla(sol_tablero, f)
    except Exception as e:
        print(f"Error al escribir en el fichero de salida '{output_path}': {e}", file=sys.stderr)
        sys.exit(1)

# Ejecutar main() solo si este fichero se llama directamente
if __name__ == "__main__":
    main()













    # 3. Mostrar la instancia en pantalla
    formatted_instance = formato_tablero(n, instancia)
    print(formatted_instance)

    # 4. CREACIÓN DEL PROBLEMA
    problem = constraint.Problem()

    # --------- VARIABLES ---------
    variables = [(i,j) for i in range(n) for j in range(n)] # Tupla (i,j)

    # --------- DOMINIO ---------
    problem.addVariables(variables, range(2)) #0 es blanco (O), 1 es negro(X)

    # --------- RESTRICCIONES ---------
    # Valores iniciales
    for i in range(n):
        for j in range(n):
            if instancia[i][j] == 'X':
                problem.addConstraint(lambda v: v== 1, [(i,j)])
            elif instancia[i][j] == 'O':
                problem.addConstraint(lambda v: v==0, [(i,j)])
    
    # Restricciones de equilibrio
    if n % 2 == 0:
        target_sum = n//2

        # Equilibrio de discos en filas
        for i in range(n):
            row_vars = [(i, j) for j in range(n)]
            problem.addConstraint(constraint.ExactSumConstraint(target_sum), row_vars)
            
        # Equilibrio de discos en columnas
        for j in range(n):
            col_vars = [(i, j) for i in range(n)]
            problem.addConstraint(constraint.ExactSumConstraint(target_sum), col_vars)
        
    # Restricciones de secuencia
    # Límite de secuencia en filas
    for i in range(n):
        for j in range(n - 2):
            vars_seq = [(i, j), (i, j + 1), (i, j + 2)]
            problem.addConstraint(restriccion_secuencia, vars_seq)
            
    # Límite de secuencia en columnas
    for j in range(n):
        for i in range(n - 2):
            vars_seq = [(i, j), (i + 1, j), (i + 2, j)]
            problem.addConstraint(restriccion_secuencia, vars_seq)
    

# 5. Cómputo de Soluciones (compute the solutions)
    solutions = problem.getSolutions()
    
    # 6. Mostrar resultados en pantalla
    print(f"#{len(solutions)} soluciones encontradas.")
    
    # 7. Escribir la Solución en el Fichero de Salida
    try:
        os.makedirs(os.path.dirname(output_file) or '.', exist_ok=True)
        
        with open(output_file, 'w') as f:
            f.write(formatted_instance + "\n") # Escribe la instancia
            
            if solutions:
                first_solution = solutions[0]
                formatted_solution = formato_tablero(n, first_solution)
                f.write(formatted_solution + "\n") # Escribe la primera solución
            else:
                f.write("No se encontraron soluciones.\n")
                
    except Exception as e:
        print(f"Error al escribir en el fichero de salida '{output_file}': {e}", file=sys.stderr)
        sys.exit(1)