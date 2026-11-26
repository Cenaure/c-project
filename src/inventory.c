#include "screen.h"
#include "inventory.h"

const Item items[ITEM_COUNT] = {
    {"Carbon",            TYPE_RESOURCE,   10},
    {"Ferrite Dust",      TYPE_RESOURCE,   15},
    {"Sodium",            TYPE_RESOURCE,   20},
    {"Di-hydrogen",       TYPE_RESOURCE,   25},
    {"Chromatic Metal",   TYPE_RESOURCE,   40},
    {"Launch Thruster",   TYPE_TECH,       500},
    {"Atlas Seed",        TYPE_TECH,       1000}
};

static const char *type_names[TYPE_COUNT] = {"RESOURCES", "TECH"};

void inventory_init(Inventory *inv)
{
    int i;

    for (i = 0; i < ITEM_COUNT; i++) {
        inv->count[i] = 0;
    }
}

void inventory_add(Inventory *inv, int item, int amount)
{
    inv->count[item] += amount;
}

// Returns 1 if the items were removed, 0 if the player doesn't have enough.
int inventory_remove(Inventory *inv, int item, int amount)
{
    if (inv->count[item] < amount) {
        return 0;
    }
    inv->count[item] -= amount;
    return 1;
}

// Inventory screen: lists all items grouped by type. Any key closes it.
void inventory_show(Inventory *inv, int units)
{
    int type, i;
    int row = 2;

    clear();
    mvprintw(0, 0, "=== EXOSUIT INVENTORY ===   Units: %d", units);

    for (type = 0; type < TYPE_COUNT; type++) {
        mvprintw(row, 0, "%s", type_names[type]);
        row++;
        for (i = 0; i < ITEM_COUNT; i++) {
            if (items[i].type == type) {
                mvprintw(row, 2, "%-18s x%d", items[i].name, inv->count[i]);
                row++;
            }
        }
        row++;
    }

    mvprintw(row, 0, "Press any key to close");
    refresh();
    getch();
}
