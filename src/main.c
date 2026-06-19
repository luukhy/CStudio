#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

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

int write_wav(char *file_name, struct Audio audio)
{
    uint32_t duration = audio.duration;
    uint32_t rate = audio.rate;
    uint32_t frame_count = get_audio_frame_count(audio);
    uint16_t chan_num = audio.chan_num;
    uint16_t bits = audio.bits;

    uint32_t length = get_audio_length(audio);
    float multiplier = get_audio_multiplier(audio);
    int16_t byte;

    FILE *wav_file = fopen(file_name, "w");

    if (wav_file == NULL)
    {
        printf("Output file couldn't be opened!\n");
        return 4;
    }

    //// WAVE Header Data
    fwrite("RIFF", 1, 4, wav_file);
    uint32_t chunk_size = length + 44 - 8;
    fwrite(&chunk_size, 4, 1, wav_file);
    fwrite("WAVE", 1, 4, wav_file);
    fwrite("fmt ", 1, 4, wav_file);
    uint32_t subchunk1_size = 16;
    fwrite(&subchunk1_size, 4, 1, wav_file);
    uint16_t fmt_type = 1; // 1 = PCM
    fwrite(&fmt_type, 2, 1, wav_file);
    fwrite(&chan_num, 2, 1, wav_file);
    fwrite(&rate, 4, 1, wav_file);
    // (Sample Rate * BitsPerSample * Channels) / 8
    uint32_t byte_rate = rate * bits * chan_num / 8;
    fwrite(&byte_rate, 4, 1, wav_file);
    uint16_t block_align = chan_num * bits / 8;
    fwrite(&block_align, 2, 1, wav_file);
    fwrite(&bits, 2, 1, wav_file);

    // Marks the start of the data
    fwrite("data", 1, 4, wav_file);
    fwrite(&length, 4, 1, wav_file); // Data size
    for (uint32_t i = 0; i < frame_count; i++)
    {
        byte = (audio.channel_1[i] * multiplier);
        fwrite(&byte, 2, 1, wav_file);
        byte = (audio.channel_2[i] * multiplier);
        fwrite(&byte, 2, 1, wav_file);
    }
    fclose(wav_file);
    free(audio.channel_1);
    free(audio.channel_2);
    return 0;
}

int main()
{
    uint32_t duration = 1;
    uint32_t rate = 96000; // Sample rate
    uint16_t chan_num = 2; // Number of channels
    uint16_t bits = 16;    // Bit depth
    float sync_level = 1.0;

    struct Audio white_noise = define_audio(duration, rate, chan_num, bits, sync_level);
    int write_res = write_wav("../new_file.wav", white_noise);

    if (write_res != 4)
    {
        printf("wrote to file\n");
    }
    printf("%d\n", write_res);
    return 0;
}