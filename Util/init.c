#include <ncurses.h>
#include <string.h>
// #include <stdio.h>

#include "init.h"

void initMenu () {
	initscr();
	noecho();

	int height, width;
	getmaxyx(stdscr, height, width);

	WINDOW *menuwin = newwin (height, width, 0, 0);
	refresh();

	box(menuwin, 0, 0);
	
	mvwprintw(menuwin, height - 1, width - 11, "AF - 2019");
	mvwprintw(menuwin, 1, width / 2 - 19, "_________   _________   _________");
	mvwprintw(menuwin, 2, width / 2 - 20, "/\\   _____\\ /\\   _____\\ /\\   _____\\");
	mvwprintw(menuwin, 3, width / 2 - 20, "\\ \\  \\____/ \\ \\  \\____/_\\ \\  \\____/");
	mvwprintw(menuwin, 4, width / 2 - 19, "\\ \\  \\      \\ \\______  \\\\ \\  \\");
	mvwprintw(menuwin, 5, width / 2 - 18, "\\ \\  \\______\\_ _____\\  \\\\ \\  \\______");
	mvwprintw(menuwin, 6, width / 2 - 17, "\\ \\________\\ /\\________\\\\ \\________\\");
	mvwprintw(menuwin, 7, width / 2 - 16, "\\/________/ \\/________/ \\/________/");
	mvwprintw(menuwin, 8, width / 2 - 9, "Cine Stie Castiga!");

	wrefresh(menuwin);
	
	/*string *butoane = malloc (4 * sizeof(string));
	butoane[0] = "New Game";
	butoane[1] = "Resume";
	butoane[2] = "Quit";
*/

	string butoane[3] = {"New Game", "Resume", "Quit"};
	int curent = 0;
	//while (1) {
		int i;
		for (i = 0; i < 3; ++i) {
			if (i == curent) {
				wattron(menuwin, A_REVERSE);
			}
			mvwprintw(menuwin, height * 3 / 5 + i, width / 2, butoane[i].c_str());
			wattroff(menuwin, A_REVERSE);
		}

	//}

	getch();
	endwin();
}