#include <ncurses.h>
#include <string>
#include "Archivo.hpp"

using std::string;
WINDOW* terminal;
Archivo fichero;

const string RAIZ = "./Home", ARCHDIR = "Archivo(s) y Directorio(s)", USR = "Usuario(s)", PC = "Maquina(s)";
const char COMILLAD = 34;

string USUARIO = "rafa", MAQUINA = "mipc", RUTA = "";
int contln = 0;

void moverCursor(int row, int col = 0){
    wmove(terminal, row, col);
    if (row == 0)
    {
        contln = 0;
    }
}

void changeInfo(string param, bool tipo){
    if (tipo){
        USUARIO = param;
    }else{
        MAQUINA = param;
    }
}

void dirRegresar(){
    if (fichero.getPath() != RAIZ){
        RUTA = fichero.dirAnterior();
    }else{
        moverCursor(++contln);
        waddstr(terminal, "Ya se encuentra en el directorio raiz");
    }
}

void dirCambio(string fich){
    fichero.listarFichero();
    if (fichero.fichExist(fich, Archivo::isDir)){
        fichero.dirSiguiente(fich);
        RUTA = fichero.getPath();
    }else{
        moverCursor(++contln);
        waddstr(terminal, "El Archivo/Directorio no existe");
    }
}

void printFichero(){
    int col, cont = 0, found;
    contln++;
    fichero.eliminarListaFichero();
    fichero.listarFichero();
    if (fichero.getFicheroActualTam()){
        col = getmaxy(terminal) / 4;
        for (size_t i = 0; i < fichero.getFicheroActualTam(); i++){
            if (cont > 3){
                cont = 0;
                contln++;
            }
            moverCursor(contln, (cont++ * col));
            found = fichero.getFicheroActual()[i].find('$');
            if (found != string::npos){
                string temp = fichero.getFicheroActual()[i].erase(found);
                wattrset(terminal, COLOR_PAIR(2));
                waddstr(terminal, temp.c_str());
            }else{
                wattrset(terminal, COLOR_PAIR(1));
                waddstr(terminal, fichero.getFicheroActual()[i].c_str());
            }            
        }
    }else{
        moverCursor(++contln);
        waddstr(terminal, "No hay nada que listar");
    }
    
}

void infoUsuario(){
    wattrset(terminal, COLOR_PAIR(3));
    //RECUERDA CONVERTIRLO A C_STRING
    wprintw(terminal, "%s@%s", USUARIO.c_str(), MAQUINA.c_str());
    wattrset(terminal, COLOR_PAIR(1));
    waddch(terminal, ':');
    wattrset(terminal, COLOR_PAIR(2));
    //RECUERDA CONVERTIRLO A C_STRING
    wprintw(terminal, "~%s", RUTA.substr(1).c_str());
    wattrset(terminal, COLOR_PAIR(1));
    waddstr(terminal, "$ ");
    wrefresh(terminal);
}

bool noChEspecial(string str, string error){
    int dotCnt = 0;
    for (size_t i = 0; i < str.length(); i++){
        char ch = str.at(i); 
        if (ch != 46 && !(ch >= 64 && ch <= 90) && !(ch >= 97 && ch <= 122) && !(ch >= 48 && ch <= 57)){
            moverCursor(++contln);
            wprintw(terminal, "Los %s son Alfanumericos (Los archivos tienen un punto antes del tipo)", error.c_str());    
            return false;
        }
        if (ch == 46){
            dotCnt++;
        }
    }
    if (dotCnt > 1){
        moverCursor(++contln);
        waddstr(terminal, "Solo se permite un caracter de PUNTO");
        return false;
    }else{
        return true;
    }
}

string getNombreFich(int pos, string entrada, string error){
    int posF = entrada.find(COMILLAD, (pos + 1));
    if (posF != string::npos){
        string fich = entrada.substr((pos + 1) , (posF - (pos + 1)));
        if (!(fich.empty())){
            if (noChEspecial(fich, error)){
                return fich;
            }    
        }
    }else{
        moverCursor(++contln);
        wprintw(terminal, "No se puso el %s entre comillas dobles", error.c_str());
        return "";
    }
}

void ejecucion(){
    char ch;
    string entrada = "";
    bool exit = false;
    infoUsuario();
    while(!exit){
        ch = wgetch(terminal);
        if (ch == '\n'){
            if (entrada.find("exit") != string::npos
                && entrada.find("exit") == 0){
                entrada.clear();
                exit = true;
            }else if (entrada.find("clear") != string::npos
                && entrada.find("clear") == 0){
                wclear(terminal);
                moverCursor(0);
                entrada.clear();
            }else if(entrada.find("ls") != string::npos
                && entrada.find("ls") == 0){
                printFichero();
                entrada.clear();
            }else if(entrada.find("cd") != string::npos
                && entrada.find("cd") == 0){
                int cdFound = entrada.find("cd"), cdName;
                if (entrada.find("cd/", cdFound) != string::npos){
                    dirRegresar();
                    entrada.clear();
                }else if((cdName = entrada.find(COMILLAD)) != string::npos){
                    string goodName = getNombreFich(cdName, entrada, ARCHDIR);
                    if(goodName != ""){
                        dirCambio(goodName);
                    }
                    entrada.clear();
                }else{
                    wprintw(terminal, "<%s> Argumentos incorrectos para cd", entrada.c_str());
                }
                entrada.clear();
            }else if(entrada.find("changeUser") != string::npos
                && entrada.find("changeUser") == 0){
                int cdName;
                if((cdName = entrada.find(COMILLAD)) != string::npos){
                    string goodName = getNombreFich(cdName, entrada, USR);
                    if(goodName != ""){
                        changeInfo(goodName, true);
                    }
                }else{
                    moverCursor(++contln);
                    wprintw(terminal, "<%s> Argumentos incorrectos para ChangeUser", entrada.c_str());
                }
                entrada.clear();
            }else if(entrada.find("changeMachine") != string::npos
                && entrada.find("changeMachine") == 0){
                int cdName;
                if((cdName = entrada.find(COMILLAD)) != string::npos){
                    string goodName = getNombreFich(cdName, entrada, PC);
                    if(goodName != ""){
                        changeInfo(goodName, false);
                    }
                }else{
                    moverCursor(++contln);
                    wprintw(terminal, "<%s> Argumentos incorrectos para ChangeMachine", entrada.c_str());
                }
                entrada.clear();
            }else{
                moverCursor(++contln);
                wprintw(terminal, "<%s> No es un comando valido", entrada.c_str());
                entrada.clear();
            }
            moverCursor(++contln);
            infoUsuario();
        }else{
            entrada += ch;    
        }
    }
}

int main(){
    RUTA = RAIZ;
    fichero.setPath(RUTA);
    terminal = initscr();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    ejecucion();
    endwin();
    return 0;
}