/**
 * ==============================================================================
 *                          Merge Sort 
 * AUTOR: Diego Ariel Araneda Campusano
 * FUENTES:
 *   1. TheAlgorithms/C-Plus-Plus (repositorio base referencial):
 *      https://github.com/TheAlgorithms/C-Plus-Plus/blob/master/sorting/merge_sort.cpp
 * ==============================================================================
 * 
 * 1. LIBRERÍAS:
 * ------------------------------------------------------------------------------
 * - <vector> : Contenedor de memoria contigua estándar para manejar los arreglos
 *              dinámicos de entrada y los subarreglos auxiliares temporales.
 * 
 * 2. ESTRUCTURAS DE DATOS
 * ------------------------------------------------------------------------------
 * - std::vector<int> (Entrada): Arreglo mutable pasado por referencia estricta (&) 
 *   para evitar copias innecesarias en cada nivel del árbol de recursión.
 * - std::vector<int> L, R (Memoria Auxiliar): Dos arreglos temporales asignados 
 *   dinámicamente en la etapa de combinación para sostener las mitades izquierda 
 *   y derecha antes de fusionarlas en orden.
 *
 * 
 * 3. FUNCIONAMIENTO:
 * ------------------------------------------------------------------------------
 * El alma de Merge Sort se centra "Dividir y Conquistar". Este divide el problema en 
 * dos subproblemas idénticos calculando el punto medio, resuelve recursivamente cada mitad 
 * hasta alcanzar el caso base (subarreglos de tamaño 0 o 1, que por definición están ordenados) 
 * y luego finaliza el trabajo en la etapa de mezcla (merge). Durante la mezcla, 
 * dos punteros recorren simultáneamente los subarreglos ordenados insertando siempre 
 * el elemento menor en el arreglo original, preservando el orden. 
 * ==============================================================================
 */

#include <vector>

namespace {
    void merge(std::vector<int>& arr, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        std::vector<int> L(n1), R(n2);
        for (int i = 0; i < n1; i++) L[i] = arr[left + i];
        for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2) {
            if (L[i] <= R[j]) arr[k++] = L[i++];
            else arr[k++] = R[j++];
        }
        while (i < n1) arr[k++] = L[i++];
        while (j < n2) arr[k++] = R[j++];
    }

    void merge_sort_rec(std::vector<int>& arr, int left, int right) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            merge_sort_rec(arr, left, mid);
            merge_sort_rec(arr, mid + 1, right);
            merge(arr, left, mid, right);
        }
    }
}

void merge_sort(std::vector<int>& arr) {
    if (!arr.empty()) {
        merge_sort_rec(arr, 0, arr.size() - 1);
    }
}