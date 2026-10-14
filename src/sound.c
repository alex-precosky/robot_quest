#include "sound.h"
#include "cbtfx.h"
#include "hUGEDriver.h"
#include <gb/gb.h>

extern const hUGESong_t sample_song;

/**
 * Module for music and sound effects!
 */

void sound_init(void)
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

void sound_stop(void)
{
    __critical
    {
        remove_VBL(hUGE_dosound);
        remove_VBL(CBTFX_update);
        hUGE_mute_channel(HT_CH1, 1);
        hUGE_mute_channel(HT_CH2, 1);
        hUGE_mute_channel(HT_CH3, 1);
        hUGE_mute_channel(HT_CH4, 1);
    }
}

void sound_play_sfx(const unsigned char *sfx)
{
    CBTFX_init(sfx);
}
