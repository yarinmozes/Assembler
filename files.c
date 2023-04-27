
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "assembler.h"
/***************************************************************************************************/
/*
 * if the opening failed gives a appropriate messages to stderr and exit the program
 */
FILE* open_file(const char* file_name, const char* open_type){
    FILE* file_pointer;
    file_pointer = fopen(file_name, open_type);

    if (file_pointer == NULL) {
        stop(FOPEN,"");
    }

    return file_pointer;
}
/**************************************************************************************************/
/*
 * opens an .ob file that is known to be the machine code for the imaginary cpu.
 */
FILE* open_machine_code(char* file_name, const char* mode){
    int to_open_length = (int)strlen(file_name) + TYPE_MAX_LENGTH;
    /*to make the .ob file name*/
    char* to_open = allocate_memory(to_open_length, CHAR);
    FILE* machine_code;
    strcpy(to_open, file_name);
    strcat(to_open, ".ob\0");
    machine_code = open_file(to_open, mode);
    free(to_open);
    to_open = NULL;
    return machine_code;
}
/**************************************************************************************************/
/*
 * makes .ent file using a file name without file type
 */
FILE* create_ent_files(char* name_without_type){
    int ent_file_name_length = (int)strlen(name_without_type) + TYPE_MAX_LENGTH + 1;
    /*to make the .ent file name*/
    char* ent_file_name = allocate_memory(ent_file_name_length, CHAR);
    FILE* ent_file;
    strcpy(ent_file_name, name_without_type);/*copies the file name */
    strcat(ent_file_name, ".ent\0");/*adds the type*/
    ent_file = open_file(ent_file_name, "w+");
    free(ent_file_name);
    ent_file_name = NULL;
    return ent_file;
}
/**************************************************************************************************/
/*
 *makes .ext file using a file name without file type
 */
FILE* create_ext_files(char* name_without_type){
    int ext_file_name_length = (int)strlen(name_without_type) + TYPE_MAX_LENGTH + 1;
    /*to make the .ext file name*/
    char* ext_file_name = allocate_memory(ext_file_name_length, CHAR);
    FILE* ext_file;
    strcpy(ext_file_name, name_without_type);/*copies the file name without the type*/
    strcat(ext_file_name, ".ext\0");/*adds the type*/
    ext_file = open_file(ext_file_name, "w+");
    free(ext_file_name);
    ext_file_name = NULL;
    return ext_file;
}
/**************************************************************************************************/
/*
 * extracts the file name without the file type.
 */
char* get_file_name_without_type(char* file_name){
    unsigned int length = strlen(file_name);
    char* i = file_name + length;
    while (*i != '.'){
        i--;
    }
    *i = '\0';
    return file_name;
}
/**************************************************************************************************/
/*
 * removes .ob .ent .ext files using a name without file type
 */
void remove_output_files(char* file_name){
    remove_ob_file(file_name);
    remove_ent_file(file_name);
    remove_ext_file(file_name);
}
/**************************************************************************************************/
/*
 * removes "temp.exe" file.
 */
void remove_temp_file(){
    remove("temp.TXT");
}
/**************************************************************************************************/
/*
 * removes .ob file using a name without file type
 */
void remove_ob_file(char* file_name){
    int to_remove_length = (int)strlen(file_name) + TYPE_MAX_LENGTH;
    /*to make the .ob file name*/
    char* to_remove = allocate_memory(to_remove_length, CHAR);
    strcpy(to_remove, file_name);/*copies the file name without the type*/
    strcat(to_remove, ".ob\0");
    remove(to_remove);/*removes*/
    free(to_remove);
    to_remove = NULL;
}
/**************************************************************************************************/
/*
 * removes .ent file using a name without file type
 */
void remove_ent_file(char* file_name){
    int to_remove_length = (int)strlen(file_name) + TYPE_MAX_LENGTH + 1;
    /*to make the .ent file name*/
    char* to_remove = allocate_memory(to_remove_length, CHAR);
    strcpy(to_remove, file_name);/*copies the file name without the type*/
    strcat(to_remove, ".ent\0");
    remove(to_remove);/*removes*/
    free(to_remove);
    to_remove = NULL;
}
/**************************************************************************************************/
/*
 * removes .ext file using a name without file type
 */
void remove_ext_file(char* file_name){
    int to_remove_length = (int)strlen(file_name) + TYPE_MAX_LENGTH + 1;
    /*to make the .ent file name*/
    char* to_remove = allocate_memory(to_remove_length, CHAR);
    strcpy(to_remove, file_name);/*copies the file name without the type*/
    strcat(to_remove, ".ext\0");
    remove(to_remove);/*removes*/
    free(to_remove);
    to_remove = NULL;
}
/**************************************************************************************************/
/*
 * checks which files are unnecessary and removes them(.ob, .ext, .ent)
 */
void remove_unnecessary_files(char* name_without_type, const char* error_found, const char is_external, const char is_entry){
    if (*error_found == TRUE) {
        remove_output_files(name_without_type);
    } else{
        if (is_external == FALSE){
            remove_ext_file(name_without_type);
        }
        if (is_entry == FALSE){
            remove_ent_file(name_without_type);
        }
    }
}

/**************************************************************************************************/
/*
 * prints instruction line words to machine code.
 */
void print_code_words(FILE* machine_code, char* line, int last_IC, int num_of_words, ...) {
    int i = num_of_words;
    va_list arg_pointer;
    word *word_to_print;
    va_start(arg_pointer, num_of_words);
    while (i) {/*while there are word to print*/
        PRINT_ADDRESS;
        word_to_print = va_arg(arg_pointer, word*);
        /*if the word is a label, prints the label name to the file*/
        if (word_to_print->is_label == TRUE || word_to_print->is_jump == TRUE){
            print_label(machine_code, line, word_to_print);
        } else {/*prints maked word*/
            unsigned int to_print = word_to_print->word;
            PRINT_CODE_WORD;
        }
        i--;
        last_IC++;
    }
}
/**************************************************************************************************/
/*
 * prints a label's name to the machine code with a '?' or "?&" before(in "jump to label")
 */
void print_label(FILE* machine_code, const char* line, word* word_to_print) {
    char *label_name = get_until_white_char(line, word_to_print->label_index);
    unsigned int label_length = strlen(label_name);/*for loop boundaries*/
    unsigned int i = label_length;
    PRINT_LABEL;
    /*calculates the number of chars that the print captures,
     * calculates the number of spaces to print to save space for hex print*/
    if (label_length < HEX_PRINT_LENGTH) {
        i++;
        if (word_to_print->is_jump == TRUE){
            i++;/*the '&' counts*/
        }
        /*print spaces to save space for hex print*/
        while (i < HEX_PRINT_LENGTH) {
            fprintf(machine_code, " ");
            i++;
        }
    }
    fprintf(machine_code, "\n");
    free(label_name);
    label_name = NULL;
}
/**************************************************************************************************/
/*
 * prints the data image to machine code.
 */

void print_data(FILE* machine_code, data_image* data, countLines* counters){
    data_image* data_pointer = data;
    unsigned int data_to_print;
    while (data->next != NULL){/*prints the data image*/
        data_to_print =  data->word.word;
        PRINT_DATA_WORD;
        data = data->next;
        free(data_pointer);
        data_pointer = data;
    }
    /*prints the last word*/
    data_to_print =  data->word.word;
    PRINT_LAST_DATA_WORD;
    free(data);
}
/**************************************************************************************************/
/*
 * prints the words count in the first line of the machine code(.ob)
 */
void print_words_count(FILE* machine_code, countLines* counters){
    fprintf(machine_code,"%d %d\n", counters->IC-100, counters->DC);
}
/**************************************************************************************************/
/*
 * prints visual indication for the "report_error" function
 */
void print_visual_indication(int index, const char* line) {
    int i = index;
    if (i == 1) {
        puts("_^");
        return;
    }
    while (i > 1) {
        putchar('_');
        i--;
    }
    puts("^");
    printf("Starting at the character ' %c '\n", *(line + index));
}
/**************************************************************************************************/
/*
 * prints the name and the address of the an external label to .ext file
 */
void print_extern(FILE* file, symbol* external, char* current_address){
    int address = (int)strtod(current_address, NULL);
    fprintf(file, "%s ", external->name);
    fprintf(file, "%07d\n", address);
}
/**************************************************************************************************/
/*
 * prints the name and the address of the an entry label to the .ent file
 */
void print_entry(FILE* file, symbol* entry, int IC){
    int address = entry->address;
    /*order lines has IC address addition*/
    if (entry->sentence_type == DATA){
        address += IC;
    }
    fprintf(file, "%s ", entry->name);
    fprintf(file, "%07d\n", address);
}
/**************************************************************************************************/
/*
 * prints an error summary. 
 */
void print_errors_summary(char* file_name, int errors_count) {
    printf("\nFILE: %s\n", file_name);
    printf("%d WARNING !! errors founds\n", errors_count);
    puts("No output files created"
         "\n==============================================================================================");
}
/**************************************************************************************************/
/*stops with exit code.*/

void stop(int exit_type, const char* to_print) {
    if (strcmp(to_print, "") != 0 && (exit_type == EXIT_FAILURE || exit_type == EXIT_SUCCESS)) {
        fputs(to_print, stderr);
        exit(exit_type);
    } else
        switch (exit_type) {
            case MEMORY: {
                fputs("Not enough memory\n The program was terminated\n", stderr);
                exit(EXIT_FAILURE);
            }
            case FOPEN: {
                fputs("Can't open the file. The program was terminated\n", stderr);
                exit(EXIT_FAILURE);
            }
            default:
                exit(EXIT_FAILURE);
        }
}
