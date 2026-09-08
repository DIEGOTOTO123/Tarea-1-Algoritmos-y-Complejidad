CXX = g++
CXXFLAGS = -O3 -Wall -std=c++17
PYTHON = python3

TARGET = matrix_multiplication
SRCS = matrix_multiplication.cpp \
       algorithms/naive.cpp \
       algorithms/strassen.cpp

.PHONY: all compile inputs run plots run-plot dev completo clean

# Solo compila el ejecutable en C++
all: $(TARGET)
compile: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

# Genera los 144 archivos de matrices entrando al directorio scripts/
inputs:
	mkdir -p data/matrix_input
	cd scripts && $(PYTHON) matrix_generator.py

# Solo ejecuta el benchmark en C++ (asume inputs ya generados)
run: $(TARGET)
	./$(TARGET)

# Solo genera los gráficos PNG desde el CSV existente
plots:
	cd scripts && $(PYTHON) plot_generator.py

# MODO PRUEBAS: Compila, ejecuta mediciones y plotea al instante
run-plot: $(TARGET)
	./$(TARGET)
	cd scripts && $(PYTHON) plot_generator.py

# Alias corto para pruebas rápidas
dev: run-plot

# MODO ABSOLUTO: Genera inputs desde cero, compila, mide y plotea
completo: inputs $(TARGET)
	./$(TARGET)
	cd scripts && $(PYTHON) plot_generator.py

clean:
	rm -f $(TARGET)