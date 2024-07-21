#include "level.h"
#include "title_screen.h"

int main(void)
{
    title_screen_result_t tsr;

    tsr = run_title_screen();
    run_level(tsr.chosen_bot);

    return 0;
}
