#ifndef MENU_H
#define MENU_H

#include <stdint.h>
#include <string.h>

typedef struct MenuItem {
    char* name;
    uint8_t numberOfChildren;
    struct MenuItem** children;
    struct MenuItem* parent;
    void (*entryPoint)();
} MenuItem;

MenuItem* mainMenu;

typedef struct {
    void (*init)();
    void (*show)(MenuItem* menuItem);
} IMenu;

IMenu menu;

#endif // MENU_H
