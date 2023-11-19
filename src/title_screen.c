#include "title_screen.h"
#include "entity.h"
#include "res/sprites.h"
#include <gb/gb.h>
#include <gbdk/console.h>
#include <gbdk/font.h>
#include <stdint.h>
#include <stdio.h>

const uint8_t ALX_BOT_SPRITE_NUM = 0;
const uint8_t SRNA_BOT_SPRITE_NUM = 1;

enum tile_indexes {
 ALX_BOT_THROBBER_FRAME_0 = 0,
 ALX_BOT_THROBBER_FRAME_1,

 SRNA_BOT_THROBBER_FRAME_0,
 SRNA_BOT_THROBBER_FRAME_1,

 SPRITE_TILE_COUNT
};

uint8_t alx_bot_throbber_tile_seq[] = {ALX_BOT_THROBBER_FRAME_0, ALX_BOT_THROBBER_FRAME_1};
uint8_t srna_bot_throbber_tile_seq[] = {SRNA_BOT_THROBBER_FRAME_0, SRNA_BOT_THROBBER_FRAME_1};

entity_t entity_alx_bot = { 0 };
entity_t entity_srna_bot = { 0 };

static void load_sprites()
{
    set_sprite_data(0, SPRITE_TILE_COUNT, sprites);

    entity_init(&entity_alx_bot, ALX_BOT_SPRITE_NUM);
    entity_init(&entity_srna_bot, SRNA_BOT_SPRITE_NUM);

    entity_set_tile_sequence(&entity_alx_bot, alx_bot_throbber_tile_seq, 2);
    entity_set_tile_sequence(&entity_srna_bot, srna_bot_throbber_tile_seq, 2);
}

static void draw_text()
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
}

static void animate_sprites()
{
    move_sprite(ALX_BOT_SPRITE_NUM, 40, 100);
    move_sprite(SRNA_BOT_SPRITE_NUM, 120, 100);

    entity_animate(&entity_alx_bot);
    entity_animate(&entity_srna_bot);
}

void run_title_screen()
{
    draw_text();
    load_sprites();

    LCDC_REG |= LCDCF_OBJON; /* Enable the drawing of sprites */
    DISPLAY_ON;

    while (1) {
        /* Skip four VBLs (slow down animation) */
        for(uint8_t i = 0; i < 8; i++) {
            vsync();
        }

        animate_sprites();
    }
}
