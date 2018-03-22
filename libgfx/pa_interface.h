#ifndef PA_INTERFACE_H
#define PA_INTERFACE_H

#include <stdint.h>
#include <portaudio.h>

int audio_interface_init(void);

int audio_interface_play_sound(int channels, int sample_rate, int len, uint16_t* data);

int audio_interface_isplaying(int id);

void audio_interface_stop_sound(int id);

#endif