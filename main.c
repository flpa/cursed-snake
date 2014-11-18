#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ncurses.h>
/* for usleep */
#include <unistd.h>

const int FIELD_SIZE = 20;

void      print_field(char field[FIELD_SIZE][FIELD_SIZE]);
char      get_char(bool hero_has_spawned);
void	  swap(char *pc1, char *pc2);

int
main()
{
	/* Initialize seed for rand() */
	srand(time(NULL));

	char field[FIELD_SIZE][FIELD_SIZE];
	int x = rand() % FIELD_SIZE;
	int y = rand() % FIELD_SIZE;

	int i, j;
	for (i = 0; i < FIELD_SIZE; i++) {
		for (j = 0; j < FIELD_SIZE; j++) {
			if (i == y && j == x) {
				field[i][j] = 's';
			} else {
				field[i][j] = '.';
			}
		}
	}

	WINDOW *window = initscr(); /* Init ncurses */
	char input;
	bool running = true;
	nodelay(window, true);
	char direction;

	do {
		print_field(field);
		usleep(1000 * 500);
		input = getch();
		if (input != ERR) {
			direction = input;
		}

		int new_x = x;
		int new_y = y;

		switch (direction) {
		case 'h':
			new_x--;
			break;
		case 'j':
			new_y++;
			break;
		case 'k':
			new_y--;
			break;
		case 'l':
			new_x++;
			break;
		case 'q':
			running = false;
			break;
		}

		/*	if (new_x >= 0 && new_x < FIELD_SIZE
			    && new_y >= 0 && new_y < FIELD_SIZE) {
				char target = game.field[new_y][new_x];
				switch (target) {
				case '$':
					game.score++;
				case '.':
					game.movement--;
					game.field[new_y][new_x] = 'T';
					game.field[game.hero_y][game.hero_x] = '.';
					game.hero_x = new_x;
					game.hero_y = new_y;
					break;
				}

			}
		*/

		clear();
		swap(&field[y][x], &field[new_y][new_x]);
		x = new_x;
		y = new_y;
	} while (running);

	endwin();

	return EXIT_SUCCESS;
}

void
swap(char *pc1, char *pc2)
{
	char t = *pc1;
	*pc1 = *pc2;
	*pc2 = t;
}

/*GameData
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
}*/

void
print_field(char field[FIELD_SIZE][FIELD_SIZE])
{
	int i, j;
	for (i = 0; i < FIELD_SIZE; i++) {
		for (j = 0; j < FIELD_SIZE; j++) {
			printw("%c", field[i][j]);
		}

		printw("\n");
	}
	refresh();  /* trigger ncurses printing to screen */
}
