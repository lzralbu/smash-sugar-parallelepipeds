#ifndef MENU_H
#define MENU_H

#include "animation.h"

typedef struct {
    Animation controlsAnimation;
    Animation footerAnimation;
} Menu;

void menuStart(Menu *menu);
void menuUpdate(Menu *menu, void *game);

#endif // MENU_H
