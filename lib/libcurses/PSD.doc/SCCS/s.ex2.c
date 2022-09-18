h06094
s 00000/00000/00184
d D 8.1 93/06/08 18:32:11 bostic 4 3
c 4.4BSD snapshot (revision 8.1)
e
s 00002/00002/00182
d D 1.2 93/06/08 18:31:56 bostic 3 1
c do roff .c files by hand
e
s 00002/00002/00182
d R 8.1 93/06/04 16:44:34 bostic 2 1
c 4.4BSD snapshot (revision 8.1); add 1993 to copyright
e
s 00184/00000/00000
d D 1.1 92/12/30 15:34:18 elan 1 0
c date and time created 92/12/30 15:34:18 by elan
e
u
U
t
T
I 1
D 3
.\" Copyright (c) 1992 The Regents of the University of California.
.\" All rights reserved.
E 3
I 3
.\" Copyright (c) 1992, 1993
.\"	 The Regents of the University of California.  All rights reserved.
E 3
.\"
.\" %sccs.include.redist.roff%
.\"
.\"     %W% (Berkeley) %G%
.\"
#include <curses.h>
#include <stdio.h>
#include <signal.h>

#define YSIZE LINES
#define XSIZE COLS

static int quit();

/*
 * This program fills the screen up with characters and the allows the user to
 * manipulate the text on the screen using some basic commands.
 * Nothing fancy, just a demonstration of the elementary features of the
 * curses(3) package.
 */
main()
{
	int i, j, c, n, d = 0;
	char id[100];
	int hh = 0;
	int curx, cury, base, arg;
	
	initscr();
	signal(SIGINT, quit);
	crmode();
	noecho();
	nonl();
	delwin(stdscr);
	stdscr = newwin(YSIZE, XSIZE, 0, 0);
	flushok(stdscr, TRUE);
	scrollok(stdscr, TRUE);
	erase();
	refresh();

	move(0,0);
	refresh();
	for (i = 0; i < YSIZE + 2; i++) {
		sprintf(id, "%d: ", i);
		addstr(id);
		for (j = 0; j < XSIZE - strlen(id); j++)
			addch('0' + (i % 10));
	}
	c = getchar();
	base = 2;
	curx = cury = 0;
	move(0, 0);
	refresh();

	/*
	 * The screen manipulator has the following commands:
	 * 'D' - clear to the end of the current line.
	 * 'B' - clear to the bottom of the screen.
	 * 'E' - erase the screen.
	 * 's' - enter standout mode.
	 * 'e' - exit standout mode.
	 * 'd' n - delete n lines below cursor line.
	 * 'i' n - insert n lines below cursor line.
	 * 'q' - quit.
	 * 'f' - move cursor one position to the right.
	 * 'b' - move cursor one position to the left.
	 * 'n' - move cursor one line down.
	 * 'p' - move cursor one line up.
	 * 'h' - home cusor.
	 * 'l' - force refresh.
	 * 'r' - simulate a carriage return.
	 *
	 * All other characters are ignored.
	 */
	for(;;) {
		switch(c = getchar()) {
		case 'D':
			clrtoeol();
			refresh();
			continue;
		case 'B':
			clrtobot();
			refresh();
			continue;
		case 'E':
			erase();
			refresh();
			continue;
		case 's':
			standout();
			continue;
		case 'e':
			standend();
			continue;
		case 'd':
			arg = getchar() - '0';
			for (i = 0; i < arg; i++)
				deleteln();
			refresh();
			continue;
		case 'i':
			arg = getchar() - '0';
			for (i = 0; i < arg; i++)
				insertln();
			refresh();
			continue;
		case 'q':
			quit();
		case 'f':
			if (curx < XSIZE - 1)
				curx++;
			else {
				cury++;
				curx = 0;
			}
			break;
		case 'b':
			if (curx == 0) {
				cury--;
				curx = XSIZE - 1;
			} else
				curx--;
			break;
		case 'n':
			cury++;
			break;
		case 'p':
			cury--;
			break;
		case 'h':
			curx = cury = 0;
			break;
		case 'l':
			wrefresh(curscr);
			continue;
		case 'r':   /* return */
		{
			int x, y;
			getyx(stdscr, y, x);
			move(y+1, 0);
			insertln();
			move(y, x);
			clrtoeol();
			refresh();
			continue;
		}
		default:
			continue;
		}

		if (cury < 0) {
			base--;
			move(0, 0);
			insertln();
			sprintf(id, "%d: ", base);
			addstr(id);
			for (j = 0; j < XSIZE - strlen(id) - 2; j++) 
				addch('0' + (base % 10));
			cury++;
		} else if (cury >= YSIZE) {
			move(0, 0);
			deleteln();
			move(YSIZE - 1, 0);
			sprintf(id, "%d: ", base + YSIZE);
			addstr(id);
			for (j = 0; j < XSIZE - strlen(id) - 2; j++)
				addch('0' + ((base + YSIZE) % 10));
			cury--;
			base++;
		}
		move(cury, curx);
		refresh();
	}
}

int
quit()
{
	erase();
	refresh();
	endwin();
	exit(0);
}
E 1
