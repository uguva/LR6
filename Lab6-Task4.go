package main

import (
	"fmt"
)

type State struct {
	board string
	steps int
}

// Функция возвращает минимальное количество ходов или -1, если решения нет
func getMinFlipsToCheckerboard(N, M int, startState string) int {
	target1 := ""
	target2 := ""
	for i := 0; i < N; i++ {
		for j := 0; j < M; j++ {
			if (i+j)%2 == 0 {
				target1 += "0"
				target2 += "1"
			} else {
				target1 += "1"
				target2 += "0"
			}
		}
	}

	queue := []State{{board: startState, steps: 0}}
	visited := make(map[string]bool)
	visited[startState] = true

	flip := func(b byte) byte {
		if b == '0' {
			return '1'
		}
		return '0'
	}

	for len(queue) > 0 {
		curr := queue[0]
		queue = queue[1:]

		if curr.board == target1 || curr.board == target2 {
			return curr.steps
		}

		for i := 0; i < N; i++ {
			for j := 0; j < M; j++ {
				idx := i*M + j

				// Переворот по горизонтали (соседние в строке)
				if j+1 < M {
					bytes := []byte(curr.board)
					bytes[idx] = flip(bytes[idx])
					bytes[idx+1] = flip(bytes[idx+1])
					nextState := string(bytes)
					if !visited[nextState] {
						visited[nextState] = true
						queue = append(queue, State{board: nextState, steps: curr.steps + 1})
					}
				}

				// Переворот по вертикали (соседние в столбце)
				if i+1 < N {
					bytes := []byte(curr.board)
					bytes[idx] = flip(bytes[idx])
					bytes[idx+M] = flip(bytes[idx+M])
					nextState := string(bytes)
					if !visited[nextState] {
						visited[nextState] = true
						queue = append(queue, State{board: nextState, steps: curr.steps + 1})
					}
				}
			}
		}
	}
	return -1
}

func main() {
	var N, M int
	fmt.Println("=== Задача №4: Доска с монетами (Go) ===")
	fmt.Print("Введите сначала количество строк (N), затем количество столбцов (M) через пробел: ")
	_, err := fmt.Scan(&N, &M)
	if err != nil {
		return
	}

	startState := ""
	fmt.Printf("\nВведите начальное состояние доски построчно.\n")
	fmt.Printf("Используйте '0' для орла и '1' для решки.\n")
	fmt.Printf("Каждая строка должна содержать ровно %d символов:\n\n", M)

	for i := 0; i < N; i++ {
		var rowInput string
		fmt.Printf("Строка %d из %d: ", i+1, N)
		fmt.Scan(&rowInput)

		// Валидация длины
		for len(rowInput) != M {
			fmt.Printf("[ОШИБКА] Длина строки должна быть %d! Повторите ввод строки %d: ", M, i+1)
			fmt.Scan(&rowInput)
		}
		startState += rowInput
	}

	result := getMinFlipsToCheckerboard(N, M, startState)

	fmt.Println("--------------------------------------------------")
	if result != -1 {
		fmt.Printf("Результат: минимальное число ходов для сборки шахматного порядка = %d\n", result)
	} else {
		fmt.Println("Результат: из данного состояния получить шахматный порядок невозможно (-1).")
	}
	fmt.Println("--------------------------------------------------")
}
