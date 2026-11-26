#include "screen.h"
#include <stdio.h>
#include "dialog.h"
#include "map.h"
#include "quest.h"
#include "inventory.h"

static const Word words[WORD_COUNT] = {
    {"Zofilmikr",  "Traveller"},
    {"afle",    "I"},
    {"drityin",  "hungry"},
    {"udut",   "gift"},
    {"lussu", "bring"},
    {"omvari",  "Carbon"}
};

// known[i] is 1 if the player has learned word i, 0 if not
static int known[WORD_COUNT];

// What the Gek says, as word numbers: "Traveller! I hungry. Bring Carbon, gift!"
#define SENTENCE_LENGTH 6
static const int sentence[SENTENCE_LENGTH] = {
    WORD_TRAVELLER, WORD_I, WORD_HUNGRY, WORD_BRING, WORD_CARBON, WORD_GIFT
};

// Answers the player can choose. The last one is "Leave".
#define CHOICE_COUNT 4
#define CHOICE_LEAVE 3
static const int gift_items[3] = {ITEM_SODIUM, ITEM_CARBON, ITEM_FERRITE};
#define GIFT_AMOUNT 5

void dialog_init(void)
{
    int i;

    for (i = 0; i < WORD_COUNT; i++) {
        known[i] = 0;
    }
}

int dialog_known_count(void)
{
    int i;
    int count = 0;

    for (i = 0; i < WORD_COUNT; i++) {
        if (known[i]) {
            count++;
        }
    }
    return count;
}

// Called when the player presses E on a Knowledge Stone.
// Teaches the first word the player doesn't know yet.
void dialog_read_stone(char *message)
{
    int i;

    for (i = 0; i < WORD_COUNT; i++) {
        if (!known[i]) {
            known[i] = 1;
            sprintf(message, "The Knowledge Stone teaches you a Gek word: %s = %s",
                    words[i].gek, words[i].english);
            return;
        }
    }
    sprintf(message, "You already know every Gek word.");
}

// Print one word at the cursor: in English if known, in Gek if not.
static void print_word(int word)
{
    if (known[word]) {
        attron(A_BOLD);
        printw("%s ", words[word].english);
        attroff(A_BOLD);
    } else {
        attron(COLOR_PAIR(COLOR_ATLAS));
        printw("%s ", words[word].gek);
        attroff(COLOR_PAIR(COLOR_ATLAS));
    }
}

// Give 5 of an item to the Gek. Only Carbon is what it wants.
static void give_gift(Player *player, int item, char *message)
{
    if (!inventory_remove(&player->inv, item, GIFT_AMOUNT)) {
        sprintf(message, "You need %d %s.", GIFT_AMOUNT, items[item].name);
        return;
    }

    if (item != ITEM_CARBON) {
        sprintf(message, "The Gek takes your %s but looks disappointed.", items[item].name);
        return;
    }

    inventory_add(&player->inv, ITEM_THRUSTER, 1);
    quest_set_status(QUEST_GEK, QUEST_DONE);
    sprintf(message, "The Gek is happy! It gives you a %s.", items[ITEM_THRUSTER].name);
}

// Talk screen with its own loop, like the shop.
void dialog_talk_gek(Player *player)
{
    int selected = 0;
    int key;
    int i;
    int row;
    char message[100] = "";

    // Meeting the Gek for the first time starts the quest
    if (quest_get_status(QUEST_GEK) == QUEST_NOT_STARTED) {
        quest_set_status(QUEST_GEK, QUEST_ACTIVE);
        sprintf(message, "New quest: The Gek's Request");
    }

    while (1) {
        clear();
        mvprintw(0, 0, "=== GEK TRADER ===");

        if (quest_get_status(QUEST_GEK) == QUEST_DONE) {
            mvprintw(2, 0, "The Gek bows to you happily.");
            mvprintw(4, 0, "%s", message);
            mvprintw(6, 0, "Press any key to leave");
            refresh();
            getch();
            return;
        }

        // The sentence, word by word
        mvprintw(2, 0, "The Gek says:");
        move(3, 2);
        for (i = 0; i < SENTENCE_LENGTH; i++) {
            print_word(sentence[i]);
        }

        // The answers
        for (i = 0; i < CHOICE_COUNT; i++) {
            if (i == selected) {
                mvprintw(i + 5, 0, ">");
            }
            if (i == CHOICE_LEAVE) {
                mvprintw(i + 5, 2, "Leave");
            } else {
                mvprintw(i + 5, 2, "Give %d %s", GIFT_AMOUNT, items[gift_items[i]].name);
            }
        }

        mvprintw(10, 0, "%s", message);

        // Little dictionary of the words learned so far
        mvprintw(12, 0, "Gek words learned: %d/%d", dialog_known_count(), WORD_COUNT);
        row = 13;
        for (i = 0; i < WORD_COUNT; i++) {
            if (known[i]) {
                mvprintw(row, 2, "%-6s = %s", words[i].gek, words[i].english);
                row++;
            }
        }

        mvprintw(row + 1, 0, "W/S select   Enter confirm");
        refresh();

        key = getch();

        if ((key == 'w' || key == KEY_UP) && selected > 0) {
            selected--;
        } else if ((key == 's' || key == KEY_DOWN) && selected < CHOICE_COUNT - 1) {
            selected++;
        } else if (key == '\n' || key == KEY_ENTER) {
            if (selected == CHOICE_LEAVE) {
                return;
            }
            give_gift(player, gift_items[selected], message);
        }
    }
}
