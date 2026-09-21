#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <random>

using namespace std;

int presupuesto;
int numPrendas;
vector<vector<int>> catalogo;
vector<int> minimos;
vector<int> maximos;
vector<vector<bool>> visitado;

void genUniforme(int C, int K, mt19937& rng) {
    uniform_int_distribution<int> precio(10, 200);
    numPrendas = C;
    presupuesto = 200 * C;
    catalogo.assign(numPrendas, vector<int>(K));
    for (int i = 0; i < numPrendas; ++i) {
        for (int j = 0; j < K; ++j) catalogo[i][j] = precio(rng);
        sort(catalogo[i].begin(), catalogo[i].end(), greater<int>());
    }
}

void genAdversarioSinPoda(int C, int K, mt19937& rng) {
    uniform_int_distribution<int> precio(1, 50);
    numPrendas = C;
    presupuesto = 200 * C;
    catalogo.assign(numPrendas, vector<int>(K));
    for (int i = 0; i < numPrendas; ++i) {
        for (int j = 0; j < K; ++j) catalogo[i][j] = precio(rng);
        sort(catalogo[i].begin(), catalogo[i].end(), greater<int>());
    }
}


void genAjustado(int C, int K, mt19937& rng) {
    uniform_int_distribution<int> precio(50, 200);
    numPrendas = C;
    catalogo.assign(numPrendas, vector<int>(K));
    int suma_max = 0;
    for (int i = 0; i < numPrendas; ++i) {
        for (int j = 0; j < K; ++j) catalogo[i][j] = precio(rng);
        sort(catalogo[i].begin(), catalogo[i].end(), greater<int>());
        suma_max += catalogo[i].front();
    }
    presupuesto = (int)(suma_max * 0.85);
}

void generar(int nivel, vector<int>& s) { s[nivel]++; }
void retroceder(int& nivel, vector<int>& s) { s[nivel] = -1; nivel--; }

bool solucion(int nivel, int sumaNivel) {
    return nivel == numPrendas - 1 && sumaNivel <= presupuesto;
}

void actualizarSuma(int nivel, const vector<int>& s, vector<int>& suma) {
    int prev = (nivel > 0) ? suma[nivel - 1] : 0;
    suma[nivel] = prev + catalogo[nivel][s[nivel]];
}

void vectoresAuxiliares() {
    minimos.assign(numPrendas + 1, 0);
    maximos.assign(numPrendas + 1, 0);
    for (int i = numPrendas - 1; i >= 0; --i) {
        minimos[i] = minimos[i + 1] + catalogo[i].back();
        maximos[i] = maximos[i + 1] + catalogo[i].front();
    }
}


bool criterio_sin_poda(int nivel, int sumaNivel, int) {
    if (sumaNivel > presupuesto) return false;
    if (nivel >= numPrendas - 1) return false;
    return true;
}

int backtracking_sin_poda() {
    vector<int> s(numPrendas, -1);
    vector<int> suma(numPrendas, 0);
    int nivel = 0, opt = -1;
    while (nivel != -1) {
        generar(nivel, s);
        if (s[nivel] >= (int)catalogo[nivel].size()) { retroceder(nivel, s); continue; }
        actualizarSuma(nivel, s, suma);
        if (suma[nivel] > presupuesto) continue;
        if (solucion(nivel, suma[nivel])) {
            if (suma[nivel] > opt) opt = suma[nivel];
            continue;
        }
        if (criterio_sin_poda(nivel, suma[nivel], opt)) {
            nivel++;
            s[nivel] = -1;
        }
    }
    return opt;
}


bool criterio_con_poda(int nivel, int sumaNivel, int opt) {
    if (sumaNivel > presupuesto) return false;
    if (nivel >= numPrendas - 1) return false;
    if (sumaNivel + minimos[nivel + 1] > presupuesto) return false;
    if (sumaNivel + maximos[nivel + 1] <= opt) return false;
    if (visitado[nivel][sumaNivel]) return false;
    return true;
}

int backtracking_con_poda() {
    vector<int> s(numPrendas, -1);
    vector<int> suma(numPrendas, 0);
    visitado.assign(numPrendas, vector<bool>(presupuesto + 1, false));
    int nivel = 0, opt = -1;
    while (nivel != -1) {
        generar(nivel, s);
        if (s[nivel] >= (int)catalogo[nivel].size()) { retroceder(nivel, s); continue; }
        actualizarSuma(nivel, s, suma);
        if (suma[nivel] > presupuesto) continue;
        if (solucion(nivel, suma[nivel])) {
            if (suma[nivel] > opt) opt = suma[nivel];
            continue;
        }
        if (criterio_con_poda(nivel, suma[nivel], opt)) {
            visitado[nivel][suma[nivel]] = true;
            nivel++;
            s[nivel] = -1;
        }
    }
    return opt;
}

double mediana(vector<double> v) {
    if (v.empty()) return 0.0;
    sort(v.begin(), v.end());
    int n = (int)v.size();
    return (n % 2 == 0) ? (v[n/2 - 1] + v[n/2]) / 2.0 : v[n/2];
}

double cronometrar(int (*f)(), bool con_aux) {
    if (con_aux) vectoresAuxiliares();
    auto ini = chrono::high_resolution_clock::now();
    f();
    auto fin = chrono::high_resolution_clock::now();
    return chrono::duration<double, milli>(fin - ini).count();
}

int main() {
    mt19937 rng((unsigned)time(NULL));

    {
        FILE* f = freopen("resultados_tiempos_sinpoda.csv", "w", stdout);
        (void)f;
        cout << "C, Tiempo Sin Poda (ms)\n";

        int K = 2;
        int reps = 51;
        for (int C = 2; C <= 28; C += 2) {
            vector<double> tiempos;
            tiempos.reserve(reps);
            for (int r = 0; r < reps; ++r) {
                int tipo = r % 2;
                if (tipo == 0) genUniforme(C, K, rng);
                else genAdversarioSinPoda(C, K, rng);
                tiempos.push_back(cronometrar(backtracking_sin_poda, false));
            }
            cout << fixed << setprecision(6)
                 << C << ", " << mediana(tiempos) << "\n";
            cout.flush();
        }
    }

    {
        FILE* f = freopen("resultados_tiempos_conpoda.csv", "w", stdout);
        (void)f;
        cout << "C, Tiempo Con Poda (ms)\n";

        int K = 20;
        int reps = 201;

        for (int C : {5, 10, 15, 20, 25, 30, 35, 40, 50, 60,
                      70, 80, 90, 100, 120, 140, 160, 180, 200}) {
            vector<double> tiempos;
            tiempos.reserve(reps);
            for (int r = 0; r < reps; ++r) {
                genAjustado(C, K, rng);
                tiempos.push_back(cronometrar(backtracking_con_poda, true));
            }
            cout << fixed << setprecision(6)
                 << C << ", " << mediana(tiempos) << "\n";
            cout.flush();
        }
    }

    FILE* f = freopen("CON", "w", stdout);
    (void)f;
    fprintf(stderr,
        "Generados resultados_tiempos_sinpoda.csv y resultados_tiempos_conpoda.csv\n");
    return 0;
}
