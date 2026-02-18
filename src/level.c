#include "bots.h"
#include "entity.h"
#include "level.h"
#include "res/level_map.h"
#include "res/level_map_tiles.h"
#include "res/sprites.h"
#include "sprite_idx.h"
#include <gb/gb.h>
#include <gbdk/console.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

const uint8_t SPRITE_NUM_PLAYER_BOT = 0;

static entity_t s_entity_player_bot;

static uint8_t *s_player_bot_tile_seq = NULL;

static const uint8_t GRID_TO_SCREEN_OFFSET_X = 21;
static const uint8_t GRID_TO_SCREEN_OFFSET_Y = 28;
static const uint8_t GRID_CELL_WIDTH = 8;
static const uint8_t GRID_CELL_HEIGHT = 8;

typedef struct {
    uint8_t pos_x;
    uint8_t pos_y;
} position_t;

static position_t grid_to_xy(position_t grid_position);
static bool is_entity_at_vertex(const entity_t *entity);

void init_gfx(enum bot selected_bot)
{
    // Load Background tiles and then map
    set_bkg_data(0, 18u, level_map_tiles);
    set_bkg_tiles(0, 0, level_mapWidth, level_mapHeight, level_map);

    // Turn the background map on to make it visible
    SHOW_BKG;

    // Load sprites
    set_sprite_data(0, SPRITE_TILE_COUNT, sprites);
    entity_init(&s_entity_player_bot, SPRITE_NUM_PLAYER_BOT);

    if (selected_bot == BOT_ALEX) {
        s_player_bot_tile_seq = alx_bot_tile_seq;
    } else {
        s_player_bot_tile_seq = srna_bot_tile_seq;
    }

    entity_set_tile_sequence(&s_entity_player_bot, s_player_bot_tile_seq, 2);

    position_t player_start_grid = {
        .pos_x = 0,
        .pos_y = 0,
    };
    position_t player_start_screen = grid_to_xy(player_start_grid);

    entity_set_pos(&s_entity_player_bot, player_start_screen.pos_x, player_start_screen.pos_y);

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
    } else {
        entity_set_input_dir_bitfield(&s_entity_player_bot, 0);
    }
}

static void update_velocities()
{
    if (is_entity_at_vertex(&s_entity_player_bot)) {
        if (s_entity_player_bot.input_dir_bitfield & INPUT_DIR_LEFT) {
            s_entity_player_bot.velocity = VELOCITY_LEFT;
        } else if (s_entity_player_bot.input_dir_bitfield & INPUT_DIR_RIGHT) {
            s_entity_player_bot.velocity = VELOCITY_RIGHT;
        } else if (s_entity_player_bot.input_dir_bitfield & INPUT_DIR_UP) {
            s_entity_player_bot.velocity = VELOCITY_UP;
        } else if (s_entity_player_bot.input_dir_bitfield & INPUT_DIR_DOWN) {
            s_entity_player_bot.velocity = VELOCITY_DOWN;
        } else {
            s_entity_player_bot.velocity = VELOCITY_STOP;
        }

    }
}

static void update_positions()
{
    uint8_t dx = 0;
    uint8_t dy = 0;

    if(s_entity_player_bot.velocity == VELOCITY_LEFT) {
        dx = -1;
    } else if (s_entity_player_bot.velocity == VELOCITY_RIGHT) {
        dx = 1;
    } else if (s_entity_player_bot.velocity == VELOCITY_DOWN) {
        dy = 1;
    } else if (s_entity_player_bot.velocity == VELOCITY_UP) {
        dy = -1;
    }

    entity_set_pos(&s_entity_player_bot, s_entity_player_bot.pos_x + dx, s_entity_player_bot.pos_y + dy);
}

static void move_entities()
{
    move_sprite(SPRITE_NUM_PLAYER_BOT, s_entity_player_bot.pos_x, s_entity_player_bot.pos_y);
}

/**
 * @brief Check if the entity is on top of a grid vertex
 * @param entity - The entity to inspect the position of
 * @return True if the entity is on top of a grid vertex
 */
static bool is_entity_at_vertex(const entity_t *entity) {
    if (((entity->pos_x - GRID_TO_SCREEN_OFFSET_X) % GRID_CELL_WIDTH == 0) &&
        ((entity->pos_y - GRID_TO_SCREEN_OFFSET_Y) % GRID_CELL_HEIGHT == 0)) {
        return true;
    } else {
        return false;
    }
}

/**
 * @param grid_position - A position on the game grid
 * @return The xy screen position of the game grid position
 */
static position_t grid_to_xy(position_t grid_position)
{
    position_t screen_position;
    screen_position.pos_x = GRID_TO_SCREEN_OFFSET_X + grid_position.pos_x * GRID_CELL_WIDTH;
    screen_position.pos_y = GRID_TO_SCREEN_OFFSET_Y + grid_position.pos_y * GRID_CELL_HEIGHT;
    return screen_position;
}

static void animate_player_bot() {
    static uint8_t animation_frame_counter = 0;
    animation_frame_counter++;
    if (animation_frame_counter >= 8) { // Animate every 8 VBLs
        entity_animate(&s_entity_player_bot);
        animation_frame_counter = 0;
    }
}

void run_level(enum bot selected_bot, const joypads_t *joypads)
{
    cls();
    (void)selected_bot;

    gotoxy(0, 8);

    if (selected_bot == BOT_ALEX) {
        printf(" # ALX BOT START #");
    } else {
        printf("# SRNA BOT START #");
    }

    /* Pause for a bit with the 'level start' shown */
    uint16_t pause_start = sys_time;
    while ((sys_time - pause_start) < 100) {
        vsync();
    }

    init_gfx(selected_bot);

    while (1) {
        update_inputs(joypads);
        update_velocities();
        update_positions();
        move_entities();

        animate_player_bot();

        vsync();
    }
}
