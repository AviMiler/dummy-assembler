#ifndef CHEKCERS_H
#define CHEKCERS_H 

#include "data.h"
#include "structures.h"



line_data checkLine(char line[LINE_SIZE+1],FILE *,int );/*Gets a line from a file checks for errors and returns the data*/
int checkMacroLabel(char * ,char * ,int ,FILE *,char *);/*Checks macro label errors*/
int checkEndMacro(FILE * ,char * ,char * ,int);/*get a file, checks for macro errors and deploys the Macros*/

#endif
