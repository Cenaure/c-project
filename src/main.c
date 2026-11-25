#include "screen.h"
#include <locale.h>
#include <string.h>
#include "map.h"
#include "player.h"
#include "inventory.h"
#include "shop.h"
#include "quest.h"

// Show player info and the last message below the map.
void draw_status(Player *player, char *message)
{
    char tile = map_get_tile(player->x, player->y);

    // The map with its border takes rows 0 to MAP_HEIGHT + 1
    mvprintw(MAP_HEIGHT + 3, 0, "HP: %d/%d   Units: %d   Position: %d,%d",
             player->hp, player->max_hp, player->units, player->x, player->y);

    if (tile == TILE_STATION) {
        mvprintw(MAP_HEIGHT + 4, 0, "You found a Space Station. Press E to trade.");
    } else if (tile == TILE_ATLAS) {
        mvprintw(MAP_HEIGHT + 4, 0, "The Atlas Interface hums in front of you...");
    } else if (tile == TILE_SHIP && strcmp(message, "") == 0) {
        if (quest_get_status(QUEST_REPAIR_SHIP) == QUEST_DONE) {
            mvprintw(MAP_HEIGHT + 4, 0, "Your starship, repaired and ready to fly.");
        } else {
            mvprintw(MAP_HEIGHT + 4, 0, "Your crashed starship. Press E to repair it.");
        }
    } else {
        mvprintw(MAP_HEIGHT + 4, 0, "%s", message);
    }

    map_draw_legend(MAP_HEIGHT + 6);
    mvprintw(MAP_HEIGHT + 7, 0, "Move: WASD/arrows   E interact   I inventory   J quests   Q quit");
}

int main(void)
{
    Player player;
    int key;
    int running = 1;
    char message[100] = "";

    setlocale(LC_ALL, ""); // use the terminal's UTF-8, needed for line borders
    initscr();             // start ncurses
    noecho();              // don't print pressed keys
    cbreak();              // read keys without pressing Enter
    keypad(stdscr, TRUE);  // allow arrow keys
    curs_set(0);           // hide the cursor
    map_init_colors();

    map_load();
    player_init(&player);
    quest_init();

    while (running) {
        clear();
        map_draw();
        player_draw(&player);
        draw_status(&player, message);
        refresh();

        key = getch();
        strcpy(message, "");

        if (key == 'w' || key == KEY_UP) {
            player_move(&player, 0, -1);
        } else if (key == 's' || key == KEY_DOWN) {
            player_move(&player, 0, 1);
        } else if (key == 'a' || key == KEY_LEFT) {
            player_move(&player, -1, 0);
        } else if (key == 'd' || key == KEY_RIGHT) {
            player_move(&player, 1, 0);
        } else if (key == 'i') {
            inventory_show(&player.inv, player.units);
        } else if (key == 'j') {
            quest_show(&player);
        } else if (key == 'e') {
            if (map_get_tile(player.x, player.y) == TILE_STATION) {
                shop_open(&player);
            } else if (map_get_tile(player.x, player.y) == TILE_SHIP) {
                quest_repair_ship(&player, message);
            }
        } else if (key == 'q') {
            running = 0;
        }

        // Walking onto a resource tile mines it
        player_mine(&player, message);
    }

    endwin(); // stop ncurses
    return 0;
}
