/**
 * ==============================================================================
 * ALGORITMO:       Algoritmo de Strassen para Multiplicación de Matrices
 * ARCHIVO:         code/matrix_multiplication/algorithms/strassen.cpp
 * FUENTES:
 *   1. TheAlgorithms/C-Plus-Plus (referencia estructural base adaptada):
 *      https://github.com/TheAlgorithms/C-Plus-Plus/blob/master/divide_and_conquer/strassens_multiplication.cpp
 *   2. ByteQuest (2023). "Strassen Algorithm Visually Explained":
 *      https://www.youtube.com/watch?v=2IgZuVGwEb0
 * ==============================================================================
 * 
 * 1. LIBRERÍAS:
 * ------------------------------------------------------------------------------
 * - <vector> : Contenedor dinámico contiguo bidimensional para representar matrices.
 * 
 * 2. ESTRUCTURAS DE DATOS:
 * ------------------------------------------------------------------------------
 * - using Matrix = std::vector<std::vector<int>> : Representación de matrices densas.
 * 
 * 3. FUNCIONAMIENTO:
 * ------------------------------------------------------------------------------
 * Reduce las 8 multiplicaciones cuadrantes estándar de Divide y Vencerás a 7
 * productos matriciales mediante combinaciones lineales auxiliares (M1 a M7),
 * reduciendo la complejidad asintótica de O(N^3) a O(N^2.807).
 * ==============================================================================
 */

#include <vector>

using Matrix = std::vector<std::vector<int>>;

// Declaración previa de naive_multiply para el caso base
Matrix naive_multiply(const Matrix& A, const Matrix& B);

namespace {
    Matrix add(const Matrix& A, const Matrix& B) {
        size_t n = A.size();
        Matrix C(n, std::vector<int>(n));
        for (size_t i = 0; i < n; ++i)
            for (size_t j = 0; j < n; ++j)
                C[i][j] = A[i][j] + B[i][j];
        return C;
    }

    Matrix sub(const Matrix& A, const Matrix& B) {
        size_t n = A.size();
        Matrix C(n, std::vector<int>(n));
        for (size_t i = 0; i < n; ++i)
            for (size_t j = 0; j < n; ++j)
                C[i][j] = A[i][j] - B[i][j];
        return C;
    }
}

Matrix strassen_multiply(const Matrix& A, const Matrix& B) {
    size_t n = A.size();

    // Caso base: umbral donde la sobrecarga de copias no justifica la recursión
    if (n <= 64 || (n % 2 != 0)) {
        return naive_multiply(A, B);
    }

    size_t k = n / 2;
    Matrix a11(k, std::vector<int>(k)), a12(k, std::vector<int>(k)),
           a21(k, std::vector<int>(k)), a22(k, std::vector<int>(k)),
           b11(k, std::vector<int>(k)), b12(k, std::vector<int>(k)),
           b21(k, std::vector<int>(k)), b22(k, std::vector<int>(k));

    // Partición de matrices en 4 cuadrantes de tamaño k x k
    for (size_t i = 0; i < k; ++i) {
        for (size_t j = 0; j < k; ++j) {
            a11[i][j] = A[i][j];
            a12[i][j] = A[i][j + k];
            a21[i][j] = A[i + k][j];
            a22[i][j] = A[i + k][j + k];

            b11[i][j] = B[i][j];
            b12[i][j] = B[i][j + k];
            b21[i][j] = B[i + k][j];
            b22[i][j] = B[i + k][j + k];
        }
    }

    // Las 7 multiplicaciones canónicas de Strassen
    Matrix M1 = strassen_multiply(add(a11, a22), add(b11, b22));
    Matrix M2 = strassen_multiply(add(a21, a22), b11);
    Matrix M3 = strassen_multiply(a11, sub(b12, b22));
    Matrix M4 = strassen_multiply(a22, sub(b21, b11));
    Matrix M5 = strassen_multiply(add(a11, a12), b22);
    Matrix M6 = strassen_multiply(sub(a21, a11), add(b11, b12));
    Matrix M7 = strassen_multiply(sub(a12, a22), add(b21, b22));

    // Reconstrucción de los cuatro cuadrantes de salida
    Matrix c11 = add(sub(add(M1, M4), M5), M7);
    Matrix c12 = add(M3, M5);
    Matrix c21 = add(M2, M4);
    Matrix c22 = add(sub(add(M1, M3), M2), M6);

    // Ensamble de la matriz final C
    Matrix C(n, std::vector<int>(n));
    for (size_t i = 0; i < k; ++i) {
        for (size_t j = 0; j < k; ++j) {
            C[i][j]         = c11[i][j];
            C[i][j + k]     = c12[i][j];
            C[i + k][j]     = c21[i][j];
            C[i + k][j + k] = c22[i][j];
        }
    }

    return C;
}