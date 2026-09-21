#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <random>

using namespace std;

struct Pareja {
    int u;
    int v;
    int beneficio;
};

bool compararParejas(const Pareja& a, const Pareja& b) {
    if (a.beneficio != b.beneficio) return a.beneficio > b.beneficio;
    if (a.u != b.u) return a.u < b.u;
    return a.v < b.v;
}

vector<Pareja> generarCandidatos(const vector<vector<int>>& A,
                                 const vector<vector<int>>& T, int N) {
    vector<Pareja> C;
    C.reserve((long long)N * (N - 1) / 2);
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            int b = (A[i][j] + A[j][i]) * (T[i][j] + T[j][i]);
            C.push_back({i, j, b});
        }
    }
    sort(C.begin(), C.end(), compararParejas);
    return C;
}

bool Solucion(const vector<Pareja>& S, int N) {
    return static_cast<int>(2 * S.size()) >= (N - 1);
}

int voraz(const vector<vector<int>>& A, const vector<vector<int>>& T, int N) {
    vector<Pareja> S;
    vector<bool> asignado(N, false);
    vector<Pareja> C = generarCandidatos(A, T, N);
    int beneficioTotal = 0;
    int idx = 0;
    while (idx < static_cast<int>(C.size()) && !Solucion(S, N)) {
        Pareja x = C[idx++];
        if (!asignado[x.u] && !asignado[x.v]) {
            S.push_back(x);
            asignado[x.u] = true;
            asignado[x.v] = true;
            beneficioTotal += x.beneficio;
        }
    }
    return beneficioTotal;
}

void genUniforme(int N, vector<vector<int>>& A, vector<vector<int>>& T,
                 mt19937& rng) {
    uniform_int_distribution<int> d(1, 9);
    A.assign(N, vector<int>(N, 0));
    T.assign(N, vector<int>(N, 0));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (i != j) { A[i][j] = d(rng); T[i][j] = d(rng); }
}

void genAdversario(int N, vector<vector<int>>& A, vector<vector<int>>& T,
                   mt19937& rng) {
    uniform_int_distribution<int> d(0, 1);
    A.assign(N, vector<int>(N, 0));
    T.assign(N, vector<int>(N, 0));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (i != j) {
                A[i][j] = d(rng) ? 9 : 1;
                T[i][j] = d(rng) ? 9 : 1;
            }
}

void genCorrelacionado(int N, vector<vector<int>>& A, vector<vector<int>>& T,
                       mt19937& rng) {
    uniform_int_distribution<int> d(1, 9);
    uniform_int_distribution<int> dr(-1, 1);
    A.assign(N, vector<int>(N, 0));
    T.assign(N, vector<int>(N, 0));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (i != j) {
                int v = d(rng);
                A[i][j] = v;
                int t = v + dr(rng);
                T[i][j] = max(1, min(9, t));
            }
}

double mediana(vector<double> v) {
    if (v.empty()) return 0.0;
    sort(v.begin(), v.end());
    int n = (int)v.size();
    return (n % 2 == 0) ? (v[n/2 - 1] + v[n/2]) / 2.0 : v[n/2];
}

int main() {
    mt19937 rng((unsigned)time(NULL));

    cout << "N, Tiempo (ms)\n";

    vector<int> tamanos = {
        4, 6, 8, 12, 16, 24, 32, 48, 64, 96,
        128, 192, 256, 384, 512, 768, 1024, 1536, 2048
    };
    int repeticiones = 51;

    for (int N : tamanos) {
        vector<double> tiempos;
        tiempos.reserve(repeticiones);

        for (int r = 0; r < repeticiones; ++r) {
            vector<vector<int>> A, T;
            int tipo = r % 3;
            if (tipo == 0) genUniforme(N, A, T, rng);
            else if (tipo == 1) genAdversario(N, A, T, rng);
            else genCorrelacionado(N, A, T, rng);

            auto ini = chrono::high_resolution_clock::now();
            voraz(A, T, N);
            auto fin = chrono::high_resolution_clock::now();

            double t = chrono::duration<double, milli>(fin - ini).count();
            tiempos.push_back(t);
        }
        double m = mediana(tiempos);
        cout << fixed << setprecision(6) << N << ", " << m << "\n";
        cout.flush();
    }

    return 0;
}
