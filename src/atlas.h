#pragma once

#include "player.h"

// How much Chromatic Metal the Atlas needs to make an Atlas Seed.
// 16 is the number that keeps coming back in No Man's Sky, and the
// planet has exactly 16 Chromatic Metal if you mine every 'm' tile.
#define ATLAS_METAL_AMOUNT 16

void atlas_talk(Player *player, char *message);
int atlas_try_ending(Player *player);
