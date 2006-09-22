/* $Id: firework.c,v 1.23 2006/09/22 15:39:30 wmcbrine Exp $ */

#include <stdio.h>
#include <signal.h>
#include <curses.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

#define DELAYSIZE 200

void myrefresh(void);
void get_color(void);
void explode(int, int);

int main(int argc, char **argv)
{
	int start, end, row, diff, flag, direction, seed;

#ifdef XCURSES
	Xinitscr(argc, argv);
#else
	initscr();
#endif
	nodelay(stdscr, TRUE);
	noecho();

	if (has_colors())
		start_color();

	seed = time((time_t *)0);
	srand(seed);
	flag = 0;
       
	while (getch() == ERR)		/* loop until a key is hit */
	{
		do {
			start = rand() % (COLS -3);
			end = rand() % (COLS - 3);
			start = (start < 2) ? 2 : start;
			end = (end < 2) ? 2 : end;
			direction = (start > end) ? -1 : 1;
			diff = abs(start - end);

		} while (diff < 2 || diff >= LINES - 2);

		attrset(A_NORMAL);

		for (row = 0; row < diff; row++)
		{
			mvaddstr(LINES - row, row * direction + start,
				(direction < 0) ? "\\" : "/");

			if (flag++)
			{
				myrefresh();
				clear();
				flag = 0;
			}
		}

		if (flag++)
		{
			myrefresh();
			flag = 0;
		}

		explode(LINES - row, diff * direction + start);
		clear();
		myrefresh();
	}

	endwin();

	return 0;
}

void explode(int row, int col)
{
	clear();
	mvaddstr(row, col, "-");
	myrefresh();

	--col;

	get_color();
	mvaddstr(row - 1, col, " - ");
	mvaddstr(row,     col, "-+-");
	mvaddstr(row + 1, col, " - ");
	myrefresh();

	--col;

	get_color();
	mvaddstr(row - 2, col, " --- ");
	mvaddstr(row - 1, col, "-+++-");
	mvaddstr(row,     col, "-+#+-");
	mvaddstr(row + 1, col, "-+++-");
	mvaddstr(row + 2, col, " --- ");
	myrefresh();

	get_color();
	mvaddstr(row - 2, col, " +++ ");
	mvaddstr(row - 1, col, "++#++");
	mvaddstr(row,     col, "+# #+");
	mvaddstr(row + 1, col, "++#++");
	mvaddstr(row + 2, col, " +++ ");
	myrefresh();

	get_color();
	mvaddstr(row - 2, col, "  #  ");
	mvaddstr(row - 1, col, "## ##");
	mvaddstr(row,     col, "#   #");
	mvaddstr(row + 1, col, "## ##");
	mvaddstr(row + 2, col, "  #  ");
	myrefresh();

	get_color();
	mvaddstr(row - 2, col, " # # ");
	mvaddstr(row - 1, col, "#   #");
	mvaddstr(row,     col, "     ");
	mvaddstr(row + 1, col, "#   #");
	mvaddstr(row + 2, col, " # # ");
	myrefresh();
}

void myrefresh(void)
{
	napms(DELAYSIZE);
	move(LINES - 1, COLS - 1);
	refresh();
}

void get_color(void)
{
	static short tbl[] =
	{
		COLOR_RED, COLOR_BLUE, COLOR_GREEN, COLOR_CYAN,
		COLOR_RED, COLOR_MAGENTA, COLOR_YELLOW, COLOR_WHITE
	};

	chtype bold = (rand() % 2) ? A_BOLD : A_NORMAL;
	init_pair(1, tbl[rand() % 8], COLOR_BLACK);
	attrset(COLOR_PAIR(1) | bold);
}
