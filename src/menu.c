#include "menu.h"
#include "drivers/oled.h"
#include <stdio.h>
#include <stdlib.h>

IMenuItem mainMenu = {
    .name = "Main Menu",
    .numberOfChildren = 0,
    .children = NULL
};

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

}

const IMenuItem children[] = {
    { .name = "First", .numberOfChildren = 0, .children = NULL },
    { .name = "Second", .numberOfChildren = 0, .children = NULL },
    { .name = "Third", .numberOfChildren = 0, .children = NULL }
};

static void menu_show(IMenuItem menuItem) {
    oled.clear();
    oled.pos(0, 32);
    oled.print(menuItem.name);
    printf("%d\n", menuItem.numberOfChildren);
    for (uint8_t i = 0; i < menuItem.numberOfChildren; i++) {
        oled.pos(i + 1, 0);
        oled.print(menuItem.children[i]->name);
    }
}

IMenu menu = {
    .init = menu_init,
    .show = menu_show
};
