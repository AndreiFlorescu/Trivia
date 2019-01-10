#include <ncurses.h>
#include <stdio.h>

#include "Util/init.h"

int main (int argc, char ** argv) {

	if (argc == 1) {
		printf ("[Eroare]: Nu s-au dat argumente in linia de comanda.\n");
		return 1;
	}

	initMenu(argv[1]);

	return 0;
}