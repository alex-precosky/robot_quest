#include "level.h"
#include "res/level_map.h"
#include "res/level_map_tiles.h"
#include <gb/gb.h>
#include <gbdk/console.h>
#include <stdio.h>

void init_gfx(void)
{
    // Load Background tiles and then map
    set_bkg_data(0, 18u, level_map_tiles);
    set_bkg_tiles(0, 0, level_mapWidth, level_mapHeight, level_map);

    // Turn the background map on to make it visible
    SHOW_BKG;
}

void run_level()
{
    cls();

    gotoxy(0, 8);
    printf("### LEVEL START ###");

    init_gfx();

    while (1) {
        vsync();
    }
}
