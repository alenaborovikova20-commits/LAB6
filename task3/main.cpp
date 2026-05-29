#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

using namespace std;

// LU-разложение
bool luDecomposition(const vector<vector<double>>& A, vector<vector<double>>& L, vector<vector<double>>& U) {
    int n = A.size();
    L.assign(n, vector<double>(n, 0.0));
    U.assign(n, vector<double>(n, 0.0));

    for (int i = 0; i < n; i++) {
        L[i][i] = 1.0;
        for (int j = i; j < n; j++) {
            double sum = 0.0;
            for (int k = 0; k < i; k++)
                sum += L[i][k] * U[k][j];
            U[i][j] = A[i][j] - sum;
        }
        for (int j = i + 1; j < n; j++) {
            double sum = 0.0;
            for (int k = 0; k < i; k++)
                sum += L[j][k] * U[k][i];
            L[j][i] = (A[j][i] - sum) / U[i][i];
        }
    }
    return true;
}

// Ly = b
vector<double> solveForward(const vector<vector<double>>& L, const vector<double>& b) {
    int n = L.size();
    vector<double> y(n, 0.0);
    for (int i = 0; i < n; i++) {
        double sum = 0.0;
        for (int j = 0; j < i; j++)
            sum += L[i][j] * y[j];
        y[i] = b[i] - sum;
    }
    return y;
}

// Ux = y
vector<double> solveBackward(const vector<vector<double>>& U, const vector<double>& y) {
    int n = U.size();
    vector<double> x(n, 0.0);
    for (int i = n - 1; i >= 0; i--) {
        double sum = 0.0;
        for (int j = i + 1; j < n; j++)
            sum += U[i][j] * x[j];
        x[i] = (y[i] - sum) / U[i][i];
    }
    return x;
}

void solveIteration(const vector<vector<double>>& A, const vector<double>& b, double eps) {
    int n = A.size();
    vector<double> x(n, 0.0); 
    int iter = 0;
    double diff;
    
    cout << "\nИтерационная таблица:" << endl;
    
    do {
        iter++;
        diff = 0;
        for (int i = 0; i < n; i++) {
            double oldVal = x[i];
            double sum = 0;
            
            for (int j = 0; j < n; j++) {
                if (i != j) sum += A[i][j] * x[j];  
            }
            
            x[i] = (b[i] - sum) / A[i][i];
            
            // Максимальная разница
            if (abs(x[i] - oldVal) > diff) diff = abs(x[i] - oldVal);
        }
        
        cout << " " << setw(2) << iter << "\t";
        for (int i = 0; i < n; i++) {
            cout << fixed << setprecision(4) << setw(8) << x[i] << "\t";
        }
        cout << endl;
    } while (diff > eps);
}

int main() {
    const double M = 1.08;
    const double N = 0.22;
    const double P = -1.16;
    const double EPS = 1e-3;

    vector<vector<double>> A = {
        { M,    -0.04,  0.21,   -1.16 },
        { 0.25, -1.23,  N,      -0.09 },
        { -0.21, N,     0.8,    -0.13 },
        { 0.15, -1.31,  0.06,   P    }
    };
    vector<double> b = { -1.24, P, 2.56, M };

    
    solveIteration(A, b, EPS); 
    cout << endl;  
    // LU-МЕТОД
    vector<vector<double>> L, U;
    luDecomposition(A, L, U);
    
    vector<double> y = solveForward(L, b);
    vector<double> x_lu = solveBackward(U, y);

    cout << "LU:" << endl;
    cout << fixed << setprecision(4);
    for (int i = 0; i < 4; i++) {
        cout << x_lu[i];
        if (i < 3) cout << "    ";
    }
    cout << endl;
    
    return 0;
}