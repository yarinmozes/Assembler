#ifndef ASSEMBLER_LINE_ANALYZER_H
#define ASSEMBLER_LINE_ANALYZER_H
#include "data.h"



#define NUM_OF_OPERATORS 15

/*****************************************************************************************************************************************************/

#define REG_CONDITION ((index+1) < sentence->length && *(sentence->line+index+1) >= '0' && *(sentence->line+index+1) <= '7') &&\
(((index > 0 && ((*(sentence->line+index-1) == ' ' || *(sentence->line+index-1) == '\t' || *(sentence->line+index-1) == ',')))\
||(index == 0)) && counting->num_of_quotation_marks == 0)


/*****************************************************************************************************************************************************/


#define COMMENT_CONDITION indexes->semicolon_index == indexes->first_char_index && indexes->semicolon_index >= 0

/*****************************************************************************************************************************************************/

#define COLON_CASE if (!(*colon_found)) {\
indexes->colon_index = index;\
*colon_found = 1;\
find_label(sentence, *indexes);}\
counting->num_of_colons++;\
break;

/*****************************************************************************************************************************************************/

#define SEMICOLON_CASE if (!(*semicolon_found)) {\
indexes->semicolon_index = index;\
if(index == 0 || (first_char_found == 1 && indexes->first_char_index == index)){return;}\
*semicolon_found = 1;}\
break;

/*****************************************************************************************************************************************************/

#define DOT_CASE  if (!(*dot_found)) {\
indexes->dot_index = index;\
*dot_found = 1;\
find_data_order(sentence, indexes->dot_index);}\
counting->num_of_dots++;\
break;

/*****************************************************************************************************************************************************/

#define QUOT_MARK_CASE if (!counting->num_of_quotation_marks) {\
indexes->first_quotation_mark_index = index;}else{\
indexes->second_quotation_mark_index = index;\
} counting->num_of_quotation_marks++;\
break;

/*****************************************************************************************************************************************************/

#define REG_CASE if (REG_CONDITION) {counting->num_of_regs++;\
if (counting->num_of_regs <= 2) {\
if (counting->num_of_regs == 1) {\
indexes->first_reg_index = index;\
} else if (counting->num_of_regs == 2) {\
indexes->second_reg_index = index;}}\
}break;
/*****************************************************************************************************************************************************/

#define OPERATOR_CONDITION (i == 0 || *(sentence->line+i-1) == ' ' || *(sentence->line+i-1) == '\t')\
&& (i == sentence->length-1 || *(sentence->line+i+3) == ' ' || *(sentence->line+i+3) == '\t')\
&& counting->num_of_quotation_marks == 0
#define STOP_OPERATOR_CONDITION ((i == 0 || *(sentence->line+i-1) == ' ' || *(sentence->line+i-1) == '\t')\
&& (i == sentence->length-1 || *(sentence->line+i+4) == ' ' || *(sentence->line+i+4) == '\t' || *(sentence->line+i+4) == '\0')\
&& counting->num_of_quotation_marks == 0)




/*****************************************************************************************************************************************************/

/**
 * analyzes a sentence and stores the components, flags, indexes, counting, etc... in the arguments
 */
void analyze_sentence(line* sentence, line_indexes* indexes, countLines* counting);
/**
 * checks if the line is a comment or an argument. stores the results in "sentence"
 */
void empty_or_comment_line_check (line* sentence, line_indexes* indexes);
/*****************************************************************************************************************************************************/
/**
 * defines if a sentence is code or data (instruction/order). stores the results in "sentence"
 */
void define_sentence_type(line* sentence, countLines counting, line_indexes indexes);
/*****************************************************************************************************************************************************/
/**
 * finds and store line components. counts, indexes, etc...
 */
void find_line_components(line* sentence, line_indexes* indexes, countLines* counting);
/*****************************************************************************************************************************************************/
/**
 * sets label's external and sentence type flags.
 */
void prepare_label(line* sentence, int data_index);
/*****************************************************************************************************************************************************/
/**
 * finds label declaration and stores its name in the "sentence" variable. 
 */
void find_label(line* sentence, line_indexes indexes);
/*****************************************************************************************************************************************************/
/**
 * gets a string, it it's a legal operator returns TRUE, else FALSE.
 */
char recognize_operator(char* operator, int* opcode, int* function);
/*****************************************************************************************************************************************************/
/**
 * gets an index and enters the next chars of the code line to the operator_name variables in "sentence" variable.
 */
void check_for_operator(countLines* counting, line_indexes* indexes, line* sentence, int i);
/*****************************************************************************************************************************************************/
/**
 * initiates an operator recognition check, gets the index to check for operator from.
 * if the index >= line length - 2, does nothing
 */
void operator_check(line* sentence, countLines* counting, line_indexes* indexes, int index);
/*****************************************************************************************************************************************************/
/**
 * checks if a char is a sign or 'r' (for regs) and updates relevant flags
 */
void signs_check(line* sentence, countLines* counting, line_indexes* indexes, char curr_char, char* colon_found,char* semicolon_found, char* dot_found, char first_char_found, int index);
/*****************************************************************************************************************************************************/
/**
 * checks if data was found and if order was ended in an order line and updates the flags that are give as arguments.
 */
void data_check(line_indexes* indexes, char* order_ended, char* data_found, char dot_found, char curr_char, int index);
/*****************************************************************************************************************************************************/
/**
 *checks if the char in the current index is an operand and updates the operands count and indexes.
 */
void operands_check(line* sentence, countLines* counting, line_indexes* indexes, char* operand_ended, char curr_char, int index);
/*****************************************************************************************************************************************************/
/**
 * checks for the first char in the line.
 */
void first_char_check(line_indexes* indexes, char* first_char_found, char curr_char, int index);
/*****************************************************************************************************************************************************/
/**
 * given a dot index, gets the order after the dot and enters it to the "order" variable in "sentence".
 */
void find_data_order(line* sentence, int dot_index);
/*****************************************************************************************************************************************************/
/**
 * checks if an order exists in the current code line. 
 */
char is_order(countLines counting, line_indexes indexes);
/*****************************************************************************************************************************************************/
#endif 
