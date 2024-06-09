#include "title_screen.h"
#include "SFX_00.h"
#include "SFX_01.h"
#include "cbtfx.h"
#include "entity.h"
#include "hUGEDriver.h"
#include "res/sprites.h"
#include <gb/gb.h>
#include <gbdk/console.h>
#include <gbdk/font.h>
#include <stdint.h>
#include <stdio.h>

extern const hUGESong_t sample_song;

const uint8_t ALX_BOT_SPRITE_NUM = 0;
const uint8_t SRNA_BOT_SPRITE_NUM = 1;
const uint8_t BOT_SELECT_ARROW_SPRITE_NUM = 2;

const uint8_t ALEX_BOT_X_POS = 40;
const uint8_t SERENA_BOT_X_POS = 120;
const uint8_t ARROW_Y_POS = 90;

enum tile_indexes {
    ALX_BOT_THROBBER_FRAME_0 = 0,
    ALX_BOT_THROBBER_FRAME_1,

    SRNA_BOT_THROBBER_FRAME_0,
    SRNA_BOT_THROBBER_FRAME_1,

    BOT_SELECTED_ARROW_FRAME_0,
    BOT_SELECTED_ARROW_FRAME_1,

    BOT_SELECT_ARROW_FRAME_0 = 5,
    BOT_SELECT_ARROW_FRAME_1,
    BOT_SELECT_ARROW_FRAME_2,
    BOT_SELECT_ARROW_FRAME_3,
    BOT_SELECT_ARROW_FRAME_4,
    BOT_SELECT_ARROW_FRAME_5,

    SPRITE_TILE_COUNT
};

/* In the title screen, we can use the D-Pad to toggle left and right between
   bots, then once one is chosen, hang in the title screen for a little while,
   but don't let us move the selection cursor */
enum title_screen_state {
    STATE_CHOOSING_BOT = 0,
    STATE_BOT_CHOSEN
};

typedef struct {
    uint8_t state;
    uint16_t systime_entered; /**< counted in VBL interrupts */
} StateInfo;
static StateInfo s_state;

static void change_state(enum title_screen_state new_state)
{
    s_state.state = new_state;
    s_state.systime_entered = sys_time;
}

static enum title_screen_state get_state()
{
    return s_state.state;
}

static uint8_t s_alx_bot_throbber_tile_seq[] = { ALX_BOT_THROBBER_FRAME_0, ALX_BOT_THROBBER_FRAME_1 };
static uint8_t s_srna_bot_throbber_tile_seq[] = { SRNA_BOT_THROBBER_FRAME_0, SRNA_BOT_THROBBER_FRAME_1 };
static uint8_t bot_select_tile_seq[] = { BOT_SELECT_ARROW_FRAME_0, BOT_SELECT_ARROW_FRAME_1, BOT_SELECT_ARROW_FRAME_2, BOT_SELECT_ARROW_FRAME_3, BOT_SELECT_ARROW_FRAME_4, BOT_SELECT_ARROW_FRAME_5 };
static uint8_t bot_selected_tile_seq[] = { BOT_SELECTED_ARROW_FRAME_0, BOT_SELECTED_ARROW_FRAME_1 };

static entity_t s_entity_alx_bot = { 0 };
static entity_t s_entity_srna_bot = { 0 };
entity_t s_entity_bot_select_arrow = { 0 };

joypads_t s_joypads;

typedef enum {
    ARROW_POS_ALX,
    ARROW_POS_SRNA
} arrow_position_e;

static void init_sound()
{
    /* Initialize sound hardware */
    NR52_REG = 0x80;
    NR51_REG = 0xFF;
    NR50_REG = 0x77;

    __critical
    {
        add_VBL(hUGE_dosound);
        add_VBL(CBTFX_update);
        hUGE_init(&sample_song);
    }
}

static void load_sprites()
{
    set_sprite_data(0, SPRITE_TILE_COUNT, sprites);

    entity_init(&s_entity_alx_bot, ALX_BOT_SPRITE_NUM);
    entity_init(&s_entity_srna_bot, SRNA_BOT_SPRITE_NUM);
    entity_init(&s_entity_bot_select_arrow, BOT_SELECT_ARROW_SPRITE_NUM);

    /* Initialize select the left bot */
    entity_set_pos(&s_entity_bot_select_arrow, ALEX_BOT_X_POS, ARROW_Y_POS);

    entity_set_tile_sequence(&s_entity_alx_bot, s_alx_bot_throbber_tile_seq, 2);
    entity_set_tile_sequence(&s_entity_srna_bot, s_srna_bot_throbber_tile_seq, 2);
    entity_set_tile_sequence(&s_entity_bot_select_arrow, bot_select_tile_seq, 6);

    entity_set_pos(&s_entity_alx_bot, ALEX_BOT_X_POS, 100);
    entity_set_pos(&s_entity_srna_bot, SERENA_BOT_X_POS, 100);
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
    printf("Copyright 2024");
}

static void update_inputs()
{
    joypad_ex(&s_joypads);
    if (s_joypads.joy0 & J_LEFT) {
        entity_set_input_dir_bitfield(&s_entity_bot_select_arrow, INPUT_DIR_LEFT);
    } else if (s_joypads.joy0 & J_RIGHT) {
        entity_set_input_dir_bitfield(&s_entity_bot_select_arrow, INPUT_DIR_RIGHT);
    } else if (s_joypads.joy0 & J_START) {
        change_state(STATE_BOT_CHOSEN);
        entity_set_tile_sequence(&s_entity_bot_select_arrow, bot_selected_tile_seq, 2);
        CBTFX_init(SFX_01);
    } else {
        s_entity_bot_select_arrow.input_dir_bitfield = 0;
    }
}

static void move_entities()
{
    if (s_entity_bot_select_arrow.input_dir_bitfield & INPUT_DIR_LEFT) {
        if (s_entity_bot_select_arrow.pos_x != ALEX_BOT_X_POS) {
            entity_set_pos(&s_entity_bot_select_arrow, ALEX_BOT_X_POS, ARROW_Y_POS);
            CBTFX_init(SFX_00);
        }
    } else if (s_entity_bot_select_arrow.input_dir_bitfield & INPUT_DIR_RIGHT) {
        if (s_entity_bot_select_arrow.pos_x != SERENA_BOT_X_POS) {
            entity_set_pos(&s_entity_bot_select_arrow, SERENA_BOT_X_POS, ARROW_Y_POS);
            CBTFX_init(SFX_00);
        }
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
    change_state(STATE_CHOOSING_BOT);

    init_sound();
    draw_text();
    load_sprites();

    const uint8_t num_joypads = 1;
    joypad_init(num_joypads, &s_joypads);

    LCDC_REG |= LCDCF_OBJON; /* Enable the drawing of sprites */
    DISPLAY_ON;

    while (1) {

        /* Skip four VBLs (slow down animation) */
        for (uint8_t i = 0; i < 8; i++) {

            if (get_state() == STATE_CHOOSING_BOT) {
                update_inputs();
                move_entities();
            }

            vsync();
        }

        animate_sprites();
    }
}
