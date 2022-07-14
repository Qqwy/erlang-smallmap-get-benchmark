.PHONY: gccO3 gccO2 clangO3 clangO2 clean main

main: gccO3 gccO2 clangO3 clangO2

gccO3: *.c *.cpp
	gcc -c -fPIC -O3 -g -Wall -Werror implementations.c
	g++ -c -fPIC -O3 -g -Wall -Werror main.cpp
	g++ -o gccO3 implementations.o main.o
	rm *.o

gccO2: *.c *.cpp
	gcc -c -fPIC -O2 -g -Wall -Werror implementations.c
	g++ -c -fPIC -O2 -g -Wall -Werror main.cpp
	g++ -o gccO2 implementations.o main.o
	rm *.o

clangO3: *.c *.cpp
	clang -c -fPIC -O3 -g -Wall -Werror implementations.c
	clang++ -c -fPIC -O3 -g -Wall -Werror main.cpp
	clang++ -o clangO3 implementations.o main.o
	rm *.o

clangO2: *.c *.cpp
	clang -c -fPIC -O2 -g -Wall -Werror implementations.c
	clang++ -c -fPIC -O2 -g -Wall -Werror main.cpp
	clang++ -o clangO2 implementations.o main.o
	rm *.o

clean:
	-rm *.o gccO3 gccO2 clangO3 clangO2
