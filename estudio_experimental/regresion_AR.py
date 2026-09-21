"""
Regresion para el algoritmo voraz (Avance Rapido) - PID 705.

Lee resultados_tiempos_AR.csv (generado por tiempos_AR.cpp con
espaciado multiplicativo denso y mezcla de tres distribuciones de
instancias) y ajusta el modelo teorico
        t(n) = a * n^2 * log2(n) + b
Genera dos graficas: ajuste lineal (sobre x = n^2 log n) y
escala log-log con la pendiente experimental.
"""

import csv
import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import linregress

filename = "resultados_tiempos_AR.csv"

N, T = [], []
with open(filename, "r", encoding="utf-8") as f:
    reader = csv.reader(f)
    next(reader)
    for row in reader:
        if not row:
            continue
        N.append(float(row[0]))
        T.append(float(row[1]))

N = np.array(N)
T = np.array(T)

# ----- Ajuste t = a * n^2 * log2(n) + b -----
X = N ** 2 * np.log2(N)
slope, intercept, r_value, _, _ = linregress(X, T)
R2 = r_value ** 2
T_pred = slope * X + intercept

print("--- Avance Rapido (PID 705) ---")
print(f"Numero de puntos: {len(N)}")
print(f"Modelo: t = a * n^2 * log2(n) + b")
print(f"  a = {slope:.4e} ms")
print(f"  b = {intercept:.4e} ms")
print(f"  R^2 = {R2:.6f}")

plt.figure(figsize=(9, 5.5))
plt.plot(N, T, "o", color="tab:red", markersize=8, label="Datos experimentales")
plt.plot(N, T_pred, "-", color="tab:blue", linewidth=2,
         label=r"Ajuste $t = a\, n^{2}\log_{2} n + b$")
plt.xlabel("N (numero de alumnos)")
plt.ylabel("Tiempo (ms)")
plt.title(rf"Avance Rapido: ajuste a $\Theta(n^{{2}}\log n)$  (R$^2$={R2:.4f})")
plt.legend(loc="upper left", fontsize=11)
plt.grid(True, linestyle="--", alpha=0.5)
plt.tight_layout()
plt.savefig("ajuste_AR.png", dpi=200)
plt.close()

# ----- Analisis en escala log-log -----
logN = np.log2(N)
logT = np.log2(T)
sl2, ic2, r2v, _, _ = linregress(logN, logT)
print(f"\nEscala log-log:")
print(f"  pendiente = {sl2:.4f}  (teorico para n^2 log n: ~ 2.10-2.30)")
print(f"  R^2       = {r2v**2:.6f}")

plt.figure(figsize=(9, 5.5))
plt.plot(logN, logT, "o", color="tab:red", markersize=8, label="Datos en log-log")
plt.plot(logN, sl2 * logN + ic2, "-", color="tab:blue", linewidth=2,
         label=f"Recta de regresion (pendiente {sl2:.3f})")
plt.xlabel(r"$\log_{2} N$")
plt.ylabel(r"$\log_{2} t$ (ms)")
plt.title(rf"Avance Rapido: escala log-log  (R$^2$={r2v**2:.4f})")
plt.legend(loc="upper left", fontsize=11)
plt.grid(True, which="both", linestyle="--", alpha=0.5)
plt.tight_layout()
plt.savefig("loglog_AR.png", dpi=200)
plt.close()

print("\nGraficas guardadas: ajuste_AR.png, loglog_AR.png")
