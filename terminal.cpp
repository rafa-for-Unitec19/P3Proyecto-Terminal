#include <ncurses.h>
#include <string>
#include <dirent.h>


using std::string;

WINDOW* terminal;
string USUARIO = "rafa", MAQUINA = "mipc", RUTA = "./home";
int contln = 0;

void moverCursor(int row){
    wmove(terminal, row, 0);
    if (row == 0)
    {
        contln = 0;
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

WINDOW* ventana(WINDOW* ventana){

}

void ejecucion(){
    char ch;
    string entrada = "";
    bool exit = false;
    infoUsuario();
    while(!exit){
        ch = wgetch(terminal);
        if (ch == '\n'){
            moverCursor(++contln);
            infoUsuario();
            int loEncontro = entrada.find("exit");
            if (entrada.find("exit") != string::npos){
                exit = true;
            }
            if (entrada.find("clear") != string::npos)
            {
                wclear(terminal);
                moverCursor(0);
                infoUsuario();
            }
            
        }else{
            entrada += ch;    
        }
    }
}

int main(){
    terminal = initscr();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    ejecucion();
    endwin();
    return 0;
}