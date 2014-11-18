#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ncurses.h>

const int FIELD_SIZE = 20;

/* model */
typedef struct {
	char field[FIELD_SIZE][FIELD_SIZE];
	int hero_x, hero_y, movement, score;
} GameData;

/* prototypes */
GameData  init_game();
void      print_field(GameData game);
char      get_char(bool hero_has_spawned);

int
main()
{
	/* Initialize seed for rand() */
	srand(time(NULL));

	GameData game = init_game();

	initscr(); /* Init ncurses */
	char input;
	bool running = true;

	do {
		print_field(game);
		input = getch();

		int new_x = -1;
		int new_y = -1;

		switch (input) {
		case 'h':
			new_x = game.hero_x - 1;
			new_y = game.hero_y;
			break;
		case 'j':
			new_y = game.hero_y + 1;
			new_x = game.hero_x;
			break;
		case 'k':
			new_y = game.hero_y - 1;
			new_x = game.hero_x;
			break;
		case 'l':
			new_y = game.hero_y;
			new_x = game.hero_x + 1;
			break;
		case 'q':
			running = false;
			break;
		}

		if (new_x >= 0 && new_x < FIELD_SIZE
		    && new_y >= 0 && new_y < FIELD_SIZE) {
			char target = game.field[new_y][new_x];
			switch (target) {
			case '$':
				game.score++;
			/* FALLTHROUGH */
			case '.':
				game.movement--;
				game.field[new_y][new_x] = 'T';
				game.field[game.hero_y][game.hero_x] = '.';
				game.hero_x = new_x;
				game.hero_y = new_y;
				break;
			}

			clear();
		}
	} while (running && game.movement > 0);

	endwin();

	return EXIT_SUCCESS;
}

GameData
init_game()
{
	GameData game;
	int right_border = FIELD_SIZE - 1;

	int i, j;
	for (i = 0; i < FIELD_SIZE; i++) {
		for (j = 0; j < FIELD_SIZE; j++) {
			if (i == 0 || i == right_border ||
			    j == 0 || j == right_border) {
				game.field[i][j] = '%';
			} else {
				game.field[i][j] = get_char(hero_has_spawned);
			}
		}
	}

	return game;
}

void
print_field(GameData game)
{
	int i, j;
	for (i = 0; i < FIELD_SIZE; i++) {
		for (j = 0; j < FIELD_SIZE; j++) {
			printw("%c", game.field[i][j]);
		}

		printw("\n");
	}
	printw("Move: %d\nScore: %d", game.movement, game.score);
	refresh();  /* trigger ncurses printing to screen */
}
