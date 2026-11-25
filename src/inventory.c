#include "screen.h"
#include "inventory.h"

const Item items[ITEM_COUNT] = {
    {"Carbon",            TYPE_RESOURCE,   10},
    {"Ferrite Dust",      TYPE_RESOURCE,   15},
    {"Sodium",            TYPE_RESOURCE,   20},
    {"Di-hydrogen",       TYPE_RESOURCE,   25},
    {"Chromatic Metal",   TYPE_RESOURCE,   40},
    {"Life Support Gel",  TYPE_CONSUMABLE, 50},
    {"Shield Battery",    TYPE_CONSUMABLE, 60},
    {"Ion Battery",       TYPE_CONSUMABLE, 70},
    {"Repair Kit",        TYPE_CONSUMABLE, 90},
    {"Nanite Cluster",    TYPE_CONSUMABLE, 120},
    {"Hyperdrive Core",   TYPE_TECH,       800},
    {"Launch Thruster",   TYPE_TECH,       500},
    {"Atlas Seed",        TYPE_TECH,       1000},
    {"Gravitino Ball",    TYPE_TECH,       300},
    {"Vortex Cube",       TYPE_TECH,       400}
};

static const char *type_names[3] = {"RESOURCES", "CONSUMABLES", "TECH & CURIOSITIES"};

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

    for (type = 0; type < 3; type++) {
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
