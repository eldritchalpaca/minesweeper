#include <iostream>
#include <ncurses.h>
#include <vector>
#include <cmath>

#include "tile.h"

#define EASY    20
#define MEDIUM  30
#define HARD    50

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
  COLOR_WHITE,  //7
  COLOR_RED     //8
};

void init_terminal(void)
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

void reset_terminal(void)
{
  endwin();
}

void draw_grid(void) {
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

void fill_tile(game_board *board, int x, int y) {
  int color;
  if (board->board[y][x]->get_value() != 0) {
    if (board->board[y][x]->check_if_bomb()) {
      color = colors[0];
    } else {
      color = colors[board->board[y][x]->get_value()];
    }
    attron(COLOR_PAIR(color));
    mvaddch(io_y(y), io_x(x), board->board[y][x]->check_if_bomb() ? '<' : board->board[y][x]->get_value() + '0');
    attroff(COLOR_PAIR(color));
    refresh();
  }
  else {
    mvaddch(io_y(y), io_x(x), ' ');
  }
}

void show_grid(game_board *board) {
  for (int y = 0; y < BOARD_Y; ++y) {
    for (int x = 0; x < BOARD_X; ++x) {
      fill_tile(board, x, y);
    }
  }
}

void grid_init() {
  clear();
  draw_grid();
  for (int y = 0; y < BOARD_Y; ++y) {
    for (int x = 0; x < BOARD_X; ++x) {
      attron(COLOR_PAIR(COLOR_CYAN));
      mvaddch(io_y(y), io_x(x), '.');
      attroff(COLOR_PAIR(COLOR_CYAN));
      refresh();
    }
  }
}

bool on_tile_click(game_board *board, int x, int y) {
  fill_tile(board, x, y);
  board->board[y][x]->is_clicked = true;
  board->board[y][x]->is_flagged = false;
  if (board->board[y][x]->get_value() == 0) {
    std::vector<tile *> v = board->get_tile_neighbors(x, y);
    for (tile *t : v) {
      if (!t->is_clicked) {
        on_tile_click(board, t->get_x(), t->get_y());
      }
    }
  }
  if (board->board[y][x]->check_if_bomb()) {
    return true;
  }
  return false;
}

void on_tile_flag(game_board *b, tile *t) {
  if (!t->is_clicked) {
    t->is_flagged ? b->decrement_flags() : b->increment_flags();
    t->is_flagged = !t->is_flagged;
    attron(COLOR_PAIR(t->is_flagged ? COLOR_WHITE : COLOR_CYAN));
    mvaddch(io_y(t->get_y()), io_x(t->get_x()), t->is_flagged ? '<' : '.');
    attroff(COLOR_PAIR(t->is_flagged ? COLOR_WHITE : COLOR_CYAN));
  }
}

bool on_middle_click(game_board *b, tile *t) {
  bool defeated = false;
  std::vector<tile *> v;

  if (t->is_clicked && t->get_value() == b->check_tile_neighbors_flags(t->get_x(), t->get_y())) {
    v = b->get_tile_neighbors(t->get_x(), t->get_y());
    
    for (tile *tl : v) {
      if (!tl->is_flagged) {
        defeated = on_tile_click(b, tl->get_x(), tl->get_y());
        tl->is_clicked = true;
        if (defeated) {
          return true;
        }
      }
    }
  }
  return false;
}

int level_select() {
  int in;

  clear();
  mvprintw(0, 0, "0");
  mvprintw(23, 0, "0");
  mvprintw(0, 79, "0");
  mvprintw(23, 79, "0");

  mvprintw(2, 27, "Please select a difficulty:");
  mvprintw(6, 36, "1: easy");
  mvprintw(8, 36, "2: medium");
  mvprintw(10, 36, "3: hard");

  do {
    in = getch() - '0';
  } while (in != 1 && in != 2 && in != 3);

  switch (in) {
    case 1:
      return EASY;
    case 2: 
      return MEDIUM;
    case 3:
      return HARD;
    default:
      return 200;
  }
}

void game_loop() {
    int in;
    int num_bombs;
    MEVENT event;
    game_board *board;
    bool defeated = false;
    bool generated = false;

    clear();

    num_bombs = level_select();

    grid_init();

    do {
    in = getch();
    if (getmouse(&event) == OK && in_bounds(x(event.x), y(event.y))) {
      if (!generated) {
        board = new game_board(num_bombs, x(event.x), y(event.y));
        generated = true;
      }

      if (event.bstate & BUTTON1_CLICKED) {
        defeated = on_tile_click(board, x(event.x), y(event.y));
      }
      else if (event.bstate & BUTTON2_CLICKED) {
        defeated = on_middle_click(board, board->board[y(event.y)][x(event.x)]);
      }
      else if (event.bstate & BUTTON3_CLICKED) {
        on_tile_flag(board, board->board[y(event.y)][x(event.x)]);
      }
    }
    
    
  } while (!defeated && in != 'Q' && !board->have_won());

  if (board->have_won()) {
    mvprintw(26, 0, "yay you won (click anywhere to continue)");
  }
  else {
    mvprintw(26, 0, "you lost (click anywhere to continue)");
  }
  show_grid(board);
  
  getch();
}

void title_screen() {
    int in;
    MEVENT event;

    clear();

    mvprintw(0, 0, "0");
    mvprintw(23, 0, "0");
    mvprintw(0, 79, "0");
    mvprintw(23, 79, "0");

    mvprintw(3, 6, "  __  __  ___  _  _  ___  ___ __      __ ___  ___  ___  ___  ___ ");
    mvprintw(4, 6, " |  \\/  ||_ _|| \\| || __|/ __|\\ \\    / /| __|| __|| _ \\| __|| _ \\");
    mvprintw(5, 6, " | |\\/| | | | | .` || _| \\__ \\ \\ \\/\\/ / | _| | _| |  _/| _| |   /");
    mvprintw(6, 6, " |_|  |_||___||_|\\_||___||___/  \\_/\\_/  |___||___||_|  |___||_|_\\");

    mvprintw(15, 34, "by alix noble");
    mvprintw(17, 27, "--click anywhere to start--");

    do {
      in = getch();
    } while (getmouse(&event) != OK && in != 'Q');
}

int main(int argc, char const *argv[])
{
    int in;

    init_terminal();

    title_screen();

    do {
      game_loop();

      clear();

      mvprintw(2, 26, "Would you like to play again?");
      mvprintw(6, 38, "1: yes");
      mvprintw(8, 38, "2: no");

      do {
        in = getch() - '0';
      } while (in != 1 && in != 2);

    } while (in == 1);
    
    reset_terminal();
    return 0;
}
