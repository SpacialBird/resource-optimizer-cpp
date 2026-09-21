#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

struct Pareja {
    int u, v, beneficio;
};

bool comparar(const Pareja& a, const Pareja& b) {
    if (a.beneficio != b.beneficio) return a.beneficio > b.beneficio;
    if (a.u != b.u) return a.u < b.u;
    return a.v < b.v;
}

vector<Pareja> generarCandidatos(const vector<vector<int>>& A,
                                 const vector<vector<int>>& T, int N) {
    vector<Pareja> C;
    for (int i = 0; i < N; ++i)
        for (int j = i + 1; j < N; ++j)
            C.push_back({i, j, (A[i][j] + A[j][i]) * (T[i][j] + T[j][i])});
    sort(C.begin(), C.end(), comparar);
    return C;
}

bool Solucion(const vector<Pareja>& S, int N) {
    return static_cast<int>(2 * S.size()) >= (N - 1);
}

struct Resultado {
    vector<Pareja> S;
    int beneficio;
};

Resultado voraz(const vector<vector<int>>& A,
                const vector<vector<int>>& T, int N) {
    vector<Pareja> S;
    vector<bool> asignado(N, false);
    vector<Pareja> C = generarCandidatos(A, T, N);
    int total = 0, idx = 0;
    while (idx < (int)C.size() && !Solucion(S, N)) {
        Pareja x = C[idx++];
        if (!asignado[x.u] && !asignado[x.v]) {
            S.push_back(x);
            asignado[x.u] = asignado[x.v] = true;
            total += x.beneficio;
        }
    }
    return {S, total};
}

void test_ejemplo_enunciado() {
    vector<vector<int>> A = {{0,5,6},{4,0,3},{2,1,0}};
    vector<vector<int>> T = {{0,5,3},{3,0,2},{1,5,0}};
    auto r = voraz(A, T, 3);
    assert(r.beneficio == 72);
    assert(r.S.size() == 1);
    assert(r.S[0].u == 0 && r.S[0].v == 1);
    cout << "[OK] test_ejemplo_enunciado\n";
}

void test_n2_unica_pareja() {
    vector<vector<int>> A = {{0,3},{2,0}};
    vector<vector<int>> T = {{0,4},{1,0}};
    auto r = voraz(A, T, 2);
    assert(r.beneficio == 25);
    cout << "[OK] test_n2_unica_pareja\n";
}

void test_n4_dos_parejas() {
    vector<vector<int>> A = {
        {0,9,1,1},
        {9,0,1,1},
        {1,1,0,9},
        {1,1,9,0}
    };
    vector<vector<int>> T = A;  // Misma estructura
    auto r = voraz(A, T, 4);
    assert(r.S.size() == 2);
    // Beneficio = (18*18) + (18*18) = 648
    assert(r.beneficio == 648);
    cout << "[OK] test_n4_dos_parejas\n";
}

void test_no_repeticion() {
    int N = 8;
    vector<vector<int>> A(N, vector<int>(N, 0));
    vector<vector<int>> T(N, vector<int>(N, 0));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (i != j) { A[i][j] = (i + j) % 9 + 1; T[i][j] = (i*j) % 9 + 1; }
    auto r = voraz(A, T, N);
    vector<bool> visto(N, false);
    for (auto& p : r.S) {
        assert(!visto[p.u] && !visto[p.v]);
        visto[p.u] = visto[p.v] = true;
    }
    cout << "[OK] test_no_repeticion\n";
}

int main() {
    test_ejemplo_enunciado();
    test_n2_unica_pareja();
    test_n4_dos_parejas();
    test_no_repeticion();
    cout << "Todos los tests del AR superados.\n";
    return 0;
}
