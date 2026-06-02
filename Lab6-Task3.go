package main

import (
	"fmt"
	"math"
)

// Коэффициенты варианта
const (
	M   = 1.09
	N   = -0.16
	P   = 0.84
	EPS = 0.001 // Точность epsilon
)

func main() {
	// Текущие приближения
	x := []float64{0.0, 0.0, 0.0, 0.0}
	nextX := make([]float64, 4)

	iteration := 0

	fmt.Printf("Точность epsilon = %v\n\n", EPS)
	fmt.Printf("%3s | %10s | %10s | %10s | %10s | %10s\n", "N", "X1", "X2", "X3", "X4", "En")
	fmt.Println("--------------------------------------------------------------")

	for {
		// Формулы итерации
		nextX[0] = (-1.24 + 0.04*x[1] - 0.21*x[2] + 1.16*x[3]) / M
		nextX[1] = (P - 0.25*x[0] + 0.16*x[2] + 0.09*x[3]) / -1.23
		nextX[2] = (2.56 + 0.21*x[0] + 0.16*x[1] + 0.13*x[3]) / 0.8
		nextX[3] = (M - 0.15*x[0] + 1.31*x[1] - 0.06*x[2]) / P

		// Вычисляем погрешность En = max|x_new - x_old|
		En := 0.0
		for i := 0; i < 4; i++ {
			En = math.Max(En, math.Abs(nextX[i]-x[i]))
		}

		// Вывод строки таблицы
		// %3d - ширина 3, %10.5f - ширина 10, точность 5 знаков
		fmt.Printf("%3d | %10.5f | %10.5f | %10.5f | %10.5f | %10.5f\n",
			iteration, nextX[0], nextX[1], nextX[2], nextX[3], En)

		// Копируем nextX в x (в Go нельзя просто написать x = nextX, так как это ссылка)
		copy(x, nextX)
		iteration++

		if iteration > 1000 {
			fmt.Println("Метод не сходится (превышен лимит итераций).")
			break
		}

		if En <= EPS {
			break
		}
	}

	fmt.Println("--------------------------------------------------------------")
	fmt.Printf("Решение найдено за %d итераций.\n\n", iteration)

	fmt.Println("--- Результаты решения ---")
	fmt.Printf("x1 = %.6f\n", x[0])
	fmt.Printf("x2 = %.6f\n", x[1])
	fmt.Printf("x3 = %.6f\n", x[2])
	fmt.Printf("x4 = %.6f\n", x[3])
}
