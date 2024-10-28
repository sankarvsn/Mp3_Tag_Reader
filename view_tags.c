#include<stdio.h>
#include "mp3_reader.h"
#include<string.h>
#include "types.h"
#include<stdlib.h>

Status store_tags(MP_READ * mp3_reader)     //function defination to store the data
{
        for(int i=0 ;i<6 ;i++)
        {
            if(i == 0)          //storing the details of title
            {
                mp3_reader->tag_pos[i]=ftell(mp3_reader->fptr_mp3);
                strcpy(mp3_reader -> title_tag, tag_read(mp3_reader->fptr_mp3,mp3_reader));
                mp3_reader -> title_size = size_read(mp3_reader->fptr_mp3,mp3_reader);
                strcpy(mp3_reader -> title_name , name_read(mp3_reader->fptr_mp3,mp3_reader, mp3_reader -> title_size));
            }
            if(i == 1 )     //storing the details of album
            {
                if(strstr(mp3_reader -> title_tag,"TIT2")!=NULL)    //checking the title tag
                {
                mp3_reader->tag_pos[i]=ftell(mp3_reader->fptr_mp3);
                strcpy(mp3_reader->artist_tag, tag_read( mp3_reader->fptr_mp3,mp3_reader));
                mp3_reader -> artist_size = size_read(mp3_reader->fptr_mp3,mp3_reader);
                strcpy(mp3_reader -> artist_name , name_read(mp3_reader->fptr_mp3,mp3_reader, mp3_reader ->artist_size));
                }
            }
            if(i == 2)      //storing the details of album
            {
                if(strstr(mp3_reader -> artist_tag,"TPE1") != NULL)     //checking the artist tag
                {
                mp3_reader->tag_pos[i]=ftell(mp3_reader->fptr_mp3);
                strcpy(mp3_reader->album_tag, tag_read( mp3_reader->fptr_mp3,mp3_reader));
                mp3_reader -> album_size = size_read(mp3_reader->fptr_mp3,mp3_reader);
                strcpy(mp3_reader -> album_name ,name_read(mp3_reader->fptr_mp3,mp3_reader, mp3_reader ->album_size));
                }
            }
            if( i == 3)     //storing the details of year
            {
                if(strstr(mp3_reader -> album_tag,"TALB") != NULL)      //checking the album tag
                {
                mp3_reader->tag_pos[i]=ftell(mp3_reader->fptr_mp3);
                strcpy(mp3_reader-> Year_tag, tag_read( mp3_reader->fptr_mp3,mp3_reader));
                mp3_reader -> Year_size = size_read(mp3_reader->fptr_mp3,mp3_reader);
                strcpy(mp3_reader -> Year , name_read(mp3_reader->fptr_mp3,mp3_reader, mp3_reader ->Year_size));
                }
            }
            if( i == 4)     //storing the details of genre
            {
                if(strstr(mp3_reader -> Year_tag,"TYER") != NULL)       //checking the year tag
                {
                mp3_reader->tag_pos[i]=ftell(mp3_reader->fptr_mp3);
                strcpy(mp3_reader->Content_tag, tag_read( mp3_reader->fptr_mp3,mp3_reader));
                mp3_reader -> Content_size = size_read(mp3_reader->fptr_mp3,mp3_reader);
                strcpy(mp3_reader -> Content_type , name_read(mp3_reader->fptr_mp3,mp3_reader, mp3_reader ->Content_size));
                }
            }
            if( i == 5)     //storing the details of comments
            {
                if(strstr(mp3_reader -> Content_tag,"TCON") != NULL)    //checking the genre tag
                {
                mp3_reader->tag_pos[i]=ftell(mp3_reader->fptr_mp3);
                strcpy(mp3_reader->Comment_tag, tag_read( mp3_reader->fptr_mp3,mp3_reader));
                mp3_reader -> Comment_size = size_read(mp3_reader->fptr_mp3,mp3_reader);
               strcpy( mp3_reader -> Comment , name_read(mp3_reader->fptr_mp3,mp3_reader, mp3_reader ->Comment_size));
                }
            }
        }
        return e_success;
}
void Print()        //function defination to print the details
{
    red();
    printf("ERROR : ./a.out : INVALID ARGUMENTS\n");
    reset();
    green();
    printf("USAGE : \n");
    reset();
    cyan();
    printf("To view please pass like : ./a.out -v mp3filename\n");
    printf(" To edit please pass like : ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename \n");
    printf(" To get help pass like : ./a.out --help \n");
    reset();
}

Status open_files(MP_READ * mp3_reader)     //function defination to open files
{
    // Src Image file (beautiful.bmp)
    mp3_reader -> fptr_mp3 = fopen(mp3_reader->file_name, "r");     //opening the main file in read mode
    // Do Error handling
    if (mp3_reader -> fptr_mp3 == NULL)
    {
        /* it will tell why the file is not opening */
    	perror("fopen");
        red();
    	fprintf(stderr, "ERROR: Unable to open file %s\n", mp3_reader->file_name);
        reset();
    	return e_failure;
    }
    return e_success;
}

char* tag_read( FILE * fptr_mp3,MP_READ *mp3_reader)        //function defination to store the tags
{
    char tag_buffer[4]  = {"0"};
    fread(tag_buffer,4,1,fptr_mp3);
    tag_buffer[4] ='\0';
    strcpy(mp3_reader->gen_tag,tag_buffer);
    return mp3_reader->gen_tag;
}
int size_read(FILE*fptr_mp3,MP_READ *mp3_reader)            //function defination to store the sizes
{
    int size_buffer = 0; 
    fread(&size_buffer, 4, 1, fptr_mp3);
    size_buffer = __builtin_bswap32(size_buffer);
    return size_buffer;  // Return the read size value
}
char *name_read(FILE *fptr_mp3,MP_READ *mp3_reader,int size)    //function defination to store the names/data
{
    char name_buffer[size];
    fseek(fptr_mp3, 3, SEEK_CUR);  
    fread(name_buffer, size - 1, 1, fptr_mp3);
    name_buffer[size - 1] = '\0';  // Null-terminate the string
    strcpy(mp3_reader -> gen_tag,name_buffer);
    return mp3_reader->gen_tag;
}