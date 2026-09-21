"""
Regresion para el problema "Nos Vamos de Boda" (PID 901).

Lee dos CSV producidos por tiempos_BT.cpp:
  - resultados_tiempos_sinpoda.csv  (C de 2 a 28, K=2)
  - resultados_tiempos_conpoda.csv  (C de 5 a 200, K=20)

Realiza:
  1. Ajuste exponencial   t = a * 2^C + b           (sin poda)
  2. Ajuste polinomico    t = a * C^2 + b           (con poda)
  3. Grafica comparativa de ambas series.
"""

import csv
import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import linregress


def cargar(fichero):
    C, T = [], []
    with open(fichero, "r", encoding="utf-8") as f:
        r = csv.reader(f)
        next(r)
        for row in r:
            if not row:
                continue
            C.append(float(row[0]))
            T.append(float(row[1]))
    return np.array(C), np.array(T)


# =============================================================
#  Sin poda: ajuste exponencial t = a * 2^C + b
# =============================================================
C_np, T_np = cargar("resultados_tiempos_sinpoda.csv")
X_np = 2.0 ** C_np
slope_np, intercept_np, r_np, _, _ = linregress(X_np, T_np)
T_np_pred = slope_np * X_np + intercept_np

print("--- Backtracking sin poda ---")
print(f"Numero de puntos: {len(C_np)}")
print(f"Modelo: t = a * 2^C + b")
print(f"  a = {slope_np:.4e}  ms")
print(f"  b = {intercept_np:.4e}  ms")
print(f"  R^2 = {r_np**2:.6f}")

plt.figure(figsize=(9, 5.5))
plt.plot(C_np, T_np, "o", color="tab:red", markersize=8, label="Datos experimentales")
plt.plot(C_np, T_np_pred, "-", color="tab:blue", linewidth=2,
         label=r"Ajuste $t = a\cdot 2^{C} + b$")
plt.xlabel("C (numero de prendas)")
plt.ylabel("Tiempo (ms)")
plt.title(rf"BT sin poda: ajuste a $\Theta(2^{{C}})$  (R$^2$={r_np**2:.4f})")
plt.legend(loc="upper left", fontsize=11)
plt.grid(True, linestyle="--", alpha=0.5)
plt.tight_layout()
plt.savefig("ajuste_sin_poda.png", dpi=200)
plt.close()


# =============================================================
#  Con poda: ajuste t = a * C^2 + b  (cota O(n*M*K) con M~C, K=20)
# =============================================================
C_p, T_p = cargar("resultados_tiempos_conpoda.csv")
X_p = C_p ** 2
slope_p, intercept_p, r_p, _, _ = linregress(X_p, T_p)
T_p_pred = slope_p * X_p + intercept_p

print("\n--- Backtracking con poda y memoizacion ---")
print(f"Numero de puntos: {len(C_p)}")
print(f"Modelo: t = a * C^2 + b")
print(f"  a = {slope_p:.4e}  ms")
print(f"  b = {intercept_p:.4e}  ms")
print(f"  R^2 = {r_p**2:.6f}")

# Verificacion adicional con regresion log-log
logC = np.log2(C_p)
logT = np.log2(T_p)
sl_ll, ic_ll, r_ll, _, _ = linregress(logC, logT)
print(f"  Pendiente log-log = {sl_ll:.4f}  (teorica para C^2: ~ 2.00)")
print(f"  R^2 log-log       = {r_ll**2:.6f}")

plt.figure(figsize=(9, 5.5))
plt.plot(C_p, T_p, "o", color="tab:green", markersize=8, label="Datos experimentales")
plt.plot(C_p, T_p_pred, "-", color="tab:red", linewidth=2,
         label=r"Ajuste $t = a\cdot C^{2} + b$")
plt.xlabel("C (numero de prendas)")
plt.ylabel("Tiempo (ms)")
plt.title(rf"BT con poda: ajuste a $\Theta(C^{{2}})$  (R$^2$={r_p**2:.4f})")
plt.legend(loc="upper left", fontsize=11)
plt.grid(True, linestyle="--", alpha=0.5)
plt.tight_layout()
plt.savefig("ajuste_con_poda.png", dpi=200)
plt.close()


# =============================================================
#  Comparacion en escala semilogaritmica
# =============================================================
plt.figure(figsize=(9, 5.5))
plt.semilogy(C_np, T_np, "o-", color="tab:red", markersize=7,
             label="Sin poda (K=2)")
plt.semilogy(C_p, T_p, "s-", color="tab:green", markersize=7,
             label="Con poda y memoizacion (K=20)")
plt.xlabel("C (numero de prendas)")
plt.ylabel("Tiempo (ms) [escala log]")
plt.title("BT: comparacion sin poda vs. con poda")
plt.grid(True, which="both", linestyle="--", alpha=0.5)
plt.legend(fontsize=11)
plt.tight_layout()
plt.savefig("comparacion_BT.png", dpi=200)
plt.close()


# =============================================================
#  Grafica adicional: tiempos en escala lineal por separado
# =============================================================
fig, axes = plt.subplots(1, 2, figsize=(13, 5))
axes[0].plot(C_np, T_np, "o-", color="tab:red", markersize=7)
axes[0].set_title("BT sin poda (K=2)")
axes[0].set_xlabel("C")
axes[0].set_ylabel("Tiempo (ms)")
axes[0].grid(True, linestyle="--", alpha=0.5)

axes[1].plot(C_p, T_p, "s-", color="tab:green", markersize=7)
axes[1].set_title("BT con poda y memoizacion (K=20)")
axes[1].set_xlabel("C")
axes[1].set_ylabel("Tiempo (ms)")
axes[1].grid(True, linestyle="--", alpha=0.5)

plt.tight_layout()
plt.savefig("tiempos_BT.png", dpi=200)
plt.close()

print("\nGraficas guardadas: ajuste_sin_poda.png, ajuste_con_poda.png,")
print("                   comparacion_BT.png, tiempos_BT.png")
