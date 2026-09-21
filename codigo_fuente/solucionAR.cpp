#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Estructura para representar un candidato (pupitre)
struct Pareja {
    int u;
    int v;
    int beneficio;
};

// Criterio para ordenar la lista de candidatos de mayor a menor beneficio
bool compararParejas(const Pareja& a, const Pareja& b) {
    if (a.beneficio != b.beneficio) {
        return a.beneficio > b.beneficio;
    }
    // Desempate por índices para consistencia
    if (a.u != b.u) return a.u < b.u;
    return a.v < b.v;
}

//Calcula el beneficio y devuelve la lista ordenada
vector<Pareja> generarCandidatos(const vector<vector<int>>& Amistad, const vector<vector<int>>& Trabajo, int N) {
    vector<Pareja> C;
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            int b = (Amistad[i][j] + Amistad[j][i]) * (Trabajo[i][j] + Trabajo[j][i]);
            C.push_back({i, j, b});
        }
    }
    sort(C.begin(), C.end(), compararParejas);
    return C;
}

//Finaliza si ya se sentaron todos o queda uno
bool Solucion(const vector<Pareja>& S, int N) {
    return static_cast<int>(2 * S.size()) >= (N - 1);
}

//Extrae el elemento más prometedor de C
Pareja seleccionar(const vector<Pareja>& C, int idx) {
    return C[idx];
}

//Comprueba que ambos alumnos no estén asignados
bool factible(const vector<bool>& asignado, const Pareja& x) {
    return !asignado[x.u] && !asignado[x.v];
}

//Añade la pareja a la solución
void insertar(vector<Pareja>& S, const Pareja& x) {
    S.push_back(x);
}

// Estructura auxiliar para devolver el resultado y el beneficio
struct ResultadoVoraz {
    vector<Pareja> S;
    int beneficioTotal;
    bool haySolucion;
};

ResultadoVoraz voraz(const vector<vector<int>>& Amistad, const vector<vector<int>>& Trabajo, int N) {
    vector<Pareja> S; // S <- vacio
    vector<bool> asignado(N, false); // asignado <- array de N booleanos a FALSO
    vector<Pareja> C = generarCandidatos(Amistad, Trabajo, N); // C <- generarCandidatos(...)
    
    int beneficioTotal = 0;
    int idx = 0; // Índice lógico para evitar el coste del borrado

    while (idx < static_cast<int>(C.size()) && !Solucion(S, N)) {
        Pareja x = seleccionar(C, idx); // x <- seleccionar(C)
        
        idx++; // Simulamos C <- C - {x} avanzando el índice
        
        if (factible(asignado, x)) {
            insertar(S, x);
            asignado[x.u] = true;
            asignado[x.v] = true;
            beneficioTotal += x.beneficio;
        }
    }
    
    // if (S es vacio) "No hay solucion"
    if (S.empty() && N > 1) {
        return {S, 0, false};
    }
    
    return {S, beneficioTotal, true};
}

void procesarCaso() {
    int N;
    cin >> N;

    vector<vector<int>> Amistad(N, vector<int>(N, 0));
    vector<vector<int>> Trabajo(N, vector<int>(N, 0));

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (i != j) {
                cin >> Amistad[i][j];
            }
        }
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (i != j) {
                cin >> Trabajo[i][j];
            }
        }
    }

    ResultadoVoraz res = voraz(Amistad, Trabajo, N);

    if (!res.haySolucion) {
        cout << "No hay solucion\n";
        return;
    }

    cout << res.beneficioTotal << "\n";
    

    vector<bool> finalAsignado(N, false);
    for (const auto& p : res.S) {
        cout << p.u << " " << p.v << " ";
        finalAsignado[p.u] = true;
        finalAsignado[p.v] = true;
    }
    
    // Imprimir el alumno sobrante (si lo hay)
    for (int i = 0; i < N; ++i) {
        if (!finalAsignado[i]) {
            cout << i << " ";
        }
    }
    cout << "\n";
}

int main() {
    int casos;
    cin >> casos;
    while (casos--) {
        procesarCaso();
    }
    return 0;
}