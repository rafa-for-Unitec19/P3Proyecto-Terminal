salida: terminal.cpp Archivo.hpp ArchivoIO.hpp
	g++ -o salida $^ -lncurses

clean:
	rm -f salida core
