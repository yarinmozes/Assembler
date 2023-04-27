/*
 ******************************************************
 * This program parsing assembly code to machine code *
 ******************************************************
 * */
#include <stdio.h>
#include "symbol.h"
#include <stdlib.h>
#include "pass.h"

int main (int argc, char* argv[]){
    char error_found = FALSE;
    FILE* temp_machine_code;/*temporary file for first pass parsing. */
    FILE* machine_code;/*.ob file*/
    FILE *source;
    char* file_name_without_type;
    int i = 1;
    /*checks if files provided*/
    if (argc == 1) 
    {
        /*prints this error*/
        stop(EXIT_FAILURE, "no assembly code was provided.");
    }
    /*Component of each file provided, all programs are executed through this loop*/
    while (i < argc) {
        /*a struct that holds all the necessary counters for the program*/
        countLines counters;

        symbol* symbol_table = allocate_memory(ONE_UNIT, SYMBOL);/* handle malloc failure*/
        *(symbol_table->name) = '\0';
        symbol_table->next = NULL;

        source = open_file(*(argv + i), "r");/*handle fopen fail*/
        file_name_without_type = get_file_name_without_type(*(argv + i));
        machine_code = open_machine_code(file_name_without_type, "w+");

        temp_machine_code = open_file("temp.TXT", "w+");

        first_pass(source, temp_machine_code, symbol_table, &counters, &error_found);
        if (error_found == FALSE) {
            print_words_count(machine_code, &counters);
            /*completes the label coding in the machine code and produces .ext .ent*/
            second_pass(machine_code, temp_machine_code, symbol_table, source, &counters, &error_found,file_name_without_type);
        } else{
            fclose(machine_code);
            remove_output_files(file_name_without_type);
        }

        free_symbol_table(symbol_table);
        fclose(temp_machine_code);
        remove_temp_file();
        
        if (error_found == TRUE){
            print_errors_summary(file_name_without_type, counters.error_num);
        }

        error_found = FALSE;
        i++;
    }
    return 0;
}
