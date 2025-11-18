#!/usr/bin/env python3

#1. Importación de la librería
from constraint import *

#2. Definición de una variable como nuestro problema
problem = Problem()

#3. Creación de las variables junto con sus dominios
''' Crea 2 variables a y b, que tienen como dominio [1,2]'''
problem.addVariable('a', [1,2])
problem.addVariable('b', [1,2])

''' Crea las variables 'a' y 'b', ambas con el dominio [1,2,3]'''
problem.addVariables("ab", [1,2,3])

''' Crea las variables 'a' y 'b', ambas con el dominio [0,1,2]'''
problem.addVariables(['a', 'b'], range(3))

#4. Creación de las restricciones de nuestro problema
''' Se crea una funcifón lambda que recibe dos parámetros que se corresponden 
con los valores de las variables 'a' y 'b', y comprueba que 'a' es mayor que 'b''''
problem.addConstraint(lambda a, b: a!=b, ('a','b'))
''' Forma alternativa'''
def greater (a,b):
    if a > b:
        return True
problem.addConstraint(greater, ('a', 'b'))

''' Se comprueba que los valores de las variables comprendidas entre 'a' y 'h' es diferente'''
def notEqual (*args):   # array que contiene el valor de las 8 variables que se están considerando
    for i in range (len(args)):
        for j in range(i+1, len(args)):
            if i != j and args[i] == args[j]:
                reutrn False
    return True

problem.addConstraint(notEqual, ('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'))

'''En el caso de que sea más simple, podría hacerse así'''
problem.addConstraint(lambda a, b: a!=b, ('A', 'R'))

'''Comprueba que el valor de una variable es diferente al de las otras'''
problem.addConstraint(AllDifferentConstraint(), ['A', 'R'])

'''Consecutividad'''
def consecutive (a,b):
    if b > a:
        return True
problem.addConstraint(consecutive, ('R','Y'))

'''Otras funciones predefinidad para construir restricciones: 
    - AllDifferentConstraint: Fuerza a que el valor de las variabels indicadas sea diferente. Si no se 
    indican las variables, se asume que son todas las variables. 
    - AllEqualConstraint: La contraria que la anterior. Fuerza a que todas las variables indicadas tomen el mismo valor
    - MaxSumConstraint: Fuerza a que la suma de las variables indicadas resulte como máximo un determinado valor
    - ExactSumConstraint: Fuerza a que la suma de las variables indicadas resulte exactamente el valor indicado.'''


#5. Recuperación de la solución o soluciones
''' La librería ofrece 2 funciones: getSolution() y getSolutions(). La primera recupera una única solución, y la segunda
todas las posibles soluciones que satisfacen las restricciones.'''
problem.getSolutions

print(problem.getSolution()) # para imprimir la solución

