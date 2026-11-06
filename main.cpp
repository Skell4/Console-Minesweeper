

#include <vector>
#include <iostream>
#include <random>
#include <windows.h>
#include <atomic>
#include <thread>
#include <mutex>
#include <chrono>
#include "camp.hpp"

#define EASY 1
#define MEDIUM 2
#define HARD 3
#define INSANE 4
#define IMPOSSIBLE 5


class Game{
    int l = 15;
    int c = 25;
    int m = 25;
    short int d = EASY;
    void printMenu()
    {
        std::cout<<"\033[1;1H\033[48;2;120;120;120m                                      \n"
            <<"\033[48;2;120;120;120m  \033[48;2;110;110;110m                                  \033[48;2;120;120;120m  \n"
            <<"\033[48;2;120;120;120m  \033[48;2;110;110;110m                                  \033[48;2;120;120;120m  \n"
            <<"\033[48;2;120;120;120m  \033[48;2;110;110;110m                                  \033[48;2;120;120;120m  \n"
            <<"\033[48;2;120;120;120m  \033[48;2;110;110;110m                                  \033[48;2;120;120;120m  \n"
            <<"\033[48;2;120;120;120m  \033[48;2;110;110;110m                                  \033[48;2;120;120;120m  \n"
            <<"\033[48;2;120;120;120m  \033[48;2;110;110;110m                                  \033[48;2;120;120;120m  \n"
            <<"\033[48;2;120;120;120m  \033[48;2;110;110;110m                                  \033[48;2;120;120;120m  \n"
            <<"\033[48;2;120;120;120m  \033[48;2;110;110;110m                                  \033[48;2;120;120;120m  \n"
            <<"\033[48;2;120;120;120m  \033[48;2;110;110;110m                                  \033[48;2;120;120;120m  \n"
            <<"\033[48;2;120;120;120m  \033[48;2;110;110;110m                                  \033[48;2;120;120;120m  \n"
            <<"\033[48;2;120;120;120m  \033[48;2;110;110;110m                                  \033[48;2;120;120;120m  \n"
            <<"\033[48;2;120;120;120m                                      ";

        std::cout<<"\033[38;2;190;190;190m\033[48;2;20;120;20m\033[3;24H    Easy    "
            <<"\033[48;2;210;90;19m\033[5;24H   Medium   "
            <<"\033[48;2;210;20;20m\033[7;24H    Hard    "
            <<"\033[48;2;152;33;207m\033[9;24H   Insane   "
            <<"\033[48;2;26;26;26m\033[11;24H Impossible \033[48;2;110;110;110m";

        if(d==EASY)
        {
            std::cout<<"\033[3;21H->";
        }
        else if(d==MEDIUM)
        {
            std::cout<<"\033[5;21H->";
        }
        else if(d==HARD)
        {
            std::cout<<"\033[7;21H->";
        }
        else if(d==INSANE)
        {
            std::cout<<"\033[9;21H->";
        }
        else if(d==IMPOSSIBLE)
        {
            std::cout<<"\033[11;21H->";
        }

        std::cout<<"\033[3;5H\033[38;2;170;10;10mMinesweeper";

        std::cout<<"\033[9;6H\033[48;2;194;172;8m             "
                 <<"\033[10;5H\033[38;2;30;30;30m     PLAY!     "
                 <<"\033[11;6H             ";

        std::cout.flush();
    }

    void difficulty(short int difficulty)
    {
        if(difficulty==EASY)
        {
            l = 15;
            c = 25;
            m = 25;
        }
        else if(difficulty == MEDIUM)
        {
            l = 20;
            c = 35;
            m = 60;
        }
        else if(difficulty == HARD)
        {
            l = 26;
            c = 43;
            m = 180;
        }
        else if(difficulty == INSANE)
        {
            l = 28;
            c = 45;
            m = 250;
        }
        else if(difficulty == IMPOSSIBLE)
        {
            l = 28;
            c = 45;
            m = 400;
        }
        else
        {
            throw;
        }
        d = difficulty;
    }

public:

    void start()
    {
        threadRunning = true;
        gameActive = true;

        Camp camp(l,c,m);
        camp.print();
        camp.printBg();
        std::thread t(printTime,l,c);
        t.detach();
        while(true)
        {
            GameInfo info = camp.tick();

            if(info.lost)
            {
                break;
            }
            else if(info.win)
            {
                break;
            }
        }

        threadRunning = false;
        gameActive = false;
    }

    bool menu()
    {
        system("cls");
        printMenu();
        while(true)
        {
            MouseClick mouse = getMouseClick();
            if(!mouse.button)
            {
                continue;
            }

            if(mouse.position.l == 2 && mouse.position.c > 22 && mouse.position.c <= 34)
            {
                difficulty(EASY);
                printMenu();
            }
            else if(mouse.position.l == 4 && mouse.position.c > 22 && mouse.position.c <= 34)
            {
                difficulty(MEDIUM);
                printMenu();
            }
            else if(mouse.position.l == 6 && mouse.position.c > 22 && mouse.position.c <= 34)
            {
                difficulty(HARD);
                printMenu();
            }
            else if(mouse.position.l == 8 && mouse.position.c > 22 && mouse.position.c <= 34)
            {
                difficulty(INSANE);
                printMenu();
            }
            else if(mouse.position.l == 10 && mouse.position.c > 22 && mouse.position.c <= 34)
            {
                difficulty(IMPOSSIBLE);
                printMenu();
            }
            else if(mouse.position.l >= 8 && mouse.position.l <= 10 && mouse.position.c >= 4 && mouse.position.c <= 18)
            {
                return true;
            }
        }
    }
};

int main()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);

    std::cout.sync_with_stdio(false);

    Game game;
    while(true)
    {
        if(game.menu())
        {
            std::cout<<"\033[1;1H";
            game.start();
            Sleep(200);
            getMouseClick();
            system("cls");
        }
    }

}


