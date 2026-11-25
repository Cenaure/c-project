#pragma once

#include "inventory.h"

typedef struct {
    int x;
    int y;
    int hp;
    int max_hp;
    int units; // no man's sky money
    Inventory inv;
} Player;

void player_init(Player *player);
void player_move(Player *player, int dx, int dy);
void player_draw(Player *player);
int player_mine(Player *player, char *message);

