#include "screen.h"
#include <stdio.h>
#include "quest.h"
#include "inventory.h"
#include "dialog.h"
#include "atlas.h"

static Quest quests[QUEST_COUNT] = {
    {"Repair your starship",
     "Your ship crashed at the Landing Site. Gather materials or buy them at the space station to fix it.",
     QUEST_NOT_STARTED},
    {"The Gek's Request",
     "A Gek in the Sodium Fields wants something. Learn its words from stones (K).",
     QUEST_NOT_STARTED},
    {"The Atlas Path",
     "A red sphere in the Atlas Ruins says you were here 16 times before.",
     QUEST_NOT_STARTED}
};

#define REPAIR_ITEM_COUNT 4
static const int repair_items[REPAIR_ITEM_COUNT]   = {ITEM_FERRITE, ITEM_CARBON, ITEM_DIHYDROGEN, ITEM_THRUSTER};
static const int repair_amounts[REPAIR_ITEM_COUNT] = {12, 10, 3, 2};

#define REPAIR_REWARD 300

static const char *statuses[3] = {"Not started", "Active", "Done"};

// The repair quest is active from the start, the player wakes up next to the ship.
void quest_init(void)
{
    quests[QUEST_REPAIR_SHIP].status = QUEST_ACTIVE;
}

int quest_get_status(int quest)
{
    return quests[quest].status;
}

void quest_set_status(int quest, int status)
{
    quests[quest].status = status;
}

// Called when the player presses E on the ship tile.
void quest_repair_ship(Player *player, char *message)
{
    int i;
    int item;

    if (quests[QUEST_REPAIR_SHIP].status == QUEST_DONE) {
        sprintf(message, "Your starship is already repaired.");
        return;
    }

    // First check that the player has everything
    for (i = 0; i < REPAIR_ITEM_COUNT; i++) {
        item = repair_items[i];
        if (player->inv.count[item] < repair_amounts[i]) {
            sprintf(message, "Not enough %s: %d/%d. Press J to see the quest.",
                    items[item].name, player->inv.count[item], repair_amounts[i]);
            return;
        }
    }

    // Then use up the materials
    for (i = 0; i < REPAIR_ITEM_COUNT; i++) {
        inventory_remove(&player->inv, repair_items[i], repair_amounts[i]);
    }

    quests[QUEST_REPAIR_SHIP].status = QUEST_DONE;
    player->units += REPAIR_REWARD;
    sprintf(message, "Starship repaired! Quest complete, reward: %d Units.", REPAIR_REWARD);
}

// Quest log screen. Any key closes it.
void quest_show(Player *player)
{
    int i;
    int item;
    int row = 2;

    clear();
    mvprintw(0, 0, "=== QUEST LOG ===");

    for (i = 0; i < QUEST_COUNT; i++) {
        // Quests the player hasn't found yet stay hidden
        if (quests[i].status == QUEST_NOT_STARTED) {
            continue;
        }
        mvprintw(row, 0, "%s [%s]", quests[i].title, statuses[quests[i].status]);
        mvprintw(row + 1, 2, "%s", quests[i].description);
        row += 3;
    }

    // Show what is still needed for the ship
    if (quests[QUEST_REPAIR_SHIP].status == QUEST_ACTIVE) {
        mvprintw(row, 0, "Starship repair needs:");
        row++;
        for (i = 0; i < REPAIR_ITEM_COUNT; i++) {
            item = repair_items[i];
            mvprintw(row, 2, "%-16s %d/%d", items[item].name,
                     player->inv.count[item], repair_amounts[i]);
            row++;
        }
        mvprintw(row, 0, "Go to your ship and press E to repair it.");
        row += 2;
    }

    if (quests[QUEST_GEK].status == QUEST_ACTIVE) {
        mvprintw(row, 0, "Gek words learned: %d/%d", dialog_known_count(), WORD_COUNT);
        row += 2;
    }

    // Next step of the Atlas story
    if (quests[QUEST_ATLAS].status == QUEST_ACTIVE) {
        if (player->inv.count[ITEM_ATLAS_SEED] == 0) {
            mvprintw(row, 0, "Bring Chromatic Metal to the Atlas: %d/%d",
                     player->inv.count[ITEM_CHROMATIC], ATLAS_METAL_AMOUNT);
        } else {
            mvprintw(row, 0, "Take the Atlas Seed to your repaired starship.");
        }
        row++;
    }

    mvprintw(row + 1, 0, "Press any key to close");
    refresh();
    getch();
}
