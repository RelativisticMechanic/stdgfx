#include "stdgfx.h"

#define STB_VORBIS_IMPLEMENTATION
#include "stb_vorbis.c"



SOUND* gfx_load_sound(char* filename)
{
    SOUND* snd = calloc(1, sizeof(SOUND));
    // Decode to PCM
    snd->length = stb_vorbis_decode_filename(filename, &snd->channels, &snd->sample_rate, &snd->data);
    fprintf(stderr, "sample rate from stb_vorbis: %d\n", snd->sample_rate);
    return snd;
}

int gfx_play_sound(SOUND* snd)
{
    return __gfx_backend_play_sound(snd->channels, snd->sample_rate, snd->length, snd->data);
}

void gfx_stop_sound(int id)
{
    __gfx_backend_stop_sound(id);
}

int gfx_isplaying_sound(int id)
{
    return __gfx_backend_isplaying_sound(id);
}

void gfx_free_sound(SOUND* snd)
{
    free(snd->data);
    free(snd);
}