#ifndef INVENTORY_H
#define INVENTORY_H

// Item types
#define TYPE_RESOURCE   0
#define TYPE_CONSUMABLE 1
#define TYPE_TECH       2

// Resources (mined from tiles)
#define ITEM_CARBON      0
#define ITEM_FERRITE     1
#define ITEM_SODIUM      2
#define ITEM_DIHYDROGEN  3
#define ITEM_CHROMATIC   4
// Consumables
#define ITEM_LIFE_GEL    5
#define ITEM_SHIELD_BAT  6
#define ITEM_ION_BAT     7
#define ITEM_REPAIR_KIT  8
#define ITEM_NANITES     9
// Tech and curiosities
#define ITEM_HYPERDRIVE  10
#define ITEM_THRUSTER    11
#define ITEM_ATLAS_SEED  12
#define ITEM_GRAVITINO   13
#define ITEM_VORTEX_CUBE 14

#define ITEM_COUNT 15

typedef struct {
    const char *name;
    int type;
    int price; // buy price, selling gives half
} Item;

typedef struct {
    int count[ITEM_COUNT]; // how many of each item the player has
} Inventory;

// Table with all items, defined in inventory.c
extern const Item items[ITEM_COUNT];

void inventory_init(Inventory *inv);
void inventory_add(Inventory *inv, int item, int amount);
int inventory_remove(Inventory *inv, int item, int amount);
void inventory_show(Inventory *inv, int units);

#endif
