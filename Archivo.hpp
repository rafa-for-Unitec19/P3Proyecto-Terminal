#ifndef ARCHIVO_H
#define ARCHIVO_H
#include <dirent.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <cstdlib>

using std::string;
using std::vector;

class Archivo
{
    string path;
    vector<string> ficheroActual;
    vector<vector<string>> dirBorrado;
public:
    static const bool isDir = true;
    static const bool isArch = true;
    Archivo(){};
    ~Archivo(){}
    void setPath(string p){this->path = p;};
    void listarFichero(){
        DIR *pdir = NULL;    //Puntero al Fichero
        pdir = opendir(path.c_str()); // Abre el fichero en el path especifico
        struct dirent *pent = NULL;// Struct con informacion de los ficheros en el directorio

        if (pdir == NULL){                 
            std::cout << "\nERROR! pdir could not be initialised correctly";
            exit(3);
        }
        while (pent = readdir(pdir)){ //Recorre los Ficheros
            if (pent == NULL){ // Doble chequeo del Mensaje de Error
                std::cout << "\nERROR! pent could not be initialised correctly";
            }
            if (pent->d_type == DT_REG){
                std::stringstream strm;
                strm << pent->d_name;
                string temp = strm.str();
                ficheroActual.push_back(temp);
            }else if (pent->d_type == DT_DIR){
                std::stringstream strm;
                strm << pent->d_name;
                if (strm.str() != "." && strm.str() != ".."){
                    string temp = strm.str();
                    if (!this->isDeleted(temp))
                    {
                        temp += '$';
                        ficheroActual.push_back(temp);
                    }   
                }                
            }
            
        }
        closedir(pdir);
    };

    string dirAnterior(){
        int pos = path.find_last_of("/");
        path.erase(pos, string::npos);
        ficheroActual.clear();
        return path;
    };

    string dirSiguiente(string fich){
        path += "/" + fich;
        ficheroActual.clear();
        return path;
    };

    bool fichExist(string fich, bool type){
        for (size_t i = 0; i < ficheroActual.size(); i++){
            if (type){
                if (ficheroActual[i].find(fich) != string::npos 
                    && ficheroActual[i].find('$') != string::npos){
                    return true;
                }
            }else{
                if (ficheroActual[i].find(fich) != string::npos){
                    return true;
                }
            } 
        }
        return false;
    };

    string createDir(string name){
        string path = "mkdir -p " + this->getPath() + "/" + name;
        const int dir_err = system(path.c_str());
        if (-1 == dir_err){
            return "Error creando el directorio!";
        }else {
            return "<" + name + "> Directorio Creado!";
        }
    }

    string delFichero(string name){
        if (fichExist(name, true)){
            vector<string> temp = {name, this->getPath()};
            dirBorrado.push_back(temp);
            temp.clear();
            for (size_t i = 0; i < ficheroActual.size(); i++)
            {
                if (ficheroActual[i].find(name) != string::npos 
                    && ficheroActual[i].find('$') != string::npos){
                    ficheroActual.erase(ficheroActual.begin() + i);
                }
            }
            return "<" + name + "> Directorio Borrado!";
        }else{
            return "No se puede borrar un Directorio que no existe";
        } 
    }

    bool isDeleted(string dir){
        for (size_t i = 0; i < dirBorrado.size(); i++)
        {
            if (dir == dirBorrado[i][0] && this->getPath() == dirBorrado[i][1]){
                return true;
            }
        }
        return false;
    }

    string getPath() const {
        return path;
    };

    vector<string> getFicheroActual(){
        return ficheroActual;
    }

    int getFicheroActualTam() const{
        return ficheroActual.size();
    }

    void eliminarListaFichero(){
        ficheroActual.clear();
    }
};

#endif