#ifndef SOLUCION_BACKTRACKING_H
#define SOLUCION_BACKTRACKING_H

#include <vector>

extern int presupuesto, numPrendas;
extern std::vector<std::vector<int>> catalogo;
extern std::vector<int> minimos;
extern std::vector<int> maximos;
extern std::vector<std::vector<bool>> visitado;

void generar(int nivel, std::vector<int>& solucionAct);
void retroceder(int &nivel, std::vector<int> &s);
bool solucion(int nivel, int sumaNivel);
bool criterio(int nivel, int sumaNivel, int solucionOptima);
void actualizarSuma(int nivel, const std::vector<int> &solucionAct, std::vector<int> &suma);
void vectoresAuxiliares();
int backtracking();
void leerArbol();
void imprimirResultado(int resultado);

#endif