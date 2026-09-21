#include <bits/stdc++.h>
using namespace std;

int presupuesto, numPrendas;
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

bool criterio(int nivel, int sumaNivel, int solucionOptima) {
    if (sumaNivel > presupuesto) { return false; }
    if (nivel >= numPrendas - 1) { return false; }
    if (sumaNivel + minimos[nivel + 1] > presupuesto) { return false; }
    if (sumaNivel + maximos[nivel + 1] <= solucionOptima) { return false; }
    if (visitado[nivel][sumaNivel]) { return false; }
    return true;
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

void leerArbol() {
    cin >> presupuesto >> numPrendas;
    catalogo.assign(numPrendas, {});
    for (int i = 0; i < numPrendas; i++) {
        int K;
        cin >> K;

        catalogo[i].resize(K);
        for (int j = 0; j < K; j++) {
            cin >> catalogo[i][j];
        }

        sort(catalogo[i].begin(), catalogo[i].end(), greater<int>());
    }
}

void imprimirResultado(int resultado) {
    if (resultado == -1) { cout << "no solution\n"; }
    else { cout << resultado << "\n"; }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "Ejecutando BT con poda: " << endl;

    int casos;
    cin >> casos;

    while (casos--) {
        leerArbol();
        vectoresAuxiliares();
        int resultado = backtracking();
        imprimirResultado(resultado);
    }

    return 0;
}