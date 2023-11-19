#include "entity.h"
#include <gb/gb.h>

void entity_init(entity_t* entity, uint8_t sprite_num)
{
    entity->sprite_num = sprite_num;
    entity->tile_seq = NULL;
    entity->tile_seq_len = 0;
    entity->current_tile_idx = 0;

    /* Clear all sprite properties */
    set_sprite_prop(sprite_num, 0);
}

void entity_set_tile_sequence(entity_t* entity, uint8_t* tile_seq, uint8_t tile_seq_len)
{
    entity->tile_seq = tile_seq;
    entity->tile_seq_len = tile_seq_len;
    entity->current_tile_idx = 0;
}

void entity_animate(entity_t* entity)
{
    entity->current_tile_idx = (entity->current_tile_idx + 1) % entity->tile_seq_len;
    set_sprite_tile(entity->sprite_num, entity->tile_seq[entity->current_tile_idx]);
}

void entity_set_pos(entity_t* entity, uint8_t x, uint8_t y)
{
    entity->pos_x = x;
    entity->pos_y = y;
}

void entity_set_input_dir_bitfield(entity_t* entity, uint8_t input_dir_bitfield)
{
    entity->input_dir_bitfield = input_dir_bitfield;
}
