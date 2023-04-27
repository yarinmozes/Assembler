#include "data.h"
/***************************************************************************************************/

/*initializes line variables for next line processing. */

void initialize_line_variables(line* sentence, countLines* counting, line_indexes* indexes){
    initialize_counters(counting);
    initialize_indexes(indexes);
    initialize_line(sentence);
}
/***************************************************************************************************/

/*initializes line counting to zero*/

void initialize_counters(countLines* counting){
    counting->num_of_quotation_marks = 0;
    counting->num_of_dots = 0;
    counting->num_of_operators = 0;
    counting->num_of_colons = 0;
    counting->num_of_regs = 0;
    counting->num_of_operands = 0;
    counting->num_of_commas = 0;
}
/***************************************************************************************************/

/*initializes line indexes to -1*/

void initialize_indexes(line_indexes* indexes){
    indexes->first_char_index = NOT_FOUND;
    indexes->first_reg_index = NOT_FOUND;
    indexes->first_quotation_mark_index = NOT_FOUND;
    indexes->second_quotation_mark_index = NOT_FOUND;
    indexes->second_reg_index = NOT_FOUND;
    indexes->dot_index = NOT_FOUND;
    indexes->colon_index = NOT_FOUND;
    indexes->semicolon_index = NOT_FOUND;
    indexes->last_comma_index = NOT_FOUND;
    indexes->operator_index = NOT_FOUND;
    indexes->first_operand_index = NOT_FOUND;
    indexes->second_operand_index = NOT_FOUND;
    indexes->data_index = NOT_FOUND;
}
/***************************************************************************************************/

/*initializes line variables*/

void initialize_line(line* sentence){
    sentence->flags.is_empty_line = FALSE;
    sentence->flags.is_data = FALSE;
    sentence->flags.is_code = FALSE;
    sentence->flags.is_comment = FALSE;
    sentence->code_parts.opcode = NOT_FOUND;
    sentence->code_parts.function = NOT_FOUND;
    *(sentence->data_parts.order) = '\0';
    sentence->label.address = 0;
    *(sentence->label.name) = '\0';
}
