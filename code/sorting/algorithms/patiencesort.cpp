/**
 * ==============================================================================
 *                 Patience Sort 
 * 
 * AUTOR: Diego Ariel Araneda Campusano
 * FUENTES:
 *   1. GeeksforGeeks (referencia didáctica inicial):
 *      https://www.geeksforgeeks.org/dsa/patience-sorting/
 *   2. Stable Sort (2019). "Longest Increasing Subsequence O(n log n) dynamic 
 *      programming Java source code":
 *      https://www.youtube.com/watch?v=22s1xxRvy28
 * ==============================================================================
 * 
 * 1. LIBRERÍAS:
 * ------------------------------------------------------------------------------
 * - <vector>    : Manejo contiguo de las pilas dinámicas y del arreglo ordenado final.
 * - <queue>     : Provee `std::priority_queue`, utilizada como Min-Heap para la fusión.
 * - <algorithm> : Provee `std::lower_bound` para ejecutar búsqueda binaria en O(log k).
 * 
 * 2. ESTRUCTURAS DE DATOS:
 * ------------------------------------------------------------------------------
 * - struct Node: Tupla liviana `(val, pile_idx)` con sobrecarga de `operator>` 
 *   para modelar un árbol de prioridad de tipo Min-Heap.
 * - std::vector<std::vector<int>> piles: Vector de vectores donde cada fila representa 
 *   una pila de cartas. Cada pila mantiene sus elementos en orden descendente.
 * - std::vector<int> top_elements: Vector auxiliar que refleja exclusivamente el 
 *   tope visible de cada pila. Dado que estos topes son monótonamente crecientes 
 *   de izquierda a derecha, permiten el uso de búsqueda binaria.
 * - std::priority_queue<Node, ...>: Min-Heap de tamaño k (donde k es el número de pilas) 
 *   que permite extraer el mínimo global de todos los topes en O(log k).
 * 
 * 4. EL ALMA DEL ALGORITMO Y SU FUNCIONAMIENTO:
 * ------------------------------------------------------------------------------
 * El alma de este algoritmo se inspira directamente en el solitario de cartas "Patience". 
 * Su lógica opera bajo un teorema fundamental de combinatoria: el número de pilas 
 * resultantes (k) equivale exactamente a la longitud de la subsecuencia creciente más 
 * larga (LIS) del arreglo. En la primera fase, cada número entrante se coloca en la 
 * primera pila disponible cuyo tope sea mayor o igual a él; si no existe, funda una 
 * nueva pila a la derecha. La invariante de que los topes forman una secuencia 
 * estrictamente creciente permite localizar la pila correcta en O(log k) mediante 
 * `std::lower_bound`. En la segunda fase, la extracción no es una búsqueda exhaustiva, 
 * sino una fusión múltiple de k vías: un Min-Heap mantiene los k topes activos, 
 * extrayendo siempre el menor absoluto en tiempo O(log k) y reabasteciéndose con el 
 * siguiente elemento de la pila correspondiente. La complejidad total es O(N log k), 
 * lo que vuelve al algoritmo asombrosamente rápido (casi lineal O(N)) cuando los datos 
 * ya vienen parcialmente ordenados en sentido decreciente (pocas pilas), degradando a 
 * O(N log N) en el caso general aleatorio.
 * ==============================================================================
 */

#include <vector>
#include <queue>
#include <algorithm>

namespace {
    struct Node {
        int val;
        int pile_idx;
        bool operator>(const Node& other) const { return val > other.val; }
    };
}

void patience_sort(std::vector<int>& arr) {
    if (arr.size() <= 1) return;

    std::vector<std::vector<int>> piles;
    std::vector<int> top_elements;

    for (int x : arr) {
        auto it = std::lower_bound(top_elements.begin(), top_elements.end(), x);
        int idx = std::distance(top_elements.begin(), it);

        if (it == top_elements.end()) {
            piles.push_back({x});
            top_elements.push_back(x);
        } else {
            piles[idx].push_back(x);
            top_elements[idx] = x;
        }
    }

    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> min_heap;
    for (size_t i = 0; i < piles.size(); ++i) {
        min_heap.push({piles[i].back(), static_cast<int>(i)});
        piles[i].pop_back();
    }

    size_t out_idx = 0;
    while (!min_heap.empty()) {
        Node cur = min_heap.top();
        min_heap.pop();
        arr[out_idx++] = cur.val;

        if (!piles[cur.pile_idx].empty()) {
            min_heap.push({piles[cur.pile_idx].back(), cur.pile_idx});
            piles[cur.pile_idx].pop_back();
        }
    }
}