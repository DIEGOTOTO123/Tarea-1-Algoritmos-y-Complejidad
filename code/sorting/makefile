CXX = g++
CXXFLAGS = -O3 -Wall -std=c++17
PYTHON = python3

TARGET = sorting
SRCS = sorting.cpp \
       algorithms/sort.cpp \
       algorithms/mergesort.cpp \
       algorithms/quicksort.cpp \
       algorithms/patiencesort.cpp

PLOT_SCRIPT = plot_generator.py
GEN_SCRIPT  = array_generator.py

.PHONY: all compile inputs run plots run-plot dev completo clean

# solo compila el ejecutable en C++
all: $(TARGET)
compile: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

# Genera los archivos de arreglos entrando al directorio scripts/
inputs:
	mkdir -p data/array_input
	cd scripts && $(PYTHON) $(GEN_SCRIPT)

# Solo ejecuta el benchmark en C++ (asume inputs ya generados)
run: $(TARGET)
	./$(TARGET)

# Solo genera los gráficos PNG desde el CSV existente
plots:
	cd scripts && $(PYTHON) $(PLOT_SCRIPT)

# MODO PRUEBAS: Compila, ejecuta mediciones y plotea al instante
run-plot: $(TARGET)
	./$(TARGET)
	cd scripts && $(PYTHON) $(PLOT_SCRIPT)

# Alias corto para pruebas rápidas
dev: run-plot

# MODO ABSOLUTO: Genera inputs desde cero, compila, mide y plotea
completo: inputs $(TARGET)
	./$(TARGET)
	cd scripts && $(PYTHON) $(PLOT_SCRIPT)

clean:
	rm -f $(TARGET)