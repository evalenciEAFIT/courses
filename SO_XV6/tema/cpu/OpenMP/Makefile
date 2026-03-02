CC = gcc
CFLAGS = -Wall -O2 
OMPFLAGS = -fopenmp

all: matrix_seq matrix_omp big_mult_seq big_mult_omp

matrix_seq: matrix_seq.c
	$(CC) $(CFLAGS) -o matrix_seq matrix_seq.c

matrix_omp: matrix_omp.c
	$(CC) $(CFLAGS) $(OMPFLAGS) -o matrix_omp matrix_omp.c

big_mult_seq: big_mult_seq.c
	$(CC) $(CFLAGS) -o big_mult_seq big_mult_seq.c

big_mult_omp: big_mult_omp.c
	$(CC) $(CFLAGS) $(OMPFLAGS) -o big_mult_omp big_mult_omp.c

run: all
	@echo "======================================"
	@echo " Ejecutando Matrices: Secuencial vs OpenMP"
	@echo "======================================"
	./matrix_seq
	@echo ""
	./matrix_omp
	@echo ""
	@echo "======================================"
	@echo " Ejecutando BigNums (80 Digitos): Secuencial vs OpenMP"
	@echo "======================================"
	./big_mult_seq
	@echo ""
	./big_mult_omp

clean:
	rm -f matrix_seq matrix_omp big_mult_seq big_mult_omp
