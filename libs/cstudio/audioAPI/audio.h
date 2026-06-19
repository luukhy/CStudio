#ifndef AUDIO_HEADER
#define AUDIO_HEADER

#include<stdint.h>

struct Audio
{
    float *channel_1;
    float *channel_2;
    uint32_t duration;
    uint32_t rate;     // Sample rate
    uint16_t chan_num; // Number of channels
    uint16_t bits;     // Bit depth
    int16_t byte;
    float sync_level;
};

uint32_t get_audio_frame_count(struct Audio audio);
uint32_t get_audio_length(struct Audio audio);
uint32_t get_audio_multiplier(struct Audio audio);
struct Audio define_audio(int duration, int rate, int chan_num, uint16_t bits, float sync_level);

#endif