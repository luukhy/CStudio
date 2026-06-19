#include<stdint.h>
#include<stdlib.h>

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

uint32_t get_audio_frame_count(struct Audio audio)
{
    return audio.duration * audio.rate;
}

uint32_t get_audio_length(struct Audio audio)
{
    return get_audio_frame_count(audio) * audio.chan_num * audio.bits / 8;
}

uint32_t get_audio_multiplier(struct Audio audio)
{
    return 32767 / audio.sync_level;
}

struct Audio define_audio(int duration, int rate, int chan_num, uint16_t bits, float sync_level)
{
    struct Audio audio;
    audio.duration = duration;
    audio.rate = rate;         // Sample rate
    audio.chan_num = chan_num; // Number of channels
    audio.bits = bits;         // Bit depth
    audio.sync_level = sync_level;

    uint32_t frame_count = get_audio_frame_count(audio);
    float *channel_1 = (float *)malloc(frame_count * sizeof(float));
    float *channel_2 = (float *)malloc(frame_count * sizeof(float));
    for (uint32_t i = 0; i < frame_count; i++)
    {
        channel_1[i] = (float)(rand() - RAND_MAX / 2) / RAND_MAX;
        channel_2[i] = (float)(rand() - RAND_MAX / 2) / RAND_MAX;
    }
    audio.channel_1 = channel_1;
    audio.channel_2 = channel_2;
    return audio;
}