#include "game.h"
#include "graphics.h"

#include <stdlib.h>
#include <time.h>

int running = true;
long tick_counter = 0;
struct tile game_data[WIDTH*HEIGHT];
struct pill pill;
int drop_timer;

int get_i(struct coords coords)
{
    if (coords.y < 0 || !is_pill_in_bounds(coords))
        return -1;
    return coords.x + coords.y * WIDTH;
}

struct coords co(int x, int y)
{
    return (struct coords) {x, y};
}

struct coords add_coords(struct coords a, struct coords b)
{
    return co(a.x + b.x, a.y + b.y);
}

void new_pill()
{
    pill = (struct pill) {co(3, 0), false, rand()%3, rand()%3};
    drop_timer = 0;
}

struct coords get_other_pill_coords(bool invert)
{
    return add_coords(pill.coords, (pill.up ^ invert) ? co(0, -1) : co(1, 0));
}

bool is_pill_in_bounds(struct coords coords)
{
    return (coords.x >= 0 && coords.x < WIDTH && coords.y < HEIGHT);
}

bool can_move_pill(struct coords off)
{
    struct coords other_pill_coords = get_other_pill_coords(false);
    struct coords n_coords = add_coords(pill.coords, off);
    struct coords n_other_coords = add_coords(other_pill_coords, off);
    return (is_pill_in_bounds(n_coords) && is_pill_in_bounds(n_other_coords) && (get_tile_c(n_coords).id == TILE_EMPTY) && (get_tile_c(n_other_coords).id == TILE_EMPTY || get_tile_c(n_other_coords).id == TILE_ERROR));
}

bool can_rotate_pill()
{
    struct coords other_pill_coords = get_other_pill_coords(true);
    int other_id = get_tile_c(other_pill_coords).id;
    return (is_pill_in_bounds(other_pill_coords) && (other_id == TILE_EMPTY || other_id == TILE_ERROR));
}

void rotate_pill()
{
    if (!can_rotate_pill())
    {
        return;
    }
    if (pill.up)
    {
        int temp_col = pill.col_0;
        pill.col_0 = pill.col_1;
        pill.col_1 = temp_col;
    }
    pill.up = !pill.up;
    draw(tick_counter);
}

void drop_pill()
{
    while (!update())
    {
    }
}

void fast_fall()
{
    drop_timer = 0;
    update();
}

bool move_pill(struct coords off)
{
    if (!can_move_pill(off))
    {
        return false;
    }
    pill.coords = add_coords(pill.coords, off);
    draw(tick_counter);
    return true;
}

void key_down(int c)
{
    if (pill.coords.x == -1)
        return;
    switch (c)
    {
        case 'k':
            rotate_pill();
            break;
        case 'j':
            move_pill(co(-1, 0));
            break;
        case 'l':
            move_pill(co(1, 0));
            break;
        case ' ':
            drop_pill();
            break;
        case 'a':
            fast_fall();
            break;
    }
}

struct coords get_other_side(int x, int y, int id)
{
    switch (id)
    {
        case TILE_LEFT:
            return co(x+1, y);
        case TILE_TOP:
            return co(x, y+1);
        case TILE_RIGHT:
            return co(x-1, y);
        case TILE_BOTTOM:
            return co(x, y-1);
    }
    return co(-1, -1);
}

struct tile get_tile(int index)
{
    if (index < 0 || index >= WIDTH*HEIGHT)
    {
        return (struct tile){TILE_ERROR, COLOR_ERROR};
    }
    return game_data[index];
}

struct tile get_tile_c(struct coords coords)
{
    return get_tile(get_i(coords));
}

bool check_updates()
{
    bool ret = false;
    ret |= connect_4(false);
    ret |= connect_4(true);
    return ret;
}

bool connect_4(bool dir)
{
    bool destroyed_something = false;
    for (int i = 0; i < WIDTH-(dir?0:3); i++)
    {
        for (int j = 0; j < HEIGHT-(dir?3:0); j++)
        {
            int watching_color = get_tile_c(co(i, j)).color;
            if (watching_color == COLOR_ERROR)
                continue;
            bool can_be_4 = true;
            for (int k = 0; k < 4; k++)
            {
                struct tile curr_tile = get_tile_c(co(i+(dir?0:k), j+(dir?k:0)));
                if (curr_tile.id == TILE_EMPTY || curr_tile.id == TILE_ERROR || curr_tile.color != watching_color)
                    can_be_4 = false;
            }
            if (!can_be_4)
                continue;
            destroyed_something = true;
            for (int k = 0; k < 4; k++)
            {
                destroy_tile(co(i+(dir?0:k), j+(dir?k:0)));
            }
        }
    }
    return destroyed_something;
}

bool clear_destroyed()
{
    bool ret = false;
    for (int i = 0; i < WIDTH; i++)
    {
        for (int j = 0; j < HEIGHT; j++)
        {
            if (get_tile_c(co(i, j)).id != TILE_DELETED)
                continue;
            ret = true;
            game_data[get_i(co(i, j))] = (struct tile) {TILE_EMPTY, COLOR_ERROR};
        }
    }
    return ret;
}

bool gravity()
{
    bool ret = false;
    for (int j = HEIGHT-1; j >= 0; j--)
    {
        for (int i = 0; i < WIDTH; i++)
        {
            int tile_id = get_tile_c(co(i, j)).id;
            if (!(tile_id == TILE_LEFT || tile_id == TILE_RIGHT || tile_id == TILE_TOP || tile_id == TILE_BOTTOM || tile_id == TILE_SINGLE))
                continue;
            struct coords other_side = get_other_side(i, j, tile_id);            
            if ((j < HEIGHT-1 && get_tile_c(co(i, j+1)).id == TILE_EMPTY) &&
                (other_side.x == -1 || get_tile_c(other_side).id == TILE_EMPTY ||
                 (other_side.y < HEIGHT-1 && get_tile_c(add_coords(other_side, co(0, 1))).id == TILE_EMPTY)))
            {
                game_data[get_i(co(i, j+1))] = get_tile_c(co(i, j));
                game_data[get_i(co(i, j))] = (struct tile) {TILE_EMPTY, COLOR_ERROR};
                ret = true;
            }
        }
    }
    return ret;
}

void destroy_tile(struct coords coords)
{
    struct tile curr_tile = get_tile_c(coords);
    struct coords other_coords = get_other_side(coords.x, coords.y, curr_tile.id);
    game_data[get_i(coords)].id = TILE_DELETED;
    if (other_coords.x == -1)
        return;
    int other_id = get_tile_c(other_coords).id;
    if (other_id == TILE_LEFT || other_id == TILE_TOP || other_id == TILE_RIGHT || other_id == TILE_BOTTOM)
        game_data[get_i(co(other_coords.x, other_coords.y))].id = TILE_SINGLE;
}

void solidify_pill()
{
    game_data[get_i(pill.coords)] = (struct tile){(pill.up ? TILE_BOTTOM : TILE_LEFT), pill.col_0};
    struct coords other_pill_coords = get_other_pill_coords(false);
    int index = get_i(other_pill_coords);
    if (index != -1)
    {
        game_data[index]= (struct tile){(pill.up ? TILE_TOP : TILE_RIGHT), pill.col_1};
    }

    pill.coords = co(-1, -1);
    draw(tick_counter);
}

bool update()
{
    if (move_pill(co(0, 1)))
        return false;
    solidify_pill();
    return true;
}

void place_virus(int start, int offset)
{
    int index = start;
    while (offset || game_data[index].id != TILE_EMPTY)
    {
        if (game_data[index].id == TILE_EMPTY)
        {
            offset--;
        }
        index++;
    }
    game_data[index].id = TILE_ENEMY;
    game_data[index].color = rand()%3;
}

void spawn_viruses(int virus_level)
{
    if (virus_level < 0)
    {
        virus_level = 0;
    }
    if (virus_level > 20)
    {
        virus_level = 20;
    }

    int MIN_INDEX = WIDTH*(HEIGHT-12);
    int MAX_INDEX = WIDTH*HEIGHT;
    int virus_count = virus_level*4+4;
    int placed_viruses = 0;
    while (placed_viruses < virus_count)
    {
        place_virus(MIN_INDEX, rand()%(MAX_INDEX-MIN_INDEX-placed_viruses));
        placed_viruses++;
    }
}

void init(void)
{
    srand(time(NULL));

    for (int i = 0; i < WIDTH*HEIGHT; i++)
    {
        game_data[i].id = TILE_EMPTY;
        game_data[i].color = COLOR_ERROR;
    }
    spawn_viruses(10);

    new_pill();
}

void loop(void)
{
    tick_counter++;
    if (clear_destroyed())
    {
        draw(tick_counter);
        return;
    }
    if (gravity())
    {
        draw(tick_counter);
        return;
    }
    if (check_updates())
    {
        draw(tick_counter);
        return;
    }
    if (pill.coords.x == -1)
    {
        new_pill();
        draw(tick_counter);
        return;
    }
    drop_timer++;
    if (drop_timer >= DROP_TICKS)
    {
        drop_timer = 0;
        update();
        draw(tick_counter);
        return;
    }
    draw(tick_counter);
}
