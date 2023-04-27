#ifndef ASSEMBLER_FIRST_PASS_H
#define ASSEMBLER_FIRST_PASS_H
#ifndef ASSEMBLER_SECOND_PASS_H
#define ASSEMBLER_SECOND_PASS_H
#define ENTRY_ORDER_LENGTH 6
#include "assembler.h"
#include "data.h"
#include <stdio.h>

/***************************************************************************************************/
/**
 * the first pass of the assembler.
 */
void first_pass(FILE* source, FILE* machine_code, symbol* symbol_table, countLines * counters, char* error_found);
/***************************************************************************************************/
/**
 * free struct line
 */
void free_line(line* sentence);
/***************************************************************************************************/
#endif 

/**
 * completes the labels code and makes the .ext .ent files 
 */
void second_pass(FILE * machine_code, FILE* temp_machine_code, symbol * symbol_table, FILE * input_file,countLines* counters, char* error_found, char* file_name_without_type);
/***************************************************************************************************/
/**
 * completes the labels code and copies the temp file to the .ob file and sets the is_external flag
 */
void code_symbols(FILE* machine_code, FILE* temp_machine_code, symbol* symbol_table, countLines* counters, FILE* externals_file, char* is_external, char* error_found);
/***************************************************************************************************/
/**
 * looks for the .entry orders end writes to .ent
 *
 */
char add_entries(FILE* input_file, symbol* symbol_table, FILE* entries_file, countLines* counters, char* error_found);
/***************************************************************************************************/
/**
 * gets a line and if .entry exists returns the label with no white chars before or after
 */
char* get_entry(char* line);
/***************************************************************************************************/
/**
 * closes .ent .ext .ob
 */
void close_files(FILE *externals_file, FILE *entries_file, FILE *machine_code);
/***************************************************************************************************/




#endif 
