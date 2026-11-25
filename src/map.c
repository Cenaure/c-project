#include "screen.h"
#include "map.h"

// All areas of the planet. Every row has exactly MAP_WIDTH characters.
// The border is not stored here, map_draw() draws it around the map.
static const char planet[AREA_COUNT][MAP_HEIGHT][MAP_WIDTH + 1] = {
    { // 0: Landing Site
        "...c..........##.................f......",
        "....X.........##......c.................",
        "..S.......................###...........",
        "............c.............###.....n.....",
        "....###.................................",
        "....###.........f.......c...........##..",
        "....................................##..",
        "..f..........##.........................",
        ".............##.......##.........c......",
        "....c.................##................",
        "................n.................###...",
        "..##....................c.........###...",
        "..##........f...........................",
        "..................................f....."
    },
    { // 1: Ferrite Plains
        ".............f....................##....",
        "...f..................##..........##....",
        "..............f.......##...f............",
        "....###.........................f.......",
        "....###.....f...........m...............",
        "....................................###.",
        "..f..........###......f.............###.",
        ".............###........................",
        ".....f....................##.....f......",
        "............c.............##............",
        "..##..............................f.....",
        "..##..........f........m................",
        "................................##......",
        "....f...................f.......##......"
    },
    { // 2: Sodium Fields
        "..............n.........................",
        "...n....................##.......n......",
        "............###.........##..............",
        "......n.....###.....................h...",
        "......................n.................",
        "..##............h................###....",
        "..##......................n......###....",
        "............n...........................",
        "....h.................##........n.......",
        "..............###.....##................",
        "..n...........###.................h.....",
        "..........................n.............",
        "...###......h...................##......",
        "...###..................n.......##......"
    },
    { // 3: Atlas Ruins
        ".......................m................",
        "..##..........h...................##....",
        "..##..................###.........##....",
        "............m.........###...............",
        "....h...........................m.......",
        "..............#########.................",
        "..............#.......#.........h.......",
        "..............#...A...#.................",
        "..............###...###.................",
        "...m..............................##....",
        "........................h.........##....",
        "..###...................................",
        "..###........m...................h......",
        "........................................"
    }
};

// The map we change during the game (mined tiles become ground).
static char map[AREA_COUNT][MAP_HEIGHT][MAP_WIDTH];

// The area the player is in right now
static int current_area = 0;

// Background -1 means the terminal's normal background.
void map_init_colors(void)
{
    if (!has_colors()) {
        return;
    }
    start_color();
    use_default_colors();

    init_pair(COLOR_CARBON,     COLOR_RED,     -1);
    init_pair(COLOR_FERRITE,    COLOR_WHITE,   -1);
    init_pair(COLOR_SODIUM,     COLOR_YELLOW,  -1);
    init_pair(COLOR_DIHYDROGEN, COLOR_BLUE,    -1);
    init_pair(COLOR_CHROMATIC,  COLOR_GREEN,   -1);
    init_pair(COLOR_STATION,    COLOR_CYAN,    -1);
    init_pair(COLOR_ATLAS,      COLOR_MAGENTA, -1);
    init_pair(COLOR_PLAYER,     COLOR_MAGENTA,   -1);
    init_pair(COLOR_SHIP,       COLOR_WHITE,   COLOR_RED);
}

// Copy all areas into the map we use during the game.
void map_load(void)
{
    int a, x, y;

    for (a = 0; a < AREA_COUNT; a++) {
        for (y = 0; y < MAP_HEIGHT; y++) {
            for (x = 0; x < MAP_WIDTH; x++) {
                map[a][y][x] = planet[a][y][x];
            }
        }
    }
    current_area = 0;
}

int map_get_area(void)
{
    return current_area;
}

void map_set_area(int area)
{
    current_area = area;
}

// Go to the neighbouring area in direction dx, dy (for example 1, 0 = right).
// Returns 1 if there is an area there, 0 if it is the end of the world.
int map_go_to_area(int dx, int dy)
{
    int col = current_area % WORLD_COLS + dx;
    int row = current_area / WORLD_COLS + dy;

    if (col < 0 || col >= WORLD_COLS || row < 0 || row >= WORLD_ROWS) {
        return 0;
    }
    current_area = row * WORLD_COLS + col;
    return 1;
}

char map_get_tile(int x, int y)
{
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) {
        return TILE_ROCK;
    }
    return map[current_area][y][x];
}

void map_set_tile(int x, int y, char tile)
{
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) {
        return;
    }
    map[current_area][y][x] = tile;
}

// The player can walk anywhere except rocks and outside the map.
int map_is_walkable(int x, int y)
{
    if (map_get_tile(x, y) == TILE_ROCK) {
        return 0;
    }
    return 1;
}

// Which color pair a tile uses, 0 means no color.
static int tile_color(char tile)
{
    if (tile == TILE_CARBON)     return COLOR_CARBON;
    if (tile == TILE_FERRITE)    return COLOR_FERRITE;
    if (tile == TILE_SODIUM)     return COLOR_SODIUM;
    if (tile == TILE_DIHYDROGEN) return COLOR_DIHYDROGEN;
    if (tile == TILE_CHROMATIC)  return COLOR_CHROMATIC;
    if (tile == TILE_STATION)    return COLOR_STATION;
    if (tile == TILE_ATLAS)      return COLOR_ATLAS;
    if (tile == TILE_SHIP)       return COLOR_SHIP;
    return 0;
}

// Draw one tile at screen position (row, col) in its color.
static void draw_tile(int row, int col, char tile)
{
    int color = tile_color(tile);

    if (tile == TILE_GROUND) {
        mvaddch(row, col, ' ');
    } else if (tile == TILE_ROCK) {
        mvaddch(row, col, TILE_ROCK);
    } else {
        attron(COLOR_PAIR(color) | A_BOLD);
        mvaddch(row, col, tile);
        attroff(COLOR_PAIR(color) | A_BOLD);
    }
}

// Draw a # border around the map.
// An arrow in the middle of a side means you can walk to another area there.
static void map_draw_border(void)
{
    int bottom = MAP_HEIGHT + 1;
    int right = MAP_WIDTH + 1;
    int col = current_area % WORLD_COLS;
    int row = current_area / WORLD_COLS;

    mvhline(0, 0, '#', MAP_WIDTH + 2);
    mvhline(bottom, 0, '#', MAP_WIDTH + 2);
    mvvline(1, 0, '#', MAP_HEIGHT);
    mvvline(1, right, '#', MAP_HEIGHT);

    if (row > 0)              mvaddch(0, right / 2, ACS_UARROW);
    if (row < WORLD_ROWS - 1) mvaddch(bottom, right / 2, ACS_DARROW);
    if (col > 0)              mvaddch(bottom / 2, 0, ACS_LARROW);
    if (col < WORLD_COLS - 1) mvaddch(bottom / 2, right, ACS_RARROW);
}

// Tile (x, y) is drawn at screen position (x + 1, y + 1), inside the border.
void map_draw(void)
{
    int x, y;

    map_draw_border();

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            draw_tile(y + 1, x + 1, map[current_area][y][x]);
        }
    }
}

// Show what each colored letter means.
void map_draw_legend(int row)
{
    draw_tile(row, 0, TILE_CARBON);      mvprintw(row, 2,  "Carbon");
    draw_tile(row, 10, TILE_FERRITE);    mvprintw(row, 12, "Ferrite");
    draw_tile(row, 21, TILE_SODIUM);     mvprintw(row, 23, "Sodium");
    draw_tile(row, 31, TILE_DIHYDROGEN); mvprintw(row, 33, "Di-hydrogen");
    draw_tile(row, 46, TILE_CHROMATIC);  mvprintw(row, 48, "Chromatic");
    draw_tile(row, 59, TILE_STATION);    mvprintw(row, 61, "Station");
    draw_tile(row, 70, TILE_ATLAS);      mvprintw(row, 72, "Atlas");
}
