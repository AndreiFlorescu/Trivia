#include <ncurses.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "init.h"

void printQuestion (int curQuest, int height, int width, WINDOW *menuwin, question *round, int secChoice) {
	wattron(menuwin, A_BOLD);
	mvwprintw(menuwin, height / 2 - 4, width / 2 - strlen(round[curQuest].quest) / 2 - 1, "%s", round[curQuest].quest);
	wattroff(menuwin, A_BOLD);

	if (round[curQuest].corAnsw == 0 || secChoice == 0 || secChoice == -1)
		mvwprintw(menuwin, height / 2 + 2, width / 5 - 2, "A) %s", round[curQuest].answ[0]);
	if (round[curQuest].corAnsw == 1 || secChoice == 1 || secChoice == -1)
		mvwprintw(menuwin, height / 2 + 2, width * 5 / 7 - 5, "B) %s", round[curQuest].answ[1]);
	if (round[curQuest].corAnsw == 2 || secChoice == 2 || secChoice == -1)
		mvwprintw(menuwin, height / 2 + 4, width / 5 - 2, "C) %s", round[curQuest].answ[2]);
	if (round[curQuest].corAnsw == 3 || secChoice == 3 || secChoice == -1)
		mvwprintw(menuwin, height / 2 + 4, width * 5 / 7 - 5, "D) %s", round[curQuest].answ[3]);
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

		fscanf (q_file, "%d\n", &round[i].corAnsw);
	}
	return;
}

void freeMem (question *round, int nr) {
	int i;
	for (i = 0; i < nr; ++i) {
		free(round[i].quest);
	}
	free(round);
}

void refreshGameScreen (WINDOW *menuwin, int height, int width, question *round, char option[][10], Game *game, int *curent, int col) {
	int i;

	wclear(menuwin);
	box(menuwin, 0, 0);
		
	// Printam scorul
	mvwprintw(menuwin, height / 4, width * 4 / 5, "SCOR: ");
	wattron(menuwin, COLOR_PAIR(col));
	mvwprintw(menuwin, height / 4, width * 4 / 5 + 6, "%d", game->score);
	wattroff(menuwin, COLOR_PAIR(col));
	// Pritam timpul
	time_t mytime;
	char *time_str;
	mytime = time(NULL);
	time_str = ctime(&mytime);
	time_str[strlen(time_str) - 1] = '\0';
	mvwprintw(menuwin, 0, width - strlen(time_str) - 6, "[ %s ]", time_str);
		
	if (game->help[0] == 1) {
		*curent = 1;
	}
	if (game->help[1] == 1) {
		*curent = 0;
	}

	// Printam optiunile
	for (i = 0; i < 2; ++i) {
		if (i == *curent) {
			wattron(menuwin, A_REVERSE);
		}
		if (game->help[i] == 0) {
			mvwprintw(menuwin, height - 1, 4 + i * 14, "%s", option[i]);
		}
		wattroff(menuwin, A_REVERSE);
	}

	printQuestion(game->quest, height, width, menuwin, round, game->fiftyVar);

	wrefresh(menuwin);
}

void startGame (char *qFileName, int height, int width, WINDOW *menuwin, Game *game) {
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

	int curent = 0;
	int keyInput;
	int flag = 0;

	while (game->quest < nr) {
		refreshGameScreen (menuwin, height, width, round, option, game, &curent, 4);

		keyInput = wgetch(menuwin);

		if (keyInput != 'a' && keyInput != 'b' && keyInput != 'c' && keyInput != 'd' ) {
			switch (keyInput) {
				case KEY_LEFT:
					curent--;
					break;
				case KEY_RIGHT:
					curent++;
					break;
				case 'q':
					flag = 1;
					break;
				case 10:
					if (curent == 0) {
						game->fiftyVar = rand() % 4;
						while (game->fiftyVar == round[game->quest].corAnsw) {
							game->fiftyVar = rand() % 4;
						}
					} else { 
						game->quest++;
						wclear(menuwin);
					}
					game->help[curent] = 1;
				
				default:
					break;
			}

			if (flag == 1) {
				break;
			}

			if (curent < 0) {
				curent = 0;
			}
			if (curent > 1) {
				curent = 1;
			}
			
		} else {
			if (keyInput - 'a' == round[game->quest].corAnsw) {
				game->score += 10;
				game->rightAnsw++;
				refreshGameScreen (menuwin, height, width, round, option, game, &curent, 2);
				wattron(menuwin, COLOR_PAIR(2));
			} else {
				game->score -= 5;
				refreshGameScreen (menuwin, height, width, round, option, game, &curent, 3);
				wattron(menuwin, COLOR_PAIR(3));
			}

			wattron(menuwin, A_BOLD);
			wattron(menuwin, A_BLINK);

			switch (keyInput) {
				case 'a':
					mvwprintw(menuwin, height / 2 + 2, width / 5 - 2, "A) %s", round[game->quest].answ[0]);
					break;
				case 'b':
					mvwprintw(menuwin, height / 2 + 2, width * 5 / 7 - 5, "B) %s", round[game->quest].answ[1]);
					break;
				case 'c':
					mvwprintw(menuwin, height / 2 + 4, width / 5 - 2, "C) %s", round[game->quest].answ[2]);
					break;
				case 'd':
					mvwprintw(menuwin, height / 2 + 4, width * 5 / 7 - 5, "D) %s", round[game->quest].answ[3]);
					break;
				default:
					break;
			}
			wattroff(menuwin, A_BOLD);
			wattroff(menuwin, A_BLINK);

			if (keyInput - 'a' == round[game->quest].corAnsw) {
				wattroff(menuwin, COLOR_PAIR(2));
			} else {
				wattroff(menuwin, COLOR_PAIR(3));
			}

			wrefresh(menuwin);
			sleep(2.1);

			game->quest++;
			wclear(menuwin);
			game->fiftyVar = -1;
		
			
		}
	}

	if (game->quest == nr) {
		game->state = 0;
		wclear(menuwin);
		box(menuwin, 0, 0);	
		mytime = time(NULL);
		time_str = ctime(&mytime);
		time_str[strlen(time_str) - 1] = '\0';
		mvwprintw(menuwin, 0, width - strlen(time_str) - 6, "[ %s ]", time_str);
		mvwprintw(menuwin, height / 2 - 3, width / 2 - 8, "Scor final: %d", game->score);
		
		mvwprintw(menuwin, height / 2 - 1, width / 2 - 11, "Raspunsuri corecte: ");
		wattron(menuwin, COLOR_PAIR(2));
		mvwprintw(menuwin, height / 2 - 1, width / 2 + 9, "%d", game->rightAnsw);
		wattroff(menuwin, COLOR_PAIR(2));

		mvwprintw(menuwin, height / 2, width / 2 - 11, "Raspunsuri gresite: ");
		wattron(menuwin, COLOR_PAIR(3));
		mvwprintw(menuwin, height / 2, width / 2 + 9, "%d", nr - game->rightAnsw - game->help[1]);
		wattroff(menuwin, COLOR_PAIR(3));

		if (game->help[1] == 1) {
			mvwprintw(menuwin, height / 2 + 1, width / 2 - 10, "Raspunsuri sarite: 1");
		}

		mvwprintw(menuwin, height / 2 + 5, width / 2 - 25, "Apasati orice tasta pentru a va intoarce in meniu");
		wgetch(menuwin);
	}

	freeMem(round, nr);
	wclear(menuwin);
	return;
}

void printTitle (WINDOW *menuwin, int height, int width) {
	//start_color();
	wattron(menuwin, COLOR_PAIR(1));
	mvwprintw(menuwin, height - 3, width - 28, "By Andrei Florescu - 2019");
	mvwprintw(menuwin, 2, width / 2 - 19, "_________   _________   _________");
	mvwprintw(menuwin, 3, width / 2 - 20, "/\\   _____\\ /\\   _____\\ /\\   _____\\");
	mvwprintw(menuwin, 4, width / 2 - 20, "\\ \\  \\____/ \\ \\  \\____/_\\ \\  \\____/");
	mvwprintw(menuwin, 5, width / 2 - 19, "\\ \\  \\      \\ \\______  \\\\ \\  \\");
	mvwprintw(menuwin, 6, width / 2 - 18, "\\ \\  \\______\\_ _____\\  \\\\ \\  \\______");
	mvwprintw(menuwin, 7, width / 2 - 17, "\\ \\________\\ /\\________\\\\ \\________\\");
	mvwprintw(menuwin, 8, width / 2 - 16, "\\/________/ \\/________/ \\/________/");
	mvwprintw(menuwin, 11, width / 2 - 9, "Cine Stie Castiga!");
	wattroff(menuwin, COLOR_PAIR(1));
}

void initMenu (char *qFileName) {
	initscr();
	noecho();
	curs_set(0);
	start_color();

	init_pair(1, COLOR_CYAN, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_RED, COLOR_BLACK);
	init_pair(4, COLOR_WHITE, COLOR_BLACK);

	time_t mytime;
	char *time_str;
	Game game;

	int height, width;
	getmaxyx(stdscr, height, width);

	WINDOW *menuwin = newwin (height, width, 0, 0);
	refresh();

	keypad(menuwin, true);
	

	char button[3][12];
	strcpy(button[0], "New Game");
	strcpy(button[1], "Resume Game");
	strcpy(button[2], "Quit");

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
			mvwprintw(menuwin, height * 3 / 5 + i + 1, width / 2 - strlen(button[i]) / 2, "%s", button[i]);
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
					
					game.quest = 0;
					game.score = 0;
					game.help[0] = 0;
					game.help[1] = 0;
					game.fiftyVar = -1;
					game.state = 1;
					game.rightAnsw= 0;
					startGame(qFileName, height, width, menuwin, &game);
					break;
				case 1:
					if (game.state == 0) {
						break;
					} else {
						startGame(qFileName, height, width, menuwin, &game);
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