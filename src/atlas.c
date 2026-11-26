#include "screen.h"
#include <stdio.h>
#include "atlas.h"
#include "quest.h"
#include "inventory.h"

// Story texts, one string per line on the screen
#define INTRO_LINES 10
static const char *intro[INTRO_LINES] = {
    "A big red sphere floats between the broken walls.",
    "It doesn't make a sound, but you hear it anyway.",
    "",
    "  16",
    "  16 times you came here. 16 times you forgot.",
    "  you don't remember the crash either, do you",
    "",
    "  bring me 16 Chromatic Metal. i make you a seed.",
    "",
    "New quest: The Atlas Path"
};

#define SEED_LINES 6
static const char *seed[SEED_LINES] = {
    "The metal is gone from your hands. There is a small warm seed instead.",
    "",
    "  put it in your ship. go.",
    "  last time you stayed. don't stay.",
    "",
    "You received an Atlas Seed."
};

#define ENDING_LINES 9
static const char *ending[ENDING_LINES] = {
    "You push the seed into the hyperdrive. The engine coughs, then roars.",
    "The ruins get small. The planet gets small. The stars smear.",
    "",
    "  16 times you never left.",
    "  this is 17.",
    "  see you soon, traveller",
    "",
    "=== THE END ===",
    "Thanks for playing!"
};

// Show a story screen. Any key closes it.
static void show_story(const char *lines[], int count)
{
    int i;

    clear();
    mvprintw(0, 0, "=== THE ATLAS ===");
    for (i = 0; i < count; i++) {
        mvprintw(i + 2, 0, "%s", lines[i]);
    }
    mvprintw(count + 3, 0, "Press any key to continue");
    refresh();
    getch();
}

// Called when the player presses E on the Atlas Interface.
void atlas_talk(Player *player, char *message)
{
    int status = quest_get_status(QUEST_ATLAS);

    // First visit: the story begins
    if (status == QUEST_NOT_STARTED) {
        quest_set_status(QUEST_ATLAS, QUEST_ACTIVE);
        show_story(intro, INTRO_LINES);
        return;
    }

    if (status == QUEST_DONE) {
        sprintf(message, "The Atlas is silent now.");
        return;
    }

    // The player already has a seed (made here or bought at the station)
    if (player->inv.count[ITEM_ATLAS_SEED] > 0) {
        sprintf(message, "The Atlas waits. Take the Atlas Seed to your starship.");
        return;
    }

    if (!inventory_remove(&player->inv, ITEM_CHROMATIC, ATLAS_METAL_AMOUNT)) {
        sprintf(message, "The Atlas waits for Chromatic Metal: %d/%d",
                player->inv.count[ITEM_CHROMATIC], ATLAS_METAL_AMOUNT);
        return;
    }

    inventory_add(&player->inv, ITEM_ATLAS_SEED, 1);
    show_story(seed, SEED_LINES);
}

// Called when the player presses E on the starship.
// If the ship is repaired and the player carries a seed, play the ending.
// Returns 1 if the game is over, 0 otherwise.
int atlas_try_ending(Player *player)
{
    if (quest_get_status(QUEST_ATLAS) != QUEST_ACTIVE) {
        return 0;
    }
    if (quest_get_status(QUEST_REPAIR_SHIP) != QUEST_DONE) {
        return 0;
    }
    if (!inventory_remove(&player->inv, ITEM_ATLAS_SEED, 1)) {
        return 0;
    }

    quest_set_status(QUEST_ATLAS, QUEST_DONE);
    show_story(ending, ENDING_LINES);
    return 1;
}
