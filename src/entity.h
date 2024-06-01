#include <stdint.h>
#include <stdlib.h>

/**
 * Entities are something drawn on the screen. Each has an associated sprite,
 * and is animated each tick of a game update loop
 */

typedef enum {
    INPUT_DIR_UP = 1 << 0,
    INPUT_DIR_DOWN = 1 << 1,
    INPUT_DIR_LEFT = 1 << 2,
    INPUT_DIR_RIGHT = 1 << 3,
    INPUT_START = 1 << 4
} input_dir_e;

/**
 * A structure representing something we draw on the screen and data associated
 * with that
 */
typedef struct {
    uint8_t sprite_num; /** 0-39 - Which of the 40 sprites in the gameboy's spirte list this is */
    uint8_t* tile_seq; /** A list of indices into tile memory that this sprite should animate in sequence */
    uint8_t tile_seq_len;
    uint8_t current_tile_idx; /** The index into tile_sequence that is currently being drawn */

    uint8_t pos_x;
    uint8_t pos_y;

    uint8_t input_dir_bitfield; /** Directional control input, from enum
                                    input_dir_e. Could be from a joypad, or from
                                    AI */
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

/**
 * @brief Set an entity's location
 */
void entity_set_pos(entity_t* entity, uint8_t x, uint8_t y);

/**
 * @brief Set an entity's input direction bitfield
 * @param bitfield - Bit field of bits from enum input_dir_e
 */
void entity_set_input_dir_bitfield(entity_t* entity, uint8_t input_dir_bitfield);
