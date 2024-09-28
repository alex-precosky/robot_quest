#pragma once

#include "bots.h"
#include <gb/gb.h>

typedef struct {
    enum bot chosen_bot;
} title_screen_result_t;

/**
 * @brief Start the title screen. Blocks until the player has started a game
 * @param joypads - An initialized joypads struct
 * @return The result of the title screen - which bot what chosen
 */
title_screen_result_t run_title_screen(const joypads_t *joypads);
