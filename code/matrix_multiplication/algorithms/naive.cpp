/**
 * ==============================================================================
 *       Multiplicación Clásica / Ingenua de Matrices (Naive O(N^3))
 * ARCHIVO:         code/matrix_multiplication/algorithms/naive.cpp
 * FUENTES:
 *   1. Dev-XYS/Algorithms (implementación base de referencia):
 *      https://github.com/Dev-XYS/Algorithms/blob/master/Matrix-Multiplication(Naive).cpp
 *   2. ByteQuest (2023). "Strassen Algorithm Visually Explained":
 *      https://www.youtube.com/watch?v=2IgZuVGwEb0
 * ==============================================================================
 * 
 * 1. LIBRERÍAS:
 * ------------------------------------------------------------------------------
 * - <vector> : Contenedor dinámico contiguo bidimensional para representar 
 *              matrices en memoria sin límites rígidos de tamaño fijo.
 * 
 * 2. RESUMEN FUNCIONAL DEL ARCHIVO:
 * ------------------------------------------------------------------------------
 * Provee la interfaz `naive_multiply(const Matrix& A, const Matrix& B)` que 
 * computa el producto matricial directo C = A * B para matrices de dimensiones 
 * arbitrarias compatibles, retornando una nueva estructura en memoria.
 * 
 * 3. FUNCIONAMIENTO:
 * ------------------------------------------------------------------------------
 * Se basa en la definición algebraica tradicional del producto interno (filas por 
 * columnas). Para calcular cada entrada C[i][j], se calcula la suma ponderada del 
 * producto de los elementos de la fila i de la matriz A por la columna j de la 
 * matriz B:
 *                    C[i][j] = sum_{k=0}^{P-1} (A[i][k] * B[k][j])
 * 
 * Al componerse de tres bucles anidados que iteran sobre las filas de A (M), las 
 * columnas de B (N) y la dimensión compartida (P), ejecuta invariablemente 
 * Theta(M * N * P) multiplicaciones y sumas escalares. En el caso de matrices 
 * cuadradas de N x N, la complejidad temporal estricta es O(N^3).
 * ==============================================================================
 */

#include <vector>

using Matrix = std::vector<std::vector<int>>; // 

Matrix naive_multiply(const Matrix& A, const Matrix& B) {
    size_t m = A.size();       // Filas de A
    size_t p = A[0].size();    // Columnas de A / Filas de B
    size_t n = B[0].size();    // Columnas de B

    Matrix C(m, std::vector<int>(n, 0));

    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            for (size_t k = 0; k < p; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return C;
}