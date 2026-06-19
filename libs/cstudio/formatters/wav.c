#include <audio.h>
#include <stdio.h>
#include <stdlib.h>

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