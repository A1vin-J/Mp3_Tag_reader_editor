#include "mp3.h"

//Parser for normal frames
void parse_normal_frame(Frame *frame) 
{
    if (frame->encode_byte == 0 || frame->encode_byte == 3) 
    {
        printf("%s\n", frame->data);
    } 
    else if (frame->encode_byte == 1 || frame->encode_byte == 2) 
    {
        uint32_t i = 0;

        if(frame->data[0] == 0xff && frame->data[1] == 0xfe) i = 2;

        for ( ; i < frame->size - 1; i += 2)   //skip BOM and print actual data
        {
            if (frame->data[i] != 0x00)
            {
                putchar(frame->data[i]);
            }
        }
        printf("\n");
    } 
    else 
    {
        printf("Unsupported encoding\n");
    }
}

//Parser for COMM frame
void parse_COMM_frame(Frame *frame) 
{
    if (!frame || !frame->data) return;

    uc *ptr = frame->data;
    int pos = 0;

    pos += 3;   //skip language code (3 bytes)

    if(ptr[pos] == 0xff && ptr[pos+1] == 0xfe) pos += 2;

    // skip descriptor string (depends on encoding, null-terminated)

    if (frame->encode_byte == 0x00)   //ISO-8859-1
    { 
        while (ptr[pos] != '\0' && pos < frame->size) pos++;
        pos++;  // skip null byte

        printf("%s\n", &ptr[pos]);
    } 
    else if (frame->encode_byte == 0x01 || frame->encode_byte == 0x02) // UTF-16 or UTF-16BE
    { 
        while (ptr[pos] != '\0' && ptr[pos+1] != '\0')   // double null termination
        {
            pos += 2;
            if (pos >= frame->size) break;
        }
        pos += 2;   // skip null terminator

        if(ptr[pos] == 0xff && ptr[pos+1] == 0xfe) pos += 2;

        if(frame->encode_byte == 0x01)
        {
            for (uint32_t i = pos; i < frame->size - 1; i += 2)   //skip BOM and print actual data
            {
                if (ptr[i] != 0x00)
                {
                    putchar(ptr[i]);
                }
            }
            printf("\n");
        }
        
        else
        {
            for (uint32_t i = pos; i < frame->size - 1; i += 2)   //skip BOM and print actual data
            {
                if (ptr[i+1] != 0x00)
                {
                    putchar(ptr[i+1]);
                }
            }
            printf("\n");
        }
    }

    else if (frame->encode_byte == 0x03) // UTF-8
    {
        while (ptr[pos] != '\0' && pos < frame->size) pos++;
        pos++;   // skip null byte

        printf("%s\n", &ptr[pos]);
    }
    
}

//Viewing the frames
void view_tag(Frame *frame) 
{
    if (strcmp(frame->ID, "COMM") == 0) {
        view_COMM_tag(frame);
    } else {
        view_normal_tag(frame);
    }
}

//Viewing normal frames
void view_normal_tag(Frame *frame)
{
    // printf("%s: ", get_tag_label(frame->ID));
    parse_normal_frame(frame);
}

//Viewing COMM frame
void view_COMM_tag(Frame *frame)
{
    // printf("%s: ", get_tag_label(frame->ID));
    parse_COMM_frame(frame);
}

//Printing the frames
void print_frames(Tag_data *meta_data)
{
    printf("MP3 Tag Reader :\n");
    printf("----------------------------------------\n");

    printf("%-10s : %s V2.%u.%u\n","Version", meta_data->tag, meta_data->ver_major, meta_data->ver_minor);

    printf("%-10s : ","Title");
    for (int i = 0; i < meta_data->frame_count; i++)
    {
        if(strcmp(meta_data->frames[i].ID, "TIT2") == 0 )
        {
            view_tag(&meta_data->frames[i]);
            break;
        }
        else if(i == meta_data->frame_count-1) printf("-\n");
    }

    printf("%-10s : ","Album");
    for (int i = 0; i < meta_data->frame_count; i++)
    {
        if(strcmp(meta_data->frames[i].ID, "TALB") == 0 )
        {
            view_tag(&meta_data->frames[i]);
            break;
        }
        else if(i == meta_data->frame_count-1) printf("-\n");
    }

    printf("%-10s : ","Year");
    for (int i = 0; i < meta_data->frame_count; i++)
    {
        if(strcmp(meta_data->frames[i].ID, "TYER") == 0 )
        {
            view_tag(&meta_data->frames[i]);
            break;
        }
        else if(i == meta_data->frame_count-1) printf("-\n");
    }

    printf("%-10s : ","Track");
    for (int i = 0; i < meta_data->frame_count; i++)
    {
        if(strcmp(meta_data->frames[i].ID, "TRCK") == 0 )
        {
            view_tag(&meta_data->frames[i]);
            break;
        }
        else if(i == meta_data->frame_count-1) printf("-\n");
    }

    printf("%-10s : ","Genre");
    for (int i = 0; i < meta_data->frame_count; i++)
    {
        if(strcmp(meta_data->frames[i].ID, "TCON") == 0 )
        {
            view_tag(&meta_data->frames[i]);
            break;
        }
        else if(i == meta_data->frame_count-1) printf("-\n");
    }

    printf("%-10s : ","Artist");
    for (int i = 0; i < meta_data->frame_count; i++)
    {
        if(strcmp(meta_data->frames[i].ID, "TPE1") == 0 )
        {
            view_tag(&meta_data->frames[i]);
            break;
        }
        else if(i == meta_data->frame_count-1) printf("-\n");
    }

    printf("%-10s : ","Comments");
    for (int i = 0; i < meta_data->frame_count; i++)
    {
        if(strcmp(meta_data->frames[i].ID, "COMM") == 0 )
        {
            view_tag(&meta_data->frames[i]);
            break;
        }
        else if(i == meta_data->frame_count-1) printf("-\n");
    }
    
    printf("----------------------------------------\n");
}