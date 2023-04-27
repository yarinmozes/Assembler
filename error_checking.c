#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "errors.h"
#include "assembler.h"

/*
 * checks order line, the order itself and the data. 
 */
void check_order_line(line* sentence, line_indexes* indexes, countLines* counting, char* error_found){
    if (!is_order_valid(sentence)) {/*if order is not valid*/
        print_error(sentence->line, NO_SUCH_ORDER, counting, indexes->dot_index);
        *error_found = TRUE;
        return;
    }
    find_string_errors(sentence, indexes, counting, error_found);
    find_data_errors(sentence, indexes, counting, error_found);
    find_extern_entry_errors(sentence, indexes, counting, error_found);
}
/***************************************************************************************************/
/*
 * checks if the order in the line.
 */
char is_order_valid(line* sentence) {
    char is_valid = TRUE;/*to return*/
    if (!strcmp(sentence->data_parts.order, "data") || !strcmp(sentence->data_parts.order, "string") ||!strcmp(sentence->data_parts.order, "entry") || !strcmp(sentence->data_parts.order, "extern")) {
        return is_valid;
    }
    is_valid = FALSE;
    return is_valid;
}
/***************************************************************************************************/
/*
 * finds and reports string errors. 
 */
void find_string_errors(line* sentence, line_indexes* indexes, countLines* counting, char* error_found) {
    char is_string_order;

    if (!strcmp(sentence->data_parts.order, "string")){
        is_string_order = TRUE;
    } else{
        is_string_order = FALSE;
    }

    if (counting->num_of_quotation_marks == 0){
        if (is_string_order == TRUE){
            print_error(sentence->line, ORDER_NO_STR, counting);
            *error_found = TRUE;
        }
    }

    if (counting->num_of_quotation_marks == 1) {
        print_error(sentence->line, SINGLE_QUOTMARK, counting, indexes->first_quotation_mark_index);
        *error_found = TRUE;
    }

    if (counting->num_of_quotation_marks >= 2) {
        find_two_quotes_errors(sentence, counting, indexes, is_string_order, error_found);
    }

    if (is_string_order == TRUE) {
        check_pre_order_chars(sentence, indexes, counting, error_found);
    }
}
/***************************************************************************************************/
/*
 * finds errors in a line with with a valid string. 
 */
void find_two_quotes_errors(line* sentence, countLines* counting, line_indexes* indexes, char is_string_order, char* error_found){
    int index_of_word_after_string = find_next_word(sentence->line, indexes->second_quotation_mark_index + 1);
    int index_of_word_after_order = find_next_word(sentence->line, indexes->dot_index + (int)strlen(sentence->data_parts.order) + 1);
    if (is_string_order == FALSE) 
    {
        print_error(sentence->line, STR_NO_ORDER, counting);
        *error_found = TRUE;
    }
    if (indexes->second_quotation_mark_index < index_of_word_after_string && index_of_word_after_string < sentence->length){
        print_error(sentence->line, CODE_AFTER_QUOTE, counting, index_of_word_after_string);
        *error_found = TRUE;
    }
    if (is_string_order == TRUE){ /*checks string errors with .string order*/
        if (indexes->first_quotation_mark_index < indexes->dot_index){
            print_error(sentence->line, STR_BEFORE_STR_ORDER, counting, indexes->second_quotation_mark_index);
            *error_found = TRUE;
        }
        /*if there are chars between .string and the string*/
        if (index_of_word_after_order < indexes->first_quotation_mark_index){
            print_error(sentence->line, CHARS_BETWEEN_ORDER_STR, counting, index_of_word_after_order);
            *error_found = TRUE;
        }
        /*if there is no space before the string*/
        if (indexes->first_quotation_mark_index > 0 && *(sentence->line + indexes->first_quotation_mark_index-1) != ' ' &&
                indexes->first_quotation_mark_index > 0 && *(sentence->line + indexes->first_quotation_mark_index-1) != '\t'){
            print_error(sentence->line, NO_SPACE_BEFORE_QUOTE, counting, indexes->first_quotation_mark_index);
            *error_found = TRUE;
        }
    }
}
/***************************************************************************************************/
/*
 * finds .data order line errors.
 */
void find_data_errors(line* sentence, line_indexes* indexes, countLines* counting, char* error_found){
    short int is_data_order;
    /*makes the is_data_order flag*/
    if(!strcmp(sentence->data_parts.order,"data"))
    {
        is_data_order = TRUE;
    } else{
        is_data_order = FALSE;
    }
    if (counting->num_of_quotation_marks == 0)
    {
        if (is_data_order == TRUE){
            if (!is_data_values_valid(sentence, indexes, counting)){
                *error_found = TRUE;
            }
            /*checks for unexpected chars before .data*/
            check_pre_order_chars(sentence, indexes, counting, error_found);
        }
        if (is_data_order == FALSE){
            /*if .data not exist but there are data values, reports error*/
            if (is_data_values_valid(sentence, indexes, counting) == TRUE){
                print_error(sentence->line, DATA_NO_ORDER, counting);
                *error_found = TRUE;
            }
        }
    }
}
/***************************************************************************************************/
/*
 *checks if data values are valid. 
 */
char is_data_values_valid(line* sentence, line_indexes* indexes, countLines* counting){
    char is_data_values_valid = TRUE;
    char is_data_order;
    /*sets the is_data_order flag*/
    if(!strcmp(sentence->data_parts.order,"data"))
    {
        is_data_order = TRUE;
    } else{
        is_data_order = FALSE;
    }
    /*if no data and .data exist, reports error and is_data_values_valid = FALSE*/
    if (indexes->data_index == NOT_FOUND){
        if (is_data_order == TRUE) {
            print_error(sentence->line, ORDER_NO_DATA, counting);
        }
        is_data_values_valid = FALSE;
    }else {
        if (check_data_values(sentence, indexes->data_index, counting) == TRUE) 
        {
            is_data_values_valid = FALSE;
        }
    }
    return is_data_values_valid;
}
/***************************************************************************************************/
/*
 * checks the data values for example 
 */
char check_data_values(line* sentence, int index, countLines* counting) {
    char error_found = FALSE;
    char last_char = '\0';
    char curr_char = *(sentence->line + index);
    char is_after_comma = FALSE;
    char did_num_appeared = FALSE;
    char is_data_order;
    /*sets is_data_order flag*/
    if (!strcmp(sentence->data_parts.order, "data")) {
        is_data_order = TRUE;
    } else {
        is_data_order = FALSE;
    }
    /*checks the values*/
    while (index < sentence->length) {
        /*checks if current char is illegal*/
        if (ILLEGAL_DATA_CHAR_CONDITION) {
            if (is_data_order == TRUE) {
                print_error(sentence->line, UNEXPECTED_CHARACTER, counting, curr_char, index);
            }
            error_found = TRUE;/*indicates that in general, the values are not valid*/
            update_loop_data_check_variables(sentence, &curr_char, &last_char, &index);
        } else {
            values_check(sentence, counting, curr_char, last_char, is_after_comma, is_data_order, &error_found,
                         did_num_appeared, index);
            check_if_after_comma(&curr_char, &is_after_comma);
            check_num_show(&did_num_appeared, curr_char);
            update_loop_data_check_variables(sentence, &curr_char, &last_char, &index);
        }
    }
        return error_found;
}
/***************************************************************************************************/
/*
 * data values check for "check_data_values" function. 
 */
void values_check(line* sentence, countLines* counting, char curr_char, int last_char, short int is_after_comma,
                         char is_data_order, char* error_found, short int did_num_appeared, int index) {
        if (is_after_comma == TRUE && curr_char == ',') {/*if two comma exists without a num between*/
            if (is_data_order == TRUE){
                print_error(sentence->line, NO_NUMS_BETWEEN_COMMAS, counting, index);
            }
            *error_found = TRUE;
        }
        if (curr_char == ',' && did_num_appeared == FALSE) {/*if a comma appeared before a num*/
            if (is_data_order == TRUE) {
                print_error(sentence->line, COMMA_NO_NUMS, counting, index-1);
            }
            *error_found = TRUE;
        }
        if ((curr_char == '-' || curr_char == '+') && ((index + 1 != sentence->length && (*(sentence->line + index + 1) < '0'
        || *(sentence->line + index + 1) > '9')) || index + 1 == sentence->length)){/*if a '-' or '+' was written not by a num*/
            print_error(sentence->line, SIGN_NOT_BY_NUM, counting, index);
            *error_found = TRUE;
        }
        if (did_num_appeared == TRUE) {
            if ((last_char == ' ' || last_char == '\t') && curr_char != ' ' && curr_char != '\t' &&
                is_after_comma == FALSE && curr_char != ',') {/*if no comma was found before a num*/
                if (is_data_order == TRUE) {
                    print_error(sentence->line, NO_COMMA_BETWEEN, counting, index);
                }
                *error_found = TRUE;
            }
            if (index == sentence->length-1 && (curr_char == ',' || is_after_comma == TRUE)){/*if there is no num shows up after a comma*/
                print_error(sentence->line, COMMA_NO_FOLLOWING_NUM, counting, index);
                *error_found = TRUE;
            }
        }
        if ((curr_char == '-' || curr_char == '+') && *(sentence->line + index - 1) != ' ' &&
                *(sentence->line + index - 1) != '\t' && *(sentence->line + index - 1) != ','){/*if '+' or '-' is not in a valid place*/
            print_error(sentence->line, ARITHMETIC_SIGN_NOT_IN_PLACE, counting, index);
            *error_found = TRUE;
        }
    }
/***************************************************************************************************/
/*
* turns on and off "is_after_comma" flag for "check_data_values" function.
*/
void check_if_after_comma(const char *curr_char, char *is_after_comma) {
    if (*curr_char == ',') {
        *is_after_comma = TRUE;
    } else if (*curr_char != ' ' && *curr_char != '\t') {
        *is_after_comma = FALSE;
    }
}
/***************************************************************************************************/
/*
 * updates loop variables for "check_data_values" function
 */
void update_loop_data_check_variables(line* sentence, char* curr_char, char* last_char, int* index){
    if (*index < sentence->length) {
        (*index)++;
        *last_char = *curr_char;
        *curr_char = *(sentence->line + (*index));
    }
}
/***************************************************************************************************/
/*
 * checks errors for .entry and .extern lines. 
 */
void find_extern_entry_errors(line* sentence, line_indexes* indexes, countLines* counting, char* error_found) {
    if (!strcmp(sentence->data_parts.order, "entry") || !strcmp(sentence->data_parts.order, "extern")) {
        check_data_label(sentence, indexes, counting, error_found);/*checks the label after the order for errors*/
        check_pre_order_chars(sentence, indexes, counting, error_found);/*checks the code before the order for errors*/
        check_after_data_label_chars(sentence, counting, indexes, error_found);/*checks the code after the label for errors*/
    }
}
/***************************************************************************************************/
/*
 * checks a label that comes as data after .extern or .entry. if errors found, turns "error_found" to TRUE, else FALSE
 */
void check_data_label(line* sentence, line_indexes* indexes, countLines* counting, char* error_found){
    int order_end_index = indexes->dot_index + (int) strlen(sentence->data_parts.order);/*end index of .extern or .entry*/
    if (indexes->data_index == NOT_FOUND) {/*label wasn't found. reports an error*/
        print_error(sentence->line, EXTERN_ENTRY_NO_LABEL, counting, order_end_index);
        *error_found = TRUE;
    } else {
        check_label(sentence, indexes, counting, indexes->data_index, error_found);
    }
}
/***************************************************************************************************/
/*
 * checks and reports if there are characters after a label in a .extern ot .entry line. 
 */
void check_after_data_label_chars(line* sentence, countLines* counting, line_indexes* indexes, char* error_found){
    char data_ended = FALSE;
    char* i = sentence->line + indexes->data_index;
    while (*i) {
        if (!data_ended && (*i == ' ' || *i == '\t')){
            data_ended = TRUE;
        }
        if (data_ended == TRUE && *i != ' ' && *i != '\t') {
            print_error(sentence->line, UNEXPECTED_CHARACTER, counting, *i, i - sentence->line);
            *error_found = TRUE;
        }
        i++;
    }
}
/***************************************************************************************************/
/*
 * checks for illegal characters before an order in an order line.
 */
void check_pre_order_chars(line* sentence, line_indexes* indexes, countLines* counting, char* error_found){
    char error_code;
    /*sets "error code" according to the order*/
    if (!strcmp(sentence->data_parts.order, "data")){
        error_code = CHARS_BEFORE_DATA_ORDER;
    }
    if (!strcmp(sentence->data_parts.order, "string")){
        error_code = CHARS_BEFORE_STR_ORDER;
    }
    if (!strcmp(sentence->data_parts.order, "extern") || !strcmp(sentence->data_parts.order, "entry")){
        error_code = CHARS_BEFORE_EXTERN_OR_ENTRY;
    }
    /*checks if there are chars between the label and the order*/
    if (*(sentence->label.name) != '\0' && indexes->colon_index < indexes->dot_index){
        /*the index of the first non-white character after the label declaration*/
        int index_of_word_after_label = find_next_word(sentence->line, indexes->colon_index+1);
        if (*(sentence->line + index_of_word_after_label) != '.') {
            print_error(sentence->line, error_code, counting, index_of_word_after_label);
            *error_found = TRUE;
        }
    } else if (*(sentence->line + indexes->first_char_index) != '.'){/*if there's no label declaration but the order is not the first in the line*/
        print_error(sentence->line, error_code, counting, indexes->first_char_index);
        *error_found = TRUE;
    }
}
/***************************************************************************************************/
/*
 * checks a code line for errors.  
 */

void check_code_line(line* sentence, line_indexes* indexes, countLines* counting, char* error_found) {
    /*checks the num of operands*/
    char is_operand_count_not_okay = check_operands_count(sentence, indexes, counting, error_found);
    /*checks the num of operators*/
    char is_operators_count_not_okay = check_operators_count(sentence, indexes, counting, error_found);
    /*if the operands and operators count okay checks the operands syntax and pre operators chars errors */
    if (!is_operand_count_not_okay && !is_operators_count_not_okay) 
    {
        check_operands_syntax(sentence, indexes, counting, error_found);
        check_pre_operator_chars(sentence, indexes, counting, error_found);
    }
}
/***************************************************************************************************/
/*
 * checks if the num of operands is legal for the operator
 */

char check_operands_count(line* sentence, line_indexes* indexes, countLines* counting, char* error_found){
    char is_error = FALSE;
    switch (sentence->code_parts.opcode) {
        case 0:{}
        case 1:{}
        case 2:{}
        case 3:{}
        case 4:{
            if (counting->num_of_operands < 2)
            {
                print_error(sentence->line, FEW_OPERANDS, counting);
                *error_found = TRUE;
                is_error = TRUE;
            }
            if (counting->num_of_operands > 2)
            {
                print_error(sentence->line, EXTRA_OPERANDS, counting);
                *error_found = TRUE;
                is_error = TRUE;
            }
            break;
        }
        case 5:{}
        case 6:{}
        case 7:{}
        case 8:{}
        case 9:{}
        case 10:{}
        case 11:{}
        case 12:{}
        case 13:{
            if (counting->num_of_operands < 1){
                print_error(sentence->line, FEW_OPERANDS, counting);
                *error_found = TRUE;
                is_error = TRUE;
            }
            if (counting->num_of_operands > 1){
                print_error(sentence->line, EXTRA_OPERANDS, counting);
                *error_found = TRUE;
                is_error = TRUE;
            }
            break;
        }
        case 14:{}
        case 15:{/*cases 14 and 15exactly 0 operands*/
            if (counting->num_of_operands > 0){
                print_error(sentence->line, EXTRA_OPERANDS, counting);
                *error_found = TRUE;
                is_error = TRUE;
            }
            break;
        }
    }
    return is_error;
}
/***************************************************************************************************/
/*
 * checks if the num of operators is greater than one.
 */
char check_operators_count(line* sentence, line_indexes* indexes, countLines* counting, char* error_found){
    char is_error = FALSE;
    if (counting->num_of_operators > 1){
        print_error(sentence->line, EXTRA_OPERATORS, counting);
        *error_found = TRUE;
        is_error = TRUE;
    }
    return is_error;
}
/***************************************************************************************************/
/*
 * checks operands syntax according to the operator and commas conditions.
 */
void check_operands_syntax(line* sentence, line_indexes* indexes, countLines* counting, char* error_found){
    switch (sentence->code_parts.opcode) {
        case 0:{}
        case 1:{}
        case 2:{}
        case 3:{}
        case 4:{
            check_source_operand_syntax(sentence, indexes, counting, error_found);
            check_dest_operand_syntax(sentence, indexes, counting, error_found);
            if (counting->num_of_commas > 1){
                print_error(sentence->line, EXTRA_COMMAS, counting);
                *error_found = TRUE;
            }
            if (counting->num_of_commas == 0){
                print_error(sentence->line, COMMA_REQUIRED, counting);
                *error_found = TRUE;
            }/*checks the last comma position*/
            if (indexes->last_comma_index != NOT_FOUND &&
            (indexes->last_comma_index < indexes->first_operand_index || indexes->last_comma_index > indexes->second_operand_index)){
                print_error(sentence->line, COMMA_NOT_IN_PLACE, counting);
                *error_found = TRUE;
            }
            break;
        }
        case 5:{}
        case 6:{}
        case 7:{}
        case 8:{}
        case 9:{}
        case 10:{}
        case 11:{}
        case 12:{}
        case 13:{
            check_dest_operand_syntax(sentence, indexes, counting, error_found);
        }
        default:{
            if (counting->num_of_commas > 0){
                print_error(sentence->line, NO_COMMAS_ALLOWED, counting);
                *error_found = TRUE;
            }
            return;
        }
    }
}
/***************************************************************************************************/
/*
 *checks source operand syntax and reports errors.
 */
void check_source_operand_syntax(line* sentence, line_indexes* indexes, countLines* counting, char* error_found){
    /*if the operand is valid. if so, checks if it fits the operator*/
    if (is_operand_valid(sentence, indexes, SOURCE_OPERAND) == TRUE) {
        /*if the operand is a num, checks the num size*/
        if (*(sentence->line + indexes->first_operand_index) == '#'){
            check_num_size(sentence->line, indexes->first_operand_index + 1, counting, error_found);
        }
        /*"jump to label" is not allowed as a source operand*/
        if (*(sentence->line+indexes->first_operand_index) == '&'){
            print_error(sentence->line, NO_SOURCE_JUMP_LABEL_ALLOWED, counting, indexes->first_operand_index);
            *error_found = TRUE;
        }
        /*opcode 4 has more restrictions, no num or reg is allowed*/
        if (sentence->code_parts.opcode == 4){
            if (*(sentence->line + indexes->first_operand_index) == '#') {
                print_error(sentence->line, NO_SOURCE_NUM_ALLOWED, counting, indexes->first_operand_index);
                *error_found = TRUE;
            }
            if (indexes->first_operand_index == indexes->first_reg_index){/*first operand can not be second reg*/
                print_error(sentence->line, NO_SOURCE_REG_ALLOWED, counting, indexes->first_operand_index);
                *error_found = TRUE;
            }
        }
    }else{
        print_error(sentence->line, NOT_AN_OPERAND , counting, indexes->first_operand_index);
        *error_found = TRUE;
    }
}
/***************************************************************************************************/
/*
 * gets a num in char*, num can be with a sign.
 */
void check_num_size(char* line, int index, countLines* counting, char* error_found){
    long num;
    if (*(line + index) == '-' || *(line + index) == '+'){
        index++;
    }
    num = strtod(line + index, NULL);
    if (num > NUM_MAX_VAL){/*checks num size*/
        print_error(line, NUM_TOO_BIG, counting, index);
        *error_found = TRUE;
    }
}
/***************************************************************************************************/
/*
 * checks destination operand syntax and reports errors. 
 */
void check_dest_operand_syntax(line* sentence, line_indexes* indexes, countLines* counting, char* error_found){
    /*if the operand is valid. if so, checks if it fits the operator*/
    if (is_operand_valid(sentence, indexes, DEST_OPERAND) == TRUE){
        if (*(sentence->line + indexes->second_operand_index) == '#'){
            /*if the operand is a num, checks the num size*/
            check_num_size(sentence->line, indexes->second_operand_index + 1, counting, error_found);
        }
        /*opcodes 1,3 can have any operand except jump to label, which is taken care of next*/
        /*opcodes 0,2-5,12 can not be a num*/
        if (sentence->code_parts.opcode == 0 ||
        (sentence->code_parts.opcode >= 2 && sentence->code_parts.opcode <= 5) ||
        sentence->code_parts.opcode == 12){
            /*checks if num*/
            if (*(sentence->line + indexes->second_operand_index) =='#'){
                print_error(sentence->line, NO_DEST_NUM_ALLOWED, counting, indexes->second_operand_index);
                *error_found = TRUE;
            }
        }
        /*opcode 9 can not be a num or a register*/
        if (sentence->code_parts.opcode == 9){

            if (*(sentence->line + indexes->second_operand_index) == '#'){
                print_error(sentence->line, NO_DEST_NUM_ALLOWED, counting, indexes->second_operand_index);
                *error_found = TRUE;
            }
            if (indexes->second_operand_index == indexes->first_reg_index ||
                    indexes->second_operand_index == indexes->second_reg_index){
                print_error(sentence->line, NO_DEST_REG_ALLOWED, counting, indexes->second_operand_index);
                *error_found = TRUE;
            }
        }else{
            if (*(sentence->line + indexes->second_operand_index) == '&'){
                print_error(sentence->line, NO_DEST_JUMP_LABEL_ALLOWED, counting, indexes->second_operand_index);
                *error_found = TRUE;
            }
        }
    } else{
        print_error(sentence->line, NOT_AN_OPERAND , counting, indexes->second_operand_index);
        *error_found = TRUE;
    }
}
/***************************************************************************************************/
/*
 * returns TRUE if operand is valid.

 */
char is_operand_valid(line* sentence, line_indexes* indexes, char source_or_dest){
    char is_valid = TRUE;/*to return*/
    char* i;/*gets the line from the second char of the operand, used to continue to check the operand's body*/
    i = check_operand_first_char_syntax(sentence, indexes, &is_valid, source_or_dest);
    check_operand_body_syntax(i, &is_valid);
    return is_valid;
}
/***************************************************************************************************/
/*
 * checks the syntax of the first char of the operand, sets the "is_valid" flag that is given as an argument.
 */
char* check_operand_first_char_syntax(line* sentence, line_indexes* indexes, char* is_valid, char source_or_dest){
    /*checks the first according to the operand*/
    if (source_or_dest == SOURCE_OPERAND){
        if (ILLEGAL_SRC_OPERAND_FIRST_CHAR_CONDITION){/*check source operand's first char*/
            *is_valid = FALSE;
        }
        return sentence->line + indexes->first_operand_index+1;
    }
    if (source_or_dest == DEST_OPERAND) {
        if (ILLEGAL_DEST_OPERAND_FIRST_CHAR_CONDITION){/*check destination operand's first char*/
            *is_valid = FALSE;
        }
        return sentence->line + indexes->second_operand_index+1;
    }
    return "";/*in case "source_or_dest" argument wasn't supplied correctly*/
}
/***************************************************************************************************/
/*
 * checks the syntax of the operand's body. sets the "is_valid" flag that is given as an argument.
 */
void check_operand_body_syntax(char* i, char* is_valid){
    while (*i && *i != ' ' && *i != '\t' && *i != ','){
        if (ILLEGAL_OPERAND_BODY_CONDITION){
            *is_valid = FALSE;
        }
        /*if the + or - is not after a # the operand not valid*/
        if ((*i == '-' || *i == '+') && *(i-1) != '#'){
            *is_valid = FALSE;
        }
        i++;
    }
}
/***************************************************************************************************/
/*
 * checks the char that come before the operator. 
 */
void check_pre_operator_chars(line* sentence, line_indexes* indexes, countLines* counting, char* error_found){
    int i;
    /*if there's a label definition the  index starts after the label definition, else, starts from the first char of the line*/
    if (*(sentence->label.name) != '\0' && indexes->colon_index < indexes->operator_index){
        i = indexes->colon_index+1;
    } else{
        i = indexes->first_char_index;
    }
    while (i < indexes->operator_index){
        /*if the current char is not a white char - reports an error*/
        if (*(sentence->line + i) != ' ' && *(sentence->line + i) != '\t'){
            print_error(sentence->line, ILLEGAL_CODE_BEFORE_OPERATOR, counting, i);
            *error_found = TRUE;
        }
        i++;
    }
}
/***************************************************************************************************/

/*
 * checks labels in a line. requires label's start index if errors found, turns "error_found" to TRUE, else FALSE
 */

void check_label(line* sentence, line_indexes* indexes, countLines* counting, int start_index, char* error_found){
    check_label_length(sentence, indexes, counting, start_index, error_found);
    check_label_syntax(sentence, indexes, counting, start_index, error_found);
}
/***************************************************************************************************/
/*
 * chacks if the length of the label is legal (1-31).
 */
void check_label_length(line* sentence, line_indexes* indexes, countLines* counting, int start_index, char* error_found){
    int label_length;
    int label_index;

    /*if that's a label definition, the length is the colon (':') index minus the first char index, and the label index is the
     * first char index*/

    if (*(sentence->label.name) != '\0' && start_index < indexes->colon_index) 
    {
        label_length = indexes->colon_index - indexes->first_char_index;
        label_index = indexes->first_char_index;
    } else{
        char* i = sentence->line + indexes->data_index;

        while (*i != ' ' && *i != '\t' && *i){
            i++;
        }
        /*calculates the length with the address of the last char of the label instead of incrementing each iteration*/
        label_length = i - sentence->line - indexes->data_index;
        label_index = indexes->data_index;
    }
    if (label_length > LABEL_MAX_LENGTH-1){
        print_error(sentence->line, LABEL_TOO_LONG, counting, label_index);
        *error_found = TRUE;
    }
}
/***************************************************************************************************/
/*
 * checks a label syntax, requires label start index. 
 */
void check_label_syntax(line* sentence, line_indexes* indexes, countLines* counting, int start_index, char* error_found){
    char curr_char = *(sentence->line + start_index);
    char* label_copy = NULL;
    int i = start_index;
    check_label_first_char(sentence, counting, curr_char, i, error_found);
    label_copy = check_label_body(sentence, counting, curr_char, &i, error_found);
    /*if it's a label definition*/
    if (*(sentence->label.name) != '\0' && start_index < indexes->colon_index) {
        /*if it's a label definition checks if there are white chars between the label and the colon (':')*/
        check_label_def_white_chars(sentence, counting, curr_char, i, error_found);
        /*a label definition has already a label copy*/
        check_if_label_reserved_word(sentence->label.name, counting, error_found);
    } else{/*if it's not a label definition*/
        check_if_label_reserved_word(label_copy, counting, error_found);
    }
    if (label_copy != NULL){
        free(label_copy);
    }
}

/***************************************************************************************************/
/*
 * checks the syntax of the label's body, returns a copy of the label. used by "check_label_syntax" function.
 * if errors found, turns "error_found" to TRUE, else FALSE
 */
char* check_label_body(line* sentence, countLines* counting, char curr_char, int* i, char* error_found){
    int label_length = 0;
    char* label_copy = allocate_memory(LABEL_MAX_LENGTH, CHAR);/*to return*/
    *label_copy = '\0';
    while (curr_char != ' ' && curr_char != '\t' && curr_char && curr_char != ':'){/*runs through the label*/
        if (ILLEGAL_LABEL_CHAR_CONDITION){/*reports illegal label syntax*/
            print_error(sentence->line, ILLEGAL_LABEL_SYNTAX, counting, *i);
            *error_found = TRUE;
        }
        /*inserts "curr_char" to "label_copy" that needs to be returned. limits the label length to LABEL_MAX_LENGTH*/
        if (label_length < LABEL_MAX_LENGTH - 1){
            *(label_copy + label_length) = curr_char;
        }
        (*i)++;
        curr_char = *(sentence->line + *i);
        label_length++;
    }
    /*inserts \'0' at the end of label_copy*/
    if (label_length < LABEL_MAX_LENGTH) {
        *(label_copy + label_length) = '\0';
    }else{
        *(label_copy + LABEL_MAX_LENGTH - 1) = '\0';
    }
    return label_copy;
}
/***************************************************************************************************/
/*
 * checks the syntax of the label's first char. used by "check_label_syntax" function. 
 */
void check_label_first_char(line* sentence, countLines* counting, char curr_char, int index, char* error_found){
    /*if the first char is a num reports an error*/
    if (curr_char >= '0' && curr_char <= '9'){
        print_error(sentence->line, LABEL_STARTS_WITH_NUM, counting, index);
        *error_found = TRUE;
        /*if the first char is not an alphabetical character reports an error*/
    }else if (ILLEGAL_LABEL_FIRST_CHAR_CONDITION){
        print_error(sentence->line, LABEL_DOESNT_START_WITH_LETTER, counting, index);
        *error_found = TRUE;
    }
}
/***************************************************************************************************/
/*
 * checks if the label is a reserved word (it's illegal).
 */
void check_if_label_reserved_word(char* label, countLines* counting, char* error_found){
    /*reserved word list*/
    char* reserved_words[NUM_OF_RESERVED_WORDS][1] = {{"mov"},{"cmp"},{"add"},{"sub"},{"lea"},{"clr"},{"not"},
                                                         {"inc"},{"dec"},{"jmp"},{"bne"},{"jsr"},{"red"},{"prn"},{"rts"},
                                                         {".data"},{".string"},{".extern"},{".entry"},{"r0"},{"r1"},{"r2"},{"r3"},{"r4"},{"r5"},{"r6"},{"r7"}};
    int i = 0;

    while (i < NUM_OF_RESERVED_WORDS){
        if (!strcmp(label, reserved_words[i][0])){
            print_error(label, LABEL_IS_RESERVED, counting);
            *error_found = TRUE;
        }
        i++;
    }
}
/***************************************************************************************************/
/*
 * checks if the curr_char is a white char
 */
void check_label_def_white_chars(line* sentence, countLines* counting, char curr_char, int index, char* error_found){
    if (curr_char == ' ' || curr_char == '\t') {
        print_error(sentence->line, SPACE_IN_LABEL, counting, index);
        *error_found = TRUE;
    }
}
/***************************************************************************************************/
/*
 * checks if the "curr_char" is a num, if so, turns "did_num_appeared" char to TRUE. used by "check_data_values" function
 */

void check_num_show(char* did_num_appeared, char curr_char) {
    if (*did_num_appeared == 0) {
        if (curr_char >= '0' && curr_char <= '9') {
            *did_num_appeared = TRUE;
        }
    }
}
