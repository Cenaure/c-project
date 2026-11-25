#ifndef MAP_H
#define MAP_H

// Size of one area
#define MAP_WIDTH  40
#define MAP_HEIGHT 14

// The world is a grid of areas: 2 columns x 2 rows = 4 areas.
// Area number = row * WORLD_COLS + col
//     0 1
//     2 3
#define WORLD_COLS 2
#define WORLD_ROWS 2
#define AREA_COUNT (WORLD_COLS * WORLD_ROWS)

// Map tiles
#define TILE_GROUND  '.'
#define TILE_ROCK    '#'
#define TILE_STATION 'S'
#define TILE_ATLAS   'A'

// Resource tiles, mined by walking on them
#define TILE_CARBON     'c'
#define TILE_FERRITE    'f'
#define TILE_SODIUM     'n'
#define TILE_DIHYDROGEN 'h'
#define TILE_CHROMATIC  'm'

// Color pair numbers
#define COLOR_CARBON     1
#define COLOR_FERRITE    2
#define COLOR_SODIUM     3
#define COLOR_DIHYDROGEN 4
#define COLOR_CHROMATIC  5
#define COLOR_STATION    6
#define COLOR_ATLAS      7
#define COLOR_PLAYER     8

void map_init_colors(void);
void map_load(void);
int map_get_area(void);
void map_set_area(int area);
int map_go_to_area(int dx, int dy);
char map_get_tile(int x, int y);
void map_set_tile(int x, int y, char tile);
int map_is_walkable(int x, int y);
void map_draw(void);
void map_draw_legend(int row);

#endif
