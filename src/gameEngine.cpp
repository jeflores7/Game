// MAIN FILE FOR DUNGEON-CRAWLING GAME

#include <cstdlib>
#include <iostream>
#include <time.h>
#include "player.h"
#include "layer.h"
#include <ncurses.h>


using namespace std;

bool startedCurses = false;

void startCurses();
void endCurses();
WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);

int main() {
	srand(time(NULL));

  layer* map;
  map = new layer(NULL, NULL, 0);

  // INITIALIZE THE PLAYER
  player* the_player = new player();
  the_player->setX(map->Get_player_x());
  the_player->setY(map->Get_player_y());
  map->AddCharacter(the_player, map->Get_player_x(),map->Get_player_y());
	
  int input;
  action command;
  
  startCurses();
  
  if(has_colors() == FALSE)
	{	endwin();
		printf("Your terminal does not support color.\n");
		exit(1);
	}
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	init_pair(2, COLOR_WHITE, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_RED, COLOR_BLACK);
	init_pair(5, COLOR_YELLOW, COLOR_BLACK);
	init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
  
  attron(A_BOLD);
  move(LINES/2-5, COLS/2-6);
  printw("overthrowing");
  attroff(A_BOLD);
  move(LINES/2-4, COLS/2-4);
  printw("OZNATSOC");
  move(LINES/2+5, COLS/2-13);
  attron(A_BLINK);
  printw("- Press any key to start -");
  attroff(A_BLINK);

  refresh();




  input = getch();
  int screenCleared = clear(); // Clearing the screen (for some reason) return an int.

  move(5, 0);
  attron(A_UNDERLINE);
  attron(A_BOLD);
  printw("Mission: \n");
  attroff(A_UNDERLINE);
  attroff(A_BOLD);
  printw("You are a brave hero hunting OZNATSOC THE OBSTREPEROUS\ndeep into his underground lair to avenge your dog, whom\nthe evil overlord kicked. Find Oznatsoc and kill him!\nWatch out for his minions along the way and collect some\ntreasure too while you're at it.\n");
  attron(A_UNDERLINE);
  attron(A_BOLD);
  printw("Instructions: \n");
  attroff(A_BOLD);
  attroff(A_UNDERLINE);
  printw("WASD or arrow keys to move\n");
  printw("i to access inventory\n");
  printw("0 to quit\n");
  move(LINES/2+5, COLS/2-13);
  attron(A_BLINK);
  printw("- Press any key to start -");
  move(LINES/2+6, COLS/2);
  attroff(A_BLINK);
  
  input = getch();

  // INITIALIZE WINDOWS
	WINDOW* mapw;
	mapw = create_newwin(23, 60, 0, 20);
	WINDOW* inventoryw;
	inventoryw = create_newwin(24, 20, 0, 0);
	WINDOW* infow;
	infow = create_newwin(1, 60, 23, 20);
	wclear(infow);
	WINDOW* characterw;
	characterw = create_newwin(24, 20, 0, 0);
	WINDOW* menuw;
	menuw = create_newwin(24, 20, 0, 0);
	WINDOW* sidepanel;

  int char_x;
  int char_y;
  character* to_move;
  bool inventory = false;
  bool menu = false;
  
  do {
		
		// Print to map window
  	   map->Print(mapw);
		wrefresh(mapw);
		
		wmove(infow, 1, 1);
		// Print to info window
		wrefresh(infow);
		
		char_x = the_player->getX();
		char_y = the_player->getY();

		input = getch();

		switch (input){
			case 'w': case KEY_UP:
			  command = MOVE_UP;
			  break;
			case 'a': case KEY_LEFT:
			  command = MOVE_LEFT;
			  break;
			case 'd': case KEY_RIGHT:
			  command = MOVE_RIGHT;
			  break;
			case 's': case KEY_DOWN:
			  command = MOVE_DOWN;
			  break;
			case 'i':
			  command = INVENTORY;
			  inventory = !inventory;
			  break;
			case 'm':
				command = MENU;
				menu = !menu;
				break;
			case '0':
			  command = QUIT;
			  break;
			default:
			  command = INVALID;
			  break;
		} // end switch
		
		// move the character
		map->MoveCharacter(char_x, char_y, command);
		
	  // UPDATE MAP LAYER
	  map = map->AscendOrDescend(the_player);
	  
	  // UPDATE SIDE PANEL
		if(inventory) {
			the_player->PrintInventory(inventoryw);
			sidepanel = inventoryw;
		}
		else if(menu) {
			sidepanel = menuw;
		}
		else {
			sidepanel = characterw;
		}
        // Print to side window
        wrefresh(sidepanel);
	  
  } while(command != QUIT);
  
  endCurses();
  
  return 0;
}

void startCurses() {
  if(startedCurses)
    refresh();
  else{
    initscr();
    cbreak();
    noecho();
    intrflush(stdscr, false);
    keypad(stdscr, true);
    atexit(endCurses);
    startedCurses = true;
  }
}

void endCurses() {
  if(startedCurses && !isendwin())
    endwin();
}

WINDOW *create_newwin(int height, int width, int starty, int startx)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);	
	
	wrefresh(local_win);

	return local_win;
}

void destroy_win(WINDOW *local_win)
{	
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wrefresh(local_win);
	delwin(local_win);
}
