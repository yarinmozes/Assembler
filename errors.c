#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "errors.h"
#include "assembler.h"
/***************************************************************************************************/
/*
 * * Checks a line for all types of errors. 
 */
char errors_check(line* sentence, line_indexes* indexes, countLines* counters){
    char error_found = FALSE;/*flag to return*/
    if (*(sentence->label.name) != '\0' && sentence->label.external == FALSE)
    {
        check_label(sentence, indexes, counters, indexes->first_char_index, &error_found);
    }
    /*if a label was declared with ":" (not within a string) and no label name was supplied*/
    if (*(sentence->label.name) == '\0' && indexes->colon_index != NOT_FOUND &&
            ((indexes->first_quotation_mark_index != NOT_FOUND && indexes->colon_index < indexes->first_quotation_mark_index) ||
            indexes->first_quotation_mark_index == NOT_FOUND)){
        print_error(sentence->line, MISSING_LABEL, counters, indexes->colon_index);
        error_found = TRUE;
    }

    /*if the line is instruction an order at once*/
    if (sentence->flags.is_data == TRUE && sentence->flags.is_code == TRUE)
    {
        print_error(sentence->line, MIXED_SENTENCE, counters);
        error_found = TRUE;
    } else if (sentence->flags.is_data == TRUE){/*if the line is an order line*/
        check_order_line(sentence, indexes, counters, &error_found);
    } else if(sentence->flags.is_code == TRUE){/*if the line is an instruction line*/
        check_code_line(sentence, indexes, counters, &error_found);
    } if (sentence->flags.is_data == FALSE && sentence->flags.is_code == FALSE){/*If the line is neither instructions nor order at once*/
        print_error(sentence->line, NO_SENTENCE_TYPE, counters);
        error_found = TRUE;
    }
    return error_found;
}
/***************************************************************************************************/
/*
 * prints an error to stdout 
 */
void print_error(char* line, char error_code, countLines* counters, ...){
    int unexpected;
    short int print_char_indication = FALSE;
    counters->error_num++;
    if (counters->error_num == 1){
        printf("============================================================================================\n"
               "ERRORS LIST:\n\n");
    }
    printf("%d) ", counters->error_num);
    printf("ERROR: ");
    switch (error_code) {
        case UNEXPECTED_CHARACTER:{
            int unexpected_index;
            va_list argp;
            va_start(argp, counters);
            unexpected = va_arg(argp, int);
            unexpected_index = va_arg(argp, int);
            va_end(argp);
            printf("%c is an unexpected character. line %d.\n", unexpected, counters->line_num);
            print_visual_indication(unexpected_index, line);
            return;
        }
        case NO_SOURCE_REG_ALLOWED:{
            print_char_indication = TRUE;
            printf("The source operand for this operator can not be a register. line %d.\n", counters->line_num);
            break;
        }

        case NO_DEST_REG_ALLOWED:{
            print_char_indication = TRUE;
            printf("The destination operand for this operator can not be a register. line %d.\n", counters->line_num);
            break;
        }

        case EXTRA_OPERATORS:{
            printf("Too much operator in line %d.\n", counters->line_num);
            break;
        }

        case ZERO_OPERANDS:{
            printf("there is no operands found in line %d.\n", counters->line_num);
            break;
        }

        case EXTRA_OPERANDS:{
            printf("Too much operands or components found in this line, please check operator's requirement. line %d.\n", counters->line_num);
            break;
        }
        case FEW_OPERANDS:{
            printf("Missing operands found in line,please check operator's requirement. line %d.\n", counters->line_num);
            break;
        }

        case NO_SOURCE_JUMP_LABEL_ALLOWED:{
            print_char_indication = TRUE;
            printf("The source operand can not be a \"jump to label\" operand. line %d.\n", counters->line_num);
            break;
        }

        case NO_DEST_JUMP_LABEL_ALLOWED:{
            print_char_indication = TRUE;
            printf("The destination operand for this operator can not be a \"jump to label\" operand. line %d.\n", counters->line_num);
            break;
        }

        case NO_DEST_NUM_ALLOWED:{
            print_char_indication = TRUE;
            printf("The destination operand for this operator can not be a num. line %d.\n", counters->line_num);
            break;
        }

        case NO_SOURCE_NUM_ALLOWED:{
            print_char_indication = TRUE;
            printf("The source operand for this operator can not be a num. line %d.\n", counters->line_num);
            break;
        }

        case NOT_AN_OPERAND:{
            print_char_indication = TRUE;
            printf("an illegal operand found in line %d.\n", counters->line_num);
            break;
        }

        case ILLEGAL_CODE_BEFORE_OPERATOR:{
            print_char_indication = TRUE;
            printf("Illegal code before the operator. line %d.\n", counters->line_num);
            break;
        }

        case NO_COMMAS_ALLOWED:{
        printf("A comma in an wrong place. line %d.\n", counters->line_num);
        break;
        }

        case EXTRA_COMMAS:{
            printf("Too much commas, only single comma is allowed here. line %d.\n", counters->line_num);
            break;
        }

        case COMMA_NOT_IN_PLACE:{
            printf("Comma is not here,commas needs to spacing between operands. line %d.\n", counters->line_num);
            break;
        }

        case COMMA_REQUIRED:{
            printf("A comma required to spacing between operands. line %d.\n", counters->line_num);
            break;
        }

        case MIXED_SENTENCE:{
            printf("Mixed code and data order in line %d.\n", counters->line_num);
            break;
        }

        case NO_SENTENCE_TYPE:{
            printf("The Sentence is not code or data order in line %d. check the syntax.\n", counters->line_num);
            break;
        }

        case SINGLE_QUOTMARK:{
            print_char_indication = TRUE;
            printf("Opening quote mark without closing it in line %d.\n", counters->line_num);
            break;
        }

        case STR_NO_ORDER:{
            printf("String without an string order before in line %d.\n", counters->line_num);
            break;
        }

        case CODE_AFTER_QUOTE:{
            print_char_indication = TRUE;
            printf("The Code was found after a string in line %d.\n", counters->line_num);
            break;
        }

        case ORDER_NO_STR:{
            printf("string order without a string afterwards. line %d.\n", counters->line_num);
            break;
        }

        case CHARS_BETWEEN_ORDER_STR:{
            print_char_indication = TRUE;
            printf("An unexpected code between the string order and the string after. line %d.\n", counters->line_num);
            break;
        }

        case NO_SPACE_BEFORE_QUOTE:{
            print_char_indication = TRUE;
            printf("A string needs to be written after a space or spaces. line %d.\n", counters->line_num);
            break;
        }

        case CHARS_BEFORE_STR_ORDER:{
            print_char_indication = TRUE;
            printf("Unexpected code before the .string order in line %d.\n", counters->line_num);
            break;
        }

        case DATA_NO_ORDER:{
            printf("The data supplied without .data order in line %d.\n", counters->line_num);
            break;
        }

        case ORDER_NO_DATA:{
            printf("The Data order supplied without data in line %d.\n", counters->line_num);
            break;
        }

        case CHARS_BEFORE_DATA_ORDER:{
            print_char_indication = TRUE;
            printf("An unexpected code before .data order in line %d.\n", counters->line_num);
            break;
        }

        case NO_COMMA_BETWEEN:{
            print_char_indication = TRUE;
            printf("There is no comma is placed between nums in line %d.\n", counters->line_num);
            break;
        }

        case COMMA_NO_NUMS:{
            print_char_indication = TRUE;
            printf("The Data can not begin with ',' without a num before. line %d.\n", counters->line_num);
            break;
        }

        case COMMA_NO_FOLLOWING_NUM:{
            print_char_indication = TRUE;
            printf("No num was found after ','. line %d.\n", counters->line_num);
            break;
        }

        case NO_NUMS_BETWEEN_COMMAS:{
            print_char_indication = TRUE;
            printf("No nums between two commas in line %d.\n", counters->line_num);
            break;
        }

        case ARITHMETIC_SIGN_NOT_IN_PLACE:{
            print_char_indication = TRUE;
            printf("Arithmetic sign can only be after ',' or spaces. line %d.\n", counters->line_num);
            break;
        }

        case SIGN_NOT_BY_NUM:{
            print_char_indication = TRUE;
            printf("Arithmetic sign is not by a num. line %d.\n", counters->line_num);
            break;
        }

        case NO_SUCH_ORDER:{
            print_char_indication = TRUE;
            printf("An illegal order in line %d.\n", counters->line_num);
            break;
        }

        case STR_BEFORE_STR_ORDER:{
            print_char_indication = TRUE;
            printf("String before string order. line %d.\n", counters->line_num);
            break;
        }

        case CHARS_BEFORE_EXTERN_OR_ENTRY:{
            print_char_indication = TRUE;
            printf("An unexpected code before .extern or .entry order in line %d.\n", counters->line_num);
            break;
        }

        case EXTERN_ENTRY_NO_LABEL:{
            print_char_indication = TRUE;
            printf(".extern or .entry order but no label was found. line %d.\n", counters->line_num);
            break;
        }

        case LABEL_TOO_LONG:{
            print_char_indication = TRUE;
            printf("The LABEL is too big. _label should be in length of 1-31. line %d.\n", counters->line_num);
            break;
        }

        case MISSING_LABEL:{
            print_char_indication = TRUE;
            printf(" LABEL is MISSING. _label should be in length of 1-31_. line %d.\n", counters->line_num);
            break;
        }

        case LABEL_STARTS_WITH_NUM:{
            print_char_indication = TRUE;
            printf("Label can begin only with a non num character. line %d.\n", counters->line_num);
            break;
        }

        case LABEL_NOT_BY_COLON:{
            print_char_indication = TRUE;
            printf("Label is not by the colon. line %d.\n", counters->line_num);
            break;
        }

        case SPACE_IN_LABEL:{
            print_char_indication = TRUE;
            printf("white chars are not allowed in a label or before a colon. line %d.\n", counters->line_num);
            break;
        }

        case LABEL_DOESNT_START_WITH_LETTER:{
            print_char_indication = TRUE;
            printf("Label can begin only with a letter. line %d.\n", counters->line_num);
            break;
        }

        case ILLEGAL_LABEL_SYNTAX:{
            print_char_indication = TRUE;
            printf("An illegal character in label. line %d.\n", counters->line_num);
            break;
        }

        case LABEL_IS_RESERVED:{
            printf("Labels can not be a reserved word. line %d.\n", counters->line_num);
            break;
        }

        case SECOND_LABEL_DEFINITION:{
            printf("Labels can not be defined more than once. line %d.\n", counters->line_num);
            break;
        }

        case ENTRY_NOT_EXIST:{
            printf("Entry order was given, but the label supplied is not define. line %d.\n", counters->line_num);
            break;
        }

        case NUM_TOO_BIG:{
            print_char_indication = 1;
            printf("The NUM is too big. line %d.\n", counters->line_num);
            break;
        }

        case LABEL_DOESNT_EXIST:{
            printf("Label doesn't exist.\n");
            break;
        }

        default:
            return;
    }

    PRINT_ERROR_DESCRIPTION
}

