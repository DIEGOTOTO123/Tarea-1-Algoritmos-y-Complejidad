/**
 * ==============================================================================
 *              Quick Sort
 * AUTOR: Diego Ariel Araneda Campusano
 * FUENTES:
 *   1. TheAlgorithms/C-Plus-Plus (base didáctica de partida, posteriormente refactorizada):
 *      https://github.com/TheAlgorithms/C-Plus-Plus/blob/master/sorting/quick_sort.cpp
 *   2. Educative. (Particion Hoare)
 *      https://www.educative.io/answers/hoares-vs-lomuto-partition-scheme-in-quicksort
 * ==============================================================================
 * 
 * 1. LIBRERÍAS:
 * ------------------------------------------------------------------------------
 * - <vector>  : Contenedor secuencial que aloja el arreglo a ordenar.
 * - <cstdlib> : Acceso a utilidades de cálculo entero y manipulación básica.
 * - <utility> : Provee `std::swap` para intercambios eficientes en memoria in-place.
 * 
 * 2. ESTRUCTURAS DE DATOS:
 * ------------------------------------------------------------------------------
 * - std::vector<int>& : Arreglo contiguo ordenado completamente in-place, sin 
 *   requerir estructuras de asignación secundaria en el heap.
 * - Call Stack del Sistema: Pila de ejecución acotada a una profundidad máxima 
 *   estricta de O(log N) gracias a la eliminación de recursión por cola.
 * 
 * 3. RESUMEN FUNCIONAL DEL ARCHIVO:
 * ------------------------------------------------------------------------------
 * Contiene la implementación robusta de Quick Sort. Utiliza la partición bidireccional 
 * de Hoare con selección de pivote en el punto medio (`mid`) y un bucle de particionado 
 * que procesa recursivamente solo la partición más pequeña, evitando el desbordamiento 
 * de pila (Stack Overflow) ante grandes volúmenes de datos (N = 10^7).
 * 
 * 4.FUNCIONAMIENTO:
 * ------------------------------------------------------------------------------
 * A diferencia de Merge Sort, Quick Sort se basa en hacer el trabajo 
 * duro ANTES de la recursión. Selecciona un valor de referencia (pivote) y reorganiza 
 * los elementos mediante dos punteros que avanzan desde los extremos hacia el centro 
 * (esquema de Hoare), deteniéndose en elementos fuera de lugar e intercambiándolos. 
 * Esta variante es notablemente superior al esquema de Lomuto frente a arreglos con 
 * claves masivamente duplicadas, ya que no sesga elementos idénticos hacia un solo 
 * lado. Al finalizar el particionado, el pivote queda como frontera natural entre 
 * elementos menores y mayores. Para garantizar un consumo espacial mínimo en la pila 
 * del sistema, se procesa recursivamente la sección más corta y se reutiliza el marco 
 * de ejecución mediante un bucle while sobre la mitad más larga, logrando un tiempo 
 * esperado de O(N log N) y una sobrecarga espacial auxiliar de apenas O(log N).
 * Este esquema un poco mas avanzado fue necesario ya que con los salidos directos de
 * repositorios, no acababa nunca la ejecución.
 * ==============================================================================
 */

#include <vector>
#include <cstdlib>
#include <utility>

namespace {
    int partition_hoare(std::vector<int>& arr, int low, int high) {
        int mid = low + (high - low) / 2;
        int pivot = arr[mid];

        int i = low - 1;
        int j = high + 1;

        while (true) {
            do {
                i++;
            } while (arr[i] < pivot);

            do {
                j--;
            } while (arr[j] > pivot);

            if (i >= j) return j;

            std::swap(arr[i], arr[j]);
        }
    }

    void quick_sort_rec(std::vector<int>& arr, int low, int high) {
        while (low < high) {
            int p = partition_hoare(arr, low, high);

            if (p - low < high - p) {
                quick_sort_rec(arr, low, p);
                low = p + 1;
            } else {
                quick_sort_rec(arr, p + 1, high);
                high = p;
            }
        }
    }
}

void quick_sort(std::vector<int>& arr) {
    if (!arr.empty()) {
        quick_sort_rec(arr, 0, arr.size() - 1);
    }
}