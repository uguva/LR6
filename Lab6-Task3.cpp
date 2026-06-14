#define NOMINMAX
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <windows.h>

using namespace std;

const double EPS = 0.001;

// 1. ПРЯМОЙ МЕТОД: Метод Гаусса с выбором главного элемента
vector<double> solveGauss(vector<vector<double>> A, vector<double> B) {
    int n = A.size();
    
    // Прямой ход
    for (int i = 0; i < n; i++) {
        // Поиск максимального элемента в столбце для устойчивости
        int maxRow = i;
        for (int k = i + 1; k < n; k++) {
            if (abs(A[k][i]) > abs(A[maxRow][i])) {
                maxRow = k;
            }
        }
        // Меняем строки местами
        swap(A[i], A[maxRow]);
        swap(B[i], B[maxRow]);

        // Обнуление элементов под главной диагональю
        for (int k = i + 1; k < n; k++) {
            double factor = A[k][i] / A[i][i];
            for (int j = i; j < n; j++) {
                A[k][j] -= factor * A[i][j];
            }
            B[k] -= factor * B[i];
        }
    }

    // Обратный ход
    vector<double> x(n);
    for (int i = n - 1; i >= 0; i--) {
        x[i] = B[i];
        for (int j = i + 1; j < n; j++) {
            x[i] -= A[i][j] * x[j];
        }
        x[i] /= A[i][i];
    }
    return x;
}

// 2. ИТЕРАЦИОННЫЙ МЕТОД: Метод Якоби (Автоматическое вычисление коэффициентов)
vector<double> solveJacobi(const vector<vector<double>>& A, const vector<double>& B, double eps) {
    int n = A.size();
    vector<double> x(n, 0.0);
    vector<double> next_x(n, 0.0);
    int iteration = 0;
    double error;

    cout << setw(3) << "N" << " | " 
         << setw(10) << "X1" << " | " 
         << setw(10) << "X2" << " | " 
         << setw(10) << "X3" << " | " 
         << setw(10) << "X4" << " | " 
         << setw(10) << "En" << "\n";
    cout << "--------------------------------------------------------------\n";

    do {
        // Программа САМА вычисляет x_i через матрицу A и вектор B
        for (int i = 0; i < n; i++) {
            double sum = 0.0;
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    sum += A[i][j] * x[j];
                }
            }
            next_x[i] = (B[i] - sum) / A[i][i];
        }

        // Оценка погрешности
        error = 0.0;
        for (int i = 0; i < n; i++) {
            error = max(error, abs(next_x[i] - x[i]));
        }

        cout << setw(3) << iteration << " | " 
             << setw(10) << fixed << setprecision(5) << next_x[0] << " | " 
             << setw(10) << next_x[1] << " | " 
             << setw(10) << next_x[2] << " | " 
             << setw(10) << next_x[3] << " | " 
             << setw(10) << error << "\n";

        x = next_x;
        iteration++;

        if (iteration > 1000) {
            cout << "Метод не сходится (превышен лимит итераций).\n";
            break;
        }

    } while (error > eps);

    cout << "--------------------------------------------------------------\n";
    cout << "Якоби: Решение найдено за " << iteration << " итераций.\n\n";
    return x;
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    // Восстановленная матрица A и вектор B из ваших уравнений
    // M = 1.09, P = 0.84
    vector<vector<double>> A = {
        { 1.09, -0.04,  0.21, -1.16},
        { 0.25, -1.23, -0.16, -0.09},
        {-0.21, -0.16,  0.80, -0.13},
        { 0.15, -1.31,  0.06,  0.84}
    };
    
    vector<double> B = {-1.24, 0.84, 2.56, 1.09};

    cout << "=== ПРЯМОЙ МЕТОД (Метод Гаусса) ===\n";
    vector<double> gauss_result = solveGauss(A, B);
    for (size_t i = 0; i < gauss_result.size(); i++) {
        cout << "x" << i + 1 << " = " << fixed << setprecision(6) << gauss_result[i] << "\n";
    }
    cout << "\n";

    cout << "=== ИТЕРАЦИОННЫЙ МЕТОД (Метод Якоби, EPS = " << EPS << ") ===\n";
    vector<double> jacobi_result = solveJacobi(A, B, EPS);
    for (size_t i = 0; i < jacobi_result.size(); i++) {
        cout << "x" << i + 1 << " = " << fixed << setprecision(6) << jacobi_result[i] << "\n";
    }

    return 0;
}
