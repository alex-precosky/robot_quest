#include <stdint.h>
#include <stdlib.h>

/**
 * Entities are something drawn on the screen. Each has an associated sprite,
 * and is animated each tick of a game update loop
 */

/**
 * A structure representing something we draw on the screen and data associated
 * with that
 */
typedef struct {
    uint8_t sprite_num; /** 0-39 - Which of the 40 sprites in the gameboy's spirte list this is */
    uint8_t* tile_seq; /** A list of indices into tile memory that this sprite should animate in sequence */
    uint8_t tile_seq_len;
    uint8_t current_tile_idx; /** The index into tile_sequence that is currently being drawn */
} entity_t;

/**
 * @brief Initialize an entity structure so that it's ready for use
 * @param entity - The entity structure to initialize
 * @param sprite_num - The number from 0-39 of the Game Boy sprite indicies this entity will occupy
 */
void entity_init(entity_t* entity, uint8_t sprite_num);

/**
 * @brief Sets which sequence of tiles should be animated for an entity
 */
void entity_set_tile_sequence(entity_t* entity, uint8_t* tile_seq, uint8_t tile_seq_len);

/**
 * @brief Animate one frame of an entity
 */
void entity_animate(entity_t* entity);
