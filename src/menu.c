#include "menu.h"
#include "drivers/oled.h"
#include <stdio.h>
#include <stdlib.h>

IMenuItem mainMenu = {
    .name = "Main Menu",
    .numberOfChildren = 0,
    .children = NULL
};

const IMenuItem children[] = {
    { .name = "First", .numberOfChildren = 0, .children = NULL },
    { .name = "Second", .numberOfChildren = 0, .children = NULL },
    { .name = "Third", .numberOfChildren = 0, .children = NULL }
};

static int8_t menu_currentSelection = 0;

static void menu_addChild(IMenuItem* parent, IMenuItem* child) {
    if (child->parent != NULL) {
        // TODO: Handle
    }

    child->parent = parent;
    parent->numberOfChildren++;

    IMenuItem* newChildren = realloc(parent->children, parent->numberOfChildren);
    if (newChildren) {
        parent->children = newChildren;
    } else {
        // TODO: Handle not enought memory
    }

    parent->children[parent->numberOfChildren - 1] = child;
}

static void menu_init() {

    for (uint8_t i = 0; i < sizeof(children) / sizeof(children[0]); i++) {
        menu_addChild(&mainMenu, &(children[i]));
    }
}

static void menu_render(IMenuItem menuItem) {
    oled.clear();
    oled.pos(0, 32);
    oled.print(menuItem.name);
    for (uint8_t i = 0; i < menuItem.numberOfChildren; i++) {
        oled.pos(i + 1, 0);

        if (menu_currentSelection == i) {
            oled.print(">");
        } else {
            oled.print(" ");
        }

        oled.print(menuItem.children[i]->name);
    }
}

static void menu_show(IMenuItem menuItem) {
    menu_render(menuItem);
}

IMenu menu = {
    .init = menu_init,
    .show = menu_show
};
