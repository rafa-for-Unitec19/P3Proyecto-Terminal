#include <ncurses.h>
#include <string>
#include <cstdlib>
#include "Archivo.hpp"
#include "ArchivoIO.hpp"

using std::string;
WINDOW* terminal;
Archivo fichero;

const string RAIZ = "./Home", ARCHDIR = "Archivo(s) y Directorio(s)", USR = "Usuario(s)", PC = "Maquina(s)";
const string colors[3] = {"black", "red", "white"};
string exeCOLOR = "black";
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
        col = getmaxy(terminal) / 3;
        for (size_t i = 0; i < fichero.getFicheroActualTam(); i++){
            if (cont > 5){
                cont = 0;
                contln++;
            }
            moverCursor(contln, (cont++ * col));
            found = fichero.getFicheroActual()[i].find('$');
            if (found != string::npos){
                string temp = fichero.getFicheroActual()[i].erase(found);
                if (exeCOLOR == colors[0])
                {
                    wattrset(terminal, COLOR_PAIR(2));
                }else if(exeCOLOR == colors[1]){
                    wattrset(terminal, COLOR_PAIR(5));
                }else{
                    wattrset(terminal, COLOR_PAIR(8));
                }
                waddstr(terminal, temp.c_str());
            }else{
                if (exeCOLOR == colors[0])
                {
                    wattrset(terminal, COLOR_PAIR(1));
                }else if(exeCOLOR == colors[1]){
                    wattrset(terminal, COLOR_PAIR(3));
                }else{
                    wattrset(terminal, COLOR_PAIR(6));
                }
                waddstr(terminal, fichero.getFicheroActual()[i].c_str());
            }            
        }
    }else{
        moverCursor(++contln);
        waddstr(terminal, "No hay nada que listar");
    }
    
}

void infoUsuario(){
    if (exeCOLOR == colors[0])
    {
        wattrset(terminal, COLOR_PAIR(3));
        wprintw(terminal, "%s@%s", USUARIO.c_str(), MAQUINA.c_str());
        wattrset(terminal, COLOR_PAIR(1));
        waddch(terminal, ':');
        wattrset(terminal, COLOR_PAIR(2));
        wprintw(terminal, "~%s", RUTA.substr(1).c_str());
        wattrset(terminal, COLOR_PAIR(1));
        waddstr(terminal, "$ ");
        wrefresh(terminal);
    }else if(exeCOLOR == colors[1]){
        wattrset(terminal, COLOR_PAIR(6));
        wprintw(terminal, "%s@%s", USUARIO.c_str(), MAQUINA.c_str());
        wattrset(terminal, COLOR_PAIR(4));
        waddch(terminal, ':');
        wattrset(terminal, COLOR_PAIR(5));
        wprintw(terminal, "~%s", RUTA.substr(1).c_str());
        wattrset(terminal, COLOR_PAIR(4));
        waddstr(terminal, "$ ");
        wrefresh(terminal);
    }else if(exeCOLOR == colors[2]){
        wattrset(terminal, COLOR_PAIR(9));
        wprintw(terminal, "%s@%s", USUARIO.c_str(), MAQUINA.c_str());
        wattrset(terminal, COLOR_PAIR(7));
        waddch(terminal, ':');
        wattrset(terminal, COLOR_PAIR(8));
        wprintw(terminal, "~%s", RUTA.substr(1).c_str());
        wattrset(terminal, COLOR_PAIR(7));
        waddstr(terminal, "$ ");
        wrefresh(terminal);
    }
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

string toLowerCase(string entrada){
    string lowerStr = "";
    for (size_t i = 0; i < entrada.size(); i++){
        lowerStr += ::tolower(entrada.at(i));
    }
    return lowerStr;
}

bool changeColor(int pos, string entrada){
    int posF = entrada.find(COMILLAD, (pos + 1));
    if (posF != string::npos){
        string clr = entrada.substr((pos + 1) , (posF - (pos + 1)));
        if (clr != ""){
            clr = toLowerCase(clr);
            if (clr == colors[0]){
                exeCOLOR = clr;
                wbkgd(terminal, COLOR_PAIR(1));
                return true;
            } else if (clr == colors[1]){
                exeCOLOR = clr;
                wbkgd(terminal, COLOR_PAIR(4));
                return true;
            } else if (clr == colors[2]){
                exeCOLOR = clr;
                wbkgd(terminal, COLOR_PAIR(7));
                return true;
            }else{
                moverCursor(++contln);
                wprintw(terminal, "EL color <%s> no es soportado", clr.c_str());
                return false;
            }
        }else{
            moverCursor(++contln);
            waddstr(terminal, "EL argumento no puede ser nulo");
            return false;
        }
    }else{
        moverCursor(++contln);
        waddstr(terminal, "No se puso el %s entre comillas dobles");
        return false;
    }
}

string chkWord(string entrada, int typePos){
    int wordPos = entrada.find("<");
    if (wordPos != string::npos && wordPos > typePos)
    {
        int closeWordPos = entrada.rfind(">");
        if (closeWordPos != string::npos && closeWordPos == entrada.length() - 1)
        {
            string word = entrada.substr((wordPos + 1), (closeWordPos - (wordPos + 1)));
            if (word != "")
            {
                if (word.find(" ") != string::npos)
                {
                    moverCursor(++contln);
                    waddstr(terminal, "NO se puede escribir mas de una palabra");
                }else{
                    return word;
                }
            }else{
                moverCursor(++contln);
                waddstr(terminal, "La palabra a escribir no puedo ser nula");
            }
        }
        else
        {
            moverCursor(++contln);
            waddstr(terminal, "'>' debe ser el unico caracter al final del comando");
        }
    }
    else
    {
        moverCursor(++contln);
        waddstr(terminal, "La palabra a escribir debe estar entre <>");
    }
    return "";
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
                    moverCursor(++contln);
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
            }else if(entrada.find("changeColor") != string::npos
                && entrada.find("changeColor") == 0){
                    int color;
                if((color = entrada.find(COMILLAD)) != string::npos){
                    if (changeColor(color, entrada))
                    {
                        wclear(terminal);
                        moverCursor(0);
                    }
                }else{
                    moverCursor(++contln);
                    wprintw(terminal, "<%s> Argumentos incorrectos para ChangeColor", entrada.c_str());
                }
                entrada.clear();
            }else if(entrada.find("mkdir") != string::npos
                && entrada.find("mkdir") == 0){
                int cdName;
                if((cdName = entrada.find(COMILLAD)) != string::npos){
                    string goodName = getNombreFich(cdName, entrada, ARCHDIR);
                    if(goodName != ""){
                        if (!fichero.isDeleted(goodName))
                        {
                            string state = fichero.createDir(goodName);
                            moverCursor(++contln);
                            wprintw(terminal, "%s", state.c_str());
                        }else{
                            moverCursor(++contln);
                            waddstr(terminal, "Por limitaciones del SO no pude crear directorios de nuevo que ya 'borro'");
                        }
                    }
                }else{
                    moverCursor(++contln);
                    wprintw(terminal, "<%s> Argumentos incorrectos para MKDIR", entrada.c_str());
                }
                entrada.clear();
            }else if(entrada.find("del") != string::npos
                && entrada.find("del") == 0){
                int cdName;
                if((cdName = entrada.find(COMILLAD)) != string::npos){
                    string goodName = getNombreFich(cdName, entrada, ARCHDIR);
                    if(goodName != ""){
                        string state = fichero.delFichero(goodName);
                        moverCursor(++contln);
                        wprintw(terminal, "%s", state.c_str());
                    }
                }else{
                    moverCursor(++contln);
                    wprintw(terminal, "<%s> Argumentos incorrectos para Del", entrada.c_str());
                }
                entrada.clear();
            }else if(entrada.find("file") != string::npos
                && entrada.find("file") == 0){
                int cdName;
                if((cdName = entrada.find(COMILLAD)) != string::npos){
                    string goodName = getNombreFich(cdName, entrada, ARCHDIR);
                    if(goodName != ""){
                        int typePos = goodName.rfind(".txt");
                        if (typePos != string::npos && typePos == (goodName.length() - 4))
                        {
                        ArchivoIO temp;
                        string path = fichero.getPath() + "/" + goodName;
                        temp.crearArchivo(path);
                        moverCursor(++contln);
                        wprintw(terminal, "<%s> Creado con Exito", goodName.c_str());
                        }else{
                            moverCursor(++contln);
                            waddstr(terminal, "<read> solo crea archivos .txt");
                        }
                    }
                }else{
                    moverCursor(++contln);
                    wprintw(terminal, "<%s> Argumentos incorrectos para file", entrada.c_str());
                }
                entrada.clear();
            }else if(entrada.find("read") != string::npos
                && entrada.find("read") == 0){
                int cdName;
                if((cdName = entrada.find(COMILLAD)) != string::npos){
                    string goodName = getNombreFich(cdName, entrada, ARCHDIR);
                    if(goodName != ""){
                        int typePos = goodName.rfind(".txt");
                        if (typePos != string::npos && typePos == (goodName.length() - 4))
                        {
                            ArchivoIO temp;
                            string path = fichero.getPath() + "/" + goodName;
                            string content = temp.leerArchivo(path, terminal);
                            if (content == "good")
                            {
                                for (size_t i = 0; i < temp.getContent().size(); i++)
                                {
                                    moverCursor(++contln);
                                    waddstr(terminal, temp.getContent()[i].c_str());
                                }
                                int col;
                                getyx(terminal, contln, col);
                            }else{
                                moverCursor(++contln);
                                wprintw(terminal, "<%s>", content.c_str());
                            }
                        }else {
                            moverCursor(++contln);
                            waddstr(terminal, "<read> solo lee archivos .txt");
                        }
                    }
                }else{
                    moverCursor(++contln);
                    wprintw(terminal, "<%s> Argumentos incorrectos para read", entrada.c_str());
                }
                entrada.clear();
            }else if(entrada.find("write") != string::npos
                && entrada.find("write") == 0){
                int cdName;
                if((cdName = entrada.find(COMILLAD)) != string::npos){
                    string goodName = getNombreFich(cdName, entrada, ARCHDIR);
                    if(goodName != ""){
                        int typePos = goodName.rfind(".txt");
                        if (typePos != string::npos && typePos == (goodName.length() - 4))
                        {
                            string word = chkWord(entrada, typePos);
                            if (word != "")
                            {
                                ArchivoIO temp;
                                string path = fichero.getPath() + "/" + goodName;
                                if (!temp.escribirArchivo(path, word))
                                {
                                    moverCursor(++contln);
                                    wprintw(terminal, "<%s> El Archivo .txt no existe");
                                }else{
                                    moverCursor(++contln);
                                    waddstr(terminal, "<%s> Palabra Agregada con exito");
                                }
                            }
                        }else {
                            moverCursor(++contln);
                            waddstr(terminal, "<write> solo lee archivos .txt");
                        }
                    }
                }else{
                    moverCursor(++contln);
                    wprintw(terminal, "<%s> Argumentos incorrectos para Write", entrada.c_str());
                }
                entrada.clear();
            }else{
                moverCursor(++contln);
                wprintw(terminal, "<%s> No es un comando valido", entrada.c_str());
                entrada.clear();
            }
            moverCursor(++contln);
            infoUsuario();
        }else if (ch == 7){
            wdelch(terminal);
            wrefresh(terminal);
            entrada.pop_back();
        }else{
            entrada += ch;    
        }
    }
}

int main(){
    RUTA = RAIZ;
    fichero.setPath(RUTA);
    terminal = initscr();
    keypad(terminal,TRUE);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_WHITE, COLOR_RED);
    init_pair(5, COLOR_BLUE, COLOR_RED);
    init_pair(6, COLOR_GREEN, COLOR_RED);
    init_pair(7, COLOR_BLACK, COLOR_WHITE);
    init_pair(8, COLOR_BLUE, COLOR_WHITE);
    init_pair(9, COLOR_GREEN, COLOR_WHITE);
    ejecucion();
    endwin();
    return 0;
}