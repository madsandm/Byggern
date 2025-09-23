#ifndef MENU_H
#define MENU_H

#include <stdint.h>
#include <string.h>

typedef struct IMenuItem {
    char* name;
    uint8_t numberOfChildren;
    struct IMenuItem** children;
    struct IMenuItem* parent;
} IMenuItem;

IMenuItem mainMenu;

typedef struct {
    void (*init)();
    void (*show)(IMenuItem menuItem);
} IMenu;

IMenu menu;

#endif // MENU_H