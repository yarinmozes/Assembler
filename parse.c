#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"
#include "assembler.h"
#include "symbol.h"
#include "errors.h"
/****************************************************************************************************************************************************************/
/*
 * parsing for the first pass. parse the source file to a semi ready machine code.
 */
void first_pass_parse(FILE* machine_code, line* sentence, line_indexes* indexes, countLines* counters, data_image* data)
{
    if (sentence->flags.is_code == TRUE)
    {
        code_parsing(machine_code, sentence, indexes, counters);
    } else if (*(sentence->data_parts.order) == 's' || *(sentence->data_parts.order) == 'd'){
        data_parsing(sentence, indexes, counters, data);
    }
}
/****************************************************************************************************************************************************************/
/*
 *parses a code line to the machine code file.
 */
void code_parsing(FILE* machine_code, line* sentence, line_indexes* indexes, countLines* counters){
    word instruction_word;/*code the instruction line*/
    word second_word;
    word third_word;
    /*The distance from last IC is used to know how many words needed*/
    int distance_from_last_IC = counters->IC - counters->last_IC;
    make_instruction_word(&instruction_word, sentence, indexes);
    if(distance_from_last_IC == 1)
    {
        print_code_words(machine_code, sentence->line, counters->last_IC, distance_from_last_IC, &instruction_word);
        return;
    }
    if (distance_from_last_IC == 2) 
    {
        make_extra_words(sentence->line, indexes, distance_from_last_IC, &second_word);
        print_code_words(machine_code, sentence->line, counters->last_IC, distance_from_last_IC, &instruction_word, &second_word);
    } else{
        make_extra_words(sentence->line, indexes, distance_from_last_IC, &second_word, &third_word);
        print_code_words(machine_code, sentence->line, counters->last_IC, distance_from_last_IC, &instruction_word, &second_word, &third_word);
    }
}
/****************************************************************************************************************************************************************/
/*
 * prepre instruction line to word in a given struct word
 */
void make_instruction_word(word* to_make, line* sentence, line_indexes* indexes)
    {
    to_make->word = 0;
    to_make->word |= ABSOLUTE;
    set_opcode(to_make, sentence->code_parts.opcode);
    set_function(to_make, sentence->code_parts.function);
    
    if (indexes->first_operand_index == NOT_FOUND && indexes->second_operand_index != NOT_FOUND)
    {
        make_dest_only_instruction(to_make, sentence->line, indexes);
    } else {
        make_full_instruction_word(to_make, indexes, sentence->line);
    }
    to_make->is_jump = FALSE;
    to_make->is_label = FALSE;
}
/****************************************************************************************************************************************************************/
/*
 * prepre a line with only a destination operand
 */
void make_dest_only_instruction(word* to_make, const char* line, line_indexes* indexes)
    {
    if (indexes->second_operand_index == indexes->first_reg_index) 
    {
        set_dest_reg(to_make, line, indexes->first_reg_index);
    }
    set_dest_adress(to_make, line, indexes);
}
/****************************************************************************************************************************************************************/
/*
 * preapre a line with 2 or 0 operands
 */
void make_full_instruction_word(word* to_make, line_indexes* indexes, const char* line){

    if (indexes->first_operand_index == indexes->first_reg_index) 
    {
        set_source_reg(to_make, line, indexes->first_reg_index);
    }


    if (indexes->second_operand_index == indexes->first_reg_index)
    {
        set_dest_reg(to_make, line, indexes->first_reg_index);
    }


    if (indexes->second_operand_index == indexes->second_reg_index) 
    {
        set_dest_reg(to_make, line, indexes->second_reg_index);
    }

    set_source_adress(line, to_make, indexes);
    set_dest_adress(to_make, line, indexes);
}
/****************************************************************************************************************************************************************/

/*
  * make the extra word of an instruction word.
 */
void make_extra_words(const char* line, line_indexes* indexes, int num_of_words, ...)
    {
    va_list arg_pointer;
    va_start(arg_pointer, num_of_words);
    if (num_of_words == 2){
        int index;
        word* second_word = va_arg(arg_pointer, word*);
     

        if(indexes->second_operand_index != NOT_FOUND && indexes->second_operand_index == indexes->first_reg_index)
        {
            index = indexes->first_operand_index;
        } else{
            index = indexes->second_operand_index;
        }
        code_word(second_word, line, index, indexes, CODE);
    }else{
        word* second_word = va_arg(arg_pointer, word*);
        word* third_word = va_arg(arg_pointer, word*);
        code_word(second_word, line, indexes->first_operand_index, indexes, CODE);
        code_word(third_word, line, indexes->second_operand_index, indexes, CODE);
    }
    va_end(arg_pointer);
}

/****************************************************************************************************************************************************************/
/*
 * codes operand or num to word struct
 */
void code_word( word* word, const char* line, int index, line_indexes* indexes, char mode){
    if (mode == CODE)
    {
        code_instruction_word(word, line, index, indexes);
    } else{
        code_num(word, line, index, DATA);
    }
}
/****************************************************************************************************************************************************************/
/*
 * codes one operand to one struct word
 */
void code_instruction_word(word* word, const char* line, int index, line_indexes* indexes){

    word->label_index = NOT_FOUND;
    word->is_label = FALSE;
    word->is_jump = FALSE;

    switch (*(line + index)) {
        case '#':{
            code_num(word, line, index, CODE);
            break;
        }
        case '&':{
            word->is_jump = TRUE;
            word->label_index = index;
            break;
        }
        default:{
            if (index != indexes->first_reg_index && index != indexes->second_reg_index) 
            {
                word->is_label = TRUE;
                word->label_index = index;
            }
        }
    }
}

/****************************************************************************************************************************************************************/

/*
 * parses an order line to the machine code file
 */

void data_parsing(line* sentence, line_indexes* indexes, countLines* counters, data_image* data){
    if (indexes->first_quotation_mark_index != NOT_FOUND){/*if the string exists it parses it*/
        parse_string_sequence(sentence->line + indexes->data_index, counters->last_DC, indexes->second_quotation_mark_index - indexes->data_index, data);
    } else{/*parses data*/
        parse_nums_sequence(sentence->line + indexes->data_index, indexes, counters->last_DC, data);
    }
}
/****************************************************************************************************************************************************************/
/*
 * * parses a sequence of nums
 */
void parse_nums_sequence(const char* nums_sequence, line_indexes* indexes, int last_DC, data_image* data){
    int index = 0;
    int DC_addition = 1;
    while (*(nums_sequence + index)){/*while the string didnt end*/
        data_image* new_data = allocate_memory(ONE_UNIT, DATA_IMAGE);
        new_data->DC = last_DC + DC_addition;/*sets the DC*/
        new_data->next = NULL;
        code_word(&new_data->word, nums_sequence, index, indexes, DATA);
        insert_data_node(data, new_data);
        get_to_next_num(&index, nums_sequence);
        DC_addition++;
    }
}
/****************************************************************************************************************************************************************/
/*
 * parses a sequence of chars
 */
void parse_string_sequence(const char* string_sequence, int last_DC, int second_quotemark_index, data_image* data){
    int index = 1;/*counts the chars to code*/
    data_image* last_data_node;
    while (index != second_quotemark_index){/*while the string hasn't ended*/
        data_image* new_data = allocate_memory(ONE_UNIT, DATA_IMAGE);
        parse_character(new_data, *(string_sequence + index), last_DC, index);
        insert_data_node(data, new_data);
        index++;
    }
    /*inserts a '\0'*/
    last_data_node = allocate_memory(ONE_UNIT, DATA_IMAGE);
    parse_character(last_data_node, 0, last_DC, index);
    insert_data_node(data, last_data_node);
}
/****************************************************************************************************************************************************************/
/*
 * parses a character to word
 */
void parse_character(data_image* new_data, char character, int last_DC, int index) {
    new_data->DC = last_DC + index;
    new_data->next = NULL;
    new_data->word.word = character;
}
/****************************************************************************************************************************************************************/
/*
 * returns the num in a string that starts in the index
 */
char* get_num(const char* line, int index){
    char* num_str = allocate_memory(NUMBER_ALLOWED_LENGTH, CHAR);
    int i = index;
    int r = 0;
    char curr_char = *(line + i);
    while (curr_char && curr_char != ' ' && curr_char != '\t' && curr_char != ','){
        *(num_str + r) = curr_char;
        r++;
        i++;
        curr_char = *(line + i);
    }
    *(num_str + r) = '\0';
    return num_str;
}
/****************************************************************************************************************************************************************/
/*
 * gets to the next num in a string and stores the index in the "index" arg
 */
void get_to_next_num(int* index, const char* line){
    char curr_char = *(line + *index);
    /*skips current num*/
    while (curr_char && curr_char != ' ' && curr_char != '\t' && curr_char != ','){
        (*index)++;
        curr_char = *(line + *index);
    }
    /*skips white chars and commas, to the next num*/
    while (curr_char && (curr_char == ' ' || curr_char == '\t' || curr_char == ',')){
        (*index)++;
        curr_char = *(line + *index);
    }
}
/****************************************************************************************************************************************************************/
/*
 * codes one num to a struct word. 
 */

void code_num(word* word, const char* line, int index, char mode){
    char is_minus_sign = FALSE;
    char* to_code;

    if (mode == CODE) 
    {
        index++;

    }

    if (*(line + index) == '-' || *(line + index) == '+')
    {
        if (*(line + index) == '-') {
            is_minus_sign = TRUE;
        }
        index++;
    }
    to_code = get_num(line, index);
    word->word = strtod(to_code, NULL);

    if (is_minus_sign == TRUE)
    {
        word->word ^= ONES_COMP_MASK;
        word->word += COMPLEMENT_TO_TWO;
    }

    if (mode == CODE) 
    {
        word->word <<= 3u;
        word->word |= ABSOLUTE;
    }
    free(to_code);
    to_code = NULL;
}
/****************************************************************************************************************************************************************/
/*
 * inserts the data image node to the data image.
 */
void insert_data_node(data_image* head, data_image* to_insert){
    data_image* curr_pointer = head;
    if (head->is_head_filled == FALSE)
    {
        head->word = to_insert->word;
        head->DC = to_insert->DC;
        head->is_head_filled = TRUE;
        free(to_insert);
        to_insert = NULL;
        return;
    }
    while (curr_pointer->next != NULL){
        curr_pointer = curr_pointer->next;
    }
    curr_pointer->next = to_insert;
}

/****************************************************************************************************************************************************************/

/*
 * sets the opcode in a given struct words bitfield according to the given opcode
 */
void set_opcode(word* word, int opcode){
    switch (opcode) {
        case 1:{
            word->word |= OPCODE_ONE;
            break;
        }
        case 2:{
            word->word |= OPCODE_TWO;
            break;
        }
        case 4:{
            word->word |= OPCODE_FOUR;
            break;
        }
        case 5:{
            word->word |= OPCODE_FIVE;
            break;
        }
        case 9:{
            word->word |= OPCODE_NINE;
            break;
        }
        case 12:{
            word->word |= OPCODE_TWELVE;
            break;
        }
        case 13:{
            word->word |= OPCODE_THIRTEEN;
            break;
        }
        case 14:{
            word->word |= OPCODE_FOURTEEN;
            break;
        }
        case 15:{
            word->word |= OPCODE_FIFTEEN;
            break;
        }
    }
}
/****************************************************************************************************************************************************************/
/*
 * sets the function in a given struct words bitfield, according to a given function
 */
void set_function(word* word, int function){
    switch (function) {
        case 1:{
            word->word |= FUNC_ONE;
            break;
        }
        case 2:{
            word->word |= FUNC_TWO;
            break;
        }
        case 3:{
            word->word |= FUNC_THREE;
            break;
        }
        case 4:{
            word->word |= FUNC_FOUR;
            break;
        }
    }
}
/****************************************************************************************************************************************************************/
/*
 * returns the destination address code according to the code line and indexes
 */
void set_dest_adress(word* word, const char* line, line_indexes* indexes){
    int adress = get_dest_adress(line, indexes);
    switch (adress) {
        case 1:{
            word->word |= FIRST_DEST_ADDRESS;
            break;
        }
        case 2:{
            word->word |= SECOND_DEST_ADDRESS;
            break;
        }
        case 3:{
            word->word |= THIRD_DEST_ADDRESS;
            break;
        }
    }
}
/****************************************************************************************************************************************************************/
/*
 * sets the source adress in a given struct words bitfield.
 */
void set_source_adress(const char* line, word* word, line_indexes* indexes){
    int adress = get_source_adress(line, indexes);
    switch (adress) {
        case 1:{
            word->word |= FIRST_SOURCE_ADDRESS;
            break;
        }
        case 2:{
            word->word |= SECOND_SOURCE_ADDRESS;
            break;
        }
        case 3:{
            word->word |= THIRD_SOURCE_ADDRESS;
            break;
        }
    }
}
/****************************************************************************************************************************************************************/
/*
 * sets the source register in a given struct words bitfield.
 */
void set_source_reg(word* word, const char* line, int reg_index){
    switch (*(line + reg_index + 1)) {
        case '1':{
            word->word |= SOURCE_REG_ONE;
            break;
        }
        case '2':{
            word->word |= SOURCE_REG_TWO;
            break;
        }
        case '3':{
            word->word |= SOURCE_REG_THREE;
            break;
        }
        case '4':{
            word->word |= SOURCE_REG_FOUR;
            break;
        }
        case '5':{
            word->word |= SOURCE_REG_FIVE;
            break;
        }
        case '6':{
            word->word |= SOURCE_REG_SIX;
            break;
        }
        case '7':{
            word->word |= SOURCE_REG_SEVEN;
            break;
        }
    }
}
/****************************************************************************************************************************************************************/
/*
 * sets the destination register in a given struct words bitfield
 */
void set_dest_reg(word* word, const char* line, int reg_index){
    switch (*(line + reg_index + 1)) {
        case '1':{
            word->word |= DEST_REG_ONE;
            break;
        }
        case '2':{
            word->word |= DEST_REG_TWO;
            break;
        }
        case '3':{
            word->word |= DEST_REG_THREE;
            break;
        }
        case '4':{
            word->word |= DEST_REG_FOUR;
            break;
        }
        case '5':{
            word->word |= DEST_REG_FIVE;
            break;
        }
        case '6':{
            word->word |= DEST_REG_SIX;
            break;
        }
        case '7':{
            word->word |= DEST_REG_SEVEN;
            break;
        }
    }
}
/****************************************************************************************************************************************************************/
/*
 * returns the source address code according to the code line and indexes
 */

int get_source_adress(const char* line, line_indexes* indexes){
    /*no operand or a num*/
    if (indexes->first_operand_index == NOT_FOUND || *(line + indexes->first_operand_index) == '#') {
        return 0;
    } else if (*(line + indexes->first_operand_index) == '&') {
        return 2;
    } else if (indexes->first_operand_index == indexes->first_reg_index){
        return 3;
    } else{/*regular label*/
        return 1;
    }
}
/****************************************************************************************************************************************************************/
/*
 * returns the destination adress code according to the code line and indexes
 */
int get_dest_adress(const char* line, line_indexes* indexes){

    if ((indexes->first_operand_index == NOT_FOUND && indexes->second_operand_index == NOT_FOUND) || *(line + indexes->second_operand_index) == '#') {
        return 0;
    } else if (*(line + indexes->second_operand_index) == '&') {
        return 2;
    } else if (indexes->second_operand_index == indexes->first_reg_index ||
    indexes->second_operand_index == indexes->second_reg_index){
        return 3;
    } else{
        return 1;
    }
}
/****************************************************************************************************************************************************************/
/*
  * calculates the num of words for the line
 */
void calculate_num_of_words(line* sentence, line_indexes indexes, countLines* counters){
    if (sentence->flags.is_code == TRUE)
    {
        calculate_instruction_word(counters);
    }else{
        calculate_order_word(sentence, indexes, counters);
    }
}
/****************************************************************************************************************************************************************/
/*
 * calculates the num of words for an instruction line
 */
void calculate_instruction_word(countLines* counters){
    counters->last_IC = counters->IC;
    counters->IC += counters->num_of_operands - counters->num_of_regs + ONE_WORD;/*one word for the assembly line*/
}
/****************************************************************************************************************************************************************/
/*
 * calculates the num of words for an order line
 */
void calculate_order_word(line* sentence, line_indexes indexes, countLines* counters){

    if (strcmp(sentence->data_parts.order, "extern") != 0 && strcmp(sentence->data_parts.order, "entry") != 0) {
        counters->last_DC = counters->DC;
   
        counters->DC += counters->num_of_quotation_marks >= 2 ?
                        indexes.second_quotation_mark_index - indexes.first_quotation_mark_index :
                        counters->num_of_commas + 1;
    }
}
/****************************************************************************************************************************************************************/
/*
 * codes a "jump to label" (& label) to the machine code.
 */
void code_jump(FILE* machine_code, symbol* symbol_table, char* i, countLines* counters, label_address* label_address, char* curr_address, char* error_found){
    char* symbol_name = get_symbol_name(i);
    symbol* symbol_to_code = get_symbol(symbol_table, symbol_name);
    int distance;
    if (symbol_to_code == NULL) 
    {
        print_error(i, LABEL_DOESNT_EXIST, counters);
        *error_found = TRUE;
    }
    else {

        distance = symbol_to_code->address - ((int) strtod(curr_address, NULL) - ONE_WORD);
        if (distance < 0) 
        {
            label_address->label_address_binary = abs(distance);
            label_address->label_address_binary ^= ONES_COMP_MASK;
            label_address->label_address_binary += COMPLEMENT_TO_TWO;
        }
        label_address->label_address_binary = distance;
        label_address->label_address_binary <<= 3u;
        label_address->label_address_binary |= ABSOLUTE;
        
        fprintf(machine_code, "%06x\n",
                (signed int) label_address->label_address_binary);
    }
    free(symbol_name);
}
/****************************************************************************************************************************************************************/
/*
 * codes a label to the machine code.
 * if a .ext file needs to be removed
 */
void code_label_address(FILE* machine_code, FILE* externals_file, symbol* symbol_table, char* i, countLines* counters, label_address* label_address, char* curr_address, char* error_found, char* is_external){
    char* symbol_name = get_symbol_name(i);/*no white char*/
    symbol *symbol_to_code = get_symbol(symbol_table, symbol_name);
    if (symbol_to_code == NULL) 
    {
        print_error(i, LABEL_DOESNT_EXIST, counters);
        *error_found = TRUE;
    }
    else {

        if (symbol_to_code->external == EXTERN) 
        {
            print_extern(externals_file, symbol_to_code, curr_address);
            *is_external = TRUE;
            label_address->label_address_binary |= EXTERNAL;
        } else {
            label_address->label_address_binary = symbol_to_code->address;
            if (symbol_to_code->sentence_type == DATA) 
            {
                label_address->label_address_binary += counters->IC;
            }

            label_address->label_address_binary <<= 3U;
            label_address->label_address_binary |= RELOCATABLE;
        }
        /*printing*/
        fprintf(machine_code, "%06x\n",
                (signed int) label_address->label_address_binary);
    }
    free(symbol_name);
}
