#include<stdio.h>
#include "mp3_reader.h"
#include<string.h>
#include "types.h"
#include<stdlib.h>

int main(int argc,char *argv[])
{
    
    MP_READ mp3_reader;     //structure variable
    if(argc == 1)           //checking arguments count
    {
        Print();
        exit(0);
    }
    else if(check_operation(argv) == e_view)    //calling the function
    {
        mp3_reader.file_name = argv[2];         //storing the original file name
        if(open_files(&mp3_reader) == e_success)    //opening files
        {
            if(Check_header_file(&mp3_reader) == e_success)    //checking header
            {
            fseek(mp3_reader.fptr_mp3,10,SEEK_SET);             //skipping 10 bytes in original file
                if(store_tags(&mp3_reader) == e_success)        //storing the details
                {
                green();
                printf("INFO :: Tags are validated successfully\n");
                reset();
                    if(View_tags(&mp3_reader) == e_success)     //viewing the details
                    {
                    green();
                    printf("INFO :: Dispalyed all the tags successfully\n");
                    reset();
                    }
                }
            } 
        }   
    }   
    else if(check_operation(argv) == e_help)         //checking the arguments
    {
        green();
        printf("INFO :: selected help \n");
        reset();
        Print_view();
    }
    else{
    if(argc<5)          //checking the arguments count
    {
        red();
        printf("ERROR :: INVALID ARGUMENTS to Edit\nPlease Refer help page once\n");
        reset();
        exit(0);
    }
    int ch =check_edit_operation(argv,&mp3_reader);     //checking arguments passed for the edit 
    if(argv[4] == NULL)        //checking original file name is passed
    {
        red();
        printf("ERROR ::INVALID ARGUMENTS to Edit\nPlease Refer help page once");
        reset();
        exit(0);
    }
    mp3_reader.file_name = argv[4];
    mp3_reader.fptr_mp3 = fopen(mp3_reader.file_name, "r");     //opening the original file in read mode
    mp3_reader.fptr_def = fopen("def.mpeg", "w");           //opening the duplicate file in write mode
    char image[10];
    fread(image,10, 1, mp3_reader.fptr_mp3);           //reading and writing the header
    fwrite(image,10, 1, mp3_reader.fptr_def);
    store_tags(&mp3_reader);                    //storing the data
    switch(ch)
    {
        
        case 1 :
        {
                if(edit_data(argv,&mp3_reader,0) == e_success)     //function call to edit the title
                {
                    green();
                    printf("INFO :: Title edited Successfully\n");
                    reset();
                }
        }
        break;
        case 2 :
        {
            if(edit_data(argv,&mp3_reader,1) == e_success)     //function call to edit the artist
            {
                green();
                printf("INFO :: Artist name edited successfully\n");
                reset();
            }
        }
        break;
        case 3 :
        {
            if(edit_data(argv,&mp3_reader,2) == e_success)     //function call to edit the album
            {
                green();
                printf("INFO :: Album name edited successfully\n");
                reset();
            }
        }
        break;
        case 4 :
        {
        if(edit_data(argv,&mp3_reader,3) == e_success)         //function call to edit the year
            {
                green();
                printf("INFO :: Year  edited successfully\n");
                reset();
            }
        }
        break;
        case 5:
        {
        if(edit_data(argv,&mp3_reader,4) == e_success)         //function call to edit the genre
            {
                green();
                printf("INFO :: Genre name edited successfully\n");
                reset();
            }
        }
        break;
        case 6 :
        {
        if(edit_data(argv,&mp3_reader,5) == e_success)          //function call to edit the comment
            {
                green();
                printf("INFO :: Comment edited successfully\n");
                reset();
                break;
            }
        }
        break;
    
    }
    }
    return 0;
}
int check_edit_operation(char*argv[],MP_READ * mp3_reader)      //function defination to check the arguments
{
    
    if(argv[2]==NULL)       //checking the type of operation(-v/-e/--help)
    {
    red();
    printf("ERROR :: NOT Select any option to edit\nPlease Visit help page\n");
    reset();
    exit(0);
    }
    else
    {
        /* Checking the arguments passed */
    if((strcmp(argv[1],"-e")==0))
        {
            mp3_reader -> def_file=argv[4];

            if(strcmp(argv[2], "-t")==0)
                {
                    return 1;
                }
                else if(strcmp(argv[2], "-a")==0)
                {
                    return 2;
                }
                else if(strcmp(argv[2], "-A")==0)
                {
                    return 3;
                }
                else if(strcmp(argv[2], "-y")==0)
                {
                    return 4;
                }
                else if(strcmp(argv[2],"-m") ==0)
                {
                    return 5;
                }
                else if(strcmp(argv[2],"-c")==0)
                {
                    return 6;
                }
                else
                {
                red();
                printf("ERROR :: INVALID ARGUMENTS to Edit\nPlease refer help page once\n");
                reset();
                exit(0);
                }
        }
    else
    {
        red();
        printf("ERROR :: -e is not present\n");
        reset();
    } 
    }     
}

OperationType check_operation(char*argv[])      //function defination to check the type of operation
{
    int op;
    if(strcmp(argv[1] ,"--help") ==0)
    {
        op = 1;
    } 
    else if(strcmp(argv[1], "-v")==0)
    {
        op =2;
    }

    switch(op)
    {
        case 1:
        {
            return e_help;
        }
        case 2:
        {
        green();
        printf("INFO :: Selected View operation\n");
        reset();
        if(argv[2]==NULL)
        {
            red();
            printf("ERROR :: mp3file name is not present\n");
            reset();
            exit(0);
        }
        if(strstr(argv[2],".mpeg") == NULL)
        {
        red();
        printf("ERROR :: .mpeg is not Present\n");
        reset();
        exit(0);
        }
        return e_view;
        }
    }
}

Status Check_header_file(MP_READ * mp3_reader)  //function defination to check the file is mp3 or not
{
    char buffer[3];
    rewind(mp3_reader-> fptr_mp3);          //placing the offset position to zero
    fread(buffer,3,1,mp3_reader-> fptr_mp3);
    if(strcmp(buffer,"ID3")!=0){            //check file is mp3 or not
        return e_failure;
    }
    return e_success;
}

Status View_tags(MP_READ * mp3_reader)      //function defination to print the data 
{
    print_equalto();
    printf("\n");
    yellow();
    printf("\t\tMP3 Tag Reader and Editor for ID3V2");\
    reset();
    printf("\n");
    print_equalto();
    printf("\n");
    cyan();
    printf("%-20s:\t","TITLE");
    reset();
    printf("%s\n",mp3_reader -> title_name);
    cyan();
    printf("%-20s:\t","ARTIST");
    reset();
    printf("%s\n",mp3_reader -> artist_name);
    cyan();
    printf("%-20s:\t","ALBUM");
    reset();
    printf("%s\n",mp3_reader -> album_name);
    cyan();
    printf("%-20s:\t","YEAR");
    reset();
    printf("%s\n",mp3_reader -> Year);
    cyan();
    printf("%-20s:\t","GENRE");
    reset();
    printf("%s\n",mp3_reader -> Content_type);
    cyan();
    printf("%-20s:\t","LANGUAGE");
    reset();
    printf("%s\n",mp3_reader -> Comment);
    print_equalto();
    printf("\n");
    return e_success;
}
void print_equalto()
{
    for(int i=0;i<80;i++)
    {
        printf("-");
    }
}
void Print_view()             //function defination to print view page
{
    yellow();
    printf("---------------------------------HELP MENU-----------------------------\n");
    reset();
    printf("1.  -v  -> to view mp3 file contents\n");
    printf("2.  -e  -> to edit mp3 file contents\n");
    printf("2.1.    -t  ->  to edit song title\n");
    printf("2.2.    -a  ->  to edit artist name\n");
    printf("2.3.    -A  ->  to edit album name\n");
    printf("2.4.    -y  ->  to edit year\n");
    printf("2.5.    -m  ->  to edit genre\n");
    printf("2.6.    -c  ->  to edit language\n");
    yellow();
    print_equalto();
    reset();
    printf("\n");
}