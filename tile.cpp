#include "tile.h"
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <array>
#include <algorithm>
#include <ncurses.h>

int game_board::check_tile_neighbors_bombs(int x, int y) {
    int num_bombs = 0;
    if (in_bounds(x - 1, y - 1) && board[y - 1][x - 1] && board[y - 1][x - 1]->check_if_bomb()) {
        num_bombs++;
    }
    if (in_bounds(x, y - 1)     && board[y - 1][x    ] && board[y - 1][x    ]->check_if_bomb()) {
        num_bombs++;
    }
    if (in_bounds(x + 1, y - 1) && board[y - 1][x + 1] && board[y - 1][x + 1]->check_if_bomb()) {
        num_bombs++;
    }
    if (in_bounds(x - 1, y)     && board[y    ][x - 1] && board[y    ][x - 1]->check_if_bomb()) {
        num_bombs++;
    }
    if (in_bounds(x + 1, y)     && board[y    ][x + 1] && board[y    ][x + 1]->check_if_bomb()) {
        num_bombs++;
    }
    if (in_bounds(x - 1, y + 1) && board[y + 1][x - 1] && board[y + 1][x - 1]->check_if_bomb()) {
        num_bombs++;
    }
    if (in_bounds(x, y + 1)     && board[y + 1][x    ] && board[y + 1][x    ]->check_if_bomb()) {
        num_bombs++;
    }
    if (in_bounds(x + 1, y + 1) && board[y + 1][x + 1] && board[y + 1][x + 1]->check_if_bomb()) {
        num_bombs++;
    }
    return num_bombs;
}

int game_board::check_tile_neighbors_flags(int x, int y) {
    int num_flags = 0;
    if (in_bounds(x - 1, y - 1) && board[y - 1][x - 1] && board[y - 1][x - 1]->is_flagged) {
        num_flags++;
    }
    if (in_bounds(x, y - 1)     && board[y - 1][x    ] && board[y - 1][x    ]->is_flagged) {
        num_flags++;
    }
    if (in_bounds(x + 1, y - 1) && board[y - 1][x + 1] && board[y - 1][x + 1]->is_flagged) {
        num_flags++;
    }
    if (in_bounds(x - 1, y)     && board[y    ][x - 1] && board[y    ][x - 1]->is_flagged) {
        num_flags++;
    }
    if (in_bounds(x + 1, y)     && board[y    ][x + 1] && board[y    ][x + 1]->is_flagged) {
        num_flags++;
    }
    if (in_bounds(x - 1, y + 1) && board[y + 1][x - 1] && board[y + 1][x - 1]->is_flagged) {
        num_flags++;
    }
    if (in_bounds(x, y + 1)     && board[y + 1][x    ] && board[y + 1][x    ]->is_flagged) {
        num_flags++;
    }
    if (in_bounds(x + 1, y + 1) && board[y + 1][x + 1] && board[y + 1][x + 1]->is_flagged) {
        num_flags++;
    }
    return num_flags;
}

std::vector<tile *> game_board::get_tile_neighbors(int x, int y) {
    std::vector<tile *> v;
    if (in_bounds(x - 1, y - 1) && board[y - 1][x - 1]) {
        v.push_back(board[y - 1][x - 1]);
    }
    if (in_bounds(x, y - 1)     && board[y - 1][x    ]) {
        v.push_back(board[y - 1][x    ]);
    }
    if (in_bounds(x + 1, y - 1) && board[y - 1][x + 1]) {
        v.push_back(board[y - 1][x + 1]);
    }
    if (in_bounds(x - 1, y)     && board[y    ][x - 1]) {
        v.push_back(board[y    ][x - 1]);
    }
    if (in_bounds(x + 1, y)     && board[y    ][x + 1]) {
        v.push_back(board[y    ][x + 1]);
    }
    if (in_bounds(x - 1, y + 1) && board[y + 1][x - 1]) {
        v.push_back(board[y + 1][x - 1]);
    }
    if (in_bounds(x, y + 1)     && board[y + 1][x    ]) {
        v.push_back(board[y + 1][x    ]);
    }
    if (in_bounds(x + 1, y + 1) && board[y + 1][x + 1]) {
        v.push_back(board[y + 1][x + 1]);
    }
    return v;
}

bool game_board::have_won() {
    if (num_bombs != num_flags) {
        return false;
    }

    for (tile *t : bombs) {
        if (!t->is_flagged) {
            return false;
        }
    }
    return true;
}

game_board::game_board(int num_bombs, int click_x, int click_y) : num_bombs(num_bombs), num_flags(0) {
    int x, y;
    int n = 0;
    std::vector<std::array<int, 2>> reserved;

    if (in_bounds(click_x, click_y)) {
        reserved.push_back({click_y, click_x});
    }
    if (in_bounds(click_x - 1, click_y - 1)) {
        reserved.push_back({click_y - 1, click_x - 1});
    }
    if (in_bounds(click_x, click_y - 1)) {
        reserved.push_back({click_y - 1, click_x    });
    }
    if (in_bounds(click_x + 1, click_y - 1)) {
        reserved.push_back({click_y - 1, click_x + 1});
    }
    if (in_bounds(click_x - 1, click_y)) {
        reserved.push_back({click_y    , click_x - 1});
    }
    if (in_bounds(click_x + 1, click_y)) {
        reserved.push_back({click_y    , click_x + 1});
    }
    if (in_bounds(click_x - 1, click_y + 1)) {
        reserved.push_back({click_y + 1, click_x - 1});
    }
    if (in_bounds(click_x, click_y + 1)) {
        reserved.push_back({click_y + 1, click_x    });
    }
    if (in_bounds(click_x + 1, click_y + 1)) {
        reserved.push_back({click_y + 1, click_x + 1});
    }

    for (y = 0; y < BOARD_Y; ++y) {
        for (x = 0; x < BOARD_X; ++x) {
            board[y][x] = NULL;
        }
    }
    

    srand(time(NULL));

    while (n < num_bombs) {
        x = rand() % BOARD_X;
        y = rand() % BOARD_Y;
        if (!board[y][x] && std::find(reserved.begin(), reserved.end(), std::array<int, 2> {y, x}) == reserved.end()) {
            board[y][x] = new tile(true, -1, x, y);
            bombs.push_back(board[y][x]);
            //mvprintw(26, 0, "%d: (%d, %d)\n", n, x, y);
            n++;
        }
    }

    for (y = 0; y < BOARD_Y; ++y) {
        for (x = 0; x < BOARD_X; ++x) {
            if (!board[y][x]) {
                board[y][x] = new tile(false, check_tile_neighbors_bombs(x, y), x, y);
            }
        }
    }
}

game_board::~game_board() {
    int x, y;

    for (y = 0; y < BOARD_Y; ++y) {
        for (x = 0; x < BOARD_X; ++x) {
            delete board[y][x];
        }
    }
}