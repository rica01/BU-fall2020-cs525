build:
	gcc -pg src/main.c src/image.c src/util.c src/convolution_seq.c -o convolution_seq
	gcc -fopenmp -pg src/main.c src/image.c src/util.c src/convolution_omp.c -o convolution_omp

debug:
	gcc -g src/main.c src/image.c src/util.c src/convolution_seq.c -o s.out
	gcc -fopenmp -g src/main.c src/image.c src/util.c src/convolution_omp.c -o p.out

clean:
	rm -rf convolution_seq convolution_omp s.out p.out


run:
	./convolution_seq ./images/sample_image.txt ./kernels/k0.k ./out/out.txt
	./out/out2png.py ./out/out.txt
