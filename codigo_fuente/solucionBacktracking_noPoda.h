#ifndef SOLUCION_BACKTRACKING_NO_PODA_H
#define SOLUCION_BACKTRACKING_NO_PODA_H

#include <vector>

extern int presupuesto, numPrendas;
extern std::vector<std::vector<int>> catalogo;

void generar(int nivel, std::vector<int>& solucionAct);
void retroceder(int &nivel, std::vector<int> &s);
bool solucion(int nivel, int sumaNivel);
bool criterio(int nivel, int sumaNivel, int solucionOptima);
void actualizarSuma(int nivel, const std::vector<int> &solucionAct, std::vector<int> &suma);
int backtracking();
void leerArbol();
void imprimirResultado(int resultado);

#endif