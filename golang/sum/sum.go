package main

import (
	"fmt"
	"time"
)

func sumOfNNumbers(start, stop int) int {
	sum := 0
	for i := start; i <= stop; i++ {
		sum += i
	}
	return sum
}

func write2ChanSumOfNNumbers(start, stop int, r chan int) {
	r <- sumOfNNumbers(start, stop-1)
}

func main() {
	n := 100000000
	result := n

	workers := 8
	channel := make(chan int, workers)

	start := time.Now()
	for i := 0; i < workers; i++ {
		go write2ChanSumOfNNumbers((n/workers)*i, (n/workers)*(i+1), channel)
	}

	for i := 0; i < workers; i++ {
		result += <-channel
	}

	elapsed := time.Since(start)

	fmt.Printf("Golang: Result = %d, Execution time = %v\n", result, elapsed)
}
