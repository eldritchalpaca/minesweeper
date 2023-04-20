#ifndef TILE_H
#define TILE_H

#define BOARD_X 20
#define BOARD_Y 12

#define in_bounds(x, y) (x > -1 && x < BOARD_X && y > -1 && y < BOARD_Y)

class tile {
private:
    bool is_bomb;
    int value;

public:
    tile(bool is_bomb, int value) : is_bomb(is_bomb), value(value) {}
    bool check_if_bomb() { return is_bomb; }
    int get_value() { return value; }
    ~tile() {}
};

class game_board {
private:
    int num_bombs;

public:
    int check_tile_neighbors(int x, int y);
    tile *board[BOARD_Y][BOARD_X];
    game_board(int num_bombs);
    ~game_board();
};

#endif