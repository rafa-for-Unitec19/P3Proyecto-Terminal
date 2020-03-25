#include <ncurses.h>
#include <string>
#define USUARIO "rafa"
#define MAQUINA "mipc"
#define RUTA "/home"

using std::string;

WINDOW* terminal;
int contln = 0;

void infoUsuario(){
    wattrset(terminal, COLOR_PAIR(3));
    //RECUERDA CONVERTIRLO A C_STRING
    wprintw(terminal, "%s@%s", USUARIO, MAQUINA);
    wattrset(terminal, COLOR_PAIR(1));
    waddch(terminal, ':');
    wattrset(terminal, COLOR_PAIR(2));
    //RECUERDA CONVERTIRLO A C_STRING
    wprintw(terminal, "~%s", RUTA);
    wattrset(terminal, COLOR_PAIR(1));
    waddstr(terminal, "$ ");
    wrefresh(terminal);
}

void ejecucion(){
    char ch;
    string entrada = "";
    bool exit = false;
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    infoUsuario();
    while(!exit){
        ch = wgetch(terminal);
        if (ch == '\n'){
            move(++contln, 0);
            infoUsuario();
            int loEncontro = entrada.find("exit");
            wprintw(terminal, "El contenido de Entrada: %s", entrada);
            wprintw(terminal, "Lo abra encontrado alv: %d", loEncontro);
            if (entrada.find("exit") != string::npos){
                exit = true;
            }
        }else{
            entrada += ch;    
        }
    }
}

int main(){
    terminal = initscr();
    ejecucion();
    wgetch(terminal);
    endwin();
    return 0;
}