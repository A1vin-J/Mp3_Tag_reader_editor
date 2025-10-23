#include "mp3.h"

//main function
int main(int argc, char *argv[])
{
    if(arguments_check(argc, argv)==0) return 0;

    Tag_data meta_data;
    meta_data.tag[3]='\0';
    
    //Opening and validating the input file
    FILE *fp = fopen(argv[2],"rb");
    if(fp == NULL)
    {
        printf("File does not exist!");
        return 0;   
    }
    else if((!read_tag(fp,&meta_data))==0) return 0;
    fclose(fp);

    //Encode or decode 
    do_operation(argc, argv, &meta_data);

    //Rewrite the whole file with the modified frame
    write_new_tag(&meta_data,argv[2]);
    
    //Freeing all the dynamically allocated memories
    free_dma(&meta_data);

    return 0;
}