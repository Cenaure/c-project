#include "screen.h"
#include <stdio.h>
#include "player.h"
#include "map.h"

void player_init(Player *player)
{
    player->x = 0;
    player->y = 0;
    player->max_hp = 100;
    player->hp = 100;
    player->units = 200;

    inventory_init(&player->inv);
    inventory_add(&player->inv, ITEM_LIFE_GEL, 1);
}

void player_move(Player *player, int dx, int dy)
{
    int new_x = player->x + dx;
    int new_y = player->y + dy;
    int old_area = map_get_area();

    // Leaving on one side means arriving on the opposite side of the next area
    if (new_x < 0 && map_go_to_area(-1, 0)) {
        new_x = MAP_WIDTH - 1;
    } else if (new_x >= MAP_WIDTH && map_go_to_area(1, 0)) {
        new_x = 0;
    } else if (new_y < 0 && map_go_to_area(0, -1)) {
        new_y = MAP_HEIGHT - 1;
    } else if (new_y >= MAP_HEIGHT && map_go_to_area(0, 1)) {
        new_y = 0;
    }

    if (map_is_walkable(new_x, new_y)) {
        player->x = new_x;
        player->y = new_y;
    } else {
        map_set_area(old_area);
    }
}

void player_draw(Player *player)
{
    // +1 because the map is drawn inside a border
    attron(COLOR_PAIR(COLOR_PLAYER) | A_BOLD);
    mvaddch(player->y + 1, player->x + 1, '@');
    attroff(COLOR_PAIR(COLOR_PLAYER) | A_BOLD);
}

// If the player stands on a resource tile, mine it.
// Returns 1 if something was mined, 0 otherwise.
int player_mine(Player *player, char *message)
{
    char tile = map_get_tile(player->x, player->y);
    int item;
    int amount;

    if (tile == TILE_CARBON) {
        item = ITEM_CARBON;
        amount = 5;
    } else if (tile == TILE_FERRITE) {
        item = ITEM_FERRITE;
        amount = 4;
    } else if (tile == TILE_SODIUM) {
        item = ITEM_SODIUM;
        amount = 3;
    } else if (tile == TILE_DIHYDROGEN) {
        item = ITEM_DIHYDROGEN;
        amount = 3;
    } else if (tile == TILE_CHROMATIC) {
        item = ITEM_CHROMATIC;
        amount = 2;
    } else {
        return 0;
    }

    inventory_add(&player->inv, item, amount);
    map_set_tile(player->x, player->y, TILE_GROUND);
    sprintf(message, "Mined %d %s.", amount, items[item].name);
    return 1;
}
