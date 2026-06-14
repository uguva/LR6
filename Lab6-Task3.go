package main

import (
	"fmt"
	"math"
)

// 1. ПРЯМОЙ МЕТОД: Метод Гаусса
func solveGauss(A [][]float64, B []float64) []float64 {
	n := len(B)
	
	// Копируем срезы, чтобы не испортить исходную матрицу
	aCopy := make([][]float64, n)
	bCopy := make([]float64, n)
	for i := range A {
		aCopy[i] = make([]float64, n)
		copy(aCopy[i], A[i])
		bCopy[i] = B[i]
	}

	// Прямой ход
	for i := 0; i < n; i++ {
		maxRow := i
		for k := i + 1; k < n; k++ {
			if math.Abs(aCopy[k][i]) > math.Abs(aCopy[maxRow][i]) {
				maxRow = k
			}
		}
		aCopy[i], aCopy[maxRow] = aCopy[maxRow], aCopy[i]
		bCopy[i], bCopy[maxRow] = bCopy[maxRow], bCopy[i]

		for k := i + 1; k < n; k++ {
			factor := aCopy[k][i] / aCopy[i][i]
			for j := i; j < n; j++ {
				aCopy[k][j] -= factor * aCopy[i][j]
			}
			bCopy[k] -= factor * bCopy[i]
		}
	}

	// Обратный ход
	x := make([]float64, n)
	for i := n - 1; i >= 0; i-- {
		x[i] = bCopy[i]
		for j := i + 1; j < n; j++ {
			x[i] -= aCopy[i][j] * x[j]
		}
		x[i] /= aCopy[i][i]
	}
	return x
}

// 2. ИТЕРАЦИОННЫЙ МЕТОД: Метод Якоби
func solveJacobi(A [][]float64, B []float64, eps float64) []float64 {
	n := len(B)
	x := make([]float64, n)
	nextX := make([]float64, n)
	iteration := 0
	maxIter := 1000
	var err float64

	fmt.Printf("%-3s | %-10s | %-10s | %-10s | %-10s | %-10s\n", "N", "X1", "X2", "X3", "X4", "En")
	fmt.Println("----------------------------------------------------------------")

	for {
		for i := 0; i < n; i++ {
			sum := 0.0
			for j := 0; j < n; j++ {
				if i != j {
					sum += A[i][j] * x[j]
				}
			}
			nextX[i] = (B[i] - sum) / A[i][i]
		}

		err = 0.0
		for i := 0; i < n; i++ {
			err = math.Max(err, math.Abs(nextX[i]-x[i]))
		}

		fmt.Printf("%-3d | %10.5f | %10.5f | %10.5f | %10.5f | %10.5f\n", iteration, nextX[0], nextX[1], nextX[2], nextX[3], err)

		for i := 0; i < n; i++ {
			x[i] = nextX[i]
		}
		iteration++

		if iteration > maxIter {
			fmt.Println("Метод не сходится (превышен лимит итераций).")
			break
		}
		if err <= eps {
			break
		}
	}

	fmt.Println("----------------------------------------------------------------")
	fmt.Printf("Якоби: Решение найдено за %d итераций.\n\n", iteration)
	return x
}

func main() {
	// Матрица A и вектор B
	A := [][]float64{
		{1.09, -0.04, 0.21, -1.16},
		{0.25, -1.23, -0.16, -0.09},
		{-0.21, -0.16, 0.80, -0.13},
		{0.15, -1.31, 0.06, 0.84},
	}
	B := []float64{-1.24, 0.84, 2.56, 1.09}
	eps := 0.001

	fmt.Println("=== ПРЯМОЙ МЕТОД (Метод Гаусса) ===")
	gaussResult := solveGauss(A, B)
	for i, val := range gaussResult {
		fmt.Printf("x%d = %.6f\n", i+1, val)
	}
	fmt.Println()

	fmt.Printf("=== ИТЕРАЦИОННЫЙ МЕТОД (Метод Якоби, EPS = %.3f) ===\n", eps)
	jacobiResult := solveJacobi(A, B, eps)
	for i, val := range jacobiResult {
		fmt.Printf("x%d = %.6f\n", i+1, val)
	}
}
