#include <iostream>
#include <ncurses.h>

void io_init_terminal(void)
{
  initscr();
  raw();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  start_color();
  init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
  init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
  init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
  init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
  init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
  init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
}

void io_reset_terminal(void)
{
  endwin();
}

void io_draw_grid(void) {
    attron(COLOR_PAIR(COLOR_YELLOW));
    //left side
    mvaddch(1, 0, ACS_VLINE);
    mvaddch(0, 0, ACS_ULCORNER);
    mvaddch(0, 1, ACS_HLINE);
    mvaddch(0, 2, ACS_HLINE);
    mvaddch(0, 3, ACS_HLINE);
    mvaddch(1, 2, 'a');

    for (int y = 2; y < 24; y += 2) {
      mvaddch(y + 1, 0, ACS_VLINE);
      mvaddch(y    , 0, ACS_LTEE);
      mvaddch(y    , 1, ACS_HLINE);
      mvaddch(y    , 2, ACS_HLINE);
      mvaddch(y    , 3, ACS_HLINE);
      mvaddch(y + 1, 2, 'a');
    }

    mvaddch(24, 0, ACS_LLCORNER);
    mvaddch(24, 1, ACS_HLINE);
    mvaddch(24, 2, ACS_HLINE);
    mvaddch(24, 3, ACS_HLINE);

    //right side
    mvaddch(0, 80, ACS_URCORNER);
    mvaddch(1, 80, ACS_VLINE);
    for (int y = 2; y < 24; y += 2) {
      mvaddch(y + 1, 80, ACS_VLINE);
      mvaddch(y    , 80, ACS_RTEE);
    }
    mvaddch(24, 80, ACS_LRCORNER);
    attron(COLOR_PAIR(COLOR_YELLOW));

    
    attron(COLOR_PAIR(COLOR_GREEN));
    //top
    for (int x = 4; x < 80; x += 4) {
      mvaddch(1, x    , ACS_VLINE);
      mvaddch(0, x    , ACS_TTEE);
      mvaddch(0, x + 1, ACS_HLINE);
      mvaddch(0, x + 2, ACS_HLINE);
      mvaddch(0, x + 3, ACS_HLINE);
      mvaddch(1, x + 2, 'a');
    }

    //bottom
    for (int x = 4; x < 80; x += 4) {
      mvaddch(24, x    , ACS_BTEE);
      mvaddch(24, x + 1, ACS_HLINE);
      mvaddch(24, x + 2, ACS_HLINE);
      mvaddch(24, x + 3, ACS_HLINE);
    }
    attroff(COLOR_PAIR(COLOR_GREEN));


    //inner
    for (int y = 2; y < 24; y += 2) {
      for (int x = 4; x < 80; x += 4) {
        mvaddch(y + 1, x    , ACS_VLINE);
        mvaddch(y    , x    , ACS_PLUS);
        mvaddch(y    , x + 1, ACS_HLINE);
        mvaddch(y    , x + 2, ACS_HLINE);
        mvaddch(y    , x + 3, ACS_HLINE);
        mvaddch(y + 1, x + 2, 'a');
      }
    }
  
}

int main(int argc, char const *argv[])
{
    io_init_terminal();
    attron(COLOR_PAIR(COLOR_MAGENTA));
    mvprintw(0, 0, "0");
    attroff(COLOR_PAIR(COLOR_MAGENTA));
    mvprintw(24, 0, "0");
    mvprintw(24, 80, "0");
    mvprintw(0, 80, "0");
    getch();
    io_draw_grid();
    getch();
    io_reset_terminal();
    return 0;
}