

#pragma once

#define C 10
#define R 10

#define TIME 's'
#define COVERED ' '
#define EMPTY ' '
#define MINE '+'
#define N1 '1'
#define N2 '2'
#define N3 '3'
#define N4 '4'
#define N5 '5'
#define N6 '6'
#define N7 '7'
#define N8 '8'
#define FLAG 'v'

std::atomic<bool> threadRunning(true);
std::atomic<bool> gameActive(true);
std::mutex outputMutex;

std::random_device rd;
std::mt19937 gen(rd());

void printTime(int, int);

struct pos{
    int l;
    int c;
};

struct MouseClick {
    pos position;
    bool button;
};

struct GameInfo {
    bool lost = false;
    bool win = false;
    bool quit = false;
    MouseClick mouse;
    int l;
    int c;
};

MouseClick getMouseClick();

struct Cell
{
    char type = EMPTY;
    bool covered = true;
    bool flag = false;

    Cell() = default;

    bool clicked()
    {
        covered = false;
        if(type == MINE)
        {
            return true;
        }
        return false;
    }
};

class Camp {
    std::vector<std::vector<Cell>> mat;
    int mine;
    int flags = 0;
    int l;
    int c;

    void initializeMat()
    {
        for(int i = 0; i < l; i++)
        {
            for(int j = 0; j < c; j++)
            {
                mat[i][j].type = EMPTY;
            }
        }
    }

    void placeMine()
    {

        std::uniform_int_distribution<int> distL(0,l-1);
        std::uniform_int_distribution<int> distC(0,c-1);
        int placed_mine = 0;
        pos p;
        while(placed_mine < mine)
        {
            p.l = distL(gen);
            p.c = distC(gen);
            if(mat[p.l][p.c].type != MINE)
            {
                placed_mine++;
                mat[p.l][p.c].type = MINE;
            }
        }
    }

    int contMine(int _l,int _c)
    {
        int n_mine = 0;
        if(validPos(_l-1,_c-1))
        {
            if(mat[_l-1][_c-1].type==MINE)
            {
                n_mine++;
            }
        }
        if(validPos(_l-1,_c))
        {
            if(mat[_l-1][_c].type==MINE)
            {
                n_mine++;
            }
        }
        if(validPos(_l-1,_c+1))
        {
            if(mat[_l-1][_c+1].type==MINE)
            {
                n_mine++;
            }
        }
        if(validPos(_l,_c-1))
        {
            if(mat[_l][_c-1].type==MINE)
            {
                n_mine++;
            }
        }
        if(validPos(_l,_c+1))
        {
            if(mat[_l][_c+1].type==MINE)
            {
                n_mine++;
            }
        }
        if(validPos(_l+1,_c-1))
        {
            if(mat[_l+1][_c-1].type==MINE)
            {
                n_mine++;
            }
        }
        if(validPos(_l+1,_c))
        {
            if(mat[_l+1][_c].type==MINE)
            {
                n_mine++;
            }
        }
        if(validPos(_l+1,_c+1))
        {
            if(mat[_l+1][_c+1].type==MINE)
            {
                n_mine++;
            }
        }
        return n_mine;
    }

    void placeNumber()
    {
        for(int i = 0; i < l; i++)
        {
            for(int j = 0; j < c; j++)
            {
                if(mat[i][j].type!= MINE)
                {
                    int n_mine = contMine(i,j);
                    if(n_mine == 0)
                    {
                        mat[i][j].type = EMPTY;
                    }
                    else
                    {
                        mat[i][j].type = char(n_mine+48);
                    }
                }
            }
        }
    }

    bool placeFlags(pos p)
    {
        bool r_print = false;
        if(mat[p.l][p.c].covered)
        {
            if(mat[p.l][p.c].flag)
            {
                r_print = true;
                mat[p.l][p.c].flag = false;
                flags--;
            }
            else
            {
                if(flags < mine)
                {
                    r_print = true;
                    mat[p.l][p.c].flag = true;
                    flags++;
                }
            }
        }
        return r_print;
    }

    bool cleanEmpty(int _l, int _c)
    {
        if(!validPos(_l, _c) || !mat[_l][_c].covered || mat[_l][_c].flag)
        {
            return false;
        }

        mat[_l][_c].covered = false;

        if(mat[_l][_c].type != EMPTY)
        {
            return true;
        }

        cleanEmpty(_l-1, _c-1);
        cleanEmpty(_l-1, _c);
        cleanEmpty(_l-1, _c+1);
        cleanEmpty(_l, _c-1);
        cleanEmpty(_l, _c+1);
        cleanEmpty(_l+1, _c-1);
        cleanEmpty(_l+1, _c);
        cleanEmpty(_l+1, _c+1);

        return true;
    }

    bool validPos(int _l,int _c)
    {
        if(_l < 0 || _l > l-1)
        {
            return false;
        }

        if(_c < 0 || _c > c-1)
        {
            return false;
        }
        return true;
    }

    void printFlags()
    {
        int remainingFlags = mine - flags;
        std::cout<<"\033["<<3<<";"<<c+3<<"H\033[48;2;110;110;110m\033[38;2;160;10;10m "<<FLAG<<"\033[38;2;30;30;30m          ";
        if(remainingFlags<10)
        {
            std::cout<<"\033["<<3<<";"<<c+13<<"H"<<remainingFlags;
        }
        else if(remainingFlags<100)
        {
            std::cout<<"\033["<<3<<";"<<c+12<<"H"<<remainingFlags;
        }
        else
        {
            std::cout<<"\033["<<3<<";"<<c+11<<"H"<<remainingFlags;
        }
        std::cout<<"\033["<<l+1<<";"<<1<<"H";
        std::cout.flush();
    }

    bool win()
    {
        for(int i = 0; i < l; i++)
        {
            for(int j = 0; j < c; j++)
            {
                if(mat[i][j].type!=MINE && mat[i][j].covered)
                {
                    return false;
                }
            }
        }
        return true;
    }

    void printLose()
    {
        std::cout<<"\a\033["<<7<<";"<<c+3<<"H\033[48;2;160;20;20m\033[38;2;220;220;220m Game over! "<<"\033["<<8<<";"<<c+4<<"H You lost \033[0m";
        std::cout.flush();
    }

    void printWin()
    {
        std::cout<<"\a\033["<<7<<";"<<c+3<<"H\033[48;2;20;160;20m\033[38;2;220;220;220m Game over! "<<"\033["<<8<<";"<<c+4<<"H You win  \033[0m";
        std::cout.flush();
    }

public:

    Camp(int line, int column, int total_mine) : mat(line, std::vector<Cell>(column))
    {
        mine = total_mine;
        l = line;
        c = column;

        if(mine>=line*column)
        {
            throw mine;
        }

        initializeMat();
        placeMine();
        placeNumber();
    }

    void print()
    {
        std::lock_guard<std::mutex> lock(outputMutex);

        int cont = 0;
        for(int i = 0; i < l; i ++)
        {
            for(int j = 0; j < c; j++)
            {
                cont++;
                if(mat[i][j].flag)
                {
                    if(cont%2==0)
                    {
                        std::cout<<"\033[48;2;190;190;190m\033[38;2;160;10;10m"<<FLAG<<"\033[0m";
                    }
                    else
                    {
                        std::cout<<"\033[48;2;170;170;170m\033[38;2;160;10;10m"<<FLAG<<"\033[0m";
                    }
                }
                else if(mat[i][j].covered)
                {
                    if(cont%2==0)
                    {
                        std::cout<<"\033[48;2;190;190;190m"<<COVERED<<"\033[0m";
                    }
                    else
                    {
                        std::cout<<"\033[48;2;170;170;170m"<<COVERED<<"\033[0m";
                    }
                }
                else
                {
                    if(cont%2==0)
                    {
                        std::cout<<"\033[48;2;70;70;70m";
                    }
                    else
                    {
                        std::cout<<"\033[48;2;50;50;60m";
                    }
                    switch(mat[i][j].type)
                    {
                        case N1:{
                            std::cout<<"\033[38;2;0;85;255m"<<N1;
                        break;}

                        case N2:{
                            std::cout<<"\033[38;2;28;125;26m"<<N2;
                        break;}

                        case N3:{
                            std::cout<<"\033[38;2;173;163;17m"<<N3;
                        break;}

                        case N4:{
                            std::cout<<"\033[38;2;179;84;16m"<<N4;
                        break;}

                        case N5:{
                            std::cout<<"\033[38;2;171;35;17m"<<N5;
                        break;}

                        case N6:{
                            std::cout<<"\033[38;2;158;16;163m"<<N6;
                        break;}

                        case N7:{
                            std::cout<<"\033[38;2;49;32;64m"<<N7;
                        break;}

                        case N8:{
                            std::cout<<"\033[38;2;46;46;46m"<<N8;
                        break;}

                        case MINE:{
                            std::cout<<"\033[38;2;230;30;30m"<<MINE;
                        break;}

                        case EMPTY:{
                            std::cout<<EMPTY;
                        break;}
                    }
                    std::cout<<"\033[0m";
                }
            }
            std::cout<<"\n";
        }
        printFlags();
        std::cout.flush();
    }

    GameInfo tick()
    {
        GameInfo info;
        info.c = c;
        info.l = l;
        info.mouse = getMouseClick();

        if(info.mouse.position.c >= 0 && info.mouse.position.c < c &&
           info.mouse.position.l >= 0 && info.mouse.position.l < l)
        {
            if(info.mouse.button)
            {
                if(cleanEmpty(info.mouse.position.l,info.mouse.position.c))
                {
                    if(!mat[info.mouse.position.l][info.mouse.position.c].flag)
                    {
                        info.lost = mat[info.mouse.position.l][info.mouse.position.c].clicked();
                    }

                    {
                        std::lock_guard<std::mutex> lock(outputMutex);
                        std::cout<<"\033[1;1H";
                    }
                    print();

                    if(info.lost)
                    {
                        std::lock_guard<std::mutex> lock(outputMutex);
                        printLose();
                        gameActive = false;
                    }
                    else
                    {
                        info.win = win();
                        if(info.win)
                        {
                            std::lock_guard<std::mutex> lock(outputMutex);
                            printWin();
                            gameActive = false;
                        }
                    }
                }
            }
            else
            {
                if(placeFlags(info.mouse.position))
                {
                    {
                        std::lock_guard<std::mutex> lock(outputMutex);
                        std::cout<<"\033[1;1H";
                    }
                    print();
                }
            }
        }
        return info;
    }

    void printBg()
    {
        std::lock_guard<std::mutex> lock(outputMutex);
        std::cout<<"\033["<<1<<";"<<c+1<<"H\033[48;2;110;110;110m                  "
                 <<"\033["<<2<<";"<<c+1<<"H\033[48;2;100;100;100m                \033[48;2;110;110;110m  "
                 <<"\033["<<3<<";"<<c+1<<"H\033[48;2;100;100;100m  "<<"\033["<<3<<";"<<c+15<<"H  \033[48;2;110;110;110m  "
                 <<"\033["<<4<<";"<<c+1<<"H\033[48;2;100;100;100m                \033[48;2;110;110;110m  "
                 <<"\033["<<5<<";"<<c+1<<"H\033[48;2;100;100;100m  "<<"\033["<<5<<";"<<c+15<<"H  \033[48;2;110;110;110m  ";
        if(l > 6)
        {
            for(int i = 6; i < l; i++)
            {
                std::cout<<"\033["<<i<<";"<<c+1<<"H\033[48;2;100;100;100m                \033[48;2;110;110;110m  ";
            }
        }
        std::cout<<"\033["<<l<<";"<<c+1<<"H\033[48;2;110;110;110m                  ";
        std::cout.flush();
    }
};


void printTime(int l, int c)
{
    int time = 0;
    auto lastUpdate = std::chrono::steady_clock::now();

    {
        std::lock_guard<std::mutex> lock(outputMutex);
        std::cout<<"\033["<<5<<";"<<c+3<<"H\033[48;2;110;110;110m\033[38;2;10;10;100m "<<TIME<<"\033[38;2;30;30;30m          ";
        std::cout<<"\033["<<5<<";"<<c+13<<"H"<<time;
        std::cout<<"\033["<<l+1<<";"<<1<<"H";
        std::cout.flush();
    }

    while(threadRunning.load() && gameActive.load())
    {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate);

        if(elapsed.count() >= 1000)
        {
            time++;
            lastUpdate = now;

            {
                std::lock_guard<std::mutex> lock(outputMutex);
                std::cout<<"\033["<<5<<";"<<c+3<<"H\033[48;2;110;110;110m\033[38;2;10;10;100m "<<TIME<<"\033[38;2;30;30;30m          ";
                if(time<10)
                {
                    std::cout<<"\033["<<5<<";"<<c+13<<"H"<<time;
                }
                else if(time<100)
                {
                    std::cout<<"\033["<<5<<";"<<c+12<<"H"<<time;
                }
                else if(time<1000)
                {
                    std::cout<<"\033["<<5<<";"<<c+11<<"H"<<time;
                }
                else if(time<10000)
                {
                    std::cout<<"\033["<<5<<";"<<c+10<<"H"<<time;
                }
                else
                {
                    std::cout<<"\033["<<5<<";"<<c+9<<"H"<<time;
                }
                std::cout<<"\033["<<l+1<<";"<<1<<"H";
                std::cout.flush();
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

MouseClick getMouseClick()
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD prevMode;
    GetConsoleMode(hInput, &prevMode);
    SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT);

    INPUT_RECORD inputRecord;
    DWORD events;
    MouseClick result;

    while (true)
    {
        ReadConsoleInput(hInput, &inputRecord, 1, &events);

        if (inputRecord.EventType == MOUSE_EVENT)
        {
            MOUSE_EVENT_RECORD mouse = inputRecord.Event.MouseEvent;

            if (mouse.dwEventFlags == 0)
            {
                bool leftPressed = (mouse.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) != 0;
                bool rightPressed = (mouse.dwButtonState & RIGHTMOST_BUTTON_PRESSED) != 0;

                if (leftPressed || rightPressed)
                {
                    result.button = leftPressed ? 1 : 0;
                    DWORD buttonToCheck = leftPressed ? FROM_LEFT_1ST_BUTTON_PRESSED : RIGHTMOST_BUTTON_PRESSED;

                    while (true)
                    {
                        ReadConsoleInput(hInput, &inputRecord, 1, &events);
                        if (inputRecord.EventType == MOUSE_EVENT)
                        {
                            MOUSE_EVENT_RECORD releaseCheck = inputRecord.Event.MouseEvent;
                            if (!(releaseCheck.dwButtonState & buttonToCheck))
                            {
                                break;
                            }
                        }
                    }

                    SetConsoleMode(hInput, prevMode);
                    result.position.l = mouse.dwMousePosition.Y;
                    result.position.c = mouse.dwMousePosition.X;
                    return result;
                }
            }
        }
    }
}
