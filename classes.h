#ifndef CLASSES_H
#define CLASSES_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
using namespace std;

#define ROW 20
#define COL 40

enum elem_type
{
    EMPTY_BLOCK,
    WALL_BLOCK,
    PERSON_BLOCK,
    FOOD_BLOCK,
    BULLET_BLOCK
};

enum moving_type
{
    MOVING_LEFT=-1,
    MOVING_RIGHT=+1,
    MOVING_UP=-1,
    MOVING_DOWN=+1,
    NOT_MOVING=0
};

unsigned char map[ROW][COL];

class map_game
{
private:
    /* data */
    void printf_row_buff(int row_size);
    void printf_col_buff(int col_size);
public:
    map_game(/* args */);
    ~map_game();
    void map_display();
    void map_init();
};

map_game::map_game(/* args */)
{
}

map_game::~map_game()
{
}

void map_game::map_init()
{
    int i, j;
    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COL; j++)
        {
			if(i == 0 || i == ROW -1) map[i][j]=WALL_BLOCK;
            else if (j == 0 || j == COL -1) map[i][j]=WALL_BLOCK;
            else map[i][j] = EMPTY_BLOCK;
        }
    }
}
void map_game::map_display()
{
    cout<<"\033[H\033[J";
    int i, j;
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    printf_row_buff((int)w.ws_row);
    for (i = 0; i < ROW; i++)
    {
        printf_col_buff((int)w.ws_col);
        for (j = 0; j < COL; j++)
        {
            if (EMPTY_BLOCK == map[i][j])
                cout<<" ";
            else if (FOOD_BLOCK == map[i][j])
                cout<<"[]";
			else if (WALL_BLOCK == map[i][j])
				cout<<"#";
            else if (PERSON_BLOCK == map[i][j])
                cout<<"A";
            else if (BULLET_BLOCK == map[i][j])
                cout<<"^";
            else
                cout<<"\u2588\u2588";
        }
        cout<<"\r\n";
    }
    printf_col_buff((int)w.ws_col);
    //printf("POINT: %d \tLENGTH: %d \tLIFE: %d ", snake_game_point, snake_lengh, snake_game_life);
    //return;
}

void map_game::printf_row_buff(int row_size)
{
    int buff_len;
    buff_len = (int)((row_size - ROW) / 2);
    if (0 < buff_len)
    {
        while (buff_len > 0)
        {
            buff_len--;
            cout<<"\n";
        }
    }
}

void map_game::printf_col_buff(int col_size)
{
    int buff_len;
    buff_len = (int)((col_size - 2 * COL) / 2);
    if (0 < buff_len)
    {
        while (buff_len > 0)
        {
            buff_len--;
            cout<<" ";
        }
    }
}


class display_screen : public map_game
{
private:
    /* data */
public:
    display_screen(/* args */);
    ~display_screen();
};

display_screen::display_screen(/* args */)
{ 
}

display_screen::~display_screen()
{
}

class fire_effect
{
private:
    /* data */
public:
    bool check;
    struct position_bullet{
        int x;
        int y;
    }bullet;
    fire_effect();
    ~fire_effect();
    void bullet_fly(void);
};

fire_effect::fire_effect()
{
    check = false;
}

fire_effect::~fire_effect()
{
}

void fire_effect::bullet_fly(void)
{
    
    if(check == true)
    {
        static int temp = bullet.y-2;
        map[temp][bullet.x] = EMPTY_BLOCK;
        map[bullet.y-=2][bullet.x] = BULLET_BLOCK;
        temp = bullet.y;
        if(bullet.y < 2) {
            check = false;
            map[bullet.y][bullet.x] = WALL_BLOCK;
        }
    }
}

class person : public map_game
{
private:
    /* data */
public:
    int row;
    int col;
    bool person_fire;
    person();
    ~person();
    void control_person(void);
};

person::person()
{
    row = ROW-2;
    col = 10;
    map[row][col]=PERSON_BLOCK;
    person_fire=false;
}

person::~person()
{
}

void person::control_person(void)
{
    //Keypress Event Handler
   struct termios info;
   tcgetattr(0, &info);          /* get current terminal attirbutes; 0 is the file descriptor for stdin */
   info.c_lflag &= ~ICANON;      /* disable canonical mode */
   info.c_cc[VMIN] = 0;          /* set 1 wait until at least one keystroke available */
   info.c_cc[VTIME] = 0;         /* no timeout */
   tcsetattr(0, TCSANOW, &info); /* set immediately */

    if (ferror(stdin)) { /* there was an error... */ }
        clearerr(stdin);
    /* clear stdin to avoid you've reached EOF*/

    int person_direction = NOT_MOVING;
    char input = getchar();

    
    if('a'==input && (col+MOVING_LEFT)>0)
        {
            map[row][col] = EMPTY_BLOCK;
            map[row][col+=MOVING_LEFT] = PERSON_BLOCK;
        }
    else if('d'==input && (col+MOVING_RIGHT)<(COL-1))
        {
            map[row][col] = EMPTY_BLOCK; 
            map[row][col+=MOVING_RIGHT] = PERSON_BLOCK;
        }
    else if('w'==input && (row+MOVING_UP)>0)
        {
            map[row][col] = EMPTY_BLOCK;
            map[row+=MOVING_UP][col] = PERSON_BLOCK;
        }
    else if('s'==input && (row+MOVING_DOWN)<(ROW-1))
        {
            map[row][col] = EMPTY_BLOCK;
            map[row+=MOVING_DOWN][col] = PERSON_BLOCK;
        }
    else if('p'==input)
        {
            person_fire=true;     
        }

    tcgetattr(0, &info);
    info.c_lflag |= ICANON;
    tcsetattr(0, TCSANOW, &info);
}

class backend_obj : public fire_effect
{
private:
    /* data */
public:
    backend_obj();
    ~backend_obj();
};

backend_obj::backend_obj()
{
}

backend_obj::~backend_obj()
{
}


#endif