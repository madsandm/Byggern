#ifndef MENU_H
#define MENU_H

#include <stdint.h>
#include <string.h>

typedef struct IMenuItem {
    char* name;
    uint8_t numberOfChildren;
    struct IMenuItem** children;
    struct IMenuItem* parent;
    void (*entryPoint)();
} IMenuItem;

IMenuItem mainMenu;

void menu_init();
void menu_show(IMenuItem* menuItem);

#endif // MENU_H
