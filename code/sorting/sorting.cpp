/**
 * ==============================================================================
 *            Orquestador de Pruebas y Benchmark
 * 
 * AUTOR: Diego Ariel Araneda Campusano
 * FUENTES:
 *   1. Documentación oficial cppreference.com sobre chrono y filesystem:
 *      https://en.cppreference.com/w/cpp/chrono
 *      https://en.cppreference.com/w/cpp/filesystem
 * ==============================================================================
 * 
 * 1. LIBRERÍAS:
 * ------------------------------------------------------------------------------
 * - <iostream>   : Control de mensajes informativos y de error por consola.
 * - <fstream>    : Flujos de archivo (`ifstream`, `ofstream`) para I/O masivo en disco.
 * - <vector>     : Manejo secuencial de los datos de prueba en memoria contigua.
 * - <string>     : Manipulación de identificadores y nombres de archivos.
 * - <sstream>    : Parsing de metadatos desde los nombres de archivo ({n}_{tipo}_{dom}_{rep}).
 * - <chrono>     : Reloj de alta resolución (`high_resolution_clock`) para cronometraje.
 * - <filesystem> : Recorrido iterativo de carpetas y creación automática de directorios.
 * - <algorithm>  : Ordenamiento léxico de los archivos de entrada para ejecución secuencial.
 * 
 * 2. ESTRUCTURAS DE DATOS:
 * ------------------------------------------------------------------------------
 * - std::vector<int> base_arr  : Arreglo de entrada original cargado una única vez
 *   por archivo para garantizar idénticas condiciones iniciales a todos los algoritmos.
 * - std::vector<int> arr_copia : Réplica mutable por valor entregada a cada algoritmo
 *   para ser ordenada in-place sin contaminar las pruebas posteriores.
 * - std::vector<fs::path>      : Lista de rutas ordenadas para iterar secuencialmente el dataset.
 * - std::string buffer         : Búfer de memoria (1 MB) para acelerar la escritura de salidas
 *   en `data/array_output/` evitando bloqueos por operaciones de disco con N = 10^7.
 * 
 * 3. RESUMEN FUNCIONAL DEL ARCHIVO:
 * ------------------------------------------------------------------------------
 * Lee automáticamente todos los archivos `.txt` desde `data/array_input/`, ejecuta sobre 
 * cada uno los 4 algoritmos de ordenamiento bajo cronometraje aislado, exporta los 
 * arreglos ordenados a `data/array_output/` y compila los tiempos de respuesta en 
 * formato tabular estructurado en `data/measurements/tiempos_sorting.csv`.
 * 
 * 4. FUNCIONAMIENTO:
 * ------------------------------------------------------------------------------
 * La premisa central del orquestador es garantizar la validez experimental y evitar 
 * sesgos de medición. Para asegurar esto, desacopla estrictamente las operaciones de 
 * entrada/salida (I/O) del cómputo puro: la lectura del disco y la escritura del resultado 
 * ocurren fuera de la ventana temporal de `std::chrono::high_resolution_clock`. 
 * 
 * El pipeline procesa cada archivo de prueba en 4 fases ordenadas:
 *   a) Parsing del nombre del archivo para extraer N, tipo, dominio y réplica (a/b/c).
 *   b) Carga íntegra del arreglo a memoria RAM principal.
 *   c) Ejecución y cronometraje aislado en milisegundos (`ms`) de cada algoritmo sobre
 *      una copia fresca del vector base.
 *   d) Registro directo de la métrica en el archivo CSV y persistencia del resultado ordenado.
 * 
 * Al gestionar todo el flujo desde C++, se eliminan sobrecargas externas de invocación 
 * por subprocesos y se automatiza la recolección masiva de mediciones para el análisis 
 * estadístico posterior en Python y LaTeX.
 * ==============================================================================
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

// Prototipos de los algoritmos
std::vector<int> sortArray(std::vector<int>& arr);
void merge_sort(std::vector<int>& arr);
void quick_sort(std::vector<int>& arr);
void patience_sort(std::vector<int>& arr);

// Función auxiliar para leer un arreglo desde disco
std::vector<int> leer_archivo(const fs::path& ruta) {
    std::ifstream file(ruta);
    std::vector<int> arr;
    int val;
    while (file >> val) {
        arr.push_back(val);
    }
    return arr;
}

// Función auxiliar para guardar el arreglo ordenado en array_output
void guardar_salida(const fs::path& ruta, const std::vector<int>& arr) {
    std::ofstream out(ruta);
    if (!out.is_open()) return;
    for (size_t i = 0; i < arr.size(); ++i) {
        out << arr[i] << (i + 1 < arr.size() ? " " : "");
    }
    out << "\n";
}

int main() {
    // Definición de rutas relativas a code/sorting
    fs::path input_dir = "data/array_input";
    fs::path output_dir = "data/array_output";
    fs::path meas_dir = "data/measurements";

    if (!fs::exists(input_dir)) {
        std::cerr << "Error: No existe el directorio " << input_dir << "\n";
        return 1;
    }

    fs::create_directories(output_dir);
    fs::create_directories(meas_dir);

    fs::path csv_path = meas_dir / "tiempos_sorting.csv";
    std::ofstream csv(csv_path);
    if (!csv.is_open()) {
        std::cerr << "Error al crear el archivo CSV de mediciones.\n";
        return 1;
    }

    // Encabezado del CSV
    csv << "algoritmo,n,tipo,dominio,muestra,tiempo_ms\n";

    std::vector<std::string> algoritmos = {"sort", "merge", "quick", "patience"};

    std::cout << "Iniciando experimentos de ordenamiento...\n";

    // Recorrer todos los archivos de prueba en orden
    std::vector<fs::path> archivos;
    for (const auto& entry : fs::directory_iterator(input_dir)) {
        if (entry.path().extension() == ".txt") {
            archivos.push_back(entry.path());
        }
    }
    std::sort(archivos.begin(), archivos.end());

    for (const auto& ruta_archivo : archivos) {
        std::string filename = ruta_archivo.stem().string(); // e.g. 1000_aleatorio_D1_a

        // Parsear el nombre: {n}_{tipo}_{dominio}_{muestra}
        std::stringstream ss(filename);
        std::string n_str, tipo, dominio, muestra;
        std::getline(ss, n_str, '_');
        std::getline(ss, tipo, '_');
        std::getline(ss, dominio, '_');
        std::getline(ss, muestra, '_');

        if (n_str.empty() || tipo.empty() || dominio.empty() || muestra.empty()) {
            continue; // Saltar si el archivo no sigue el patrón (como a.txt)
        }

        // Cargar el arreglo base una vez para todos los algoritmos
        std::vector<int> base_arr = leer_archivo(ruta_archivo);
        if (base_arr.empty()) continue;

        for (const auto& algo : algoritmos) {
            std::vector<int> arr_copia = base_arr;

            // Medición exclusiva del algoritmo
            auto t0 = std::chrono::high_resolution_clock::now();

            if (algo == "sort") arr_copia = sortArray(arr_copia);
            else if (algo == "merge") merge_sort(arr_copia);
            else if (algo == "quick") quick_sort(arr_copia);
            else if (algo == "patience") patience_sort(arr_copia);

            auto t1 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> ms = t1 - t0;

            // Registrar en el CSV
            csv << algo << "," << n_str << "," << tipo << "," << dominio << "," << muestra << "," << ms.count() << "\n";
            std::cout << "[" << algo << "] " << filename << " -> " << ms.count() << " ms\n";

            // Guardar salida ordenada en array_output/
            fs::path out_file = output_dir / (algo + "_" + filename + ".txt");
            guardar_salida(out_file, arr_copia);
        }
    }

    csv.close();
    std::cout << "\n¡Finalizado! Salidas en data/array_output/ y CSV en " << csv_path << "\n";
    return 0;
}