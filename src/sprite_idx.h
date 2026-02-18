// Indexes of sprites in our sprite sheet

#pragma once

#include <stdint.h>

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

extern uint8_t alx_bot_tile_seq[2];
extern uint8_t srna_bot_tile_seq[2];
