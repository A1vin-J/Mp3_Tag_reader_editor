#include "mp3.h"

//Help
void help()
{
    printf("Help menu for Mp3 Tag viewer and editor:\n\n");
    printf("For viewing the tags :  ./mp3_tag_reader -v <file_name.mp3>\n\n");
    printf("For editing the tags :  ./mp3_tag_reader -e <file_name.mp3> modifiers <tag>\n\n");
    printf("%-10s %-10s\n","Modifier","Function");
    printf("%5s %4s %-20s\n","-t"," ","Modifies Title tag");
    printf("%5s %4s %-20s\n","-T"," ","Modifies Track tag");
    printf("%5s %4s %-20s\n","-a"," ","Modifies Artist tag");
    printf("%5s %4s %-20s\n","-A"," ","Modifies Album tag");
    printf("%5s %4s %-20s\n","-y"," ","Modifies Year tag");
    printf("%5s %4s %-20s\n","-c"," ","Modifies Comments tag");
    printf("%5s %4s %-20s\n","-g"," ","Modifies Genre tag");
}

//Error
void print_error()
{
    printf("Error : .mp3_tag_reader: Invalid arguments\n");
    printf("Usage : .mp3_tag_reader -h for help\n");
}

//Freeing all the dynamically allocated memories
void free_dma(Tag_data *meta_data)
{
    for (int i = 0; i < meta_data->frame_count; i++)
    {
        free(meta_data->frames[i].data);
    }
    free(meta_data->frames);
}