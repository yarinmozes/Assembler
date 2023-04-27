#ifndef ASSEMBLER_SYMBOL_TABLE_H
#define ASSEMBLER_SYMBOL_TABLE_H
#include "data.h"

/***************************************************************************************************/
/**
 * inserts a symbol to the symbol table
 */
symbol* insert_symbol(symbol* to_insert, symbol* symbol_table, char* is_first_symbol);
/***************************************************************************************************/
/**
 * updates a symbol address in struct line pointer.
 */
void update_symbol_address(line* sentence, countLines counters);
/***************************************************************************************************/
/**
 *gets a string and a symbol table head.
 */
void* get_symbol(symbol* symbol_table, char* symbol_name);
/***************************************************************************************************/
/**
 * given a string that starts from a symbol name, returns the symbol name
 */
char* get_symbol_name(char* symbol_start);
/***************************************************************************************************/
/**
 * free the symbol table
 */
void free_symbol_table(symbol* symbol_table);
/***************************************************************************************************/
/**
 * copies source symbol node to a destination symbol node
 */
void symbol_copy(symbol* dest, symbol* source);
/***************************************************************************************************/
/**/
#endif 

