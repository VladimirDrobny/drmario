#include "graphics.h"
bool drawing = false;

void print_tile(struct tile *tile, int part, long tick_counter)
{
    switch(tile->color)
    {
        case COLOR_ERROR:
            printf("\x1b[45m");
            break;
        case COLOR_RED:
            printf("\x1b[41m");
            break;
        case COLOR_YELLOW:
            printf("\x1b[43m");
            break;
        case COLOR_BLUE:
            printf("\x1b[44m");
            break;
    }
    switch(tile->id)
    {
        case TILE_EMPTY:
            printf("\x1b[40m");
            printf("    ");
            break;
        case TILE_LEFT:
            if (part)
            {
                printf("%lc", 0x2517);
                printf("%lc", 0x2501);
                printf("%lc", 0x2501);
                printf("%lc", 0x2501);
            } else {
                printf("%lc", 0x250f);
                printf("%lc", 0x2501);
                printf("%lc", 0x2501);
                printf("%lc", 0x2501);
            }
            break;
        case TILE_TOP:
            if (part)
            {
                printf("%lc", 0x2503);
                printf("  ");
                printf("%lc", 0x2503);
            } else {
                printf("%lc", 0x250f);
                printf("%lc", 0x2501);
                printf("%lc", 0x2501);
                printf("%lc", 0x2513);
            }
            break;
        case TILE_RIGHT:
            if (part)
            {
                printf("%lc", 0x2501);
                printf("%lc", 0x2501);
                printf("%lc", 0x2501);
                printf("%lc", 0x251b);
            } else {
                printf("%lc", 0x2501);
                printf("%lc", 0x2501);
                printf("%lc", 0x2501);
                printf("%lc", 0x2513);
            }
            break;
        case TILE_BOTTOM:
            if (part)
            {
                printf("%lc", 0x2517);
                printf("%lc", 0x2501);
                printf("%lc", 0x2501);
                printf("%lc", 0x251b);
            } else {
                printf("%lc", 0x2503);
                printf("  ");
                printf("%lc", 0x2503);
            }
            break;
        case TILE_SINGLE:
            if (part)
            {
                printf("%lc", 0x2517);
                printf("%lc", 0x2501);
                printf("%lc", 0x2501);
                printf("%lc", 0x251b);
            } else {
                printf("%lc", 0x250f);
                printf("%lc", 0x2501);
                printf("%lc", 0x2501);
                printf("%lc", 0x2513);
            }
            break;
        case TILE_ENEMY:
            switch (tile->color)
            {
                case COLOR_RED:
                    if (tick_counter % 2 == 0)
                    {
                        if (part)
                        {
                            printf("%lc", 0x2560);
                            printf("%lc", 0x2569);
                            printf("%lc", 0x2569);
                            printf("%lc", 0x2563);
                        } else {
                            printf(" ");
                            printf("%lc", 0x2554);
                            printf("%lc", 0x2557);
                            printf(" ");
                        }
                    } else {
                        if (part)
                        {
                            printf("%lc", 0x255a);
                            printf("%lc", 0x256c);
                            printf("%lc", 0x256c);
                            printf("%lc", 0x255d);
                        } else {
                            printf(" ");
                            printf("%lc", 0x2554);
                            printf("%lc", 0x2557);
                            printf(" ");
                        }
                    }
                    break;
                case COLOR_YELLOW:
                    if (tick_counter % 2 == 0)
                    {
                        if (part)
                        {
                            printf("%lc", 0x256c);
                            printf("%lc", 0x255a);
                            printf("%lc", 0x255d);
                            printf("%lc", 0x256c);
                        } else {
                            printf("%lc", 0x2554);
                            printf("%lc", 0x2566);
                            printf("%lc", 0x2566);
                            printf("%lc", 0x2557);
                        }
                    } else {
                        if (part)
                        {
                            printf("%lc", 0x255a);
                            printf("%lc", 0x2569);
                            printf("%lc", 0x2569);
                            printf("%lc", 0x255d);
                        } else {
                            printf("%lc", 0x256c);
                            printf("%lc", 0x2554);
                            printf("%lc", 0x2557);
                            printf("%lc", 0x256c);
                        }
                    }
                    break;
                case COLOR_BLUE:
                    if (tick_counter % 2 == 0)
                    {
                        if (part)
                        {
                            printf("%lc", 0x2560);
                            printf("%lc", 0x2550);
                            printf("%lc", 0x2550);
                            printf("%lc", 0x2563);
                        } else {
                            printf("%lc", 0x2554);
                            printf("%lc", 0x2566);
                            printf("%lc", 0x2566);
                            printf("%lc", 0x2557);
                        }
                    } else {
                        if (part)
                        {
                            printf("%lc", 0x255a);
                            printf("%lc", 0x2569);
                            printf("%lc", 0x2569);
                            printf("%lc", 0x255d);
                        } else {
                            printf("%lc", 0x2560);
                            printf("%lc", 0x2550);
                            printf("%lc", 0x2550);
                            printf("%lc", 0x2563);
                        }
                    }
                    break;
            }
            break;
        case TILE_DELETED:
            printf("%lc", 0x259a);
            printf("%lc", 0x259a);
            printf("%lc", 0x259a);
            printf("%lc", 0x259a);
            break;
        case TILE_ERROR:
            printf("%lc", 0x259a);
            printf("%lc", 0x259a);
            printf("%lc", 0x259a);
            printf("%lc", 0x259a);
            break;
        default:
            printf("%lc", 0x259a);
            printf("%lc", 0x259a);
            printf("%lc", 0x259a);
            printf("%lc", 0x259a);
            break;
    }
}

void draw(long tick_counter)
{
    if (drawing)
        return;
    drawing = true;
    printf("\x1b[u"); // Load cursor position

    struct coords other_pill_coords = get_other_pill_coords(false);
    if (pill.coords.x == -1)
    {
        other_pill_coords = co(-1, -1);
    }

    for (int j = 0; j < HEIGHT; j++)
    {
        for (int k = 0; k < 2; k++)
        {
            for (int i = 0; i < WIDTH; i++)
            {
                struct tile current_tile;
                if (pill.coords.x == i && pill.coords.y == j)
                {
                    current_tile = (struct tile){(pill.up ? TILE_BOTTOM : TILE_LEFT), pill.col_0};
                } else if (i == other_pill_coords.x && j == other_pill_coords.y)
                {
                    current_tile = (struct tile){(pill.up ? TILE_TOP : TILE_RIGHT), pill.col_1};
                } else
                {
                    current_tile = get_tile_c(co(i,j));
                }
                print_tile(&current_tile, k, tick_counter);
            }
            printf("\n");
        }
    }
    drawing = false;
}
