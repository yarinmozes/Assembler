#ifndef ASSEMBLER_ASSEMBLER_DATA_TYPES_H
#define ASSEMBLER_ASSEMBLER_DATA_TYPES_H

#define LABEL_MAX_LENGTH 32
#define ORDER_MAX_LENGTH 7
#define OPERATOR_MAX_LENGTH 5
#define TRUE 1
#define FALSE 0
#define NOT_FOUND -1
#define EXTERN 1
#define CODE 1
#define DATA 2
/***************************************************************************************************/
typedef struct __attribute__((packed)) symbol_table{
    char name[LABEL_MAX_LENGTH];
    int address;
    char external;/*if external have the value of EXTERN (1)*/
    char sentence_type;
    struct symbol_table* next;
} symbol;

/***************************************************************************************************/
typedef struct __attribute__((packed)) {
    unsigned int label_address_binary: 24;
} label_address;
/***************************************************************************************************/
typedef struct{
    char operator_name[OPERATOR_MAX_LENGTH];
    int opcode;
    int function;
}operator;
/***************************************************************************************************/
typedef struct __attribute__((packed)){
    char order[ORDER_MAX_LENGTH];
}data;
/***************************************************************************************************/
typedef struct __attribute__((packed)){
    char is_comment;
    char is_empty_line;
    char is_code;
    char is_data;
}line_flags;
/***************************************************************************************************/
typedef struct{
    char* line;
    int length;
    symbol label;
    operator code_parts;
    data data_parts;
    line_flags flags;
}line;
/***************************************************************************************************/
typedef struct __attribute__((packed)){
    int first_char_index;
    int colon_index;
    int semicolon_index;
    int dot_index;
    int first_reg_index;
    int second_reg_index;
    int first_quotation_mark_index;
    int second_quotation_mark_index;
    int last_comma_index;
    int operator_index;
    int first_operand_index;
    int second_operand_index;
    int data_index;
}line_indexes;
/***************************************************************************************************/
typedef struct __attribute__((packed)){
    int IC;
    int last_IC;
    int DC;
    int last_DC;
    int num_of_regs;
    int num_of_colons;
    int num_of_quotation_marks;
    int num_of_dots;
    int num_of_operators;
    int line_num;
    int error_num;
    int num_of_operands;
    int num_of_commas;
}countLines;
/***************************************************************************************************/
typedef struct __attribute__((packed)){
    unsigned int word: 24;
    char is_jump;
    char is_label;
    int label_index;
}word;
/***************************************************************************************************/
typedef  struct __attribute__((packed)) data_binary_node{
    int DC;
    word word;
    char is_head_filled;
    struct data_binary_node * next;
}data_image;
/***************************************************************************************************/
/**
 * initializes line variables for next line processing.
 */
 void initialize_line_variables(line* sentence, countLines* counting, line_indexes* indexes);
/***************************************************************************************************/
/**
 * initializes counting for next line processing.
 */
 void initialize_counters(countLines* counting);
/***************************************************************************************************/
/**
 * initializes index for next line processing.
 */
 void initialize_indexes(line_indexes* indexes);
/***************************************************************************************************/
/**
 * initializes struct line for next line processing.
 */
void initialize_line(line* sentence);
/***************************************************************************************************/
/**/
#endif 
