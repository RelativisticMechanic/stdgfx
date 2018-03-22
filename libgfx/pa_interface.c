#include "pa_interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct 
{
	uint32_t sample_rate;
	uint32_t len;
	uint32_t current;
	uint32_t channels;
	uint16_t* data;
} audio_interface_callback_data;

typedef struct {
	PaStream* stream;
	audio_interface_callback_data* info;
	int used;
} audio_interface_sound;

audio_interface_sound* sound_ids = NULL;
unsigned int nsounds = 0;

static int audio_interface_callback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
	audio_interface_callback_data* info = (audio_interface_callback_data*)userData;
	uint16_t* output_buffer = (uint16_t*)outputBuffer;
	uint16_t* data_buffer = (uint16_t*)((uintptr_t)info->data + info->current * info->channels);
	uint32_t channels = info->channels;
	uint32_t length = info->len;
	for(int i = 0; i < framesPerBuffer; i++)
	{
		for(int j = 0; j < channels; j++)
		{
			if(info->current >= length)
				return 1; // We are done

			*output_buffer++ = *data_buffer++;
			info->current++;
		}
	}
	return 0;
}

int audio_interface_add_sound(PaStream* stream, audio_interface_callback_data* info)
{
	if(sound_ids == NULL)
	{
		sound_ids = calloc(16, sizeof(audio_interface_sound));
		nsounds = 16;
	}

	for(unsigned int i = 0; i < nsounds; i++)
	{
		if(sound_ids[i].used == 0)
		{
			sound_ids[i].stream = stream;
			sound_ids[i].info = info;
			sound_ids[i].used = 1;
			return i;
		}
	}

	// Not found reallocate
	audio_interface_sound* new_sounds = calloc(nsounds + 16, sizeof(audio_interface_sound));
	
	if(!new_sounds)
	{
		fprintf(stderr, "stdgfx: error allocating sound array, perhaps you have run out of memory!\n");
		return -8;
	}
	
	for(unsigned int i = 0; i < nsounds; i++)
	{
		new_sounds[i].stream = sound_ids[i].stream;
		new_sounds[i].info = sound_ids[i].info;
		new_sounds[i].used = sound_ids[i].used;
	}
	free(sound_ids);
	sound_ids = new_sounds;

	for(unsigned int i = nsounds; i < nsounds + 16; i++)
	{
		if(sound_ids[i].used == 0)
		{
			sound_ids[i].stream = stream;
			sound_ids[i].info = info;
			sound_ids[i].used = 1;
			nsounds += 16;
			return i;
		}
	}
	// Should never execute till here
	fprintf(stderr, "stdgfx: unexpected error during allocation of sound type!\n");
	return -8;
}

audio_interface_sound* audio_interface_get_sound(int id)
{
	if(id >= nsounds)
		return NULL;

	if(sound_ids[id].used == 0)
		return NULL;

	return &sound_ids[id];
}

void audio_interface_exit(void)
{
	for(int i = 0; i < nsounds; i++)
	{
		if(sound_ids[i].used == 1)
		{
			audio_interface_stop_sound(i);
		}
	}
	Pa_Terminate();
}

int audio_interface_init(void)
{
	PaError err = Pa_Initialize();
	if(err != paNoError) 
	{ 
		fprintf(stderr, "stdgfx: failed to initialize audio library.\n");
		fprintf(stderr, "log: %s\n", Pa_GetErrorText(err));
		return -1;
	}
	atexit(&audio_interface_exit);
	return 0;
}

int audio_interface_play_sound(int channels, int sample_rate, int len, uint16_t* data)
{
	PaStream* stream;

	audio_interface_callback_data* info = calloc(1, sizeof(audio_interface_callback_data));
	info->channels = channels;
	info->len = len;
	info->sample_rate = sample_rate;
	info->data = data;
	info->current = 0;

	PaError err = Pa_OpenDefaultStream(&stream, 0, channels, paInt16, sample_rate, 256, audio_interface_callback, info);
	
	if(err != paNoError) 
	{
		fprintf(stderr, "stdgfx: error opening audio stream %s\n", Pa_GetErrorText(err));
		return -1;
	}

	err = Pa_StartStream(stream);
	if(err != paNoError)
	{
		fprintf(stderr, "stdgfx: unable to start the audio stream: %s\n", Pa_GetErrorText(err));
	}
	return audio_interface_add_sound(stream, info);
}

int audio_interface_isplaying(int id)
{
	audio_interface_sound* sound = audio_interface_get_sound(id);
	if(!sound)
		return 0;

	return Pa_IsStreamActive(sound->stream);	
}

void audio_interface_stop_sound(int id)
{
	audio_interface_sound* sound = audio_interface_get_sound(id);
	if(sound)
	{
		Pa_StopStream(sound->stream);
		Pa_CloseStream(sound->stream);
		free(sound->info);
		sound->used = 0;
	}
}
