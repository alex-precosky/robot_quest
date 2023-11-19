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
const uint8_t BOT_SELECT_ARROW_SPRITE_NUM = 2;

enum tile_indexes {
 ALX_BOT_THROBBER_FRAME_0 = 0,
 ALX_BOT_THROBBER_FRAME_1,

 SRNA_BOT_THROBBER_FRAME_0,
 SRNA_BOT_THROBBER_FRAME_1,

 BOT_SELECT_ARROW_FRAME_0,
 BOT_SELECT_ARROW_FRAME_1,

 SPRITE_TILE_COUNT
};

static uint8_t s_alx_bot_throbber_tile_seq[] = {ALX_BOT_THROBBER_FRAME_0, ALX_BOT_THROBBER_FRAME_1};
static uint8_t s_srna_bot_throbber_tile_seq[] = {SRNA_BOT_THROBBER_FRAME_0, SRNA_BOT_THROBBER_FRAME_1};
uint8_t bot_select_tile_seq[] = {BOT_SELECT_ARROW_FRAME_0, BOT_SELECT_ARROW_FRAME_1};

static entity_t s_entity_alx_bot = { 0 };
static entity_t s_entity_srna_bot = { 0 };
entity_t s_entity_bot_select_arrow = { 0 };

joypads_t s_joypads;

typedef enum {
    ARROW_POS_ALX,
    ARROW_POS_SRNA
} arrow_position_e;

static void load_sprites()
{
    set_sprite_data(0, SPRITE_TILE_COUNT, sprites);

    entity_init(&s_entity_alx_bot, ALX_BOT_SPRITE_NUM);
    entity_init(&s_entity_srna_bot, SRNA_BOT_SPRITE_NUM);
    entity_init(&s_entity_bot_select_arrow, BOT_SELECT_ARROW_SPRITE_NUM);

    s_entity_bot_select_arrow.pos_x = 40;

    entity_set_tile_sequence(&s_entity_alx_bot, s_alx_bot_throbber_tile_seq, 2);
    entity_set_tile_sequence(&s_entity_srna_bot, s_srna_bot_throbber_tile_seq, 2);
    entity_set_tile_sequence(&s_entity_bot_select_arrow, bot_select_tile_seq, 2);

    entity_set_pos(&s_entity_alx_bot, 40, 100);
    entity_set_pos(&s_entity_srna_bot, 120, 100);
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

static void update_inputs()
{
    joypad_ex(&s_joypads);
    if (s_joypads.joy0 & J_LEFT) {
        entity_set_input_dir_bitfield(&s_entity_bot_select_arrow, INPUT_DIR_LEFT);
    } else if (s_joypads.joy0 & J_RIGHT) {
        entity_set_input_dir_bitfield(&s_entity_bot_select_arrow, INPUT_DIR_RIGHT);
    } else {
        s_entity_bot_select_arrow.input_dir_bitfield = 0;
    }
}

static void move_entities()
{
    if (s_entity_bot_select_arrow.input_dir_bitfield & INPUT_DIR_LEFT) {
        entity_set_pos(&s_entity_bot_select_arrow, 40, 90);
    } else if (s_entity_bot_select_arrow.input_dir_bitfield & INPUT_DIR_RIGHT) {
        entity_set_pos(&s_entity_bot_select_arrow, 120, 90);
    }

    move_sprite(ALX_BOT_SPRITE_NUM, s_entity_alx_bot.pos_x, s_entity_alx_bot.pos_y);
    move_sprite(SRNA_BOT_SPRITE_NUM, s_entity_srna_bot.pos_x, s_entity_srna_bot.pos_y);
    move_sprite(BOT_SELECT_ARROW_SPRITE_NUM, s_entity_bot_select_arrow.pos_x, s_entity_bot_select_arrow.pos_y);
}

static void animate_sprites()
{
    entity_animate(&s_entity_alx_bot);
    entity_animate(&s_entity_srna_bot);
    entity_animate(&s_entity_bot_select_arrow);
}

void run_title_screen()
{
    draw_text();
    load_sprites();

    const uint8_t num_joypads = 1;
    joypad_init(num_joypads, &s_joypads);

    LCDC_REG |= LCDCF_OBJON; /* Enable the drawing of sprites */
    DISPLAY_ON;

    while (1) {

        /* Skip four VBLs (slow down animation) */
        for(uint8_t i = 0; i < 8; i++) {
            update_inputs();
            move_entities();

            vsync();
        }

        animate_sprites();
    }
}
