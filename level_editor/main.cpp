#include <iostream>
#include <ncurses.h>

int main()
{
	initscr();
	cbreak(); // give us keys w/o pressing return
	noecho(); // don't echo pressed keys to terminal
	keypad(stdscr, TRUE); // allow us to use more keys (F1, arrow keys etc.)

	printw("Welcome to Level Editor!");
	refresh();

	getch();

	endwin();
	return 0;
}

