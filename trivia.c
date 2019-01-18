#include <ncurses.h>
#include <stdio.h>

#include "Util/init.h"

int main (int argc, char *argv[]) {

	// Verificam daca am primit argumente
	if (argc == 1) {
		printf ("[Eroare]: Nu s-au dat argumente in linia de comanda.\n");
		return 1;
	}

	// Intram in functia care porneste programul
	initMenu(argv);

	return 0;
}