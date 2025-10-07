#include "menu.h"
#include "utilities.h"
#include "drivers/oled.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "drivers/joystick.h"
#include "utilities.h"
#include <util/delay.h>

IMenuItem mainMenu = {
    .name = "Main Menu",
    .numberOfChildren = 0,
    .children = NULL,
    .entryPoint = NULL,
    .parent = NULL,
};

static int8_t menu_currentSelection = 0;

static void menu_addChild(IMenuItem* parent, IMenuItem* child) {
    if (child->parent != NULL) {
        // TODO: Handle
    }

    child->parent = parent;
    parent->numberOfChildren++;

    IMenuItem** newChildren = realloc(parent->children, parent->numberOfChildren * sizeof(IMenuItem*));
    if (newChildren) {
        parent->children = newChildren;
    } else {
        // TODO: Handle not enought memory
    }

    parent->children[parent->numberOfChildren - 1] = child;
}

static IMenuItem* menu_addItem(IMenuItem* parent, char* name) {
    IMenuItem* item = malloc(sizeof(IMenuItem));
    if (item == NULL) {
        printf("COULD NOT ALLOCATE MEMORY\n");
    }
    item->name = strdup(name);
    item->entryPoint = NULL;
    item->children = NULL;
    item->numberOfChildren = 0;
    item->parent = NULL;
    menu_addChild(parent, item);
    return item;
}

static void menu_init() {
    IMenuItem* gameMenu = menu_addItem(&mainMenu, "Games");
   // IMenuItem* documentItem = menu_addItem(&mainMenu, "Documents");
    //IMenuItem* music = menu_addItem(&mainMenu, "Music");

    IMenuItem* etchASketch = menu_addItem(gameMenu, "Etch a sketch");
    menu_addItem(gameMenu, "Snake");
    IMenuItem* pong_game = menu_addItem(gameMenu, "Pong");

    etchASketch->entryPoint = etch_a_sketch;
    pong_game->entryPoint = pong;

    //menu_addItem(documentItem, "Epstein files");
}

static void menu_render(IMenuItem* menuItem) {
    oled.clear();
    oled.pos(0, 32);
    oled.print(menuItem->name);
    for (uint8_t i = 0; i < menuItem->numberOfChildren; i++) {
        oled.pos((i + 1) * 8, 0);

        if (menu_currentSelection == i) {
            oled.print(">");
        } else {
            oled.print(" ");
        }

        oled.print(menuItem->children[i]->name);
    }
    oled.present();
}

static void menu_show(IMenuItem* menuItem) {
    IMenuItem* currentMenu = menuItem;
    menu_render(currentMenu);

    DeviceDirection lastDirection = NEUTRAL;
    while (true) {
        DeviceDirection direction = ioGrid.getDirection(joystick);
        _delay_ms(100);

        if (direction != lastDirection) {
            if (direction == UP) {
                menu_currentSelection = (currentMenu->numberOfChildren - 1 + menu_currentSelection) % currentMenu->numberOfChildren;
                menu_render(currentMenu);
            }

            if (direction == DOWN) {
                menu_currentSelection = (menu_currentSelection + 1) % currentMenu->numberOfChildren;
                menu_render(currentMenu);
            }

            if (direction == RIGHT) {
                IMenuItem* entering = currentMenu->children[menu_currentSelection];
                if (entering->numberOfChildren == 0) {
                    if (entering->entryPoint != NULL) {
                        entering->entryPoint();
                        menu_render(currentMenu);
                    }
                } else {
                    currentMenu = entering;
                    menu_currentSelection = 0;
                    menu_render(currentMenu);
                }
            }

            if (direction == LEFT) {
                IMenuItem* parent = currentMenu->parent;
                if (parent != NULL) {
                    currentMenu = parent;
                    menu_currentSelection = 0;
                    menu_render(currentMenu);
                }
            }
        }

        lastDirection = direction;
    }
}

IMenu menu = {
    .init = menu_init,
    .show = menu_show
};
