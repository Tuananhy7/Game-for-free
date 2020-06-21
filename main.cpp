#include <iostream>
#include <thread>
#include <mutex>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include "classes.h"

using namespace std;

mutex mutex_game;

void display(display_screen *screen01)
{
	mutex_game.lock();
	screen01->map_display();
	mutex_game.unlock();
	usleep(10000);
}

void control(person *person01, fire_effect *bullet01)
{
	mutex_game.lock();
    person01->control_person();
	if(person01->person_fire == true)
	{
		bullet01->bullet.x=person01->col;
		bullet01->bullet.y=person01->row;
		bullet01->check = true;
		person01->person_fire = false;
	}	
	mutex_game.unlock();
    //usleep(50000);
}

void backend(backend_obj *handler, fire_effect *bullet01)
{
	mutex_game.lock();
	bullet01->bullet_fly();
	mutex_game.unlock();
	usleep(30000);
}

int main()
{
	bool End_game = false;
    map_game map01;
	map01.map_init();
	fire_effect bullet01;
	display_screen screen01;
	person person01;
	backend_obj handler;  
		while(End_game != true)
		{
			thread th01(display, &screen01);
			thread th02(control, &person01, &bullet01);
			thread th03(backend, &handler, &bullet01);
            th01.join();
			th02.join();
			th03.join();
		}
	mutex_game.~mutex();
	return 0;
}