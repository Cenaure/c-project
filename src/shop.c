#include "screen.h"
#include <stdio.h>
#include "shop.h"
#include "inventory.h"

static void shop_buy(Player *player, int item, char *message)
{
    int price = items[item].price;

    if (player->units < price) {
        sprintf(message, "Not enough Units to buy %s.", items[item].name);
        return;
    }
    player->units -= price;
    inventory_add(&player->inv, item, 1);
    sprintf(message, "Bought 1 %s for %d Units.", items[item].name, price);
}

static void shop_sell(Player *player, int item, char *message)
{
    int price = items[item].price / 2;

    if (!inventory_remove(&player->inv, item, 1)) {
        sprintf(message, "You don't have any %s.", items[item].name);
        return;
    }
    player->units += price;
    sprintf(message, "Sold 1 %s for %d Units.", items[item].name, price);
}

// Shop screen with its own loop. Stays open until the player presses q.
void shop_open(Player *player)
{
    int selected = 0;
    int key;
    int i;
    char message[100] = "Welcome, Traveller.";

    while (1) {
        clear();
        mvprintw(0, 0, "=== GALACTIC TRADE TERMINAL ===   Units: %d", player->units);
        mvprintw(1, 0, "    %-18s %6s %6s %5s", "Item", "Buy", "Sell", "Have");

        for (i = 0; i < ITEM_COUNT; i++) {
            if (i == selected) {
                mvprintw(i + 2, 0, ">");
            }
            mvprintw(i + 2, 4, "%-18s %6d %6d %5d",
                     items[i].name, items[i].price, items[i].price / 2,
                     player->inv.count[i]);
        }

        mvprintw(ITEM_COUNT + 3, 0, "%s", message);
        mvprintw(ITEM_COUNT + 5, 0, "W/S select   B buy   X sell   Q leave");
        refresh();

        key = getch();

        if ((key == 'w' || key == KEY_UP) && selected > 0) {
            selected--;
        } else if ((key == 's' || key == KEY_DOWN) && selected < ITEM_COUNT - 1) {
            selected++;
        } else if (key == 'b') {
            shop_buy(player, selected, message);
        } else if (key == 'x') {
            shop_sell(player, selected, message);
        } else if (key == 'q') {
            return;
        }
    }
}
