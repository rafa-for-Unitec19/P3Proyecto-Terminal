#ifndef ARCHIVOIO_H
#define ARCHIVOIO_H
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <ncurses.h>

using std::string;
using std::ofstream;
using std::ifstream;
using std::ios;
using std::vector;

class ArchivoIO{
    vector<string> contenidoArr;

public:
    ArchivoIO(){};
    ~ArchivoIO(){};
    void crearArchivo(string n){
        ofstream archivo(n);
        archivo << "";
        archivo.close();
    };

    bool escribirArchivo(string n, string msg){
        ifstream existencia(n);
        if (existencia.is_open()){
            existencia.close();
            ofstream archivo;
            archivo.open(n, ios::app);
            archivo << " " << msg;
            archivo.close();
            return true;
        }else{
            existencia.close();
            return false;
        }
    }

    string leerArchivo(string n, WINDOW* TERM){
        ifstream archivo(n);
        if (archivo.fail())
            return "No existe el archivo solicitado.txt";
        else{
            if (archivo.is_open())
            {
                char linea[256];
                string contenido = "";
                while (!archivo.eof())
                {
                    archivo.getline(linea, sizeof(linea));
                    contenido = linea;
                    contenidoArr.push_back(contenido);
                }
                archivo.close();
                return "good";
            }else{
                archivo.close();
                return "No existe el archivo solicitado.txt";
            }
        }
        
    }

    vector<string> getContent(){
        return contenidoArr;
    }
};
#endif