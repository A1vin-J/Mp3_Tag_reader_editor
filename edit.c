#include "mp3.h"

//Editing frames
void edit_frame(Frame *frame, char *new_text)
{
    if (strcmp(frame->ID, "COMM") == 0) edit_COMM_frame(frame, new_text);

    else edit_normal_frame(frame, new_text);
}

//Editing normal frames
void edit_normal_frame(Frame *frame, char *new_text)
{
    frame->encode_byte = 0x00;                  // ISO-8859-1 encoding

    uint32_t new_text_len = strlen(new_text);

    uc *new_data = malloc(new_text_len);

    if (!new_data)
    {
        perror("malloc failed");
        exit(1);
    }

    memcpy(new_data, new_text, new_text_len);

    free(frame->data);

    frame->data = malloc(new_text_len);
    strcpy(frame->data,new_text);

    frame->size = new_text_len + 1;             //updated frame size
}

//Editing COMM frame
void edit_COMM_frame(Frame *frame, char *new_text)
{
    if (!frame || !frame->data) return;

    uc encoding = frame->encode_byte;
    uc *og_data = frame->data;
    
    uint32_t data_len = frame->size - 1;    // bytes in frame->data

    char lang[4] = {0}; //lang code
    memcpy(lang, og_data, 3);

    uc *descriptor = malloc(data_len + 1);  //temporary variable to hold decoded descriptor
    if (!descriptor)
    {
        perror("malloc");
        return;
    }

    uint32_t desc_index = 0;    //descriptor index

    uint32_t pos = 3;           // start of descriptor in og_data

    if (encoding == 0x00 || encoding == 0x03) 
    {
        while (pos < data_len && og_data[pos] != 0x00)  //single-byte descriptor with null terminated (ISO-8859-1 or UTF-8)
        {
            descriptor[desc_index++] = og_data[pos++];
        }

        descriptor[desc_index++] = 0x00;
    }
    else if (encoding == 0x01 || encoding == 0x02)
    {
        int endian_le = -1; // -1 = unknown, 0 = LE, 1 = BE

        if (pos + 1 < data_len)     //check BOM if present after lang code
        {
            if (og_data[pos] == 0xFF && og_data[pos+1] == 0xFE) { endian_le = 0; pos += 2; }        // BOM LE
            else if (og_data[pos] == 0xFE && og_data[pos+1] == 0xFF) { endian_le = 1; pos += 2; }   // BOM BE
        }
        
        //if no BOM then based on encoding ( 0x01 = LE, 0x02 = BE)
        if (endian_le == -1) endian_le = (encoding == 0x02) ? 1 : 0;

        //read UTF-16 code units until 0x0000 (double null)
        while (pos + 1 < data_len)
        {
            uint16_t cu;
            if (endian_le == 0)
            {
                // LE - low byte first
                cu = (uint16_t)(og_data[pos] | (og_data[pos+1] << 8));
            }
            else
            {
                // BE - high byte first
                cu = (uint16_t)((og_data[pos] << 8) | og_data[pos+1]);
            }
            pos += 2;
            if (cu == 0x0000) break;                            // end of descriptor

            // convert code unit -> ISO byte
            if (cu <= 0xFF) descriptor[desc_index++] = (uc)cu;
            else descriptor[desc_index++] = (uc)'?';            // cannot map to single-byte ISO
        }

        descriptor[desc_index++] = 0x00;
    }

    size_t new_text_len = strlen(new_text);
    uint32_t new_data_len = 3 + desc_index + (uint32_t)new_text_len + 1; // language + descriptor(in ISO with null) + new text + null

    uc *new_data = malloc(new_data_len);

    if (!new_data)
    {
        perror("malloc");
        free(descriptor);
        return;
    }

    memcpy(new_data, lang, 3);                                  //lang code
    
    memcpy(new_data + 3, descriptor, desc_index);               //descriptor in ISO
    
    memcpy(new_data + 3 + desc_index, new_text, new_text_len);  //new comment
    new_data[new_data_len - 1] = '\0';                          //null terminator for comment

    free(descriptor);
    free(frame->data);

    frame->data = new_data;
    frame->size = 1 + new_data_len; // frame size includes the encoding byte
    frame->encode_byte = 0x00;

    return;
}

//Add new frame
void add_frame(Tag_data *meta_data, char *new_text, char *frame_name)
{
    if (strcmp(frame_name, "COMM") == 0) add_COMM_frame(meta_data, new_text, frame_name);
    
    else add_normal_frame(meta_data, new_text, frame_name);
}

//Add new normal frame
void add_normal_frame(Tag_data *meta_data,char *new_text, char *frame_name)
{
    meta_data->frames = realloc(meta_data->frames, sizeof(Frame) * (meta_data->frame_count + 1));
    Frame *frame = &meta_data->frames[meta_data->frame_count];

    strncpy(frame->ID,frame_name,4);
    frame->ID[4]='\0';

    frame->size = strlen(new_text) + 1;     //updated frame size
    frame->flag[0] = 0x00;                  //flag
    frame->flag[1] = 0x00;
    frame->encode_byte = 0x00;              //encode byte

    frame->data = malloc(frame->size);
    strcpy(frame->data, new_text);

    meta_data->T_size += 10 + frame->size;
    meta_data->frame_count++;
    
}

//Add new COMM frame
void add_COMM_frame(Tag_data *meta_data,char *new_text, char *frame_name) 
{
    meta_data->frames = realloc(meta_data->frames, sizeof(Frame) * (meta_data->frame_count + 1));   //extend frame array ( +1 element)
    Frame *frame = &meta_data->frames[meta_data->frame_count];

    strncpy(frame->ID,frame_name,4);
    frame->ID[4]='\0';

    frame->size = 3 + 1 + strlen(new_text) + 1;  //lang + null descriptor + size of the string + null terminator
    frame->flag[0] = 0x00;                       //flag
    frame->flag[1] = 0x00;
    frame->encode_byte = 0x00;                   //encode byte

    frame->data = malloc(frame->size);
    strcpy(frame->data, "eng");         //lang code
    frame->data[3] = 0x00;              //null descriptor
    strcpy(&frame->data[4], new_text);  //new comment

    meta_data->T_size += 10 + frame->size;  //updated tag size
    meta_data->frame_count++;               //updated frame count
}