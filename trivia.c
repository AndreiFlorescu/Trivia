#include <ncurses.h>
#include <stdio.h>

#include "Util/init.h"

int main (int argc, char ** argv) {

	if (argc == 1) {
		printf ("[Eroare]: Nu s-au dat argumente in linia de comanda.\n");
		return 1;
	}

	int gamePaused = false;

	while (1) {
		int option = initMenu();
	
		switch (option) {
			case 0:
				startGame();
				break;
			case 1:
				if (gamePaused == false) {
					break;
				}
				break;
			case 2:
				endwin();
				return 0;
			default:
				break;
		} 
	}


	return 0;
}