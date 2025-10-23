#include "mp3.h"

//Calculating the new tag size
uint32_t new_tag_size(Tag_data *tag)
{
    uint32_t total = 0;
    for (int i = 0; i < tag->frame_count; i++)
        total += 10 + tag->frames[i].size;  // 10 bytes frame header + data
    return total;
}

//Conversion - little endian to big endian
void lil_to_big(uint32_t value, unsigned char *new_size)
{
    new_size[0] = (value >> 24) & 0xFF;
    new_size[1] = (value >> 16) & 0xFF;
    new_size[2] = (value >> 8) & 0xFF;
    new_size[3] = value & 0xFF;
}

//Writing the new file with modified frames
void write_new_tag(Tag_data *meta_data, char *filename)
{
    meta_data->T_size = new_tag_size(meta_data);

    FILE *fp_orig = fopen(filename, "rb");
    if (!fp_orig) {
        printf("Error opening file for reading.\n");
        return;
    }

    FILE *fp_temp = fopen("temp.mp3", "wb");
    if (!fp_temp) {
        printf("Error creating temporary file.\n");
        fclose(fp_orig);
        return;
    }

    //Write updated tag header
    fwrite(meta_data->tag, 1, 3, fp_temp);
    fwrite(&meta_data->ver_major, 1, 1, fp_temp);
    fwrite(&meta_data->ver_minor, 1, 1, fp_temp);
    fwrite(&meta_data->flag, 1, 1, fp_temp);

    unsigned char size_bytes[4];
    lil_to_big(meta_data->T_size, size_bytes);
    fwrite(size_bytes, 1, 4, fp_temp);

    //Write updated frames
    for (int i = 0; i < meta_data->frame_count; i++) 
    {
        Frame *frame = &meta_data->frames[i];

        fwrite(frame->ID, 1, 4, fp_temp);

        lil_to_big(frame->size, size_bytes);
        fwrite(size_bytes, 1, 4, fp_temp);

        fwrite(frame->flag, 1, 2, fp_temp);

        fwrite(&frame->encode_byte, 1, 1, fp_temp);
        fwrite(frame->data, 1, frame->size - 1, fp_temp);
    }

    //Copy remaining audio data from original file
    fseek(fp_orig, meta_data->T_size + 10, SEEK_SET); // Skip old tag header + tag size

    //Read the song data
    unsigned char song_data[1024];
    size_t song_data_size;
    while ((song_data_size = fread(song_data, 1, sizeof(song_data), fp_orig)) > 0) 
    {
        fwrite(song_data, 1, song_data_size, fp_temp);
    }

    fclose(fp_orig);
    fclose(fp_temp);

    //replace original file with modified file
    remove(filename);
    rename("temp.mp3", filename);
}