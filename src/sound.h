#pragma once

/**
 * @brief The sound module owns the audio hardware. It drives background music
 * (via hUGEDriver) and one-shot sound effects (via CBT-FX), both of which run
 * from the VBL interrupt and share the Game Boy's sound registers.
 */

/**
 * @brief Initialize the sound hardware, start the background music, and begin
 * servicing music and sound effects on the VBL interrupt.
 */
void sound_init(void);

/**
 * @brief Stop all audio: unregister the music and sound-effect drivers from the
 * VBL interrupt and mute every channel.
 */
void sound_stop(void);

/**
 * @brief Trigger a one-shot sound effect.
 * @param sfx FX Hammer sound effect data (e.g. SFX_00).
 */
void sound_play_sfx(const unsigned char *sfx);
