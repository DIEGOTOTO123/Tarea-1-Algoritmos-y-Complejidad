Markdown
# TAREA 1 INF221 2026-1

Este repositorio contiene la documentación, el código fuente, el pipeline de experimentación automatizado y el informe técnico para la Tarea 1 de la asignatura *INF221 Algoritmos y Complejidad*.

**Entrega:** archivo `.zip` vía aula.usm.cl

---

**Nombre:** Diego Ariel Araneda Campusano
**ROL:** 202473538-1
**PARALELO:** 201

## Estructura del Repositorio

- `assignment_statement/`: Enunciado oficial y archivos fuente en formato LaTeX (`.tex`).
- `code/`: Códigos fuente, suites de pruebas automatizadas y datos generados.
  - `sorting/`: Implementaciones de `std::sort`, Merge Sort, Quick Sort y Patience Sort.
  - `matrix_multiplication/`: Implementaciones de multiplicación clásica (Naive) y algoritmo de Strassen.
- `report/`: Plantilla en LaTeX (`.tex`), figuras y bibliografía para el informe técnico.
- `README.md`: Instrucciones generales del proyecto y guía de replicabilidad.

---

## Requisitos y Dependencias

Para compilar, ejecutar y replicar los experimentos se requiere:

- **Compilador C++:** `g++` o `clang++` con soporte completo para C++17 (requerido por `std::filesystem`).
- **Automatización:** GNU Make (versión 4.0 o superior).
- **Python 3:** Versión 3.8 o superior junto a sus librerías de análisis y graficación:
  ```bash
  pip install pandas matplotlib numpy
Compilación del informe (opcional): Distribución de LaTeX (TeX Live o MiKTeX) con pdflatex y biber.

**Instrucciones de Ejecución:**
Cada módulo dentro de code/ cuenta con un Makefile independiente para automatizar todo el proceso.

Para correr el flujo completo de una sola vez (generación de datos -> compilación -> benchmark -> gráficos), **ejecuta "make complete" en el directorio correspondiente**:

1. Módulo de Ordenamiento (Sorting)
    ```Bash
    cd code/sorting
    make complete
2. Módulo de Multiplicación Matricial
    ```Bash
    cd code/matrix_multiplication
    make complete
Objetivos individuales de los Makefiles
Si prefieres correr cada etapa por separado, puedes utilizar:

**make generate**: Crea las instancias sintéticas de prueba (.txt) mediante scripts de Python.

**make compile**: Compila los programas en C++ aplicando optimizaciones (-O2 / -O3).

**make run**: Ejecuta el orquestador cronometrando el tiempo neto en RAM.

**make plot**: Procesa los archivos .csv con Pandas y genera las figuras en data/plots/.

**make clean**: Elimina ejecutables y archivos temporales para reiniciar el entorno de pruebas.

## Metodología Experimental y Salidas
**Cronometraje sin interferencia de disco**: El orquestador C++ utiliza std::chrono::high_resolution_clock aislando exclusivamente el cómputo en memoria. La lectura de archivos y el guardado de resultados quedan fuera de la ventana de medición.

**Formato CSV**: Las mediciones se almacenan en tablas estructuradas dentro de data/measurements/:

    code/sorting/data/measurements/tiempos_sorting.csv

    code/matrix_multiplication/data/measurements/tiempos_matrix.csv
    
Estos archivos son el insumo directo con el que el script plotter.py calcula el promedio aritmético de las réplicas (a, b, c) y dibuja las curvas de rendimiento en escala logarítmica dentro de data/plots/.

**Salidas de cada input**: Cada salida procesada se guarda en data/array_output/ y data/matrix_output/, lo que permite validar que los algoritmos alternativos generen soluciones idénticas a los métodos de referencia.

**Compilación del Informe en LaTeX**
Para compilar el informe técnico asegurando el enlace correcto de figuras y bibliografía:

    cd report
    pdflatex report.tex
    biber report
    pdflatex report.tex
    pdflatex report.tex
El archivo resultante (report.pdf) tomará automáticamente los gráficos presentes en las carpetas data/plots/.