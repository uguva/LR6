#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_set>

using namespace std;

// Функция возвращает минимальное количество ходов или -1, если решения нет
int getMinFlipsToCheckerboard(int N, int M, const string& start_state) {
    string target1 = "", target2 = "";
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            target1 += ((i + j) % 2 == 0) ? '0' : '1';
            target2 += ((i + j) % 2 == 0) ? '1' : '0';
        }
    }

    queue<pair<string, int>> q;
    unordered_set<string> visited;

    q.push({start_state, 0});
    visited.insert(start_state);

    auto flip = [](char& c) { c = (c == '0') ? '1' : '0'; };

    while (!q.empty()) {
        auto current = q.front();
        string state = current.first;
        int steps = current.second;
        q.pop();

        if (state == target1 || state == target2) return steps;

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < M; ++j) {
                int idx = i * M + j;

                // Переворот по горизонтали (соседние монеты в строке)
                if (j + 1 < M) {
                    string next_state = state;
                    flip(next_state[idx]);
                    flip(next_state[idx + 1]);
                    if (visited.find(next_state) == visited.end()) {
                        visited.insert(next_state);
                        q.push({next_state, steps + 1});
                    }
                }
                // Переворот по вертикали (соседние монеты в столбце)
                if (i + 1 < N) {
                    string next_state = state;
                    flip(next_state[idx]);
                    flip(next_state[idx + M]);
                    if (visited.find(next_state) == visited.end()) {
                        visited.insert(next_state);
                        q.push({next_state, steps + 1});
                    }
                }
            }
        }
    }
    return -1;
}

int main() {
    setlocale(LC_ALL, "Russian");

    int N, M;
    cout << "=== Задача №4: Доска с монетами ===\n";
    cout << "Введите сначала количество строк (N), затем количество столбцов (M) через пробел: ";
    cin >> N >> M;

    string start_state = "";
    cout << "\nВведите начальное состояние доски построчно.\n";
    cout << "Используйте '0' для орла и '1' для решки.\n";
    cout << "Каждая строка должна содержать ровно " << M << " символов (например, ";
    for(int j=0; j<M; ++j) cout << (j % 2 == 0 ? "1" : "0");
    cout << "):\n\n";

    for (int i = 0; i < N; ++i) {
        string row_input;
        cout << "Строка " << i + 1 << " из " << N << ": ";
        cin >> row_input;

        while (row_input.length() != static_cast<size_t>(M)) {
            cout << "[ОШИБКА] Длина строки должна быть равна " << M << "! Повторите ввод строки " << i + 1 << ": ";
            cin >> row_input;
        }
        start_state += row_input; // Склеиваем строки в один сплошной массив данных
    }

    int result = getMinFlipsToCheckerboard(N, M, start_state);
    
    cout << "--------------------------------------------------\n";
    if (result != -1) {
        cout << "Результат: минимальное число ходов для сборки шахматного порядка = " << result << endl;
    } else {
        cout << "Результат: из данного состояния получить шахматный порядок невозможно (-1)." << endl;
    }
    cout << "--------------------------------------------------\n";

    return 0;
}