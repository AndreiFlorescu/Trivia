#include <ncurses.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "init.h"

void printQuestion (int curQuest, int height, int width, WINDOW *menuwin, question *round) {
	mvwprintw(menuwin, height / 2 - 4, width / 2 - strlen(round[curQuest].quest) / 2, "%s", round[curQuest].quest);
	mvwprintw(menuwin, height / 2 + 2, width / 5, "%s", round[curQuest].answ[0]);
	mvwprintw(menuwin, height / 2 + 2, width * 5 / 7, "%s", round[curQuest].answ[1]);
	mvwprintw(menuwin, height / 2 + 4, width / 5, "%s", round[curQuest].answ[2]);
	mvwprintw(menuwin, height / 2 + 4, width * 5 / 7, "%s", round[curQuest].answ[3]);
}

void loadQuestion (int nr, question *round, FILE *q_file) {
	int i;
	for (i = 0; i < nr; ++i) {
		round[i].quest = malloc (101 * sizeof(char));
		fgets(round[i].quest, 100, q_file);
		round[i].quest[strlen(round[i].quest) - 1] = '\0';

		fgets(round[i].answ[0], 100, q_file);
		fgets(round[i].answ[1], 100, q_file);
		fgets(round[i].answ[2], 100, q_file);
		fgets(round[i].answ[3], 100, q_file);
		round[i].answ[0][strlen(round[i].answ[0]) - 1] = '\0';
		round[i].answ[1][strlen(round[i].answ[1]) - 1] = '\0';
		round[i].answ[2][strlen(round[i].answ[2]) - 1] = '\0';
		round[i].answ[3][strlen(round[i].answ[3]) - 1] = '\0';

		fscanf (q_file, "%d", &round[i].corAnsw);
	}
	return;
}

void startGame (char *qFileName, int height, int width, WINDOW *menuwin) {
	FILE *q_file = fopen (qFileName, "r");

	time_t mytime;
	char *time_str;

	int nr;
	fscanf (q_file, "%d\n", &nr);
	question *round = malloc ((nr + 1) * sizeof(question));
	loadQuestion(nr, round, q_file);

	wclear(menuwin);

	char option[2][10];
	strcpy(option[0], "[ 50/50 ]");
	strcpy(option[1], "[ Skip ]");

	int curQuest = 0;
	int i;
	int curent = 0;
	while (1) {
		box(menuwin, 0, 0);
		
// Pritam timpul
		mytime = time(NULL);
		time_str = ctime(&mytime);
		time_str[strlen(time_str) - 1] = '\0';
		mvwprintw(menuwin, 0, width - strlen(time_str) - 4, "[%s]", time_str);
		
// Printam optiunile
		for (i = 0; i < 2; ++i) {
			if (i == curent) {
				wattron(menuwin, A_REVERSE);
			}
			mvwprintw(menuwin, height - 1, 4 + i * 14, "%s", option[i]);
			wattroff(menuwin, A_REVERSE);
		}

		printQuestion(curQuest, height, width, menuwin, round);
		wrefresh(menuwin);

		break;
	}

	wrefresh(menuwin);
	wgetch(menuwin);
	wclear(menuwin);
	return;
}

void printTitle (WINDOW *menuwin, int height, int width) {
	mvwprintw(menuwin, height - 3, width - 28, "By Andrei Florescu - 2019");
	mvwprintw(menuwin, 2, width / 2 - 19, "_________   _________   _________");
	mvwprintw(menuwin, 3, width / 2 - 20, "/\\   _____\\ /\\   _____\\ /\\   _____\\");
	mvwprintw(menuwin, 4, width / 2 - 20, "\\ \\  \\____/ \\ \\  \\____/_\\ \\  \\____/");
	mvwprintw(menuwin, 5, width / 2 - 19, "\\ \\  \\      \\ \\______  \\\\ \\  \\");
	mvwprintw(menuwin, 6, width / 2 - 18, "\\ \\  \\______\\_ _____\\  \\\\ \\  \\______");
	mvwprintw(menuwin, 7, width / 2 - 17, "\\ \\________\\ /\\________\\\\ \\________\\");
	mvwprintw(menuwin, 8, width / 2 - 16, "\\/________/ \\/________/ \\/________/");
	mvwprintw(menuwin, 11, width / 2 - 9, "Cine Stie Castiga!");
}

void initMenu (char *qFileName) {
	initscr();
	noecho();
	curs_set(0);

	time_t mytime;
	char *time_str;

	int height, width;
	getmaxyx(stdscr, height, width);

	WINDOW *menuwin = newwin (height, width, 0, 0);
	refresh();

	keypad(menuwin, true);
	
	char button[3][9];
	strcpy(button[0], "New Game");
	strcpy(button[1], "Resume");
	strcpy(button[2], "Quit");

	int gameOn = 0;
	int curent = 0;
	int keyInput;
	int i;
	while (1) {
		box(menuwin, 0, 0);
		printTitle (menuwin, height, width);

		mytime = time(NULL);
		time_str = ctime(&mytime);
		time_str[strlen(time_str) - 1] = '\0';
		mvwprintw(menuwin, 0, width - strlen(time_str) - 6, "[ %s ]", time_str);

		for (i = 0; i < 3; ++i) {
			if (i == curent) {
				wattron(menuwin, A_REVERSE);
			}
			mvwprintw(menuwin, height * 3 / 5 + i + 1, width / 2 - 4, "%s", button[i]);
			wattroff(menuwin, A_REVERSE);
		}
		wrefresh(menuwin);
	
		keyInput = wgetch(menuwin);

		switch (keyInput) {
			case KEY_UP:
				curent--;
				break;
			case KEY_DOWN:
				curent++;
				break;
			case 10:
				switch (curent) {
				case 0:
					gameOn = 1;
					startGame(qFileName, height, width, menuwin);
					break;
				case 1:
					if (gameOn == 0) {
						break;
					}
					break;
				case 2:
					endwin();
					return;
			default:
				break;
		}
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