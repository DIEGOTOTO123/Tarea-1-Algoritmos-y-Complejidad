/**
 * ==============================================================================
 *            Orquestador de Pruebas y Benchmark Matricial
 * 
 * AUTOR: Diego Ariel Araneda Campusano
 * FUENTES:
 *   1. Documentación oficial cppreference.com sobre chrono, filesystem y set:
 *      https://en.cppreference.com/w/cpp/chrono
 *      https://en.cppreference.com/w/cpp/filesystem
 *      https://en.cppreference.com/w/cpp/container/set
 * ==============================================================================
 * 
 * 1. LIBRERÍAS:
 * ------------------------------------------------------------------------------
 * - <iostream>   : Control de mensajes informativos, trazas de consola y errores.
 * - <fstream>    : Flujos de archivo (`ifstream`, `ofstream`) para I/O sobre matrices y CSV.
 * - <vector>     : Representación dinámica contigua de filas y matrices bidimensionales.
 * - <string>     : Manipulación de identificadores, nombres de archivos y serialización.
 * - <sstream>    : Parsing de cadenas para leer filas numéricas y descomponer metadatos.
 * - <chrono>     : Reloj de alta resolución (`high_resolution_clock`) para cronometraje neto.
 * - <filesystem> : Exploración iterativa del directorio de entrada y creación de carpetas.
 * - <algorithm>  : Rutinas auxiliares de ordenamiento y control sobre contenedores STL.
 * - <set>        : Deduplicación ordenada de prefijos de prueba para emparejar matrices.
 * 
 * 2. ESTRUCTURAS DE DATOS:
 * ------------------------------------------------------------------------------
 * - using Matrix = std::vector<std::vector<int>> : Matriz cuadrada densa en memoria dinámica.
 * - std::set<std::string> prefijos : Conjunto ordenado sin duplicados que consolida las
 *   instancias de prueba a partir de los sufijos `_1` y `_2`.
 * - Matrix A, Matrix B : Instancias operando cargadas en memoria principal antes del cómputo.
 * - Matrix C_naive, C_strassen : Matrices de salida resultantes calculadas por cada método.
 * - std::ofstream csv : Flujo persistente que acumula los registros en `tiempos_matrix.csv`.
 * 
 * 3. RESUMEN FUNCIONAL DEL ARCHIVO:
 * ------------------------------------------------------------------------------
 * Detecta y empareja automáticamente los archivos matriciales (`_1.txt` y `_2.txt`) en
 * `data/matrix_input/`, ejecuta sobre cada par los algoritmos Naive O(N^3) y Strassen
 * O(N^2.807) bajo cronometraje aislado, serializa los productos en `data/matrix_output/`
 * y compila las mediciones temporales tabuladas en `data/measurements/tiempos_matrix.csv`.
 * 
 * 4. FUNCIONAMIENTO:
 * ------------------------------------------------------------------------------
 * Para garantizar la validez del benchmark y eliminar sesgos por acceso a disco:
 *   a) Deduplicación de prefijos: Explora `data/matrix_input/` y extrae los nombres base
 *      únicos ({n}_{tipo}_{dominio}_{muestra}), ignorando los identificadores `_1` y `_2`.
 *   b) Carga desacoplada: Carga ambas matrices operando a memoria RAM antes de iniciar
 *      la ventana temporal de medición.
 *   c) Cronometraje neto: La captura con `std::chrono::high_resolution_clock` envuelve
 *      exclusivamente la invocación algorítmica (`naive_multiply` o `strassen_multiply`).
 *   d) Persistencia diferida: El registro en el archivo CSV y el guardado de la matriz 
 *      resultante en disco se realizan fuera del intervalo cronometrado.
 * 
 * Esta arquitectura garantiza una comparación asintótica justa y repetible entre el
 * crecimiento cúbico Theta(N^3) y la partición divide-and-conquer Theta(N^2.807).
 * ==============================================================================
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <filesystem>
#include <sstream>
#include <algorithm>
#include <set>

namespace fs = std::filesystem;
using Matrix = std::vector<std::vector<int>>;


Matrix naive_multiply(const Matrix& A, const Matrix& B);
Matrix strassen_multiply(const Matrix& A, const Matrix& B);

Matrix leer_matriz(const fs::path& ruta) {
    std::ifstream file(ruta);
    Matrix mat;
    std::string linea;
    while (std::getline(file, linea)) {
        if (linea.empty()) continue;
        std::stringstream ss(linea);
        std::vector<int> fila;
        int val;
        while (ss >> val) {
            fila.push_back(val);
        }
        if (!fila.empty()) {
            mat.push_back(fila);
        }
    }
    return mat;
}

void guardar_matriz(const fs::path& ruta, const Matrix& C) {
    std::ofstream out(ruta);
    if (!out.is_open()) return;
    for (const auto& fila : C) {
        for (size_t j = 0; j < fila.size(); ++j) {
            out << fila[j] << (j + 1 < fila.size() ? " " : "");
        }
        out << "\n";
    }
}

int main() {
    fs::path input_dir = "data/matrix_input";
    fs::path output_dir = "data/matrix_output";
    fs::path meas_dir = "data/measurements";

    if (!fs::exists(input_dir)) {
        std::cerr << "Error: Directorio no encontrado: " << input_dir << "\n";
        return 1;
    }

    fs::create_directories(output_dir);
    fs::create_directories(meas_dir);

    fs::path csv_path = meas_dir / "tiempos_matrix.csv";
    std::ofstream csv(csv_path);
    csv << "algoritmo,n,tipo,dominio,muestra,tiempo_ms\n";


    std::set<std::string> prefijos;
    for (const auto& entry : fs::directory_iterator(input_dir)) {
        std::string fname = entry.path().stem().string();
        if (fname.size() > 2 && (fname.rfind("_1") == fname.size() - 2 || fname.rfind("_2") == fname.size() - 2)) {
            prefijos.insert(fname.substr(0, fname.size() - 2));
        }
    }

    std::cout << "Ejecutando multiplicación para " << prefijos.size() << " casos de prueba...\n";

    for (const auto& base : prefijos) {
        std::stringstream ss(base);
        std::string n_str, tipo, dominio, muestra;
        std::getline(ss, n_str, '_');
        std::getline(ss, tipo, '_');
        std::getline(ss, dominio, '_');
        std::getline(ss, muestra, '_');

        fs::path ruta_A = input_dir / (base + "_1.txt");
        fs::path ruta_B = input_dir / (base + "_2.txt");

        Matrix A = leer_matriz(ruta_A);
        Matrix B = leer_matriz(ruta_B);

        if (A.empty() || B.empty()) continue;

        //  Naive Multiplication
        {
            auto t0 = std::chrono::high_resolution_clock::now();
            Matrix C_naive = naive_multiply(A, B);
            auto t1 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> ms = t1 - t0;

            csv << "naive," << n_str << "," << tipo << "," << dominio << "," << muestra << "," << ms.count() << "\n";
            std::cout << "[naive] " << base << " -> " << ms.count() << " ms\n";
            guardar_matriz(output_dir / ("naive_" + base + ".txt"), C_naive);
        }

        //  Strassen Multiplication
        {
            auto t0 = std::chrono::high_resolution_clock::now();
            Matrix C_strassen = strassen_multiply(A, B);
            auto t1 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> ms = t1 - t0;

            csv << "strassen," << n_str << "," << tipo << "," << dominio << "," << muestra << "," << ms.count() << "\n";
            std::cout << "[strassen] " << base << " -> " << ms.count() << " ms\n";
            guardar_matriz(output_dir / ("strassen_" + base + ".txt"), C_strassen);
        }
    }

    csv.close();
    std::cout << "\nBenchmark matricial finalizado. Datos guardados en: " << csv_path << "\n";
    return 0;
}