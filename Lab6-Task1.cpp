#include <iostream>
#include <vector>
#include <random>
#include <map>
#include <algorithm>
#include <iomanip>
#include <windows.h>

using namespace std;

struct Task1Result {
    int row_index;
    int start_element;
    vector<int> sequence;
};

struct Task2Result {
    vector<pair<int, int>> sorted_freqs;
    int max_count;
    vector<int> most_frequent_elements;
};

// Логика Задачи 1
Task1Result solveTask1(const vector<vector<int>>& matrix) {
    Task1Result best_result = {-1, 200, {}};

    for (size_t i = 0; i < matrix.size(); ++i) {
        if (matrix[i].empty()) continue;

        vector<int> current_seq = {matrix[i][0]};
        vector<int> longest_in_row = current_seq;

        // Поиск самой длинной последовательности в строке i
        for (size_t j = 1; j < matrix[i].size(); ++j) {
            if (matrix[i][j] > matrix[i][j - 1]) {
                current_seq.push_back(matrix[i][j]);
            } else {
                if (current_seq.size() > longest_in_row.size()) {
                    longest_in_row = current_seq;
                }
                current_seq = {matrix[i][j]};
            }
        }
        if (current_seq.size() > longest_in_row.size()) {
            longest_in_row = current_seq;
        }

        if (best_result.sequence.empty() || 
            longest_in_row.size() > best_result.sequence.size() || 
           (longest_in_row.size() == best_result.sequence.size() && longest_in_row[0] < best_result.start_element)) {
            
            best_result.row_index = static_cast<int>(i);
            best_result.start_element = longest_in_row[0];
            best_result.sequence = longest_in_row;
        }
    }
    return best_result;
}

// Логика Задачи 2
Task2Result solveTask2(const vector<vector<int>>& matrix) {
    map<int, int> counts;
    for (const auto& row : matrix) {
        for (int val : row) {
            counts[val]++;
        }
    }

    Task2Result result;

    // Перекладываем из map в vector для сортировки
    for (auto const& [val, count] : counts) {
        result.sorted_freqs.push_back({val, count});
    }

    // Сортируем по убыванию количества вхождений
    sort(result.sorted_freqs.begin(), result.sorted_freqs.end(),
              [](const pair<int, int>& a, const pair<int, int>& b) {
                  return a.second > b.second;
              });

    // Определяем максимальное количество вхождений
    result.max_count = result.sorted_freqs[0].second;

    // Собираем все элементы с этим количеством
    for (const auto& pair : result.sorted_freqs) {
        if (pair.second == result.max_count) {
            result.most_frequent_elements.push_back(pair.first);
        } else {
            break; // Так как список отсортирован, дальше идти нет смысла
        }
    }

    return result;
}

// Логика Задачи 3
vector<pair<int, int>> solveTask3(pair<int, int> king, const vector<pair<int, int>>& queens) {
    // Карта доски: 0 - пусто, 1 - ферзь
    int board[8][8] = {0};
    for (const auto& q : queens) {
        board[q.first][q.second] = 1;
    }

    vector<pair<int, int>> attackers;
    
    // 8 направлений: {d_row, d_col}
    int directions[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

    for (const auto& dir : directions) {
        int r = king.first + dir[0];
        int c = king.second + dir[1];
        
        while (r >= 0 && r < 8 && c >= 0 && c < 8) {
            if (board[r][c] == 1) {
                attackers.push_back({r, c});
                break; // Дальше ферзь не пробивает
            }
            r += dir[0];
            c += dir[1];
        }
    }

    return attackers;
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    random_device rd;
    mt19937 gen(rd());

    int choice = -1;
    while (choice != 0) {
        cout << "\n========================================\n";
        cout << "МЕНЮ ВАРИАНТА 10\n";
        cout << "1. Задача 1 (Возрастающая последовательность)\n";
        cout << "2. Задача 2 (Повторяющиеся элементы)\n";
        cout << "3. Задача 3 (Шахматы: Король и Ферзи)\n";
        cout << "0. Выход\n";
        cout << "Выберите пункт: ";
        cin >> choice;

        if (choice == 1) {
            int M, N;
            cout << "Введите размерность M и N (строки и столбцы > 5): ";
            cin >> M >> N;

            if (M <= 5 || N <= 5) {
                cout << "Ошибка: Размерность должна быть > 5.\n";
                continue;
            }

            vector<vector<int>> matrix(M, vector<int>(N));
            uniform_int_distribution<> dis(0, 100);

            cout << "Сгенерированная матрица:\n";
            for (int i = 0; i < M; ++i) {
                for (int j = 0; j < N; ++j) {
                    matrix[i][j] = dis(gen);
                    cout << setw(4) << matrix[i][j];
                }
                cout << "\n";
            }

            Task1Result res = solveTask1(matrix);

            cout << "\n--- Результат Задачи 1 ---\n";
            cout << "Номер строки: " << res.row_index << "\n";
            cout << "Начальный элемент: " << res.start_element << "\n";
            cout << "Длина последовательности: " << res.sequence.size() << "\n";
            cout << "Динамический массив: ";
            for (int val : res.sequence) cout << val << " ";
            cout << "\n";

        } else if (choice == 2) {
            int M, N;
            cout << "Введите размерность M и N (строки и столбцы > 5): ";
            cin >> M >> N;

            if (M <= 5 || N <= 5) {
                cout << "Ошибка: Размерность должна быть > 5.\n";
                continue;
            }

            vector<vector<int>> matrix(M, vector<int>(N));
            uniform_int_distribution<> dis(-20, 20);

            cout << "Сгенерированная матрица:\n";
            for (int i = 0; i < M; ++i) {
                for (int j = 0; j < N; ++j) {
                    matrix[i][j] = dis(gen);
                    cout << setw(4) << matrix[i][j];
                }
                cout << "\n";
            }

            // Получаем данные для задачи
            Task2Result res = solveTask2(matrix);

            // --- ВЫВОД ПО ВАШЕМУ ОБРАЗЦУ ---
            cout << "\n--- Результат Задачи 2 ---\n";
            cout << "Элемент | Количество вхождений\n";
            cout << "------------------------------\n";
            
            // 1. Выводим полную таблицу всех элементов
            for (const auto& pair : res.sorted_freqs) {
                cout << setw(7) << pair.first << " | " << pair.second << "\n";
            }
            cout << "------------------------------\n";

            // 2. Выводим строку с самыми частыми элементами
            cout << "Самый частый элемент(ы): ";
            for (size_t i = 0; i < res.most_frequent_elements.size(); ++i) {
                cout << res.most_frequent_elements[i] << (i == res.most_frequent_elements.size() - 1 ? "" : ", ");
            }
            cout << " (встречается " << res.max_count << " раз(а))\n";

        } else if (choice == 3) {
            pair<int, int> king;
            cout << "Введите координаты белого короля (строка и столбец 0-7): ";
            cin >> king.first >> king.second;

            int q_count;
            cout << "Введите количество чёрных ферзей: ";
            cin >> q_count;

            vector<pair<int, int>> queens(q_count);
            for (int i = 0; i < q_count; ++i) {
                cout << "Координаты ферзя " << i + 1 << " (строка столбец): ";
                cin >> queens[i].first >> queens[i].second;
            }

            vector<pair<int, int>> attackers = solveTask3(king, queens);

            cout << "\n--- Результат Задачи 3 ---\n";
            cout << "Шахматная доска:\n";
            
            char board_draw[8][8];
            for(int r=0; r<8; ++r)
                for(int c=0; c<8; ++c)
                    board_draw[r][c] = '.';
            
            for(const auto& q : queens) board_draw[q.first][q.second] = 'Q';
            board_draw[king.first][king.second] = 'K';

            for (int r = 0; r < 8; ++r) {
                for (int c = 0; c < 8; ++c) {
                    cout << board_draw[r][c] << " ";
                }
                cout << "\n";
            }

            cout << "\nАтакующие ферзи: [";
            for (size_t i = 0; i < attackers.size(); ++i) {
                cout << "[" << attackers[i].first << "," << attackers[i].second << "]";
                if (i != attackers.size() - 1) cout << ",";
            }
            cout << "]\n";

        } else if (choice != 0) {
            cout << "Неверный выбор. Попробуйте еще раз.\n";
        }
    }

    cout << "Программа завершена.\n";
    return 0;
}