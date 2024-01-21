#pragma once

#include <stdbool.h>

#define WIDTH 8
#define HEIGHT 16

#define TILE_ERROR -1
#define TILE_EMPTY 0
#define TILE_LEFT 1
#define TILE_TOP 2
#define TILE_RIGHT 3
#define TILE_BOTTOM 4
#define TILE_SINGLE 5
#define TILE_ENEMY 6
#define TILE_DELETED 7

#define COLOR_ERROR -1
#define COLOR_RED 0
#define COLOR_YELLOW 1
#define COLOR_BLUE 2

#define DROP_TICKS 5

struct tile {
    int id;
    int color;
};

struct coords {
    int x;
    int y;
};

struct pill {
    struct coords coords;
    bool up;
    int col_0;
    int col_1;
};

bool update();
void key_down(int c);
void init();
void loop();
int get_i(struct coords coords);
struct coords get_other_pill_coords(bool invert);
extern int running;
extern struct pill pill;
extern struct tile game_data[WIDTH*HEIGHT];
bool is_pill_in_bounds(struct coords coords);
struct tile get_tile_c(struct coords coords);
struct coords co(int x, int y);
struct coords add_coords(struct coords a, struct coords b);
void new_pill();
bool can_move_pill(struct coords off);
bool can_rotate_pill();
void rotate_pill();
void drop_pill();
bool move_pill(struct coords off);
struct coords get_other_side(int x, int y, int id);
struct tile get_tile(int index);
void check_direction(int checking_rows);
bool check_updates();
void solidify_pill();
void place_virus(int start, int offset);
void spawn_viruses(int virus_level);
bool connect_4(bool dir);
void destroy_tile(struct coords coords);
