#include<stdio.h>
#include "mp3_reader.h"
#include<string.h>
#include "types.h"
#include<stdlib.h>

Status edit_data(char *argv[], MP_READ *mp3_reader,int num)   //function defination to edit the data
{
    char image[200];
            
    fseek(mp3_reader->fptr_mp3,mp3_reader->tag_pos[0], SEEK_SET);     //skipping the header in main and duplicate files
    fseek(mp3_reader->fptr_def,mp3_reader->tag_pos[0], SEEK_SET);

    fread(image,mp3_reader->tag_pos[num] - mp3_reader->tag_pos[0] , 1, mp3_reader->fptr_mp3);   //skipping the tags in main file based on edit type(-t/-a/-A/-y/-m/-c)
    fwrite(image,mp3_reader->tag_pos[num] - mp3_reader->tag_pos[0], 1, mp3_reader->fptr_def);
    fread(image,4,1,mp3_reader->fptr_mp3);    //reading the tag from main file
    fwrite(image,4,1,mp3_reader->fptr_def);   //writing the tag to duplicate file

    int val=__builtin_bswap32(strlen(argv[3])+1);   //converting little endian to big endian
    fwrite(&val,4,1,mp3_reader->fptr_def);      //writing the size to duplicate file
    fseek(mp3_reader->fptr_mp3,4,SEEK_CUR);     //skipping the 4 bytes to skip the size in main file

    fread(image, 3, 1, mp3_reader->fptr_mp3);
    fwrite(image, 3, 1, mp3_reader->fptr_def);

    fwrite(argv[3],strlen(argv[3]), 1, mp3_reader->fptr_def);

    fseek(mp3_reader->fptr_mp3, mp3_reader->tag_pos[num+1], SEEK_SET);
    while (fread(image, 1, 1, mp3_reader->fptr_mp3) > 0)
    {
        fwrite(image, 1, 1, mp3_reader -> fptr_def);
    }
    return e_success;
}

/* Function Definitions */
void red () {
  printf("\033[1;31m");
}
void green()
{
    printf("\033[0;32m");
}
void yellow (){
  printf("\033[1;33m");
}

void blue ()
{
    printf("\033[0;34m ");
}

void cyan()
{
    printf("\033[0;36m ");
}
/* Function to reset text color to default */
void reset () {
  printf("\033[0m");
}

