#pragma once

#include "bots.h"
#include <gb/gb.h>

/**
 * @brief Start the game level. Blocks until the level is over
 * @param selected_bot - Which bot shall the level be played as?
 * @param joypads - An initialized joypads struct
 */
void run_level(enum bot selected_bot, const joypads_t *joypads);
