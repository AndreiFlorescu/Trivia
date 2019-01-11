#include <ncurses.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "init.h"

#define noButMenu 4

void printQuestion (int curQuest, int height, int width, WINDOW *menuwin, question *round, int secChoice) {
	wattron(menuwin, A_BOLD);
	mvwprintw(menuwin, height / 2 - 4, width / 2 - strlen(round[curQuest].quest) / 2 - 1, "%s", round[curQuest].quest);
	wattroff(menuwin, A_BOLD);

	if (round[curQuest].corAnsw == 0 || secChoice == 0 || secChoice == -1) {
		wattron(menuwin, A_BOLD);
		mvwprintw(menuwin, height / 2 + 2, width / 5 - 2, "A) ");
		wattroff(menuwin, A_BOLD);
		mvwprintw(menuwin, height / 2 + 2, width / 5 + 1, "%s", round[curQuest].answ[0]);
	}
	if (round[curQuest].corAnsw == 1 || secChoice == 1 || secChoice == -1) {
		wattron(menuwin, A_BOLD);
		mvwprintw(menuwin, height / 2 + 2, width * 5 / 7 - 5, "B) ");
		wattroff(menuwin, A_BOLD);
		mvwprintw(menuwin, height / 2 + 2, width * 5 / 7 - 2, "%s", round[curQuest].answ[1]);
	}
	if (round[curQuest].corAnsw == 2 || secChoice == 2 || secChoice == -1) {
		wattron(menuwin, A_BOLD);
		mvwprintw(menuwin, height / 2 + 4, width / 5 - 2, "C) ");
		wattroff(menuwin, A_BOLD);
		mvwprintw(menuwin, height / 2 + 4, width / 5 + 1, "%s", round[curQuest].answ[2]);
	}
	if (round[curQuest].corAnsw == 3 || secChoice == 3 || secChoice == -1) {
		wattron(menuwin, A_BOLD);
		mvwprintw(menuwin, height / 2 + 4, width * 5 / 7 - 5, "D) ");
		wattroff(menuwin, A_BOLD);
		mvwprintw(menuwin, height / 2 + 4, width * 5 / 7 - 2, "%s", round[curQuest].answ[3]);
	}
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

void printTime (WINDOW *menuwin, int height, int width) {
	time_t mytime;
	char *time_str;
	mytime = time(NULL);
	time_str = ctime(&mytime);
	time_str[strlen(time_str) - 1] = '\0';

	wattron(menuwin, COLOR_PAIR(5));
	wattron(menuwin, A_BOLD);
	mvwprintw(menuwin, 0, width - strlen(time_str) - 6, "[ %s ]", time_str);
	wattroff(menuwin, A_BOLD);
	wattroff(menuwin, COLOR_PAIR(5));
}

void refreshGameScreen (WINDOW *menuwin, int height, int width, question *round, char *option[], Game *game, int *curent, int col) {
	int i;

	wclear(menuwin);
	getmaxyx(menuwin, height, width);

	printBorder(menuwin);	

	// Instructiuni de utilizare
	wattron(menuwin, COLOR_PAIR(5));
	for (i = 1; i < width - 1; ++i) {
		mvwprintw(menuwin, height - 7, i, "-");
	}
	wattroff(menuwin, COLOR_PAIR(5));

	mvwprintw(menuwin, height - 5, width / 2 - 37, "Pentru a selecta un raspuns apasati tasta a, b, c sau d");
	mvwprintw(menuwin, height - 4, width / 2 - 37, "Pentru a selecta o varianta ajutatoare folositi sagetile si apasati ENTER");
	mvwprintw(menuwin, height - 3, width / 2 - 37, "Pentru a va intoarce in meniu apasati tasta q");

	// Printam scorul
	if (game->score > 99 || game->score < -9) {
		mvwprintw(menuwin, height / 4 - 4, width * 4 / 5 - 2, "+-----------+");
		mvwprintw(menuwin, height / 4 - 3, width * 4 / 5 - 2, "|           |");
		mvwprintw(menuwin, height / 4 - 2, width * 4 / 5 - 2, "+-----------+");

	} else {
		mvwprintw(menuwin, height / 4 - 4, width * 4 / 5 - 2, "+----------+");
		mvwprintw(menuwin, height / 4 - 3, width * 4 / 5 - 2, "|          |");
		mvwprintw(menuwin, height / 4 - 2, width * 4 / 5 - 2, "+----------+");
	}
	wattron(menuwin, A_BOLD);
	mvwprintw(menuwin, height / 4 - 3, width * 4 / 5, "SCOR: ");
	wattron(menuwin, COLOR_PAIR(col));
	mvwprintw(menuwin, height / 4 - 3, width * 4 / 5 + 6, "%d", game->score);
	wattroff(menuwin, COLOR_PAIR(col));
	wattroff(menuwin, A_BOLD);

	// Pritam timpul
	printTime(menuwin, height, width);

	if (game->help[0] >= 1) {
		*curent = 1;
	}
	if (game->help[1] >= 1) {
		*curent = 0;
	}

	// Printam optiunile
	wattron(menuwin, A_BOLD);
	wattron(menuwin, COLOR_PAIR(5));
	for (i = 0; i < 2; ++i) {
		if (i == *curent) {
			wattron(menuwin, A_REVERSE);
		}
		if (game->help[i] == 0) {
			mvwprintw(menuwin, height - 1, 4 + i * 14, "%s", option[i]);
		}
		wattroff(menuwin, A_REVERSE);
	}
	wattroff(menuwin, COLOR_PAIR(5));
	wattroff(menuwin, A_BOLD);

	printQuestion(game->quest, height, width, menuwin, round, game->fiftyVar);

	wrefresh(menuwin);
}

void printAnswCol (int keyInput, WINDOW *menuwin, int height, int width, question *round, Game *game, int col) {
	wattron(menuwin, COLOR_PAIR(col));
	wattron(menuwin, A_BOLD);
	
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

	wattroff(menuwin, COLOR_PAIR(col));
	wattroff(menuwin, A_BOLD);
}

void startGame (char *qFileName, int height, int width, WINDOW *menuwin, Game *game) {
	FILE *q_file = fopen (qFileName, "r");

	int nr;
	fscanf (q_file, "%d\n", &nr);
	question *round = malloc ((nr + 1) * sizeof(question));
	loadQuestion(nr, round, q_file);
	fclose(q_file);

	wclear(menuwin);

	char *option[] = {"[ 50/50 ]", "[ Skip ]"};

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
						game->fiftyVar = -1;
						game->quest++;
						wclear(menuwin);
					}
					game->help[curent] = game->quest + 1;
					break;
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
				wattron(menuwin, A_BLINK);
				printAnswCol (keyInput, menuwin, height, width, round, game, 2);
				wattroff(menuwin, A_BLINK);					
			} else {
				if (game->help[0] == game->quest + 1 && keyInput - 'a' != game->fiftyVar) {
					continue;
				}
				game->score -= 5;
				refreshGameScreen (menuwin, height, width, round, option, game, &curent, 3);
				
				printAnswCol (keyInput, menuwin, height, width, round, game, 3);
				
				wattron(menuwin, A_BLINK);
				printAnswCol (round[game->quest].corAnsw + 'a', menuwin, height, width, round, game, 2);		
				wattroff(menuwin, A_BLINK);										
			}

			wrefresh(menuwin);

			keypad(menuwin, FALSE);
			sleep(2.5);
			keypad(menuwin, TRUE);

			game->quest++;
			wclear(menuwin);
			game->fiftyVar = -1;
		
			
		}
	}

	if (game->quest == nr) {
		game->state = 0;
		wclear(menuwin);

		printBorder(menuwin);

		printTime(menuwin, height, width);

		mvwprintw(menuwin, height / 2 - 3, width / 2 - 8, "Scor final: ", game->score);
		wattron(menuwin, A_BOLD);
		mvwprintw(menuwin, height / 2 - 3, width / 2 + 4, "%d", game->score);
		wattroff(menuwin, A_BOLD);

		mvwprintw(menuwin, height / 2 - 1, width / 2 - 11, "Raspunsuri corecte: ");
		wattron(menuwin, COLOR_PAIR(2));
		wattron(menuwin, A_BOLD);
		mvwprintw(menuwin, height / 2 - 1, width / 2 + 9, "%d", game->rightAnsw);
		wattroff(menuwin, COLOR_PAIR(2));
		wattroff(menuwin, A_BOLD);

		mvwprintw(menuwin, height / 2, width / 2 - 11, "Raspunsuri gresite: ");
		wattron(menuwin, COLOR_PAIR(3));
		wattron(menuwin, A_BOLD);
		mvwprintw(menuwin, height / 2, width / 2 + 9, "%d", nr - game->rightAnsw - (game->help[1] == 0 ? 0 : 1));
		wattroff(menuwin, COLOR_PAIR(3));
		wattroff(menuwin, A_BOLD);

		if (game->help[1] != 0) {
			mvwprintw(menuwin, height / 2 + 1, width / 2 - 10, "Raspunsuri sarite: ");
			wattron(menuwin, A_BOLD);
			mvwprintw(menuwin, height / 2 + 1, width / 2 + 9, "1");
			wattroff(menuwin, A_BOLD);
		}

		mvwprintw(menuwin, height * 2 / 3, width / 2 - 25, "Apasati orice tasta pentru a va intoarce in meniu");
		wgetch(menuwin);
	}

	freeMem(round, nr);
	wclear(menuwin);
	return;
}

void printTitle (WINDOW *menuwin, int height, int width) {
	wattron(menuwin, COLOR_PAIR(1));
	wattron(menuwin, A_BOLD);
	mvwprintw(menuwin, height - 3, width - 28, "by Andrei Florescu - 2019");
	mvwprintw(menuwin, height / 10, width / 2 - 19, "_________   _________   _________");
	mvwprintw(menuwin, height / 10 + 1, width / 2 - 20, "/\\   _____\\ /\\   _____\\ /\\   _____\\");
	mvwprintw(menuwin, height / 10 + 2, width / 2 - 20, "\\ \\  \\____/ \\ \\  \\____/_\\ \\  \\____/");
	mvwprintw(menuwin, height / 10 + 3, width / 2 - 19, "\\ \\  \\      \\ \\______  \\\\ \\  \\");
	mvwprintw(menuwin, height / 10 + 4, width / 2 - 18, "\\ \\  \\______\\_ _____\\  \\\\ \\  \\______");
	mvwprintw(menuwin, height / 10 + 5, width / 2 - 17, "\\ \\________\\ /\\________\\\\ \\________\\");
	mvwprintw(menuwin, height / 10 + 6, width / 2 - 16, "\\/________/ \\/________/ \\/________/");
	mvwprintw(menuwin, height / 10 + 9, width / 2 - 9, "Cine Stie Castiga!");
	wattroff(menuwin, A_BOLD);
	wattroff(menuwin, COLOR_PAIR(1));
}

void printBorder (WINDOW *menuwin) {
	wattron(menuwin, COLOR_PAIR(5));
	wattron(menuwin, A_BOLD);
	box(menuwin, 0, 0);
	wattroff(menuwin, A_BOLD);
	wattroff(menuwin, COLOR_PAIR(5));
}

void initMenu (char *argv[]) {
	initscr();
	noecho();
	curs_set(0);
	start_color();

	init_pair(1, COLOR_CYAN, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_RED, COLOR_BLACK);
	init_pair(4, COLOR_WHITE, COLOR_BLACK);
	init_pair(5, COLOR_WHITE, COLOR_CYAN);

	Game game;
	game.state = 0;

	int height, width;
	getmaxyx(stdscr, height, width);

	WINDOW *menuwin = newwin (height, width, 0, 0);
	refresh();

	keypad(menuwin, TRUE);

	char *button[] = { "New Game", "Resume Game", "Quit", "Resume last session"};

	FILE *save_file;

	int curent = 0;
	int keyInput;
	int i;
	while (1) {
		wclear(menuwin);
		getmaxyx(menuwin, height, width);
		printTitle (menuwin, height, width);
		printBorder(menuwin);

		printTime(menuwin, height, width);

		wattron(menuwin, A_BOLD);
		for (i = 0; i < noButMenu; ++i) {
			if (i == curent) {
				
				wattron(menuwin, A_REVERSE);
			}
			mvwprintw(menuwin, height * 2 / 3 + i + 1, width / 2 - strlen(button[i]) / 2, "%s", button[i]);
			wattroff(menuwin, A_REVERSE);
			
		}
		wattroff(menuwin, A_BOLD);

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
					game.quest = 0;
					game.score = 0;
					game.help[0] = 0;
					game.help[1] = 0;
					game.fiftyVar = -1;
					game.state = 1;
					game.rightAnsw = 0;
					startGame(argv[1], height, width, menuwin, &game);
					break;
				case 1:
					if (game.state == 1) {
						startGame(argv[1], height, width, menuwin, &game);
					}
					break;
				case 2:
					save_file = fopen(argv[2], "w");
					fprintf(save_file, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n", 	game.quest,	
																		game.score, 
																		game.help[0],
																		game.help[1],
																		game.fiftyVar,
																		game.state,
																		game.rightAnsw);
					endwin();
					return;
				case 3:
					save_file = fopen(argv[2], "r");
					fscanf(save_file, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n", 	&game.quest,	
																		&game.score, 
																		&game.help[0],
																		&game.help[1],
																		&game.fiftyVar,
																		&game.state,
																		&game.rightAnsw);
					if (game.state == 1) {
						startGame(argv[1], height, width, menuwin, &game);
					}
					break;
			default:
				break;
		}
			default:
				break;
		}

		if (curent < 0) {
			curent = 0;
		}
		if (curent > noButMenu - 1) {
			curent = noButMenu - 1;
		}
	}
	fclose(save_file);
}