#include <iostream>
#include <ncurses.h>

int main()
{
	initscr();
	cbreak(); // give us keys w/o pressing return
	noecho(); // don't echo pressed keys to terminal
	keypad(stdscr, TRUE); // allow us to use more keys (F1, arrow keys etc.)

	mvaddstr(0, 0, "Welcome to Level Editor!");
	mvaddstr(1, 0, "");
	mvaddstr(2, 0, "What are we going to do:");
	mvaddstr(3, 0, "1) Create a [N]ew map");
	mvaddstr(4, 0, "2) [O]pen existing map to edit it");
	mvaddstr(5, 0, "3) [E]xit");
	mvaddstr(6, 0, "");
	mvaddstr(7, 0, "> ");
	refresh();
getChoice:
	echo();
	char input = getch();
	noecho();
	switch (input) {
		case '1': case 'N': case 'n':
			mvaddstr(8, 0, "Oh man we're gonna make a new map!");
			break;
		case '2': case 'O': case 'o':
			mvaddstr(8, 0, "Open file! YAY!");
			break;
		case '3': case 'E': case 'e':
			endwin();
			exit(0);
			break;
		default:
			mvaddstr(8, 0, "Unrecognized choice. Try \"1\", \"O\" or \"e\".");
			mvaddstr(7, 0, ">  ");
			move(7, 2);
			refresh();
			goto getChoice;
			break;
	}
	refresh();

	getch();

	endwin();
	return 0;
}

