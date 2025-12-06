# Compilador 
CXX := g++

# Flags de compilación
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pedantic

# Nombre del ejecutable
TARGET := parte-2

# Fuentes
SRCS := \
    parte-2.cpp \
    grafo.cpp \
    algoritmo.cpp \
    abierta.cpp \
    cerrada.cpp

# Objetos (.o) generados a partir de los .cpp
OBJS := $(SRCS:.cpp=.o)

# Regla por defecto
all: $(TARGET)

# Cómo enlazar el ejecutable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Regla genérica para compilar cada .cpp en su .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpiar archivos generados
clean:
	rm -f $(OBJS) $(TARGET)

# Limpieza 
distclean: clean

.PHONY: all clean distclean
