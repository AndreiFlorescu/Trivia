#ifndef INIT
#define INIT

typedef struct {
	char *quest;
	char answ[4][101];
	int corAnsw;
} question;

void printQuestion (int curQuest, int height, int width, WINDOW *, question *);
void loadQuestion (int nr, question *, FILE *);
void startGame (char *, int height, int width, WINDOW *);
void printTitle (WINDOW *, int height, int width);
void initMenu (char *);

#endif