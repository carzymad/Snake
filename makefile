run:main.o Snake.o
	g++ -o run Snake.o main.o -lpthread
Snake.o:Snake.cpp Snake.h def.h
	g++ -c Snake.cpp
main.o:main.cpp Snake.h def.h
	g++ -c main.cpp -lpthread
clean:
	rm main.o Snake.o run
