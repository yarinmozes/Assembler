#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler.h"
#include "pass.h"
#include "errors.h"
#include "symbol.h"
#include "parse.h"
/****************************************************************************************************************************************************************/
/*
 * finish the labels code and makes the .ext .ent files and writes to them.
 */

void second_pass(FILE * machine_code, FILE* temp_machine_code, symbol * symbol_table, FILE * input_file, countLines* counters, char* error_found,char* file_name_without_type){
    /*.ext file*/
    FILE* externals_file = create_ext_files(file_name_without_type);
    /*.ent file*/
    FILE* entries_file = create_ent_files(file_name_without_type);
    char is_entry = FALSE;
    char is_external = FALSE;
    
    code_symbols(machine_code, temp_machine_code, symbol_table, counters, externals_file, &is_external, error_found);

    if (*error_found == FALSE) {
        is_entry = add_entries(input_file, symbol_table, entries_file, counters, error_found);
    }
    close_files(externals_file, entries_file, machine_code);
    remove_unnecessary_files(file_name_without_type, error_found, is_external, is_entry);
}

/****************************************************************************************************************************************************************/
/*
 * finish the labels code and copies the temp file to the .ob file and sets the is_external flag
 */

void code_symbols(FILE* machine_code, FILE* temp_machine_code, symbol* symbol_table, countLines* counters, FILE* externals_file, char* is_external, char* error_found) {
    label_address label_address;
    char *curr_line;
    /*holds the address that is written in the line of the current line in the machine code*/
    char curr_address[NUMBER_ALLOWED_LENGTH];
    int curr_address_iterator = 0;
    int curr_char;
    int line_length;
    char *i;
    counters->line_num = 1;
    fseek(temp_machine_code, START, SEEK_SET);
    while ((curr_char = fgetc(temp_machine_code)) != EOF) {
        if (curr_char >= '0' && curr_char <= '9') {/*inserts address from the temp machine code to the "curr_address" array*/
            *(curr_address + curr_address_iterator) = (char) curr_char;
            curr_address_iterator++;
        }
        /*address ended resets "curr_address_iterator" index and puts '\0'*/
        if (curr_char == ' ' || curr_char == '\t' || curr_char == '\n' || curr_char == '\r') 
        {
            *(curr_address + curr_address_iterator) = '\0';
            curr_address_iterator = 0;
        }
        if (curr_char == '?') 
        {
            label_address.label_address_binary = 0;

            curr_line = get_line_dynamic(temp_machine_code, &line_length);
            i = curr_line;
            if (*(i) == '&') {
                i++;
                code_jump(machine_code, symbol_table, i, counters, &label_address, curr_address, error_found);
            }
            else
                code_label_address(machine_code, externals_file, symbol_table, i, counters, &label_address, curr_address, error_found, is_external);
            if (strcmp(curr_line, "") != 0)
                free(curr_line);

        }
        else {
            fputc(curr_char, machine_code);
        }
    }
}
/****************************************************************************************************************************************************************/
/*
 * checks for the .entry orders end writes to .ent
 */
char add_entries(FILE* input_file, symbol* symbol_table, FILE* entries_file, countLines* counters, char* error_found){
    char* line;
    char* entry;
    symbol* entry_symbol;
    int line_length = 0;
    char is_entry = FALSE;
    fseek(input_file, START, SEEK_SET);
    counters->line_num = 0;/*to report errors according to th line num*/
    while (strcmp(line = get_line_dynamic(input_file, &line_length), "") != 0){
        counters->line_num++;
        entry = get_entry(line);
        if (strcmp(entry, "") != 0){
            entry_symbol = get_symbol(symbol_table, entry);
            if (entry_symbol == NULL){/*if symbol isn't declared- reports an error*/
                print_error(line, ENTRY_NOT_EXIST, counters);
                *error_found = TRUE;
            } else{
                is_entry = TRUE;
                print_entry(entries_file, entry_symbol, counters->IC);
            }
        }
        free(line);
        line = NULL;
        if (strcmp(entry, "") != 0) {
            free(entry);
            entry = NULL;
        }
    }
    return is_entry;
}
/************************************************************************************************************************************/
/*
 * gets a line and if .entry exists returns the label with no white chars before or after
 */
char* get_entry(char* line){
    char* i = line;

/*holds chars from the input to check if .entry exists*/

    char entry_check[ENTRY_ORDER_LENGTH] = {'\0'};
    char* label = allocate_memory(LABEL_MAX_LENGTH, CHAR);
    int index = 0;
    *(label) = '\0';
    while (*i && *i != '\"' && *i != ',' && *i != ';'){
        if (*i == 'e'){
            strncpy(entry_check, i, ENTRY_ORDER_LENGTH - 1);/*gets the rest of the word */
            if (!strcmp(entry_check, "entry")){/*if it entry order - gets the label*/
                i += ENTRY_ORDER_LENGTH - 1;
                while (*i == ' ' || *i == '\t'){
                    i++;
                }
                strncpy(label, i, LABEL_MAX_LENGTH - 1);
                while (*(label + index)){
                    if (*(label + index) == '\t' || *(label + index) == ' '){
                        *(label + index) = '\0';
                    }
                    index++;
                }
                return label;
            }
        }
        i++;
    }
    free(label);
    return "";
}
/************************************************************************************************************************************/
/*close*/

void close_files(FILE *externals_file, FILE *entries_file, FILE *machine_code) {
    fclose(externals_file);
    fclose(entries_file);
    fclose(machine_code);
}
