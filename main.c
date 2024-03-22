#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// ANSI Colors
#define ERROR    "\x1b[31m"
#define SUCCESS   "\x1b[32m"
#define WARNING  "\x1b[33m"
#define INFO    "\x1b[34m"
#define MESSAGE    "\x1b[36m"
#define RESET   "\x1b[0m"

// custom data structures
#define string char*

// operating system
#define INITIAL_STORAGE 4
#define MAX_NUMBER_OF_READS 2
#define MAX_NUMBER_OF_WRITES 2
#define MAX_FILE_NAME_LENGTH 20
#define MAX_FILE_PATH_NAME_LENGTH 20
#define MAX_PROGRAM_NAME_LENGTH 20
#define error(str) printf(ERROR str RESET);
#define warning(str) printf(WARNING str RESET);
#define info(str) printf(INFO str RESET);
#define success(str) printf(SUCCESS str RESET);
#define message(str) printf(MESSAGE str RESET);

/*---------------------------------------------------------------------------*/
/*---------------------------=MATRIX OS 2024=--------------------------------*/
/*---------------------------------------------------------------------------*/
/*-------------------------Author:Doğukan Demirci----------------------------*/

enum IO_MODE {
    READ = 200,
    WRITE = 201,
    LOCKED = 202
};

typedef struct {
    int ID;
    char name[MAX_FILE_NAME_LENGTH];
    char path[MAX_FILE_PATH_NAME_LENGTH];
    int number_of_read;
    int number_of_write;
    bool is_file_being_read;
    bool is_file_being_written;
}File;

typedef struct program {
    int ID;
    char name[MAX_PROGRAM_NAME_LENGTH];
    bool is_reading;
    bool is_writing;
    int reading_file_ID;
    int writing_file_ID;
}Program;

typedef struct {
    int mx_file_array_cursor;
    int mx_file_array_length;
    size_t mx_file_array_size;
}FileHelper;

typedef struct {
    int mx_program_array_cursor;
    int mx_program_array_length;
    size_t mx_program_array_size;
}ProgramHelper;

// File operations
void expand_mx_file_array(File*** mx_file_array,FileHelper** file_helper);
bool is_valid_FileID(File** mx_file_array,FileHelper* file_helper,int ID);
bool is_full_mx_file_array(FileHelper* file_helper);
int generate_new_file(File*** mx_file_array,FileHelper** file_helper,File* new_file);
void deallocate_mx_file_array(File** mx_file_array,FileHelper* file_helper);
void show_file_helper(FileHelper* file_helper);
void show_file(File** mx_file_array,FileHelper* file_helper,int ID);

// Program operations
bool is_valid_ProgramID(Program** mx_program_array,ProgramHelper* program_helper,int ID);
bool is_full_mx_program_array(ProgramHelper* programHelper);
int generate_new_program(Program*** mx_program_array,ProgramHelper** program_helper,Program* new_program);
void expand_mx_program_array(Program*** mx_program_array,ProgramHelper** program_helper);
void deallocate_mx_program_array(Program** mx_program_array,ProgramHelper* program_helper);
void show_program_helper(ProgramHelper* program_helper);
void read_write_file(File*** mx_file_array,FileHelper* file_helper,Program*** mx_program_array,ProgramHelper* program_helper,int fileID,int programID,enum IO_MODE io_mode);
void release_reading_writing_file(File*** mx_file_array,FileHelper* file_helper,Program*** mx_program_array,ProgramHelper* program_helper,int programID,enum IO_MODE io_mode);
void show_program(Program** mx_program_array,ProgramHelper* program_helper,int ID);
void close_all_files(Program*** mx_program_array,ProgramHelper* program_helper,File*** mx_file_array,FileHelper* file_helper,int ID);
void display_all_files(File** mx_file_array,FileHelper* file_helper);
void display_all_programs(Program** mx_program_array,ProgramHelper* program_helper);
void show_user_options();
void shutdown(bool* os_shutdown);

int main() {
    printf("/*---------------------------=MATRIX OS 2024=--------------------------------*\\\n");

    char* first_file_name = "Lab4.pptx";
    char* first_file_path = "C:\\\\LabNotes\\\\";
    char* second_file_name = "Notes.txt";
    char* second_file_path = "C:\\\\LabNotes\\\\";

    File* firstFile = (File*) malloc(sizeof(File));
    firstFile->ID = 1;
    //firstFile->name = "Lab4.pptx";
    strcpy(firstFile->name,first_file_name);
    //firstFile->path = "C:\\\\LabNotes\\\\";
    strcpy(firstFile->path,first_file_path);
    firstFile->number_of_read = 0;
    firstFile->number_of_write = 0;
    firstFile->is_file_being_read = false;
    firstFile->is_file_being_written = false;

    File* secondFile = (File*) malloc(sizeof(File));
    secondFile->ID = 2;
    //secondFile->name = "Notes.txt";
    strcpy(secondFile->name,second_file_name);
    //secondFile->path = "C:\\\\LabNotes\\\\";
    strcpy(secondFile->path,second_file_path);
    secondFile->number_of_read = 0;
    secondFile->number_of_write = 0;
    secondFile->is_file_being_read = false;
    secondFile->is_file_being_written = false;

    char* first_program_name = "OS";
    char* second_program_name = "Kernel";

    Program* firstProgram = (Program*)malloc(sizeof(Program));
    firstProgram->ID = 1;
    //firstProgram->name = "OS";
    strcpy(firstProgram->name,first_program_name);
    firstProgram->reading_file_ID = -1;
    firstProgram->writing_file_ID = -1;

    Program* secondProgram = (Program*) malloc(sizeof(Program));
    secondProgram->ID = 2;
    //secondProgram->name = "Kernel";
    strcpy(secondProgram->name,second_program_name);
    secondProgram->reading_file_ID = -1;
    secondProgram->writing_file_ID = -1;

    FileHelper* file_helper = malloc(sizeof(FileHelper));
    file_helper->mx_file_array_cursor = 0;
    file_helper->mx_file_array_length = INITIAL_STORAGE;
    file_helper->mx_file_array_size = INITIAL_STORAGE * sizeof(File*);

    File** mx_file_array = malloc(file_helper->mx_file_array_size);

    for (int i = 0; i < file_helper->mx_file_array_length; ++i) {
        mx_file_array[i] = (File*)malloc(sizeof(File));
        mx_file_array[i]->ID = -1;
        mx_file_array[i]->number_of_read = 0;
        mx_file_array[i]->number_of_write = 0;
        mx_file_array[i]->is_file_being_written = false;
        mx_file_array[i]->is_file_being_read = false;
        //mx_file_array[i]->name = "empty";
        //mx_file_array[i]->path = "empty";
    }

    ProgramHelper* program_helper = malloc(sizeof(ProgramHelper));
    program_helper->mx_program_array_cursor = 0;
    program_helper->mx_program_array_length = INITIAL_STORAGE;
    program_helper->mx_program_array_size = INITIAL_STORAGE * sizeof(Program*);

    Program** mx_program_array = malloc(program_helper->mx_program_array_size);

    for (int i = 0; i < program_helper->mx_program_array_length; ++i) {
        mx_program_array[i] = (Program*)malloc(sizeof(Program));
        mx_program_array[i]->ID = -1;
        mx_program_array[i]->writing_file_ID = -1;
        mx_program_array[i]->reading_file_ID = -1;
        mx_program_array[i]->is_writing = false;
        mx_program_array[i]->is_reading = false;
        //mx_program_array[i]->name = "";
    }

    generate_new_file(&mx_file_array,&file_helper,firstFile);
    generate_new_file(&mx_file_array,&file_helper,secondFile);

    generate_new_program(&mx_program_array,&program_helper,firstProgram);
    generate_new_program(&mx_program_array,&program_helper,secondProgram);

    //printf("program_id :%d\n", mx_program_array[1]->ID);

    int selected_option = 0;
    bool os_shutdown = false;

    do {
        show_user_options();
        printf_s("Select an option:");
        scanf_s("%d",&selected_option);

        if (selected_option == 1) {
            File *generated_new_file = (File *)malloc(sizeof(File));
            printf_s("ID:");
            scanf_s("%d",&generated_new_file->ID);
            printf_s("\n");
            printf_s("NAME:");
            char* name = "";
            scanf_s("%s",generated_new_file->name,MAX_FILE_NAME_LENGTH);
            printf_s("\n");
            printf_s("PATH:");
            scanf_s("%s",generated_new_file->path,MAX_FILE_PATH_NAME_LENGTH);
            printf_s("\n");
            generated_new_file->number_of_write = 0;
            generated_new_file->number_of_read = 0;
            generated_new_file->is_file_being_read = false;
            generated_new_file->is_file_being_written = false;
            generate_new_file(&mx_file_array, &file_helper,generated_new_file);
            success("_GENERATING FILE IS DONE_");
            printf("\n");
        }
        else if (selected_option == 2)
        {
            Program* generated_new_program = (Program*)malloc(sizeof(Program));
            printf_s("ID:");
            scanf_s("%d",&generated_new_program->ID);
            printf_s("\n");
            printf_s("NAME:");
            scanf_s("%s",generated_new_program->name,MAX_PROGRAM_NAME_LENGTH);
            printf_s("\n");
            generated_new_program->reading_file_ID = -1;
            generated_new_program->writing_file_ID = -1;
            generate_new_program(&mx_program_array,&program_helper,generated_new_program);
            success("_GENERATING PROGRAM IS DONE_");
            printf("\n");
        }
        else if (selected_option == 3)
        {
            int programID = 0;
            int fileID = 0;
            printf_s("writing:\n");
            printf_s("Program ID:");
            scanf_s("%d",&programID);
            printf_s("File ID:");
            scanf_s("%d",&fileID);
            read_write_file(&mx_file_array,file_helper,&mx_program_array,program_helper,fileID,programID,READ);
            printf("\n");
        }
        else if (selected_option == 4)
        {
            int programID = 0;
            int fileID = 0;
            printf_s("reading:");
            printf_s("Program ID:");
            scanf_s("%d",&programID);
            printf_s("File ID:");
            scanf_s("%d",&fileID);
            read_write_file(&mx_file_array,file_helper,&mx_program_array,program_helper,fileID,programID,WRITE);
        }
        else if (selected_option == 5)
        {
            int programID = 0;
            printf_s("Program ID:");
            scanf_s("%d",&programID);
            release_reading_writing_file(&mx_file_array,file_helper,&mx_program_array,program_helper,programID,READ);
        }
        else if (selected_option == 6)
        {
            int programID = 0;
            printf_s("Program ID:");
            scanf_s("%d",&programID);
            release_reading_writing_file(&mx_file_array,file_helper,&mx_program_array,program_helper,programID,WRITE);
        }
        else if (selected_option == 7)
        {
            int programID = 0;
            printf_s("Program ID:");
            scanf_s("%d",&programID);
            close_all_files(&mx_program_array,program_helper,&mx_file_array,file_helper,programID);
        }
        else if (selected_option == 8)
        {
            display_all_files(mx_file_array,file_helper);
        }
        else if (selected_option == 9)
        {
            display_all_programs(mx_program_array,program_helper);
        }
        else if (selected_option == 10)
        {
            shutdown(&os_shutdown);
        }

    } while (!os_shutdown);

/*
    for (int i = 0; i < file_helper->mx_file_array_length; ++i) {
        printf("%d\n",mx_file_array[i]->ID );
    }

    printf("<------------------------------------------------->");
*/

    printf("good bye.");

    deallocate_mx_file_array(mx_file_array,file_helper);
    deallocate_mx_program_array(mx_program_array,program_helper);

    return 0;
}

void show_user_options()
{
    //printf("------------------------------------\n");
    printf("Generate new file (1)\n");
    printf("Generate new program (2)\n");
    printf("Read file (3)\n");
    printf("Write to file (4)\n");
    printf("Release reading file (5)\n");
    printf("Release writing file (6)\n");
    printf("Close all files for the program (7)\n");
    printf("Display all files (8)\n");
    printf("Display all programs (9)\n");
    printf("Exit (10)\n");
    printf("------------------------------------\n");
}

void display_all_programs(Program** mx_program_array,ProgramHelper* program_helper)
{
    for (int i = 0; i < program_helper->mx_program_array_length; ++i) {
        if (mx_program_array[i]->ID != -1)
            show_program(mx_program_array, program_helper, mx_program_array[i]->ID);
    }
}
void display_all_files(File** mx_file_array,FileHelper* file_helper)
{
    for (int i = 0; i < file_helper->mx_file_array_length; ++i) {
        if ( mx_file_array[i]->ID != -1)
            show_file(mx_file_array, file_helper, mx_file_array[i]->ID);
    }

}

void shutdown(bool* os_shutdown) {
    *os_shutdown = true;
}

void show_program_helper(ProgramHelper* program_helper)
{
    printf("PROGRAM_HELPER:\n");
    printf("LENGTH:%d\n",program_helper->mx_program_array_length);
    printf("CURSOR:%d\n",program_helper->mx_program_array_cursor);
    printf("SIZE:%zu\n",program_helper->mx_program_array_size);
}

void read_write_file(File*** mx_file_array,FileHelper* file_helper,Program*** mx_program_array,ProgramHelper* program_helper,int fileID,int programID,enum IO_MODE io_mode)
{
    File* file = (File*) malloc(sizeof(File));
    Program* program = (Program*) malloc(sizeof(Program));

    for (int i = 0; i < file_helper->mx_file_array_length; ++i) {
        if ((*mx_file_array)[i]->ID == fileID)
            file = (*mx_file_array)[i];
    }

    if (file == NULL) {
        printf("File does not exists.\n");
        return;
    }

    for (int i = 0; i < program_helper->mx_program_array_length; ++i) {
        if ((*mx_program_array)[i]->ID == programID)
            program = (*mx_program_array)[i];
    }

    if (program == NULL) {
        printf("Program does not exists.\n");
        return;
    }

    if (io_mode == READ) {
        if ((file->number_of_read + 1) <= MAX_NUMBER_OF_READS && (file->is_file_being_written == false)) {
            file->number_of_read++;
            file->is_file_being_read = true;
            program->is_reading = true;
            program->reading_file_ID = file->ID;
            printf("\n");
            success("_WRITING PROGRAM_")
            printf("\n");
        }
        else if (file->is_file_being_written == true)
        {
            printf("\n");
            error("_This file is being written from another program._");
            printf("\n");
        }
        else {
            printf("\n");
            error("_This file has reached MAXIMUM_CAPACITY_OF_READ._")
            printf("\n");
        }
    }
    else if (io_mode == WRITE) {
        if ((file->number_of_write + 1) <= MAX_NUMBER_OF_WRITES && (file->is_file_being_read == false)) {
            file->number_of_write++;
            file->is_file_being_written = true;
            program->is_writing = true;
            program->writing_file_ID = file->ID;
        } else {
            printf("\n");
            error("_This file has reached MAXIMUM_CAPACITY_OF_WRITE._")
        }
    }
    else if (io_mode == LOCKED) {
        printf("\n");
        warning("_This file has locked, please ensure you are administrator._")
    }

}

void show_program(Program** mx_program_array,ProgramHelper* program_helper,int ID)
{
    Program* program;

    for (int i = 0; i < program_helper->mx_program_array_length; ++i) {
        if (mx_program_array[i]->ID == ID)
            program = mx_program_array[i];
    }

    printf("-------------------------------\n");
    printf("PROGRAM:\n");
    printf("ID:%d\n",program->ID);
    printf("READING_ID:%d\n",program->reading_file_ID);
    printf("WRITING_ID:%d\n",program->writing_file_ID);
    printf("IS_WRITING:%d\n",program->is_writing);
    printf("IS_READING:%d\n",program->is_reading);
    printf("NAME:%s\n",program->name);
    printf("-------------------------------\n");
}

void close_all_files(Program*** mx_program_array,ProgramHelper* program_helper,File*** mx_file_array,FileHelper* file_helper,int ID)
{

    File* file = NULL;
    Program *program = NULL;

    for (int i = 0; i < program_helper->mx_program_array_length; ++i) {
        if ((*mx_program_array)[i]->ID == ID) {
            program = (*mx_program_array)[i];
            break;
        }
    }

    if (program == NULL)
    {
        printf("PROGRAM DOES NOT EXISTS.\n");
        return;
    }

    if (program->is_reading)
    {

        int program_reading_fileID = program->reading_file_ID;

        for (int i = 0; i < file_helper->mx_file_array_length; ++i) {
            if ((*mx_file_array)[i]->ID == program_reading_fileID) {
                file = (*mx_file_array)[i];
                break;
            }
        }

        if (file == NULL)
        {
            printf("FILE DOES NOT EXISTS WHILE CLOSING ALL FILES.\n");
            return;
        }

        release_reading_writing_file(mx_file_array,file_helper,mx_program_array,program_helper,program_reading_fileID,READ);
        program->is_reading = false;
        program->reading_file_ID = -1;
    }

    if (program->is_writing)
    {

        int program_writing_fileID = program->writing_file_ID;

        for (int i = 0; i < file_helper->mx_file_array_length; ++i) {
            if ((*mx_file_array)[i]->ID == program_writing_fileID) {
                file = (*mx_file_array)[i];
                break;
            }
        }

        if (file == NULL)
        {
            printf("FILE DOES NOT EXISTS WHILE CLOSING ALL FILES.\n");
            return;
        }

        release_reading_writing_file(mx_file_array,file_helper,mx_program_array,program_helper,program_writing_fileID,WRITE);
        program->is_writing = false;
        program->writing_file_ID = -1;
    }

}

void show_file(File** mx_file_array,FileHelper * file_helper,int ID)
{
    File* file = NULL;

    for (int i = 0; i < file_helper->mx_file_array_length; ++i) {
        if (mx_file_array[i]->ID == ID)
            file = mx_file_array[i];
    }

    printf("-------------------------------\n");
    printf("FILE:\n");
    printf("ID:%d\n",file->ID);
    printf("NUMBER_OF_READ:%d\n",file->number_of_read);
    printf("NUMBER_OF_WRITE:%d\n",file->number_of_write);
    printf("IS_READ:%d\n",file->is_file_being_read);
    printf("IS_WRITE:%d\n",file->is_file_being_written);
    printf("NAME:%s\n",file->name);
    printf("PATH:%s\n",file->path);
    printf("-------------------------------\n");
}


void release_reading_writing_file(File*** mx_file_array,FileHelper* file_helper,Program*** mx_program_array,ProgramHelper* program_helper,int programID,enum IO_MODE io_mode)
{
    File* file = NULL;
    Program* program = NULL;

    for (int i = 0; i < program_helper->mx_program_array_length; ++i) {
        if ((*mx_program_array)[i]->ID == programID)
            program = (*mx_program_array)[i];
    }

    if (program == NULL) {
        return;
    }

    if (io_mode == READ) {
        if (program->is_reading)
        {
            for (int i = 0; i < file_helper->mx_file_array_length; ++i) {
                if ((*mx_file_array)[i]->ID == program->reading_file_ID)
                    file = (*mx_file_array)[i];
            }

            if (file == NULL)
            {
                return;
            }

            if (file->number_of_read > 0 && file->number_of_read <= 2)
                file->number_of_read--;
            else if(file->number_of_read == 0)
                file->is_file_being_read = false;
            else
                printf("number of read range error!\n");

            program->reading_file_ID = -1;
            program->is_reading = false;

        }
        else
        {
            printf("\n");
            error("_Program is not reading_")
        }
    }
    else if (io_mode == WRITE)
    {
        if (program->is_writing)
        {
            for (int i = 0; i < file_helper->mx_file_array_length; ++i) {
                if ((*mx_file_array)[i]->ID == program->writing_file_ID)
                    file = (*mx_file_array)[i];
            }

            if (file == NULL)
            {
                return;
            }

            if (file->number_of_write > 0 && file->number_of_write <= 2)
                file->number_of_write--;
            else if(file->number_of_write == 0)
                file->is_file_being_written = false;
            else
                printf("number of read range error!\n");

            program->writing_file_ID = -1;
            program->is_writing = false;

        }
        else
        {
            printf("\n");
            error("_Program is not writing_");
        }
    }

}

int generate_new_program(Program*** mx_program_array,ProgramHelper** program_helper,Program* new_program)
{
    if (is_full_mx_program_array(*program_helper)) {
        printf("Program buffer has expanded.\n");
        expand_mx_program_array(mx_program_array, program_helper);
    }

    if (!is_valid_ProgramID(*mx_program_array,*program_helper,new_program->ID)) {
        printf("Program is not valid! programID:%d\n",new_program->ID);
        return -1;
    }

    (*mx_program_array)[(*program_helper)->mx_program_array_cursor++] = new_program;

    return 1;
}

bool is_valid_ProgramID(Program** mx_program_array,ProgramHelper* program_helper,int ID)
{
    for (int i = 0; i < program_helper->mx_program_array_length; ++i) {
        if ((*mx_program_array)[i].ID == ID) {
            return false;
        }
    }
    return true;
}

bool is_full_mx_program_array(ProgramHelper* program_helper)
{
    if (program_helper->mx_program_array_length == program_helper->mx_program_array_cursor)
        return true;
    return false;
}

void expand_mx_program_array(Program*** mx_program_array,ProgramHelper** program_helper)
{
    // size_t to int
    int new_capacity = (*program_helper)->mx_program_array_length * 2;

    Program** expanded_array = (Program**)realloc(*mx_program_array, sizeof(File*) * new_capacity);
    if (expanded_array == NULL) {
        printf_s("Expanded program array is null.\n");
        return;
    }

    *mx_program_array = expanded_array;

    for (size_t i = (*program_helper)->mx_program_array_length; i < new_capacity; ++i) {
        (*mx_program_array)[i] = (Program*)malloc(sizeof(Program));
        if ((*mx_program_array)[i] == NULL) {

        } else {
            (*mx_program_array)[i]->ID = -1;
        }
    }

    (*program_helper)->mx_program_array_length = new_capacity;
}

void deallocate_mx_program_array(Program** mx_program_array,ProgramHelper* program_helper)
{
    for (int i = 0; i < program_helper->mx_program_array_length; ++i) {
        free(mx_program_array[i]);
    }
    free(mx_program_array);
}

int generate_new_file(File*** mx_file_array,FileHelper** file_helper,File* new_file)
{
    if (is_full_mx_file_array(*file_helper))
        expand_mx_file_array(mx_file_array,file_helper);

    if (!is_valid_FileID(*mx_file_array,*file_helper,new_file->ID))
        return -1;

    (*mx_file_array)[(*file_helper)->mx_file_array_cursor++] = new_file;

    //show_file_helper(*file_helper);

    return true;
}

void expand_mx_file_array(File*** mx_file_array, FileHelper** file_helper) {
    size_t new_capacity = (*file_helper)->mx_file_array_length * 2;

    File** expanded_array = (File**)realloc(*mx_file_array, sizeof(File*) * new_capacity);
    if (expanded_array == NULL) {

        return;
    }

    *mx_file_array = expanded_array;

    for (size_t i = (*file_helper)->mx_file_array_length; i < new_capacity; ++i) {
        (*mx_file_array)[i] = (File*)malloc(sizeof(File));
        if ((*mx_file_array)[i] == NULL) {

        } else {
            (*mx_file_array)[i]->ID = -1;
        }
    }

    (*file_helper)->mx_file_array_length = new_capacity;
}


void deallocate_mx_file_array(File** mx_file_array,FileHelper* file_helper)
{
    for (int i = 0; i < file_helper->mx_file_array_length; ++i) {
        free(mx_file_array[i]);
    }
    free(mx_file_array);
    free(file_helper);
}

bool is_full_mx_file_array(FileHelper* file_helper)
{
    if (file_helper->mx_file_array_length == file_helper->mx_file_array_cursor)
        return true;
    else
        return false;
}

void show_file_helper(FileHelper* file_helper)
{
    printf("length:%d\n",file_helper->mx_file_array_length);
    printf("size:%zu\n",file_helper->mx_file_array_size);
    printf("cursor:%d\n",file_helper->mx_file_array_cursor);
}

bool is_valid_FileID(File** mx_file_array,FileHelper* file_helper,int ID)
{
    //show_file_helper(file_helper);
    for (int i = 0; i < file_helper->mx_file_array_length; i++) {
        //printf(">ID : %d\n" , mx_file_array[i]->ID);
        if (ID == mx_file_array[i]->ID) {
            error("ERRNO:100 The file ID exists.\n")
            return false;
        }
    }
    //printf("returned\n");
    return true;
}