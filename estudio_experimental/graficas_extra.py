"""
Graficas adicionales para la memoria.
Genera unicamente la grafica de tiempos AR en escala lineal
(la log-log la produce regresion_AR.py).
"""

import csv
import numpy as np
import matplotlib.pyplot as plt

N, T = [], []
with open("resultados_tiempos_AR.csv", "r", encoding="utf-8") as f:
    r = csv.reader(f)
    next(r)
    for row in r:
        if not row:
            continue
        N.append(float(row[0]))
        T.append(float(row[1]))
N = np.array(N)
T = np.array(T)

plt.figure(figsize=(9, 5.5))
plt.plot(N, T, "o-", color="tab:blue", markersize=7,
         label="Mediana experimental (51 reps por N)")
plt.xlabel("N (numero de alumnos)")
plt.ylabel("Tiempo (ms)")
plt.title("Avance Rapido (PID 705) - tiempos experimentales")
plt.grid(True, linestyle="--", alpha=0.5)
plt.legend()
plt.tight_layout()
plt.savefig("tiempos_AR.png", dpi=200)
plt.close()

print("Grafica guardada: tiempos_AR.png")
