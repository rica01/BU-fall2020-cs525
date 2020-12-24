debug:
	gcc -g serial_bfs.c util.c -o a.out
	mpicc -g parallel_bfs.c util.c -o b.out

build:
	gcc serial_bfs.c util.c -o serial_bfs
	mpicc parallel_bfs.c util.c -o parallel_bfs

clean:
	rm -rf serial_bfs parallel_bfs a.out b.out

run:
	./serial_bfs data.txt 0 out.txt
	cat out.txt
	mpirun -np 4 ./parallel_bfs data.txt 0 1 out.txt
