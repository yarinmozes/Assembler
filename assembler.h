#ifndef ASSEMBLER_IN_OUT_TOOLS_H
#define ASSEMBLER_IN_OUT_TOOLS_H
#include <stdio.h>
#include "data.h"

#define PRINT_ADDRESS fprintf(machine_code, "%07d ", last_IC)
#define PRINT_LABEL fprintf(machine_code, "?%s", label_name)
#define PRINT_CODE_WORD fprintf(machine_code, "%06x\n", to_print)
#define PRINT_DATA_WORD fprintf(machine_code,"%07d %06x\n", counters->last_IC + data->DC, data_to_print)
#define PRINT_LAST_DATA_WORD fprintf(machine_code,"%07d %06x", counters->last_IC + data->DC, data_to_print)
#define START 0
#define ONE_UNIT 1
#define MEMORY 12
#define FOPEN 14
#define POINTER_CHECK if (p == NULL) stop(MEMORY,"");
#define CHAR 1
#define SYMBOL 2
#define DATA_IMAGE 3
#define LINE_ASSUMED_LENGTH 100
#define TYPE_MAX_LENGTH 4
#define HEX_PRINT_LENGTH 6
/****************************************alloc.c***********************************************************/
/**
 * gets a line from a file using dynamic memory allocation.
 */
char* get_line_dynamic(FILE* file, int* length);
/***************************************************************************************************/
/**
 * reads a line from a file using dynamic memory allocation 
 */
int read_line(FILE* file, line* sentence);
/***************************************************************************************************/
/**
 * returns the first char after the spaces ' '.
 */
int skip_spaces(FILE* file);
/***************************************************************************************************/
/**
 * given an index of a white char, returns the index of the first char of the next word
 */
int find_next_word(const char* line, int index);
/***************************************************************************************************/
/**
 * returns a string from the file pointer until a white char.
 */
char* get_until_white_char(const char* line, int index);
/***************************************************************************************************/
/**
 *  in case of a fail, gives a appropriate massage and exit the program
 */

void* allocate_memory (int size, char type);
/***************************************************************************************************/
/**
 * in case of a failure, gives a appropriate massage to stderr and exit the program
 */
void* realloc_memory (void* ptr, int size, char type);



/******************************************files.c*********************************************************/
/**
 *  if the opening failed gives a appropriate massage to stderr and exit the program
 */
FILE* open_file(const char* file_name, const char* mode);
/***************************************************************************************************/
/**
 * opens an .ob file 
 */
FILE * open_machine_code(char* file_name, const char* mode);
/***************************************************************************************************/
/**
 * makes .ent file 
 */
FILE* create_ent_files(char* name_without_type);
/***************************************************************************************************/
/**
 * makes .ext file 
 */
FILE* create_ext_files(char* name_without_type);
/***************************************************************************************************/
/**
 * extracts the file name without the file type
 * changes the original string
 */
char* get_file_name_without_type(char* file_name);
/***************************************************************************************************/
/**
 * removes files using a name without file type
 */
void remove_output_files(char* file_name);
/***************************************************************************************************/
/**
 * removes "temp.exe" file(used in for making the machine code)
 */
void remove_temp_file();
/***************************************************************************************************/
/**
 * removes .ob file using a name without file type
 */
void remove_ob_file(char* file_name);
/***************************************************************************************************/
/**
 * removes .ent file using a name without file type
 */
void remove_ent_file(char* file_name);
/***************************************************************************************************/
/**
 * removes .ext file using a name without file type
 */
void remove_ext_file(char* file_name);
/***************************************************************************************************/
/**
 * remove the unnecessary files
 */
void remove_unnecessary_files(char* name_without_type, const char* error_found, char is_external, char is_entry);
/***************************************************************************************************/
/**
 * prints instruction line words to machine code
 */
void print_code_words(FILE* machine_code, char* line, int last_IC, int num_of_words, ...);
/***************************************************************************************************/
/**
 * prints a label's name to the machine code with a '?' or "?&" before(in "jump to label")
 */
void print_label(FILE* machine_code, const char* line, word* word_to_print);
/***************************************************************************************************/
/**
 * prints the data image to machine code.
 */
void print_data(FILE* machine_code,data_image* data, countLines* counters);
/***************************************************************************************************/
/**
 *prints the words count in the first line of the machine code(.ob)
 */
void print_words_count(FILE* machine_code, countLines* counters);
/***************************************************************************************************/
/**
 * prints visual indication for the "report_error" function
 */
void print_visual_indication(int index, const char* line);
/***************************************************************************************************/
/**
 * prints the name and the address of the an external label to the .ext file
 */
void print_extern(FILE* file, symbol* external, char* current_address);
/***************************************************************************************************/
/**
 *prints the name and the address of the an entry label to the .ent file
 */
void print_entry(FILE* file, symbol* entry, int IC);
/***************************************************************************************************/
/**
 * prints an error summary.
 */
void print_errors_summary(char* file_name, int errors_count);
/***************************************************************************************************/
/**
 * stops with exit code, with option to approproate massage, if a massage isn't provided, empty string is required instead:
 *
 */
void stop(int exit_type, const char* massage);
#endif 
