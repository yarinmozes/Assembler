#include <stdlib.h>
#include <string.h>
#include "symbol.h"
#include "assembler.h"

/***************************************************************************************************/
/*
 * inserts a symbol to the symbol table
 */
symbol* insert_symbol(symbol* to_insert, symbol* symbol_table, char* is_first_symbol) {
    symbol *curr_pointer = symbol_table;
    symbol* new_symbol;
    /*if first symbol- fills the head*/
    if (*is_first_symbol == TRUE){
        to_insert->next = NULL;
        symbol_copy(symbol_table, to_insert);
        *is_first_symbol = FALSE;
        return symbol_table;
    }
    /*else makes a new symbol to insert*/
    new_symbol = allocate_memory(1, SYMBOL);
    symbol_copy(new_symbol, to_insert);
    while (curr_pointer->next != NULL) {/*skips to the end of the table*/
        curr_pointer = curr_pointer->next;
    }
    /*inserts a node*/
    curr_pointer->next = new_symbol;
    new_symbol->next = NULL;
    return symbol_table;
}
/***************************************************************************************************/
/*
 * updates a symbol address in struct line pointer.
 */
void update_symbol_address(line* sentence, countLines counters){
    if (sentence->flags.is_code == TRUE){
        if (*sentence->label.name != '\0'){
            /*calculation*/
            int distance_from_last_IC = counters.IC - counters.last_IC;
            sentence->label.address = counters.IC - (distance_from_last_IC);
        }
    } else{
        if (*sentence->label.name != '\0'){
            if (strcmp(sentence->data_parts.order, "extern") != 0) {/*extern has alreadythe address of 1 (A)*/

                int distance_from_last_DC = counters.DC - counters.last_DC;
                sentence->label.address = counters.DC - (distance_from_last_DC);
            } else{
                sentence->label.address = 1;
            }
        }
    }
}
/***************************************************************************************************/
/*
 * gets a string and a symbol table head.
 */
void* get_symbol(symbol* symbol_table, char* symbol_name){
    symbol* symbol_pointer = symbol_table;
    while (symbol_pointer){
        if (!strcmp(symbol_name, symbol_pointer->name)){
            return symbol_pointer;
        }
        symbol_pointer = symbol_pointer->next;
    }
    return NULL;
}
/***************************************************************************************************/
/*
 * given a string that starts from a symbol name, returns the symbol name
 */
char* get_symbol_name(char* symbol_start){
    /*memory allocation to string to return*/
    char* symbol = allocate_memory(LABEL_MAX_LENGTH, SYMBOL);
    int i = 0;
    /*runs untill the end of the symbol and inserts the name to the string to return*/
    while (*symbol_start && *symbol_start != ' ' && *symbol_start != '\t' && *symbol_start != ',' && *symbol_start != '\n' && *symbol_start != '\r'){
        *(symbol + i) = *symbol_start;
        i++;
        symbol_start++;
    }
    *(symbol + i) = '\0';
    return symbol;
}
/***************************************************************************************************/
/*
 * free a symbol table
 */
void free_symbol_table(symbol* symbol_table){
    while (symbol_table != NULL){/*until the table ends*/
        symbol* next_pointer = symbol_table->next;
        free(symbol_table);
        symbol_table = NULL;
        symbol_table = next_pointer;
    }
}
/***************************************************************************************************/
/*
 * copies source symbol node to a destination symbol node
 */
void symbol_copy(symbol* dest, symbol* source){
    strcpy(dest->name, source->name);
    dest->address = source->address;
    dest->external = source->external;
    dest->sentence_type = source->sentence_type;
    dest->next = source->next;
}
