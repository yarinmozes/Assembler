
#ifndef ASSEMBLER_ERRORS_H
#define ASSEMBLER_ERRORS_H
#include "data.h"


#define SOURCE_OPERAND 1
#define DEST_OPERAND 2


#define UNEXPECTED_CHARACTER 1
#define NO_SOURCE_REG_ALLOWED 2
#define NO_DEST_REG_ALLOWED 3
#define EXTRA_OPERATORS 4
#define ZERO_OPERANDS 5
#define EXTRA_OPERANDS 6
#define FEW_OPERANDS 7
#define NO_SOURCE_JUMP_LABEL_ALLOWED 8
#define NO_DEST_JUMP_LABEL_ALLOWED 9
#define NO_DEST_NUM_ALLOWED 10
#define NO_SOURCE_NUM_ALLOWED 11
#define NOT_AN_OPERAND 12
#define ILLEGAL_CODE_BEFORE_OPERATOR 13
#define NO_COMMAS_ALLOWED 14
#define EXTRA_COMMAS 15
#define COMMA_NOT_IN_PLACE 16
#define COMMA_REQUIRED 17
#define MIXED_SENTENCE 18
#define NO_SENTENCE_TYPE 19
#define SINGLE_QUOTMARK 20
#define STR_NO_ORDER 21
#define CODE_AFTER_QUOTE 22
#define ORDER_NO_STR 23
#define CHARS_BETWEEN_ORDER_STR 24
#define CHARS_BEFORE_STR_ORDER 25
#define STR_BEFORE_STR_ORDER 26
#define NO_SPACE_BEFORE_QUOTE 27
#define DATA_NO_ORDER 28
#define ORDER_NO_DATA 29
#define COMMA_NO_NUMS 30
#define NO_COMMA_BETWEEN 31
#define NO_NUMS_BETWEEN_COMMAS 32
#define CHARS_BEFORE_DATA_ORDER 33
#define COMMA_NO_FOLLOWING_NUM 34
#define ARITHMETIC_SIGN_NOT_IN_PLACE 35
#define NUM_TOO_BIG 36
#define SIGN_NOT_BY_NUM 37
#define CHARS_BEFORE_EXTERN_OR_ENTRY 38
#define EXTERN_ENTRY_NO_LABEL 39
#define NO_SUCH_ORDER 40
#define LABEL_TOO_LONG 41
#define MISSING_LABEL 42
#define LABEL_STARTS_WITH_NUM 43
#define LABEL_NOT_BY_COLON 44
#define SPACE_IN_LABEL 45
#define LABEL_DOESNT_START_WITH_LETTER 46
#define ILLEGAL_LABEL_SYNTAX 47
#define LABEL_IS_RESERVED 48
#define SECOND_LABEL_DEFINITION 49
#define ENTRY_NOT_EXIST 50
#define LABEL_DOESNT_EXIST 51
#define PRINT_ERROR_DESCRIPTION puts(line);\
if (print_char_indication == 1){\
va_list argp;\
va_start(argp, counters);\
unexpected = va_arg(argp, int);\
va_end(argp);\
printf("Starting at the character ' %c '\n", *(line + unexpected));}\

#define ILLEGAL_SRC_OPERAND_FIRST_CHAR_CONDITION *(sentence->line + indexes->first_operand_index) != '&' &&\
*(sentence->line + indexes->first_operand_index) != '#' &&\
(*(sentence->line + indexes->first_operand_index) < 'A' ||\
(*(sentence->line + indexes->first_operand_index) > 'Z' && *(sentence->line + indexes->first_operand_index) < 'a')||\
*(sentence->line + indexes->first_operand_index) > 'z')

#define ILLEGAL_DEST_OPERAND_FIRST_CHAR_CONDITION *(sentence->line + indexes->second_operand_index) != '&' && \
*(sentence->line + indexes->second_operand_index) != '#' && \
(*(sentence->line + indexes->second_operand_index) < 'A' || \
(*(sentence->line + indexes->second_operand_index) > 'Z' && *(sentence->line + indexes->second_operand_index) < 'a') || \
*(sentence->line + indexes->second_operand_index) > 'z')

#define ILLEGAL_DATA_CHAR_CONDITION curr_char != ' ' && curr_char != '\t' &&\
curr_char != ',' && curr_char != '-' && curr_char != '+'\
&& (curr_char < '0' || curr_char > '9')

#define ILLEGAL_OPERAND_BODY_CONDITION (*i < '0' || (*i > '9' && *i < 'A') || (*i > 'Z' && *i < 'a') || *i > 'z') && *i != '-' && *i != '+'

#define ILLEGAL_LABEL_CHAR_CONDITION curr_char < '0' || (curr_char > '9' && curr_char < 'A') || (curr_char > 'Z' && curr_char < 'a') || curr_char > 'z'

#define ILLEGAL_LABEL_FIRST_CHAR_CONDITION curr_char < 'A' || (curr_char > 'Z' && curr_char < 'a') || curr_char > 'z'

#define NUM_OF_RESERVED_WORDS 27
#define NUM_MAX_VAL 1048575 

/******************************************errors*****************************************************/
/**
 * * Checks a line for all types of errors. Returns TRUE if errors are found, otherwise FALSE
 */
char errors_check(line* sentence, line_indexes* indexes, countLines* counting);


/**
 * prints an error to stdout 
 */
void print_error(char* line, char error_code, countLines* counting, ...);


/**
 * checks if the "curr_char" is a num
 */
void check_num_show(char* did_num_appeared, char curr_char);
/******************************************error_chacking*****************************************************/
/**
 *checks order line, from the order itself to the data.E
 */
void check_order_line(line* sentence, line_indexes* indexes, countLines* counting, char* error_found);
/***************************************************************************************************/
/**
 * checks if the order in the line (.data/.string/.extern/.entry) is valid
 */
char is_order_valid(line* sentence);
/***************************************************************************************************/
/**
 * finds and reports string errors. error flag. 
 */
void find_string_errors(line* sentence, line_indexes* indexes, countLines* counting, char* error_found);
/***************************************************************************************************/
/**
 * finds errors in a line with with a valid string. 
 */
void find_two_quotes_errors(line* sentence, countLines* counting, line_indexes* indexes, char is_string_order, char* error_found);
/***************************************************************************************************/
/**
 *finds .data order line errors.
 */
void find_data_errors(line* sentence, line_indexes* indexes, countLines* counting, char* error_found);
/***************************************************************************************************/
/**
 * checks if data values are valid. 
 */
char is_data_values_valid(line* sentence, line_indexes* indexes, countLines* counting);
/***************************************************************************************************/
/**
 *checks the data values for example (2,3,8)
 */
char check_data_values(line* sentence, int index, countLines* counting);
/***************************************************************************************************/
/**
 * data values check for "check_data_values" function.
 */
void values_check(line* sentence, countLines *counting, char curr_char, int last_char, short int is_after_comma,char is_data_order, char* error_found, short int did_num_appeared, int index);
/***************************************************************************************************/
/**
 * turns on and off "is_after_comma" flag for "check_data_values" function.
 * turns to TRUE if curr_char is ',' and FALSE if curr_char is not a white char.
 */
void check_if_after_comma(const char *curr_char, char* is_after_comma);
/***************************************************************************************************/
/**
 * updates loop variables for "check_data_values" function
 */
void update_loop_data_check_variables(line* sentence, char* curr_char, char* last_char, int* index);
/***************************************************************************************************/
/**
 * checks errors for .entry and .extern lines. 
 */
void find_extern_entry_errors(line* sentence, line_indexes* indexes, countLines* counting, char* error_found);
/***************************************************************************************************/
/**
 *checks for illegal characters before an order in an order line.
 */
void check_pre_order_chars(line* sentence, line_indexes* indexes, countLines* counting, char* error_found);
/***************************************************************************************************/
/**
 * checks and reports if there are characters after a label in a .extern ot .entry line. i
 */
void check_after_data_label_chars(line* sentence, countLines* counting, line_indexes* indexes, char* error_found);
/***************************************************************************************************/
/**
 * checks a label that comes as data after .extern or .entry. 
 */
void check_data_label(line* sentence, line_indexes* indexes, countLines* counting, char* error_found);
/***************************************************************************************************/
/**
 * checks a code line for errors. 
 */
void check_code_line(line* sentence, line_indexes* indexes, countLines* counting, char* error_found);
/***************************************************************************************************/
/**
 * checks if the num of operands is legal for the operator, reports errors. 

 */
char check_operands_count(line* sentence, line_indexes* indexes, countLines* counting, char* error_found);
/***************************************************************************************************/
/**
 * checks if the num of operators is greater than 1.
 */
char check_operators_count(line* sentence, line_indexes* indexes, countLines* counting, char* error_found);
/***************************************************************************************************/
/**
 * checks operands syntax according to the operator and commas conditions,
 */
void check_operands_syntax(line* sentence, line_indexes* indexes, countLines* counting, char* error_found);
/***************************************************************************************************/
/**
 * gets a num in char*, num can be with a sign, and reports an error if num is too large
 */
void check_num_size(char* line, int index, countLines* counting, char* error_found);
/***************************************************************************************************/
/**
 * checks source operand syntax and reports errors. 
 */
void check_source_operand_syntax(line* sentence, line_indexes* indexes, countLines* counting, char* error_found);
/***************************************************************************************************/
/**
 * checks destination operand syntax and reports errors.
 */
void check_dest_operand_syntax(line* sentence, line_indexes* indexes, countLines* counting, char* error_found);
/***************************************************************************************************/
/**
 * returns TRUE if operand is valid, else returns FALSE, requires SOURCE_OPERAND or
 */
char is_operand_valid(line* sentence, line_indexes* indexes, char source_or_dest);
/***************************************************************************************************/
/**
 * checks the char that come before the operator.
 */
void check_pre_operator_chars(line* sentence, line_indexes* indexes, countLines* counting, char* error_found);
/***************************************************************************************************/
/**
 * checks the syntax of the operand's body. sets the "is_valid" flag that is given as an argument.
 */
void check_operand_body_syntax(char* i, char* is_valid);
/***************************************************************************************************/
/**
 * checks the syntax of the first char of the operand, sets the "is_valid" flag that is given as an argument.
 */
char* check_operand_first_char_syntax(line* sentence, line_indexes* indexes, char* is_valid, char source_or_dest);
/***************************************************************************************************/
/**
 * checks labels in a line.
 */
void check_label(line* sentence, line_indexes* indexes, countLines* counting, int start_index, char* error_found);
/***************************************************************************************************/
/**
 * checks if the length of the label is legal (1-31).
 */
void check_label_length(line* sentence, line_indexes* indexes, countLines* counting, int start_index, char* error_found);
/***************************************************************************************************/
/**
 * checks a label syntax, requires label start index.
 */
void check_label_syntax(line* sentence, line_indexes* indexes, countLines* counting, int start_index, char* error_found);
/***************************************************************************************************/
/**
 * checks if the label is a reserved word .
 */
void check_if_label_reserved_word(char* label, countLines* counting, char* error_found);
/***************************************************************************************************/
/**
 * checks if the curr char is a white char, used for checking if a white char is between a label definition and the colon(':')
 */
void check_label_def_white_chars(line* sentence, countLines* counting, char curr_char, int index, char* error_found);
/***************************************************************************************************/
/**\
 * checks the syntax of the label's first char. used by "check_label_syntax" function.
 */
void check_label_first_char(line* sentence, countLines* counting, char curr_char, int index, char* error_found);
/***************************************************************************************************/
/**
 * checks the syntax of the label's body, returns a copy of the label.
 */
char* check_label_body(line* sentence, countLines* counting, char curr_char, int* i, char* error_found);
/***************************************************************************************************/

#endif 
