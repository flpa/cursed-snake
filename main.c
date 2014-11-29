#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ncurses.h>
/* for usleep */
#include <unistd.h>

const int FIELD_SIZE = 20;

typedef struct node {
	int x, y;
	struct node *next;
} Node;

void  	 lst_append(Node **head, int x, int y);
bool	 lst_contains(Node *head, int x, int y);
void  	 lst_free(Node *head);
int	 lst_length(Node *head);

void     print_field(Node *head, int, int);
void	 mark_node(Node *head, char *f);
bool	 is_one_of(char c, const char *options);
bool 	 is_outside(int coord);
void 	 random_point(int *x, int *y);

int
main()
{
	/* Initialize seed for rand() */
	srand(time(NULL));

	int x, y;
	random_point(&x, &y);

	Node *head = NULL;
	lst_append(&head, x, y);

	WINDOW *window = initscr(); /* Init ncurses */
	char input;
	bool running = true;
	nodelay(window, true);
	char direction = 'h';
	int len = 3;

	int food_x = -1;
	int food_y = -1;
	int sleep_time = 300;

	do {
		if (food_x < 0) {
			random_point(&food_x, &food_y);
		}
		print_field(head, food_x, food_y);
		usleep(1000 * sleep_time);
		input = getch();
		if (input != ERR && is_one_of(input, "hjklq")) {
			direction = input;
		}

		switch (direction) {
		case 'h':
			x--;
			break;
		case 'j':
			y++;
			break;
		case 'k':
			y--;
			break;
		case 'l':
			x++;
			break;
		case 'q':
			running = false;
			break;
		}

		clear();
		if (is_outside(x) || is_outside(y)
		    || lst_contains(head, x, y)) {
			printf("busted");
			break;
		}

		if (x == food_x && y == food_y) {
			len++;
			food_x = -1;
			food_y = -1;
			sleep_time -= 20;
		}

		lst_append(&head, x, y);
		if (lst_length(head) == len) {
			Node *old_head = head;
			head = head->next;
			free(old_head);
		}
	} while (running);
	lst_free(head);

	endwin();

	return EXIT_SUCCESS;
}

void
print_field(Node *head, int food_x, int food_y)
{
	char field[FIELD_SIZE][FIELD_SIZE];
	int i, j;

	for (i = 0; i < FIELD_SIZE; i++) {
		for (j = 0; j < FIELD_SIZE; j++) {
			field[i][j] = '.';
		}
	}

	field[food_y][food_x] = 'x';

	mark_node(head, field[0]);

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

/* simplify pointers = possible ? */
void
lst_append(Node **head, int x, int y)
{
	if (NULL == *head) {
		*head = malloc(sizeof(Node));
		(*head)->x = x;
		(*head)->y = y;
		(*head)->next = NULL;
	} else {
		lst_append(&((*head)->next), x, y);
	}
}

bool
lst_contains(Node *head, int x, int y)
{
	if (NULL == head) {
		return false;
	}
	if (head->x == x && head->y == y) {
		return true;
	}
	return lst_contains(head->next, x, y);
}

void
lst_free(Node *head)
{
	if (NULL != head) {
		lst_free(head->next);
		free(head);
	}
}

int
lst_length(Node *head)
{
	int len;
	for (len = 0; head != NULL; head = head->next) {
		len++;
	}
	return len;
}

void
mark_node(Node *head, char *f)
{
	char *p = f + head->y * FIELD_SIZE + head->x;
	if (NULL == head->next) {
		*p = 'S';
	} else {
		*p = 's';
		mark_node(head->next, f);
	}
}
