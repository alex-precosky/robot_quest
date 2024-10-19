#include "bots.h"
#include "entity.h"
#include "level.h"
#include "res/level_map.h"
#include "res/level_map_tiles.h"
#include "res/sprites.h"
#include "sprite_idx.h"
#include <gb/gb.h>
#include <gbdk/console.h>
#include <stdint.h>
#include <stdio.h>

const uint8_t SPRITE_NUM_PLAYER_BOT = 0;

static entity_t s_entity_player_bot;

static uint8_t s_player_bot_tile_seq[] = { ALX_BOT_THROBBER_FRAME_0, ALX_BOT_THROBBER_FRAME_1 };

void init_gfx(void)
{
    // Load Background tiles and then map
    set_bkg_data(0, 18u, level_map_tiles);
    set_bkg_tiles(0, 0, level_mapWidth, level_mapHeight, level_map);

    // Turn the background map on to make it visible
    SHOW_BKG;

    // Load sprites
    set_sprite_data(0, SPRITE_TILE_COUNT, sprites);
    entity_init(&s_entity_player_bot, SPRITE_NUM_PLAYER_BOT);
    entity_set_tile_sequence(&s_entity_player_bot, s_player_bot_tile_seq, 2);
    entity_set_pos(&s_entity_player_bot, 20, 20);

    LCDC_REG |= LCDCF_OBJON; /* Enable the drawing of sprites */
    DISPLAY_ON;
}

static void update_inputs(const joypads_t *joypads)
{
    joypad_ex(joypads);
    if (joypads->joy0 & J_LEFT) {
        entity_set_input_dir_bitfield(&s_entity_player_bot, INPUT_DIR_LEFT);
    } else if (joypads->joy0 & J_RIGHT) {
        entity_set_input_dir_bitfield(&s_entity_player_bot, INPUT_DIR_RIGHT);
    } else if (joypads->joy0 & J_UP) {
        entity_set_input_dir_bitfield(&s_entity_player_bot, INPUT_DIR_UP);
    } else if (joypads->joy0 & J_DOWN) {
        entity_set_input_dir_bitfield(&s_entity_player_bot, INPUT_DIR_DOWN);
    }
}

static void move_entities()
{
    move_sprite(SPRITE_NUM_PLAYER_BOT, s_entity_player_bot.pos_x, s_entity_player_bot.pos_y);
}

void run_level(enum bot selected_bot, const joypads_t *joypads)
{
    cls();
    (void)selected_bot;

    gotoxy(0, 8);
    printf("### LEVEL START ###");

    init_gfx();

    while (1) {
        update_inputs(joypads);
        move_entities();
        vsync();
    }
}
