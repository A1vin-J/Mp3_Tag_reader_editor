#ifndef MP3_H
#define MP3_H

//Header files
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

//Typedefs and structures
typedef unsigned int uint;
typedef unsigned char uc;

typedef struct frame_data  //frame structure
{
    uc ID[5];
    uint32_t size;
    uc flag[2];
    uc encode_byte;
    uc *data;
}Frame;

typedef struct tagdata  //ID3 tag structure
{
    uc tag[4];
    uint8_t ver_major;
    uint8_t ver_minor;
    uc flag;
    uint32_t T_size;
    Frame *frames;
    int frame_count;
}Tag_data;

//Validation & Error handling [operations.c]
int arguments_check(int argc, char *argv[]);
void do_operation(int argc, char *argv[], Tag_data *meta_data);

//Reading the tag data [read.c]
int read_tag(FILE *fp,Tag_data *meta_data);
uint32_t big_to_lil(FILE *fp);

//Viewing the tag data [view.c]
void parse_normal_frame(Frame *frame);
void parse_COMM_frame(Frame *frame);
void view_tag(Frame *frame);
void view_normal_tag(Frame *frame);
void view_COMM_tag(Frame *frame);
void print_frames(Tag_data *meta_data);

//Editing the tag data [edit.c]
void edit_frame(Frame *frame, char *new_text);
void edit_normal_frame(Frame *frame, char *new_text);
void edit_COMM_frame(Frame *frame, char *new_text);

//Adding new frame if not exist [edit.c]
void add_frame(Tag_data *meta_data, char *new_text, char *frame_name);
void add_normal_frame(Tag_data *meta_data, char *new_text, char *frame_name);
void add_COMM_frame(Tag_data *meta_data, char *new_text, char *frame_name);

//Miscellaneous [utils.c]
void free_dma(Tag_data * meta_data);
void help(void);
void print_error(void);

//Write the modified tag data [write.c]
uint32_t new_tag_size(Tag_data *tag);
void write_new_tag(Tag_data *meta_data, char *filename);
void lil_to_big(uint32_t value, unsigned char *new_size);

#endif