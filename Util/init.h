#ifndef INIT
#define INIT

// Structura pentru intrebari
typedef struct {
	char *quest;
	char answ[4][101];
	int corAnsw;
} question;

// Structura pentru datele jocului
typedef struct {
	int quest;
	int score;
	int help[2];
	int fiftyVar;
	int state;
	int rightAnsw;
} Game;

void printAnswCol (int keyInput, WINDOW *, int height, int width, question *, Game *, int col);
void refreshGameScreen (WINDOW *, int height, int width, question *, char *option[], Game *, int *, int col);
void printQuestion (int curQuest, int height, int width, WINDOW *, question *, int x);
void printTime (WINDOW *, int height, int width);
void printBorder(WINDOW *);
void loadQuestion (int nr, question *, FILE *);
void freeMem (question *, int nr);
void startGame (char *, int height, int width, WINDOW *, Game *);
void printTitle (WINDOW *, int height, int width);
void initMenu (char *argv[]);

#endif