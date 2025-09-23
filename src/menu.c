#include "menu.h"
#include "drivers/oled.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "drivers/joystick.h"
#include <util/delay.h>
#include "utilities.h"
#include "drivers/sram.h"

static int8_t menu_currentSelection = 0;

static void menu_addChild(MenuItem* parent, MenuItem* child) {
    if (child->parent != NULL) {
        // TODO: Handle
    }

    child->parent = parent;
    parent->numberOfChildren++;

    MenuItem** newChildren = sram.realloc(parent->children, parent->numberOfChildren * sizeof(MenuItem*));
    if (newChildren) {
        parent->children = newChildren;
    } else {
        // TODO: Handle not enought memory
    }

    parent->children[parent->numberOfChildren - 1] = child;
}

static char* copyString(char* str) {
    size_t len = strlen(str);
    char* mem = sram.malloc(len + 1);
    return strcpy(mem, str);
}

static MenuItem* menu_createItem(char* name) {
    MenuItem* item = sram.malloc(sizeof(MenuItem));
    if (item == NULL) {
        printf("COULD NOT ALLOCATE MEMORY\n");
    }
    item->name = copyString(name);
    item->entryPoint = NULL;
    item->children = NULL;
    item->numberOfChildren = 0;
    item->parent = NULL;
    return item;
}

static MenuItem* menu_addItem(MenuItem* parent, char* name) {
    MenuItem* item = menu_createItem(name);
    menu_addChild(parent, item);
    return item;
}

static void menu_init() {
    mainMenu = menu_createItem("Main Menu");

    MenuItem* gameMenu = menu_addItem(mainMenu, "Games");
    MenuItem* documentItem = menu_addItem(mainMenu, "Documents");
    MenuItem* music = menu_addItem(mainMenu, "Music");

    MenuItem* etchASketch = menu_addItem(gameMenu, "Etch a sketch");
    menu_addItem(gameMenu, "Snake");
    menu_addItem(gameMenu, "Pong");

    etchASketch->entryPoint = etch_a_sketch;

    menu_addItem(documentItem, "Epstein files");
    menu_addItem(documentItem, "Epstein files");
    menu_addItem(documentItem, "Epstein files");
    menu_addItem(documentItem, "Epstein files");
    menu_addItem(documentItem, "Epstein files");
    menu_addItem(documentItem, "Epstein files");
    menu_addItem(documentItem, "Epstein files");
}

static void menu_render(MenuItem* menuItem) {
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
}

static void menu_show(MenuItem* menuItem) {
    MenuItem* currentMenu = menuItem;
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
                MenuItem* entering = currentMenu->children[menu_currentSelection];
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
                MenuItem* parent = currentMenu->parent;
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
