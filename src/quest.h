#pragma once

#include "player.h"

// Quest numbers
#define QUEST_REPAIR_SHIP 0
#define QUEST_GEK         1

#define QUEST_COUNT 2

// Quest status
#define QUEST_NOT_STARTED 0
#define QUEST_ACTIVE      1
#define QUEST_DONE        2

typedef struct {
    const char *title;
    const char *description;
    int status;
} Quest;

void quest_init(void);
int quest_get_status(int quest);
void quest_set_status(int quest, int status);
void quest_repair_ship(Player *player, char *message);
void quest_show(Player *player);
