#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef DATA_H 
#define DATA_H


#define COMMENT_SIGN  ';'
#define LINE_SIZE 81
#define LABEL_SIZE  33
#define MEMORY_SIZE 1024
#define SYSTEM_MEMORY 100

#define NUMBER_OF_PARAMS lineData.numberOfParams
#define ERROR lineData.error
#define CMD_NUMBER lineData.cmdNumber
#define COMMAND_TYPE lineData.cmdType
#define COMMAND_LABEL lineData.label
#define COMMAND_NAME lineData.name
#define PARAM1_TYPE lineData.content.command.param1Type
#define PARAM1_NUMBER lineData.content.command.param1.number
#define PARAM1_LABEL lineData.content.command.param1.label
#define PARAM1_REG lineData.content.command.param1.reg
#define PARAM2_TYPE lineData.content.command.param2Type
#define PARAM2_NUMBER lineData.content.command.param2.number
#define PARAM2_LABEL lineData.content.command.param2.label
#define PARAM2_REG lineData.content.command.param2.reg
#define PARAM2_TYPE lineData.content.command.param2Type
#define DATA_TYPE lineData.content.instruction.dataType
#define DATA_PARAM lineData.content.instruction.dataParam.data.data
#define NUMBER_OF_DATAS lineData.content.instruction.dataParam.data.numberOfData
#define STRING_PARAM lineData.content.instruction.dataParam.string
#define ENTEX_PARAM lineData.content.entryExtern.param
#define ENTEX_TYPE lineData.content.entryExtern.entExType


#define LONG_LINE  1
#define ILLEGAL_LABEL  2
#define LONG_LABEL 3
#define MISSING_COMMAND  4
#define ILLEGAL_COMMAND  5
#define MISSING_PARAMETER  6
#define ILLEGAL_NUMBER  7
#define ILLEGAL_REGISTER 8
#define ILLEGAL_LABEL_PARAM  9
#define MISSING_COMMA 10
#define END_ERROR 11
#define ILLEGAL_PARAM  12
#define LABEL_SAVE  13
#define MISSING_POINT  14
#define MISSING_APOSTROPHES  15
#define TO_MANY_PARAMETERS 16

#endif
