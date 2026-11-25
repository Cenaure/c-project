#include "screen.h"
#include "menu.h"

static const char *options[MENU_COUNT] = {
    "Start Game",
    "Quit",
};

// Main menu screen with its own loop. Returns the chosen MENU_ option.
int menu_show(void)
{
    int selected = 0;
    int key;
    int i;

    while (1) {
        clear();
        mvprintw(0, 0, "=== NO MAN'S SKY ===");

        for (i = 0; i < MENU_COUNT; i++) {
            if (i == selected) {
                mvprintw(i + 2, 0, ">");
            }
            mvprintw(i + 2, 2, "%s", options[i]);
        }

        mvprintw(MENU_COUNT + 3, 0, "W/S select   Enter confirm");
        refresh();

        key = getch();

        if ((key == 'w' || key == KEY_UP) && selected > 0) {
            selected--;
        } else if ((key == 's' || key == KEY_DOWN) && selected < MENU_COUNT - 1) {
            selected++;
        } else if (key == '\n' || key == KEY_ENTER) {
            return selected;
        }
    }
}
