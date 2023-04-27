#include <stdlib.h>
#include <string.h>
#include "line_checker.h"

/*
 * analyzes a sentence and stores the components, flags, indexes, counting, etc... in the arguments
 */
void analyze_sentence(line* sentence, line_indexes* indexes, countLines* counting) {
    find_line_components(sentence, indexes, counting);/*fins sentence, indexes and counting*/
    define_sentence_type(sentence, *counting, *indexes);/*code or order*/
    prepare_label(sentence, indexes->data_index);
}
/*****************************************************************************************************************************************************/
/*
 * checks if the line is a comment or an argument. stores the results in "sentence"
 */
void empty_or_comment_line_check (line* sentence, line_indexes* indexes){
    if (indexes->first_char_index == NOT_FOUND) {
        sentence->flags.is_empty_line = TRUE;
    }
    if (COMMENT_CONDITION){
        sentence->flags.is_comment = TRUE;
        return;
    }
}
/*****************************************************************************************************************************************************/
/*
 * defines if a sentence is code or data 
 */
void define_sentence_type(line* sentence, countLines counting, line_indexes indexes) {
    if (counting.num_of_operators > 0) 
    {
        sentence->flags.is_code = TRUE;
    }
    if (is_order(counting, indexes) == TRUE) 
    {
        sentence->flags.is_data = TRUE;
    }
}
/*****************************************************************************************************************************************************/
/*
 * finds and store line components. counts, indexes, etc
 */
void find_line_components(line* sentence, line_indexes* indexes, countLines* counting) {
    char colon_found = FALSE;
    char semicolon_found = FALSE;
    char dot_found = FALSE;
    char first_char_found = FALSE;
    char operand_ended = FALSE;
    char order_ended = FALSE;
    char data_found = FALSE;

    int i = 0;
    char curr_char;
    while (i < sentence->length) {
        curr_char = *(sentence->line + i);
        first_char_check(indexes, &first_char_found, curr_char, i);/*turns on the first_char_found flag*/
        operator_check(sentence, counting, indexes, i);/*finds the operator if it starts from curr_char*/
        operands_check(sentence, counting, indexes, &operand_ended, curr_char, i);/*finds an operand if it starts from curr_char*/
        data_check(indexes, &order_ended, &data_found, dot_found, curr_char, i);/*sets the order_ended, data_found flags*/
        signs_check(sentence, counting, indexes, curr_char, &colon_found, &semicolon_found, &dot_found,first_char_found, i);
        

        if (curr_char == ';' && indexes->first_char_index == i) { return; }
        i++;
    }
    if (indexes->first_operand_index != NOT_FOUND && indexes->second_operand_index == NOT_FOUND) {
        indexes->second_operand_index = indexes->first_operand_index;
        indexes->first_operand_index = NOT_FOUND;
    }
}
/*****************************************************************************************************************************************************/
/*
 * sets label's external and sentence type flags. external label gets the label name.
 * in case of external label, requires the index of the label ("data_index").
 */
void prepare_label(line* sentence, int data_index){
    if (sentence->flags.is_code == TRUE) {
        /*prepares sentence type*/
        sentence->label.external = FALSE;
        sentence->label.sentence_type = CODE;
    }else{
        sentence->label.sentence_type = DATA;
        if (!strcmp(sentence->data_parts.order, "extern")) {
            /*prepares sentence type*/
            sentence->label.external = EXTERN;
            /*prepares label's name*/
            strncpy(sentence->label.name, sentence->line + data_index, LABEL_MAX_LENGTH);
        }
    }
}
/*****************************************************************************************************************************************************/
/*
 * finds label declaration and stores its name in the "sentence" variable. 
 */
void find_label(line* sentence, line_indexes indexes){

    if (indexes.first_quotation_mark_index == NOT_FOUND) {
        int i = 0;
        /*enters label's name to in the "sentence" variable.*/
        while ((i + indexes.first_char_index < indexes.colon_index) && i < LABEL_MAX_LENGTH - 1) {
            *(sentence->label.name + i) = *(sentence->line + (i + indexes.first_char_index));
            i++;
        }
        *(sentence->label.name + i) = '\0';
    }
}
/*****************************************************************************************************************************************************/

/*
 * gets a string, it it's a legal operator returns TRUE, else FALSE.
 *
 */
char recognize_operator(char* operator, int* opcode, int* function){
    int i = 0;

    char* operators_table[NUM_OF_OPERATORS][3] = {{"mov", "0", ""},{"cmp", "1", ""},{"add", "2", "1"},{"sub", "2", "2"},{"lea", "4", ""},
                                                     {"clr", "5", "1"},{"not", "5", "2"},{"inc", "5", "3"},{"dec", "5", "4"},{"jmp", "9", "1"},
                                                     {"bne","9", "2"},{"jsr", "9", "3"},{"red", "12", ""},{"prn", "13", ""},{"rts", "14", ""}
    };
    while (i < NUM_OF_OPERATORS){
        if (!strcmp(operator, operators_table[i][0])){
            *opcode = strtod(operators_table[i][1],NULL);
            *function = strtod(operators_table[i][2],NULL);
            return TRUE;
        }
        i++;
    }
    return FALSE;
}
/*****************************************************************************************************************************************************/
/*
 * gets an index i and enters the next chars of the code line to the operator_name variables in "sentence" variable.
 */
void check_for_operator(countLines* counting, line_indexes* indexes, line* sentence, int i){
    /*enters the next 3 chars to the "operator_name" variable*/
    *sentence->code_parts.operator_name = *(sentence->line + i);
    *(sentence->code_parts.operator_name+1) = *(sentence->line + i + 1);
    *(sentence->code_parts.operator_name+2) = *(sentence->line + i + 2);

    if (i < sentence->length-3){
        *(sentence->code_parts.operator_name+3) = *(sentence->line + i + 3);
        *(sentence->code_parts.operator_name+4) = '\0';
        /*if it's a "stop" operator, updates the relevant variables*/
        if (!strcmp(sentence->code_parts.operator_name,"stop") && STOP_OPERATOR_CONDITION){
            sentence->code_parts.opcode = 15;
            counting->num_of_operators++;
            return;
            /*if it's not a "stop" operator, enters a '\0' instead of the fourth char and checks if it's another operator if so updates the relevant variables*/
             
        } else{
            *(sentence->code_parts.operator_name+3) = '\0';
        }
    }
    if (recognize_operator(sentence->code_parts.operator_name, &sentence->code_parts.opcode, &sentence->code_parts.function) == TRUE
        && OPERATOR_CONDITION){
        counting->num_of_operators++;
        indexes->operator_index = i;
        return;
    }
}
/*****************************************************************************************************************************************************/
/*
 * initiates an operator recognition check.
 */
void operator_check(line* sentence, countLines* counting, line_indexes* indexes, int index){
    if (index < sentence->length - 2){
        check_for_operator(counting, indexes, sentence, index);
    }
}
/*****************************************************************************************************************************************************/

/*checks if a char is a sign or 'r' (for REGs) and updates relevant flags, counting and indexes in the given arguments.*/
void signs_check(line* sentence, countLines* counting, line_indexes* indexes, char curr_char, char* colon_found,
                        char* semicolon_found, char* dot_found, char first_char_found, int index){
    /*updates relevant flags, counting and indexes in the given arg.*/
    switch (curr_char) {
        case ':': {
            COLON_CASE
        }
        case ';': {
            SEMICOLON_CASE
        }
        case '.': {
            DOT_CASE
        }
        case 'r': {
            REG_CASE
        }
        case '\"': {
            QUOT_MARK_CASE
        }
        case ',':{
            counting->num_of_commas++;
            indexes->last_comma_index = index;
        }
    }
}
/*****************************************************************************************************************************************************/
/*
 * checks if data was found and if order was ended in an order line and updates the flags that are give as args.
 */
void data_check(line_indexes* indexes, char* order_ended, char* data_found, char dot_found, char curr_char, int index){
    /*if order ended turns "order_ended" to TRUE*/
    if (!(*order_ended) && dot_found == TRUE && (curr_char == ' ' || curr_char == '\t')){
        *order_ended = TRUE;
    }
    
    if (*order_ended == TRUE && dot_found == TRUE && !(*data_found) && curr_char != ' ' && curr_char != '\t'){
        indexes->data_index = index;
        *data_found = TRUE;
    }
}
/*****************************************************************************************************************************************************/
/*
 * checks if the char in the current index is an operand and updates the operands count and indexes.
 */
void operands_check(line* sentence, countLines* counting, line_indexes* indexes, char* operand_ended, char curr_char, int index){
    if (sentence->code_parts.opcode != NOT_FOUND){/*if an operand can exist*/
        /*checks it the operand ended and stores the result*/
        if (curr_char == ' ' || curr_char == '\t' || curr_char == ',') {
            *operand_ended = TRUE;
        }/*if an operand was found, updates the operands count and indexes. and "operand_ended" turns to FALSE*/
        if (curr_char != ' ' && curr_char != '\t' && curr_char != ',' &&*operand_ended == TRUE){
            counting->num_of_operands++;
            if (counting->num_of_operands == 1) {
                indexes->first_operand_index = index;
            }
            if (counting->num_of_operands == 2){
                indexes->second_operand_index = index;
            }
            *operand_ended = FALSE;
        }
    }
}
/*****************************************************************************************************************************************************/
/*checks for the first char in the line.
 */
void first_char_check(line_indexes* indexes, char* first_char_found, char curr_char, int index){
    /*if a char was found*/
    if (!(*first_char_found) && curr_char != ' ' && curr_char != '\t') {
        indexes->first_char_index = index;
        *first_char_found = TRUE;
    }
}
/*****************************************************************************************************************************************************/

/*
 * checks if an order exists in the current code line. returns TRUE if order was found, else FALSE
 */
char is_order(countLines counting, line_indexes indexes){
    switch (counting.num_of_dots) {/*no dot = no order*/
        case 0:{
            return FALSE;
        }
        default:{
            if (counting.num_of_quotation_marks > 1) {
                if (indexes.first_quotation_mark_index < indexes.dot_index &&
                indexes.second_quotation_mark_index > indexes.dot_index) {
                    return FALSE;
                } else {
                    return TRUE;
                }
            }
            return TRUE;
        }
    }
}
/*****************************************************************************************************************************************************/
/*
 * gets the order after the dot and enters it to the "order" variable in "sentence".
 */
void find_data_order(line* sentence, int dot_index){
    if (dot_index >= 0){

        char* i = sentence->line + dot_index+1;
        int k = 0;

        while (*i != ' ' && *i != '\t' && *i && k < ORDER_MAX_LENGTH-1) {
            *(sentence->data_parts.order + k) = *i;
            k++;
            i++;
        }
        *(sentence->data_parts.order+k) = '\0';
    }
}
