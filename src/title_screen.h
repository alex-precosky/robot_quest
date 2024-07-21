#include "bots.h"

#pragma once

typedef struct {
    enum bot chosen_bot;
} title_screen_result_t;

/**
 * @brief Start the title screen. Blocks until the player has started a game
 * @return The result of the title screen - which bot what chosen
 */
title_screen_result_t run_title_screen();
