#include <ncurses.h>
#include <string.h>

#include "init.h"

int initMenu () {
	initscr();
	noecho();

	int height, width;
	getmaxyx(stdscr, height, width);

	WINDOW *menuwin = newwin (height, width, 0, 0);
	keypad(menuwin, true);

	refresh();

	box(menuwin, 0, 0);
	
	mvwprintw(menuwin, height - 2, width - 11, "AF - 2019");
	mvwprintw(menuwin, 2, width / 2 - 19, "_________   _________   _________");
	mvwprintw(menuwin, 3, width / 2 - 20, "/\\   _____\\ /\\   _____\\ /\\   _____\\");
	mvwprintw(menuwin, 4, width / 2 - 20, "\\ \\  \\____/ \\ \\  \\____/_\\ \\  \\____/");
	mvwprintw(menuwin, 5, width / 2 - 19, "\\ \\  \\      \\ \\______  \\\\ \\  \\");
	mvwprintw(menuwin, 6, width / 2 - 18, "\\ \\  \\______\\_ _____\\  \\\\ \\  \\______");
	mvwprintw(menuwin, 7, width / 2 - 17, "\\ \\________\\ /\\________\\\\ \\________\\");
	mvwprintw(menuwin, 8, width / 2 - 16, "\\/________/ \\/________/ \\/________/");
	mvwprintw(menuwin, 11, width / 2 - 9, "Cine Stie Castiga!");

	char button[3][9];
	strcpy(button[0], "New Game");
	strcpy(button[1], "Resume");
	strcpy(button[2], "Quit");

	int curent = 0;
	int keyInput;
	int i;
	while (1) {
		
		for (i = 0; i < 3; ++i) {
			if (i == curent) {
				wattron(menuwin, A_REVERSE);
			}
			mvwprintw(menuwin, height * 3 / 5 + i + 1, width / 2 - 4, "%s", button[i]);
			wattroff(menuwin, A_REVERSE);
		}
		wrefresh(menuwin);
		wmove (menuwin, 0, 0);
	
		keyInput = wgetch(menuwin);

		switch (keyInput) {
			case KEY_UP:
				curent--;
				break;
			case KEY_DOWN:
				curent++;
				break;
			case 10:
				
				return curent;
			default:
				break;
		}

		if (curent < 0) {
			curent = 0;
		}
		if (curent > 2) {
			curent = 2;
		}

	}
}