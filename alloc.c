#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "assembler.h"
/***************************************************************************************************/
/*
 *  gets a line from a file using dynamic memory allocation. 
 */
char* get_line_dynamic(FILE* file, int* length) {
    char *str = allocate_memory(LINE_ASSUMED_LENGTH, CHAR);
    int curr_char = skip_spaces(file);/*gets from the first non space char*/
    /*if a line length exceeded LINE_ASSUMED_LENGTH, multiplies the allocation with this variable*/
    int line_length_multiplier = 1;
    *length = 1;
    *(str) = (char) curr_char;
    if (curr_char == EOF) {
        free(str);
        return "";
    }
    /*if the line have only '\n', returns '\n'*/
    if (curr_char == '\n') {
        *(str + 1) = '\0';
        return str;
    }
    /*gets the line*/
    while ((curr_char = fgetc(file)) != '\n' && curr_char != '\r' && curr_char != EOF) {
        /*if a line length exceeded LINE_ASSUMED_LENGTH, multiplies the allocation with this variable*/
        if (*length == line_length_multiplier * LINE_ASSUMED_LENGTH) {
            line_length_multiplier++;
            str = realloc_memory(str, LINE_ASSUMED_LENGTH * line_length_multiplier, CHAR);
        }
        *(str + (*length)) = (char) curr_char;
        (*length)++;
    }
   
    if (curr_char == '\r') {
        fgetc(file);
    }
    *(str + (*length)) = '\0';

    return str;
}
/***************************************************************************************************/
/*
 * reads a line from a file using dynamic memory allocation
 */
int read_line(FILE* file, line* sentence){
    sentence->line = get_line_dynamic(file, &sentence->length);

    if (!strcmp(sentence->line, ""))
        return EOF;
    return 0;
}
/***************************************************************************************************/
/*returns the first char after the spaces.*/

int skip_spaces(FILE* file){
    int ch;
    while ((ch = fgetc(file)) == ' ');
    return ch;
}
/***************************************************************************************************/
/*
 * returns the index of the first char of the next word
 */
int find_next_word(const char* line, int index){
    int str_length = (int)strlen(line);
    int i = index;
    while (i < str_length && (*(line + i) == ' ' || *(line + i) == '\t')){
        i++;
    }
    return i;
}
/***************************************************************************************************/
/*returns a string from the file pointer until a white char*/
char* get_until_white_char(const char* line, int index) {
    char* string = allocate_memory(LINE_ASSUMED_LENGTH, CHAR);/*initial allocation*/
    int i = index;
    int k = 0;
    int line_length_multiplier = 1;
    *(string) = '\0';
    while (*(line + i) && *(line + i) != ' ' && *(line + i) != '\t'){
        *(string+k) = *(line+i);
        i++;
        k++;
        if (k == line_length_multiplier* LINE_ASSUMED_LENGTH) {
            line_length_multiplier++;
            string = realloc_memory(string, LINE_ASSUMED_LENGTH * line_length_multiplier, CHAR);
            *(string + LINE_ASSUMED_LENGTH * line_length_multiplier - 1) = '\0';
        }
    }
    *(string + k) = '\0';
    return string;
}
/***************************************************************************************************/

/* in case of a fail, gives a appropriate massage  and exit the program*/

void* allocate_memory (int size, char type){

    if (type == SYMBOL) {
        symbol* p = (symbol*) malloc(sizeof(symbol) * size);
        POINTER_CHECK
        return p;
    }
    if (type == CHAR) {
        char* p = (char*) malloc(sizeof(char) * size);
        POINTER_CHECK
        return p;
    }
    if (type == DATA_IMAGE) {
        char* p = (char*) malloc(sizeof(data_image) * size);
        POINTER_CHECK
        return p;
    }


    return NULL;
}
/***************************************************************************************************/

/* in case of a fail, gives a appropriate massage and exit the program*/
void* realloc_memory (void* ptr, int size, char type){
    /*reallocation*/
    if (type == SYMBOL) {
        symbol * p = (symbol *) realloc(ptr, sizeof(symbol) * size);
        POINTER_CHECK
        return p;
    }
    if (type == CHAR) {
        char* p = (char *) realloc(ptr, sizeof(char) * size);
        POINTER_CHECK
        return p;
    }
    return NULL;
}
/***************************************************************************************************/
