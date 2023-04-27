#include <string.h>
#include <stdlib.h>
#include "pass.h"
#include "line_checker.h"
#include "errors.h"
#include "symbol.h"
#include "parse.h"
/***************************************************************************************************/
/*
 * the first pass of the assembler.
 * 1)finds errors and reports to stdout.
 * 2)calculates IC and DC
 * 3)translates the source file to a semi ready machine code.
 * at the first pass stage, labels are printed in ?LABEL format
 * 4) makes an symbol table 
 */
void first_pass(FILE* source, FILE* machine_code, symbol* symbol_table, countLines* counting, char* error_found) {
    char is_first_symbol = TRUE;
    line sentence;
    line_indexes indexes;
    data_image* data = allocate_memory(ONE_UNIT, DATA_IMAGE);/*head of the data image*/
    (data->word.word) = 0;/*initializing bitfield */
    data->next = NULL;
    data->is_head_filled = FALSE;/*flag to know if head full,*/
    data->DC = counting->DC;
    /*initializing counting */
    counting->line_num = 0;
    counting->error_num = 0;
    counting->IC = 100;
    counting->last_IC = 100;
    counting->DC = 0;
    counting->last_DC = 0;
    while (!read_line(source, &sentence)) {/*read_line*/
        initialize_line_variables(&sentence, counting, &indexes);
        /*sets the line num */
        counting->line_num++;
        if (strcmp(sentence.line, "\n") != 0 && strcmp(sentence.line, "\r") != 0) {
            analyze_sentence(&sentence, &indexes, counting);/*parsing*/
            empty_or_comment_line_check(&sentence, &indexes);
            /*continues if not a comment /empty line*/
                if (sentence.flags.is_comment == FALSE && sentence.flags.is_empty_line == FALSE) {
                    /*checks line for errors*/
                    if (errors_check(&sentence, &indexes, counting) == TRUE){
                        *error_found = TRUE;
                    }
                    if(*error_found == FALSE){/*if no errors found, continues first pass*/
                        calculate_num_of_words(&sentence, indexes, counting);/*calculates IC and DC*/
                        update_symbol_address(&sentence, *counting);/*if a label exists, sets its address*/
                        if (*sentence.label.name != '\0') {/*if a label exists, inserts it to the symbols*/
                            symbol_table = insert_symbol(&sentence.label, symbol_table, &is_first_symbol);
                        }
                        first_pass_parse(machine_code, &sentence, &indexes, counting, data);
                    }
                }
        }
        free_line(&sentence);/*free the struct line */
    }
    /*prints data image to the machine code*/
    print_data(machine_code, data, counting);
}
/***************************************************************************************************/
/*free the struct line*/
void free_line(line* sentence){
    free(sentence->line);
    sentence->line = NULL;
}
