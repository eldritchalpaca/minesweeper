#ifndef TILE_H
#define TILE_H

#include <vector>

#define BOARD_X 20
#define BOARD_Y 12

#define in_bounds(x, y) (x > -1 && x < BOARD_X && y > -1 && y < BOARD_Y)

class tile {
private:
    bool is_bomb;
    int value;
    int x;
    int y;

public:
    bool is_clicked;
    bool is_flagged;
    bool is_highlighted;
    tile(bool is_bomb, int value, int x, int y) : is_bomb(is_bomb), value(value), x(x), y(y), is_clicked(false), is_flagged(false), is_highlighted(false) {}
    bool check_if_bomb() { return is_bomb; }
    int get_value() { return value; }
    int get_x() { return x; }
    int get_y() { return y; }
    ~tile() {}
};

class game_board {
private:
    int num_bombs;
    std::vector<tile *> bombs;
    int num_flags;
public:
    tile *board[BOARD_Y][BOARD_X];

    void increment_flags() { num_flags++; }
    void decrement_flags() { if (num_flags > 0) { num_flags--; } }
    int get_num_flags() { return num_flags; }

    std::vector<tile *> get_tile_neighbors(int x, int y);
    int check_tile_neighbors_bombs(int x, int y);
    int check_tile_neighbors_flags(int x, int y);

    bool have_won();
    
    game_board(int num_bombs, int click_x, int click_y);
    ~game_board();
};

#endif