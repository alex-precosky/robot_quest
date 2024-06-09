#include "level.h"
#include <gb/gb.h>
#include <gbdk/console.h>
#include <stdio.h>

void run_level()
{
    cls();

    gotoxy(0, 8);
    printf("### LEVEL START ###");

    while (1) {
        vsync();
    }
}
