#!/usr/bin/env python3

# parte1.py
# Desciption: Resolución del puzzle BINAIRO usando python-constraint
# -----------------------------------------------------------------------------

import sys
import constraint
import math
import os

# 1. Funciones de soporte
def leer_entrada(filename):
    ''' Lee el fichero de entrada y devuelve la dimensión (n) del tablero y 
    y la instancia conmo una lista de listas que representa el estado inicial del tablero. 
    '''
    try:
        with open(filename, 'r') as f:
            lines = [line.strip() for line in f if line.strip()]
    except FileNotFoundError:
        print(f"Error: Fichero de entrada '{filename}' no encontrado", file=sys.stderr)
        sys.exit(1)
    
    if not lines:
        print("Error: Fichero de entrada vacío", file=sys.stderr)
        sys.exit(1)
    
    n = len(lines)
    if any(len(line) != n for line in lines):
        print("Error: El tablero no es cuadrado (nxn)", file=sys.stderr)
        sys.exit(1)
    
    return n,lines

def formato_tablero(n, datos_tablero):
    '''Formatea el tablero en el formato +---+---'''
    output = []
    output.append("+---" * n + "+")
    
    for i in range(n):
        linea = "|"
        for j in range(n):
            if isinstance(datos_tablero, dict):
                val = 'O' if datos_tablero.get((i,j)) == 0 else 'X'
            else:
                val = datos_tablero[i][j]
            
            linea += f"{val if val != '.' else ' '} |"
        output.append(linea)
        output.append("+---"*n+"+")
    
    return "\n".join(output)

def obtener_rutas():
    ''' Controla los argumentos de la línea de comandos. Devuelve la ruta de entrada y de salida'''
    if len(sys.argv) != 3:
        print("Formato incorrecto", file = sys.stderr)
        sys.exit(1)
    
    input_file = sys.argv[1]
    output_file = sys.argv[2]

    return input_file, output_file


# 2. Funciones de restricción 
def restriccion_secuencia(v1, v2, v3):
    ''' Verifica el límite de secuencia en fila y en columna'''
    suma_variables = v1 + v2 + v3
    return 1 <= suma_variables <= 2


# Modelado y resolución
# main
# -----------------------------------------------------------------------------

if __name__ = '__main__':

    # 1. Obtener rutas de los archivos
    input_file, output_file = obtener_rutas()

    # 2. Lectura de la instancia
    n, instancia = leer_entrada(input_file)

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