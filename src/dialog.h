#pragma once

#include "player.h"

#define WORD_TRAVELLER 0
#define WORD_I         1
#define WORD_HUNGRY    2
#define WORD_GIFT      3
#define WORD_BRING     4
#define WORD_CARBON    5

#define WORD_COUNT 6

typedef struct {
    const char *gek;
    const char *english;
} Word;

void dialog_init(void);
int dialog_known_count(void);
void dialog_read_stone(char *message);
void dialog_talk_gek(Player *player);
