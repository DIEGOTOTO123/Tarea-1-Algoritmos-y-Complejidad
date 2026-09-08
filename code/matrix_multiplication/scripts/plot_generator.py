"""
==============================================================================
                Plot Generator

FUENTES / REFS:
  1. Hunter, J. D. (2007). "Matplotlib: A 2D graphics environment". 
     Computing in Science & Engineering, 9(3), 90-95.
  2. McKinney, W. (2010). "Data Structures for Statistical Computing in Python". 
     Proceedings of the 9th Python in Science Conference, 51-56. (Pandas).
==============================================================================

1. LIBRERÍAS:
------------------------------------------------------------------------------
- os         : Resolución de rutas relativas y creación de directorios en disco.
- pandas     : Ingesta de métricas crudas, filtrado y agregación estadística (mean).
- matplotlib : Motor gráfico para el trazado de curvas y exportación a PNG en alta resolución.

2. ESTRUCTURAS DE DATOS:
------------------------------------------------------------------------------
- pandas.DataFrame : Almacén tabular bidimensional con las tuplas experimentales:
  (algoritmo, dimensión N, tipo estructural, dominio, muestra y tiempo de cómputo).
- Series / GroupBy : Agrupaciones particionadas para computar la media aritmética
  sobre las tres réplicas independientes (a, b, c) de cada instancia.

3. RESUMEN FUNCIONAL DEL ARCHIVO:
------------------------------------------------------------------------------
Procesa el archivo `data/measurements/tiempos_matrix.csv`, calcula los promedios
de tiempo para neutralizar la latencia del sistema operativo y renderiza las curvas
asintóticas comparativas [Naive O(N^3) vs Strassen O(N^2.807)] sobre base 2 en el
eje horizontal y escala logarítmica vertical, exportando las figuras finales a `data/plots/`.

4. FUNCIONAMIENTO:
------------------------------------------------------------------------------
El flujo analítico transforma las mediciones crudas en gráficas interpretables:

  * Carga y Agregación: Lee el CSV y ejecuta `groupby(['algoritmo', 'n', 'tipo', 
     'dominio'])['tiempo_ms'].mean()` para colapsar las réplicas (a, b, c) en
     un único estadístico robusto.
  * Particionado por Matrices: Itera sobre las combinaciones de estructura (densa, 
     diagonal, dispersa) y dominio (D0, D10), aislando cada escenario de prueba.
  * Escalas Logarítmicas: Aplica escala en base 2 para el eje X (`log, base=2`)
     dado que N varía en potencias de dos (16, 64, 256, 1024), y escala logarítmica
     en el eje Y para observar claramente la diferencia de pendiente entre las cotas
     polinomiales O(N^3) y O(N^2.807).
  * Exportación: Guarda cada gráfico como archivo `.png` en calidad de imprenta 
     (300 DPI) mediante `savefig(..., dpi=300)`.
==============================================================================
"""

import os
import pandas as pd
import matplotlib.pyplot as plt

BASE_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
CSV_PATH = os.path.join(BASE_DIR, "data", "measurements", "tiempos_matrix.csv")
PLOTS_DIR = os.path.join(BASE_DIR, "data", "plots")

os.makedirs(PLOTS_DIR, exist_ok=True)

def graficar():
    if not os.path.exists(CSV_PATH):
        print(f"Error: No se encontro el archivo de mediciones en {CSV_PATH}")
        return

    df = pd.read_csv(CSV_PATH)
    df_mean = df.groupby(["algoritmo", "n", "tipo", "dominio"])["tiempo_ms"].mean().reset_index()

    tipos = df["tipo"].unique()
    dominios = df["dominio"].unique()

    for tipo in tipos:
        for dom in dominios:
            plt.figure(figsize=(8, 5))
            subset = df_mean[(df_mean["tipo"] == tipo) & (df_mean["dominio"] == dom)]

            for algo in subset["algoritmo"].unique():
                data = subset[subset["algoritmo"] == algo].sort_values("n")
                plt.plot(data["n"], data["tiempo_ms"], marker="o", label=algo)

            plt.xscale("log", base=2)
            plt.yscale("log")
            plt.xlabel("Dimension de la matriz (N)")
            plt.ylabel("Tiempo promedio (ms) [Escala Log]")
            plt.title(f"Multiplicacion Matricial - {tipo.capitalize()} ({dom})")
            plt.grid(True, which="both", linestyle="--", alpha=0.5)
            plt.legend()
            plt.tight_layout()

            out_png = os.path.join(PLOTS_DIR, f"plot_{tipo}_{dom}.png")
            plt.savefig(out_png, dpi=300)
            plt.close()
            print(f"Grafico guardado: {out_png}")

if __name__ == "__main__":
    graficar()