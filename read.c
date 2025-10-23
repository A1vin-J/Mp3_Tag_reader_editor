#include "mp3.h"

//Reading the frames
int read_tag(FILE *fp, Tag_data *meta_data)
{
    if (!fp)
    {
        printf("Couldn't open file!\n");
        return -1;
    }

    fread(meta_data->tag, 3, 1, fp); // "ID3" Validation
    if(strcmp(meta_data->tag,"ID3")!=0)
    {
        printf("Invalid File type\nOnly mp3 files are valid!\n");
        return -1;
    }

    fread(&meta_data->ver_major, 1, 1, fp);
    fread(&meta_data->ver_minor, 1, 1, fp);
    fread(&meta_data->flag, 1, 1, fp);
    meta_data->T_size = big_to_lil(fp);

    meta_data->frames = NULL;
    meta_data->frame_count = 0;

    uint32_t bytes_read = 0;

    //Reading all the available frames in the input file
    while (bytes_read < meta_data->T_size)
    {
        //Increasing the frame array's size ( reallocating dynamically )
        meta_data->frames = realloc(meta_data->frames, sizeof(Frame) * (meta_data->frame_count + 1));
        Frame *frame = &meta_data->frames[meta_data->frame_count];

        //Reading Frame ID
        if (fread(frame->ID, 4, 1, fp) != 1) break;
        frame->ID[4] = '\0';

        //Reading Frame size
        frame->size = big_to_lil(fp);

        //Reading flags
        if (fread(frame->flag, 2, 1, fp) != 1) break;

        //REading the encoding type - byte
        if (fread(&frame->encode_byte, 1, 1, fp) != 1) break;

        //Data size (excluding the encoding byte)
        uint32_t data_size = frame->size - 1;

        //Dynamically allocating memory for data using data size
        frame->data = malloc(data_size);
        if (fread(frame->data, 1, data_size, fp) != data_size) break;

        bytes_read += 10 + frame->size; // frame header + size of the frame

        //Increasing Frame count
        meta_data->frame_count++;
    }
    //printf("Number of Frames read: %d\n", meta_data->frame_count-1);
    return 0;
}

//Conversion - big endian to little endian
uint32_t big_to_lil(FILE *fp)
{
    unsigned char bytes[4];

    if (fread(bytes, 1, 4, fp) != 4)
    {
        fprintf(stderr, "Error reading 4 bytes from file\n");
        return 0;
    }

    uint32_t value = (bytes[0] << 24) |     //shifting the bytes to get correct value
                     (bytes[1] << 16) |
                     (bytes[2] << 8)  |
                     (bytes[3]);

    return value;
}