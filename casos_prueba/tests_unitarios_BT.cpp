#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

int presupuesto;
int numPrendas;
vector<vector<int>> catalogo;
vector<int> minimos;
vector<int> maximos;
vector<vector<bool>> visitado;

void generar(int nivel, vector<int>& solucionAct) {
    solucionAct[nivel]++;
}

void retroceder(int &nivel, vector<int> &s) {
    s[nivel] = -1;
    nivel--;
}

bool solucion(int nivel, int sumaNivel) {
    return nivel == numPrendas - 1 && sumaNivel <= presupuesto;
}

void actualizarSuma(int nivel, const vector<int> &solucionAct, vector<int> &suma) {
    int sumaPrevia = (nivel > 0) ? suma[nivel - 1] : 0;
    suma[nivel] = sumaPrevia + catalogo[nivel][solucionAct[nivel]];
}

void vectoresAuxiliares() {
    minimos.assign(numPrendas + 1, 0);
    maximos.assign(numPrendas + 1, 0);
    for (int i = numPrendas - 1; i >= 0; i--) {
        minimos[i] = minimos[i + 1] + catalogo[i].back();
        maximos[i] = maximos[i + 1] + catalogo[i].front();
    }
}

bool criterio(int nivel, int sumaNivel, int solucionOptima) {
    if (sumaNivel > presupuesto) { return false; }
    if (nivel >= numPrendas - 1) { return false; }
    if (sumaNivel + minimos[nivel + 1] > presupuesto) { return false; }
    if (sumaNivel + maximos[nivel + 1] <= solucionOptima) { return false; }
    if (visitado[nivel][sumaNivel]) { return false; }
    return true;
}

int backtracking() {
    vector<int> solucionAct(numPrendas, -1);
    vector<int> suma(numPrendas, 0);
    visitado.assign(numPrendas, vector<bool>(presupuesto + 1, false));

    int nivel = 0;
    int solucionOptima = -1;

    while (nivel != -1) {
        generar(nivel, solucionAct);

        if (solucionAct[nivel] >= (int) catalogo[nivel].size()) {
            retroceder(nivel, solucionAct);
            continue;
        }

        actualizarSuma(nivel, solucionAct, suma);

        if (suma[nivel] > presupuesto) {
            continue;
        }

        if (solucion(nivel, suma[nivel])) {
            if (suma[nivel] > solucionOptima) { solucionOptima = suma[nivel]; }
            continue;
        }

        if (criterio(nivel, suma[nivel], solucionOptima)) {
            visitado[nivel][suma[nivel]] = true;
            nivel++;
            solucionAct[nivel] = -1;
        }
    }

    return solucionOptima;
}

void test_runner(int p, int c, const vector<vector<int>>& cat, int esperado) {
    presupuesto = p;
    numPrendas = c;
    catalogo = cat;

    for (int i = 0; i < numPrendas; i++) {
        sort(catalogo[i].begin(), catalogo[i].end(), greater<int>());
    }

    vectoresAuxiliares();
    int resultado = backtracking();
    assert(resultado == esperado);
}

int main() {
    test_runner(100, 4, {{8, 6, 4}, {5, 10}, {1, 3, 3, 7}, {50, 14, 23, 8}}, 75);
    test_runner(20, 3, {{4, 6, 8}, {5, 10}, {1, 3, 5, 5}}, 19);
    test_runner(5, 3, {{6, 4, 8}, {10, 6}, {7, 3, 1, 7}}, -1);
    test_runner(50, 3, {{20, 10}, {20, 15}, {10, 5}}, 50);
    test_runner(9, 3, {{5, 4}, {3, 2}, {4, 3}}, 9);
    test_runner(200, 2, {{50, 40}, {60, 30}}, 110);
    test_runner(100, 3, {{20}, {30}, {40}}, 90);

    cout << "Todos los tests unitarios de la version CON PODA han pasado correctamente.";

    return 0;
}
