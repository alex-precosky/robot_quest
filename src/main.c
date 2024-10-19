#include "level.h"
#include "title_screen.h"
#include <gb/gb.h>

joypads_t s_joypads;

int main(void)
{
    const uint8_t num_joypads = 1;
    joypad_init(num_joypads, &s_joypads);

    title_screen_result_t tsr;

    tsr = run_title_screen(&s_joypads);
    run_level(tsr.chosen_bot, &s_joypads);

    return 0;
}
