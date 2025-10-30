#include "mp3.h"

//Validating the cla inputs
int arguments_check(int argc, char *argv[])
{
    if(argc < 2)
    {
        print_error();
        return 0;
    }
    else if(argc == 2)       //Help
    {
        if(strcmp(argv[1],"-h")==0) 
        {
            help();
            return 0;
        }
        else 
        {
            print_error();
            return 0;
        }
    }
    else if(argc == 4) 
    {
        print_error();
        return 0;
    }
    else if(argc == 3 && (strcmp(argv[1],"-v")!=0))
    {
        print_error();
        return 0;
    }
    else if(argc == 5 && (strcmp(argv[1],"-e")!=0))
    {
        print_error();
        return 0;
    }
    else if(argc == 5)
    {
        if(strstr("-t -T -a -A -y -c -g",argv[3]) == NULL)
        {
            print_error();
            return 0;
        }
    }

    return 1;
}

//Doing the operation based on the inputs 
void do_operation(int argc, char *argv[], Tag_data *meta_data)
{
    if(argc == 3)      //Viewing Frames
    {
        if(strcmp(argv[1],"-v")==0)
        {
            print_frames(meta_data);
        }
        else print_error();
    }
    else if(argc == 5)      //Editing Frames
    {
        if(strcmp(argv[1],"-e")==0)
        {
            uint Tag_found = 0;
            char option = *(argv[3]+1);

            switch(option)
            {
                //Title
                case 't':
                for(int i = 0; i < meta_data->frame_count; i++)
                {
                    if(strcmp(meta_data->frames[i].ID,"TIT2")==0)
                    {
                        edit_frame(&meta_data->frames[i],argv[4]);
                        Tag_found = 1;
                        break;
                    }
                }

                if(!Tag_found) add_frame(meta_data,argv[4],"TIT2");
                print_frames(meta_data);
                printf("Title has been modified!\n");
                
                break;

                //Track
                case 'T':
                for(int i = 0; i < meta_data->frame_count; i++)
                {
                    if(strcmp(meta_data->frames[i].ID,"TRCK")==0)
                    {
                        edit_frame(&meta_data->frames[i],argv[4]);
                        Tag_found = 1;
                        break;
                    }
                }

                if(!Tag_found) add_frame(meta_data,argv[4],"TRCK");
                print_frames(meta_data);
                printf("Track has been modified!\n");
                break;

                //Artist
                case 'a':
                for(int i = 0; i < meta_data->frame_count; i++)
                {
                    if(strcmp(meta_data->frames[i].ID,"TPE1")==0)
                    {
                        edit_frame(&meta_data->frames[i],argv[4]);
                        Tag_found = 1;
                        break;
                    }
                }

                if(!Tag_found) add_frame(meta_data,argv[4],"TPE1");
                print_frames(meta_data);
                printf("Artist has been modified!\n");
                break;

                //Album
                case 'A':
                for(int i = 0; i < meta_data->frame_count; i++)
                {
                    if(strcmp(meta_data->frames[i].ID,"TALB")==0)
                    {
                        edit_frame(&meta_data->frames[i],argv[4]);
                        Tag_found = 1;
                        break;
                    }
                }

                if(!Tag_found) add_frame(meta_data,argv[4],"TALB");
                print_frames(meta_data);
                printf("Album has been modified!\n");
                break;

                //Year
                case 'y':
                for(int i = 0; i < meta_data->frame_count; i++)
                {
                    if(strcmp(meta_data->frames[i].ID,"TYER")==0)
                    {
                        edit_frame(&meta_data->frames[i],argv[4]);
                        Tag_found = 1;
                        break;
                    }
                }

                if(!Tag_found) add_frame(meta_data,argv[4],"TYER");
                print_frames(meta_data);
                printf("Year has been modified!\n");
                break;

                //Comments
                case 'c':
                for(int i = 0; i < meta_data->frame_count; i++)
                {
                    if(strcmp(meta_data->frames[i].ID,"COMM")==0)
                    {
                        edit_frame(&meta_data->frames[i],argv[4]);
                        Tag_found = 1;
                        break;
                    }
                }

                if(!Tag_found) add_frame(meta_data,argv[4],"COMM");
                print_frames(meta_data);
                printf("Comments has been modified!\n");
                break;

                //Genre
                case 'g':
                for(int i = 0; i < meta_data->frame_count; i++)
                {
                    if(strcmp(meta_data->frames[i].ID,"TCON")==0)
                    {
                        edit_frame(&meta_data->frames[i],argv[4]);
                        Tag_found = 1;
                        break;
                    }
                }

                if(!Tag_found) add_frame(meta_data,argv[4],"TCON");
                print_frames(meta_data);
                printf("Genre has been modified!\n");
                break;

                //Default or unsupported
                default:
                print_error();
            }
        }
    }
}