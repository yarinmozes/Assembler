#ifndef ASSEMBLER_TRANSLATOR_H
#define ASSEMBLER_TRANSLATOR_H
#include "data.h"

#define ONES_COMP_MASK 0xFFFFFFu
#define OPCODE_ONE 0x40000u
#define OPCODE_TWO 0x80000u
#define OPCODE_FOUR 0x100000u
#define OPCODE_FIVE 0x140000u
#define OPCODE_NINE 0x240000u
#define OPCODE_TWELVE 0x300000u
#define OPCODE_THIRTEEN 0x340000u
#define OPCODE_FOURTEEN 0x380000u
#define OPCODE_FIFTEEN 0x3C0000u
#define FUNC_ONE 0x8u
#define FUNC_TWO 0x10u
#define FUNC_THREE 0x18u
#define FUNC_FOUR 0x20u
#define FIRST_DEST_ADDRESS 0x800u
#define SECOND_DEST_ADDRESS 0x1000u
#define THIRD_DEST_ADDRESS 0x1800u
#define FIRST_SOURCE_ADDRESS 0x10000u
#define SECOND_SOURCE_ADDRESS 0x20000u
#define THIRD_SOURCE_ADDRESS 0x30000u
#define SOURCE_REG_ONE 0x2000u
#define SOURCE_REG_TWO 0x4000u
#define SOURCE_REG_THREE 0x6000u
#define SOURCE_REG_FOUR 0x8000u
#define SOURCE_REG_FIVE 0xA000u
#define SOURCE_REG_SIX 0xC000u
#define SOURCE_REG_SEVEN 0xE000u
#define DEST_REG_ONE 0x100u
#define DEST_REG_TWO 0x200u
#define DEST_REG_THREE 0x300u
#define DEST_REG_FOUR 0x400u
#define DEST_REG_FIVE 0x500u
#define DEST_REG_SIX 0x600u
#define DEST_REG_SEVEN 0x700u
#define ABSOLUTE 0x4u
#define RELOCATABLE 0x2u
#define EXTERNAL 0x1u
#define CODE 1
#define DATA 2
#define ONE_WORD 1
#define NUMBER_ALLOWED_LENGTH 8
#define COMPLEMENT_TO_TWO 1
/****************************************************************************************************************************************************************/
/**
 * parsing for the first pass. parse the source file to a semi ready machine code.
 */

void first_pass_parse(FILE* machine_code, line* sentence, line_indexes* indexes, countLines* counters, data_image* data);
/****************************************************************************************************************************************************************/
/**
 *parses a code line to the machine code file.
 */
void code_parsing(FILE* machine_code, line* sentence, line_indexes* indexes, countLines* counters);
/****************************************************************************************************************************************************************/
/**
 * prepre instruction line to word in a given struct word
 */
void make_instruction_word(word* to_make, line* sentence, line_indexes* indexes);
/****************************************************************************************************************************************************************/
/**
 * prepre a line with only a destination operand
 */
/**/void make_dest_only_instruction(word* to_make, const char* line, line_indexes* indexes);
/****************************************************************************************************************************************************************/
/**
 * preapre a line with 2 or 0 operands
 */

/**/void make_full_instruction_word(word* to_make, line_indexes* indexes, const char* line);
/****************************************************************************************************************************************************************/
/**
 * make the extra word of an instruction word.
 */
void make_extra_words(const char* line, line_indexes* indexes, int num_of_words, ...);
/****************************************************************************************************************************************************************/
/**
 * codes operand or num to word struct
 */
void code_word( word* word, const char* line, int index, line_indexes* indexes, char mode);
/****************************************************************************************************************************************************************/
/**
 * codes operand struct word
 */
void code_instruction_word(word* word, const char* line, int index, line_indexes* indexes);
/****************************************************************************************************************************************************************/
/**
 * parses an order line to the machine code file
 */
void data_parsing(line* sentence, line_indexes* indexes, countLines* counters, data_image* data);
/****************************************************************************************************************************************************************/
/**
 * parses a sequence of nums
 */
void parse_nums_sequence(const char* nums_sequence, line_indexes* indexes, int last_DC, data_image* data);
/****************************************************************************************************************************************************************/
/**
 * parses a sequence of chars
 */
void parse_string_sequence(const char* string_sequence, int last_DC, int second_quotemark_index, data_image* data);
/****************************************************************************************************************************************************************/
/**
 * parses a character to word
 */
void parse_character(data_image* new_data, char character, int last_DC, int index);
/****************************************************************************************************************************************************************/
/**
 * returns the num in a string that starts in the index
 */
char* get_num(const char* line, int index);
/****************************************************************************************************************************************************************/
/**
 * gets to the next num in a string and stores the index in the "index" arg.
 */
void get_to_next_num(int* index, const char* line);
/****************************************************************************************************************************************************************/
/**
 * codes one num to a struct word. 
 */
void code_num(word* word, const char* line, int index, char mode);
/****************************************************************************************************************************************************************/
/**
 * inserts the data image node to the data image.
 */
void insert_data_node(data_image* head, data_image* to_insert);
/****************************************************************************************************************************************************************/
/**
 * sets the opcode in a given struct words bitfield according to the given opcode
 */
void set_opcode(word* word, int opcode);
/****************************************************************************************************************************************************************/
/**
 * sets the function in a given struct words bitfield, according to a given function
 */
void set_function(word* word, int function);
/****************************************************************************************************************************************************************/
/**
 * sets the destination address in a given struct words bitfield
 */
void set_dest_adress(word* word, const char* line, line_indexes* indexes);
/****************************************************************************************************************************************************************/
/**
 * sets the source address in a given struct words bitfield
 */
void set_source_adress(const char* line, word* word, line_indexes* indexes);
/****************************************************************************************************************************************************************/
/**
 * sets the source reg in a given struct words bitfield
 */
void set_source_reg(word* word, const char* line, int reg_index);
/****************************************************************************************************************************************************************/
/**
 * sets the destination reg in a given struct words bitfield
 */
void set_dest_reg(word* word, const char* line, int reg_index);
/****************************************************************************************************************************************************************/
/**
 * returns the destination address code according to the code line and indexes
 */
int get_dest_adress(const char* line, line_indexes* indexes);
/****************************************************************************************************************************************************************/
/**
 * returns the source address code according to the code line and indexes
 */
int get_source_adress(const char* line, line_indexes* indexes);
/****************************************************************************************************************************************************************/
/**
 * calculates the num of words for the line
 */
void calculate_num_of_words(line* sentence, line_indexes indexes, countLines* counters);
/****************************************************************************************************************************************************************/
/**
 * calculates the num of words for an instruction line
 */
void calculate_instruction_word(countLines* counters);
/****************************************************************************************************************************************************************/
/**
 * calculates the num of words for an order line
 */
void calculate_order_word(line* sentence, line_indexes indexes, countLines* counters);
/****************************************************************************************************************************************************************/
/**
 * codes a "jump to label" (& label) to the machine code.
 */
void code_jump(FILE* machine_code, symbol* symbol_table, char* i, countLines* counters, label_address* label_address, char* curr_address, char* error_found);
/****************************************************************************************************************************************************************/
/**
 * codes a label to the machine code.
 */
void code_label_address(FILE* machine_code, FILE* externals_file, symbol* symbol_table, char* i, countLines* counters, label_address* label_address, char* curr_address, char* error_found, char* is_external);
#endif 
