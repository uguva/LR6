#define NOMINMAX
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <windows.h>

using namespace std;

// Коэффициенты
const double M = 1.09;
const double N = -0.16;
const double P = 0.84;
const double EPS = 0.001;

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    // Текущие приближения
    vector<double> x = {0.0, 0.0, 0.0, 0.0};
    vector<double> next_x(4);
    
    int iteration = 0;
    double En;

    cout << "Точность epsilon = " << EPS << "\n\n";
    cout << setw(3) << "N" << " | " 
              << setw(10) << "X1" << " | " 
              << setw(10) << "X2" << " | " 
              << setw(10) << "X3" << " | " 
              << setw(10) << "X4" << " | " 
              << setw(10) << "En" << "\n";
    cout << "--------------------------------------------------------------\n";

    do {
        // Формулы итерации (выражаем xi из i-го уравнения)
        next_x[0] = (-1.24 + 0.04*x[1] - 0.21*x[2] + 1.16*x[3]) / M;
        next_x[1] = (P - 0.25*x[0] + 0.16*x[2] + 0.09*x[3]) / -1.23;
        next_x[2] = (2.56 + 0.21*x[0] + 0.16*x[1] + 0.13*x[3]) / 0.8;
        next_x[3] = (M - 0.15*x[0] + 1.31*x[1] - 0.06*x[2]) / P;

        // Вычисляем погрешность En = max|x_new - x_old|
        En = 0;
        for (int i = 0; i < 4; ++i) {
            En = max(En, abs(next_x[i] - x[i]));
        }

        // Вывод строки таблицы
        cout << setw(3) << iteration << " | " 
                  << setw(10) << fixed << setprecision(5) << next_x[0] << " | " 
                  << setw(10) << next_x[1] << " | " 
                  << setw(10) << next_x[2] << " | " 
                  << setw(10) << next_x[3] << " | " 
                  << setw(10) << En << "\n";

        x = next_x;
        iteration++;

        if (iteration > 1000) {
            cout << "Метод не сходится (превышен лимит итераций).\n";
            break;
        }

    } while (En > EPS);

    cout << "--------------------------------------------------------------\n";
    cout << "Решение найдено за " << iteration << " итераций.\n\n";

    cout << "--- Результаты решения ---\n";
    cout << "x1 = " << fixed << setprecision(6) << x[0] << "\n";
    cout << "x2 = " << fixed << setprecision(6) << x[1] << "\n";
    cout << "x3 = " << fixed << setprecision(6) << x[2] << "\n";
    cout << "x4 = " << fixed << setprecision(6) << x[3] << "\n";

    return 0;
}