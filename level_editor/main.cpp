#include <iostream>
#include <ncurses.h>

void newMap();

int main()
{
	initscr();
	cbreak(); // give us keys w/o pressing return
	noecho(); // don't echo pressed keys to terminal
	keypad(stdscr, TRUE); // allow us to use more keys (F1, arrow keys etc.)

	newMap();
	endwin();
	return 0;

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
			newMap();
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

	endwin();
	return 0;
}

// TODO:
// Filling
// Undo

void newMap()
{
	clear();
	curs_set(0);

	const struct { int w, h, d; } mapSize = { 10, 5, 10 }; // add asking
	int level = 0;
	int input = 'r';
	int map[mapSize.d][mapSize.h][mapSize.w];
	for (int z = 0; z < mapSize.d; z++) {
		for (int y = 0; y < mapSize.h; y++) {
			for (int x = 0; x < mapSize.w; x++) {
				map[z][y][x] = 0;
			}
		}
	}
	const int xof = 1, yof = 2;
	WINDOW *mapw = newwin(mapSize.d+2, mapSize.w*2+2+1, yof, xof);
	box(mapw, 0, 0);
	struct { int x, y; } cursor = { 0, 0 };

	const int helpTextXof = 30, helpTextYof = 2, helpTextSize = 7;
	const char helpText[helpTextSize][35] = {
		"Keys:                             ",
		"arrow keys - move in current depth",
		"j/k        - up/down depth level  ",
		"space/t    - toggle tile          ",
		"z          - set                  ",
		"x          - erase                ",
		"q          - quit                 "
	};
	for (int i = 0; i < helpTextSize; i++)
		mvaddstr(helpTextYof+i, helpTextXof, helpText[i]);

	while (input != 'q') {
		mvprintw(0, 0, "Depth level: %d", level, cursor.x, cursor.y);

		for (int z = 0; z < mapSize.d; z++) {
			mvwprintw(mapw, z+1, 0, "%-2d", z);
			for (int x = 0; x < mapSize.w; x++) {
				int tile = map[z][level][x];
				mvwaddch(mapw, z+1, x*2+2,   tile ? ACS_BLOCK : ' ');
				mvwaddch(mapw, z+1, x*2+2+1, tile ? ACS_BLOCK : ' ');
			}
		}
		for (int x = 0; x < mapSize.w; x++)
			mvwprintw(mapw, 0, x*2+2, "%-2d", x);

		int onAfilledTile = map[cursor.y][level][cursor.x];
		if (onAfilledTile)
			wattron(mapw, A_REVERSE);
		mvwaddstr(mapw, cursor.y+1, cursor.x*2+2, "##");
		if (onAfilledTile)
			wattroff(mapw, A_REVERSE);
		move(yof-1, 1);
		clrtoeol();
		mvaddstr(yof-1, cursor.x*2+xof+2, "vv");
		for (int z = 0; z < mapSize.d; z++)
			mvaddch(yof+z+1, xof-1, ' ');
		mvaddch(cursor.y+yof+1, xof-1, '>');

		refresh();
		wrefresh(mapw);
		input = getch();

		switch (input) {
			// Moving =====================================================
			case KEY_LEFT:
				if (cursor.x > 0)
					cursor.x--;
				break;
			case KEY_RIGHT:
				if (cursor.x < mapSize.w-1)
					cursor.x++;
				break;
			case KEY_UP:
				if (cursor.y > 0)
					cursor.y--;
				break;
			case KEY_DOWN:
				if (cursor.y < mapSize.d-1)
					cursor.y++;
				break;
			case 'j':
				if (level > 0)
					level--;
				break;
			case 'k':
				if (level < 100) // sanity
					level++;
				break;
			// Modifying ==================================================
			case 32: // space
			case 't':
				map[cursor.y][level][cursor.x] = !map[cursor.y][level][cursor.x];
				break;
			case 'z':
				map[cursor.y][level][cursor.x] = 1;
				break;
			case 'x':
				map[cursor.y][level][cursor.x] = 0;
				break;
			default:
				break;
		}
	}

	delwin(mapw);
	curs_set(1);
}

