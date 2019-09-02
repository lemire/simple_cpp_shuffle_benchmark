benchmark: benchmark.cpp
	g++ -O3 -o benchmark benchmark.cpp -std=c++11 -Wall

clean:
	rm -r -f benchmark
