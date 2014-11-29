#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ncurses.h>
/* for usleep */
#include <unistd.h>

const int FIELD_SIZE = 20;

typedef struct point {
	int x, y;
} Point;

void     print_field(char field[FIELD_SIZE][FIELD_SIZE]);
char     get_char(bool hero_has_spawned);
void	 swap(char *pc1, char *pc2);
bool	 is_one_of(char c, const char *options);
bool 	 is_outside(int coord);
void 	 random_point(int *x, int *y);


int
main()
{
	/* Initialize seed for rand() */
	srand(time(NULL));

	char field[FIELD_SIZE][FIELD_SIZE];

	int x = 0;
	int y = 0;
	random_point(&x, &y);

	int i, j;
	for (i = 0; i < FIELD_SIZE; i++) {
		for (j = 0; j < FIELD_SIZE; j++) {
			if (i == y && j == x) {
				field[i][j] = 'S';
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
	bool spawn_food = true;

	do {
		if(spawn_food) {
			int food_x = 0;
			int food_y = 0;
			random_point(&food_x, &food_y);
			field[food_y][food_x] = 'x';
			spawn_food = false;
		}
		print_field(field);
		usleep(1000 * 500);
		input = getch();
		if (input != ERR && is_one_of(input, "hjklq")) {
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

		clear();
		if (is_outside(new_x) || is_outside(new_x)
		    || field[new_y][new_x] == 's') {
			printw("busted");
			break;
		}
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

bool
is_one_of(char c, const char *options)
{
	for (; *options != '\0'; options++) {
		if (c == *options) {
			return true;
		}
	}
	return false;
}

bool
is_outside(int coord)
{
	return coord < 0 || coord >= FIELD_SIZE;
}

void
random_point(int *x, int *y)
{
	*x = rand() % FIELD_SIZE;
	*y = rand() % FIELD_SIZE;
}
