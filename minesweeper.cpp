#include <iostream>
#include <ncurses.h>
#include <vector>
#include <cmath>

#include "tile.h"

#define io_x(x) (x * 4 + 2)
#define io_y(y) (y * 2 + 1)
#define x(io_x) ((int)round(((double)(io_x - 2) / 4.0)))
#define y(io_y) ((int)round(((double)(io_y - 1) / 2.0)))

int colors[9] = 
{
  COLOR_BLACK,  //bomb
  COLOR_BLUE,   //1
  COLOR_GREEN,  //2
  COLOR_RED,    //3
  COLOR_MAGENTA,//4
  COLOR_CYAN,   //5
  COLOR_YELLOW, //6
  COLOR_YELLOW, //7
  COLOR_RED     //8
};

void io_init_terminal(void)
{
  initscr();
  raw();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
  //mousemask(REPORT_MOUSE_POSITION, NULL);
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
    //attron(COLOR_PAIR(COLOR_YELLOW));
    //left side
    mvaddch(1, 0, ACS_VLINE);
    mvaddch(0, 0, ACS_ULCORNER);
    mvaddch(0, 1, ACS_HLINE);
    mvaddch(0, 2, ACS_HLINE);
    mvaddch(0, 3, ACS_HLINE);
    //mvaddch(1, 2, 'a');

    for (int y = 2; y < 24; y += 2) {
      mvaddch(y + 1, 0, ACS_VLINE);
      mvaddch(y    , 0, ACS_LTEE);
      mvaddch(y    , 1, ACS_HLINE);
      mvaddch(y    , 2, ACS_HLINE);
      mvaddch(y    , 3, ACS_HLINE);
      //mvaddch(y + 1, 2, 'a');
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
    //attroff(COLOR_PAIR(COLOR_YELLOW));

    
    //attron(COLOR_PAIR(COLOR_GREEN));
    //top
    for (int x = 4; x < 80; x += 4) {
      mvaddch(1, x    , ACS_VLINE);
      mvaddch(0, x    , ACS_TTEE);
      mvaddch(0, x + 1, ACS_HLINE);
      mvaddch(0, x + 2, ACS_HLINE);
      mvaddch(0, x + 3, ACS_HLINE);
      //mvaddch(1, x + 2, 'a');
    }

    //bottom
    for (int x = 4; x < 80; x += 4) {
      mvaddch(24, x    , ACS_BTEE);
      mvaddch(24, x + 1, ACS_HLINE);
      mvaddch(24, x + 2, ACS_HLINE);
      mvaddch(24, x + 3, ACS_HLINE);
    }
    //attroff(COLOR_PAIR(COLOR_GREEN));


    //inner
    for (int y = 2; y < 24; y += 2) {
      for (int x = 4; x < 80; x += 4) {
        mvaddch(y + 1, x    , ACS_VLINE);
        mvaddch(y    , x    , ACS_PLUS);
        mvaddch(y    , x + 1, ACS_HLINE);
        mvaddch(y    , x + 2, ACS_HLINE);
        mvaddch(y    , x + 3, ACS_HLINE);
        //mvaddch(y + 1, x + 2, 'a');
      }
    }
  
}

void io_fill_tile(game_board *board, int x, int y) {
  int color;
  if (board->board[y][x]->get_value() != 0) {
    if (board->board[y][x]->check_if_bomb()) {
      color = colors[0];
    } else {
      color = colors[board->board[y][x]->get_value()];
    }
    attron(COLOR_PAIR(color));
    mvaddch(io_y(y), io_x(x), board->board[y][x]->check_if_bomb() ? 'X' : board->board[y][x]->get_value() + '0');
    attroff(COLOR_PAIR(color));
    refresh();
  }
  else {
    mvaddch(io_y(y), io_x(x), ' ');
  }
}

void io_show_grid(game_board *board) {
  for (int y = 0; y < BOARD_Y; ++y) {
    for (int x = 0; x < BOARD_X; ++x) {
      io_fill_tile(board, x, y);
    }
  }
}

void io_grid_init() {
  io_draw_grid();
  for (int y = 0; y < BOARD_Y; ++y) {
    for (int x = 0; x < BOARD_X; ++x) {
      attron(COLOR_PAIR(COLOR_CYAN));
      mvaddch(io_y(y), io_x(x), '.');
      attroff(COLOR_PAIR(COLOR_CYAN));
      refresh();
    }
  }
}

bool io_on_tile_click(game_board *board, int x, int y) {
  io_fill_tile(board, x, y);
  board->board[y][x]->is_clicked = true;
  if (board->board[y][x]->get_value() == 0) {
    std::vector<tile *> v = board->get_tile_neighbors(x, y);
    for (tile *t : v) {
      if (!t->is_clicked) {
        io_on_tile_click(board, t->get_x(), t->get_y());
      }
    }
  }
  if (board->board[y][x]->check_if_bomb()) {
    return true;
  }
  return false;
}

int main(int argc, char const *argv[])
{
    int in;
    MEVENT event;
    bool defeated = false;

    game_board *board;
    board = new game_board(20);

    io_init_terminal();
    
    io_grid_init();
    
    do {
      in = getch();
      if (getmouse(&event) == OK ) {
        if ((event.bstate & BUTTON1_CLICKED) && in_bounds(x(event.x), y(event.y))) {
          defeated = io_on_tile_click(board, x(event.x), y(event.y));
          move(26, 0);
          clrtoeol();
          mvprintw(26, 0, "%d, %d", x(event.x), y(event.y));
        }
        else if (event.bstate & BUTTON2_CLICKED) {
          mvaddch(io_y(y(event.y)), io_x(x(event.x)), '%');
        }
        else if (event.bstate & BUTTON3_CLICKED) {
          mvaddch(io_y(y(event.y)), io_x(x(event.x)), '#');
        }
      }
    } while (!defeated && in != 'Q');

    io_show_grid(board);
    getch();

    io_reset_terminal();
    return 0;
}
