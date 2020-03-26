salida: terminal.cpp Archivo.hpp
	g++ -o salida $^ -lncurses

clean:
	rm -f salida core
