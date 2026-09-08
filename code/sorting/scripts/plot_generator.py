"""
==============================================================================
            Plot Generator
AUTOR: Diego Ariel Araneda Campusano
FUENTES:
  1. Hunter, J. D. (2007). "Matplotlib: A 2D graphics environment". 
     Computing in Science & Engineering, 9(3), 90-95.
  2. McKinney, W. (2010). "Data Structures for Statistical Computing in Python". 
     Proceedings of the 9th Python in Science Conference, 51-56. (Pandas).
==============================================================================

1. LIBRERÍAS:
------------------------------------------------------------------------------
- os         : Gestión de rutas relativas y creación de directorios en disco.
- pandas     : Ingesta, filtrado, indexación y agregación estadística (mean).
- matplotlib : Motor de visualización para trazado de curvas y exportación a PNG.

2. ESTRUCTURAS DE DATOS:
------------------------------------------------------------------------------
- pandas.DataFrame : Estructura tabular bidimensional en memoria que modela las
  series temporales (algoritmo, tamaño N, tipo de arreglo, dominio, muestra y tiempo).
- Series / GroupBy : Agrupaciones particionadas para calcular el tiempo medio
  sobre las tres réplicas independientes (a, b, c) por cada instancia experimental.

3. RESUMEN FUNCIONAL DEL ARCHIVO:
------------------------------------------------------------------------------
Lee los datos consolidados en `data/measurements/tiempos_sorting.csv`, promedia
las ejecuciones independientes para mitigar el ruido del sistema operativo,
construye las curvas de escala asintótica en escala log-log y exporta las
gráficas finales en alta resolución a la carpeta `data/plots/`.

4. FUNCIONAMIENTO:
------------------------------------------------------------------------------
El propósito del script es transformar datos tabulares crudos en evidencia visual
interpretable para el análisis empírico. El flujo se estructura en cuatro etapas:

  * Carga y Agregación: Se ingesta el CSV y se aplica `groupby(['algoritmo', 
     'n', 'tipo', 'dominio'])['tiempo_ms'].mean()` para colapsar las muestras 
     (a, b, c) en una media representativa, reduciendo la variabilidad de CPU.
  * Particionado por Escenarios: Itera sobre las combinaciones de permutación
     (aleatorio, ordenado, inverso, etc.) y dominio de valores (D1, D7), aislando
     cada caso de prueba en un lienzo independiente.
  * Trazado en Escala Log-Log: Los ejes de tamaño (N) y tiempo de ejecución (ms)
     se configuran en escala logarítmica (`log-log`). En este espacio, una cota
     polinomial o lineal-logarítmica O(N log N) se visualiza como una pendiente
     lineal suave, permitiendo contrastar visualmente la divergencia de comportamientos.
  * Exportación: Renderiza y persiste cada figura como imagen `.png` sin cortes
     de margen (`tight_layout`), lista para inserción directa en LaTeX.
==============================================================================
"""

import os
import pandas as pd
import matplotlib.pyplot as plt

BASE_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
CSV_PATH = os.path.join(BASE_DIR, "data", "measurements", "tiempos_sorting.csv")
PLOTS_DIR = os.path.join(BASE_DIR, "data", "plots")

os.makedirs(PLOTS_DIR, exist_ok=True)

def plot():
    df = pd.read_csv(CSV_PATH)
    df_mean = df.groupby(["algoritmo", "n", "tipo", "dominio"])["tiempo_ms"].mean().reset_index()

    for tipo in df["tipo"].unique():
        for dom in df["dominio"].unique():
            plt.figure(figsize=(8, 5))
            subset = df_mean[(df_mean["tipo"] == tipo) & (df_mean["dominio"] == dom)]

            for algo in subset["algoritmo"].unique():
                data = subset[subset["algoritmo"] == algo].sort_values("n")
                plt.plot(data["n"], data["tiempo_ms"], marker="o", label=algo)

            plt.xscale("log")
            plt.yscale("log")
            plt.xlabel("N")
            plt.ylabel("Tiempo promedio (ms)")
            plt.title(f"Sorting - {tipo.capitalize()} ({dom})")
            plt.grid(True, which="both", linestyle="--", alpha=0.5)
            plt.legend()
            plt.tight_layout()

            plt.savefig(os.path.join(PLOTS_DIR, f"plot_{tipo}_{dom}.png"), dpi=300)
            plt.close()

    print(f"Gráficos generados en: {PLOTS_DIR}")

if __name__ == "__main__":
    plot()