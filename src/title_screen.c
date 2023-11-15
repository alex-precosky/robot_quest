#include "title_screen.h"
#include "res/dungeon_map.h"
#include "res/dungeon_tiles.h"
#include <gb/gb.h>
#include <gbdk/console.h>
#include <gbdk/font.h>
#include <stdint.h>
#include <stdio.h>

void run_title_screen()
{
    font_init();
    font_t ibm_font;
    ibm_font = font_load(font_ibm);

    font_set(ibm_font);

    gotoxy(0, 1);
    printf("### ROBOT  QUEST ###");

    gotoxy(1, 3);
    printf("Choose your bot!");

    gotoxy(1, 12);
    printf("Alx-Bot");

    gotoxy(11, 12);
    printf("Srna-Bot");

    gotoxy(3, 16);
    printf("Copyright 2023");

    while (1) {
        // Game main loop processing goes here

        // TODO draw and animate the bots

        // Done processing, yield CPU and wait for start of next frame
        vsync();
    }
}
