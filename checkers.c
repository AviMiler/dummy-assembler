#include "checkers.h"

#define PRE_ASMBLR_EXIT {printf("#at preassembler: ");errorSwitch(error,lineNumber);return 1;}
#define EXIT errorSwitch(ERROR,lineNumber);return lineData;
#define DELETE_SPACES while (token && (*token==' ' || *token=='\t')) token+=1;
#define CHECK_PARAM if(!token || *token=='\0' || *token==',' || *token=='\n'){ERROR=MISSING_PARAMETER;EXIT};
#define TAKE_PARAM takeParam(token,param);
#define CHECK_NUMBER if((ok=checkNumber(&token))==ILLEGAL_NUMBER){ERROR=ILLEGAL_NUMBER;EXIT}
#define CHECK_REGISTER if(*(token)=='@') {ok=1;if(*(token+1)!='r' || (*(token+2))>'7' || (*(token+2))<'0' ||(!isspace(*(token+3))&&*(token+3)!=','&&*(token+3))) \
                                                {ERROR=ILLEGAL_REGISTER;EXIT}token+=3;}else ok=0;
#define CHECK_LABEL if((i=ok=checkLabelParam(token))==-1 || ok==-2) {ERROR=ILLEGAL_LABEL_PARAM;EXIT} token+=i;
#define CHECK_COMMA if((*token)!=','){ERROR=MISSING_COMMA;EXIT}token++;
#define CHECK_END if(token){ if(*token==',') {ERROR=TO_MANY_PARAMETERS;EXIT}\
	 	            if (!isspace(*token) && (*token)!='\0') {ERROR=END_ERROR;EXIT}}
#define CHECK_APOSTROPHES if((token[i])!='"' || !(token[i])){ERROR=MISSING_APOSTROPHES;EXIT};

char *COMMAND_LIST[] = {"mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","red","prn","jsr","rts","stop",NULL};
char *DIRECTIVE_LIST[] = {"data","string","entry","extern",NULL};

int checkNumber (char **);
int checkLabelParam(char *);
short checkCommand(char *);
int checkDirective(char *);
int errorSwitch(int,int);
int takeParam(const char *,char *);
int checkLineSize(char * ,FILE * );

int checkMacroLabel(char * token,char * line,int lineNumber,FILE * file,char *label)	{

    int i,error;

    if (checkLineSize(line,file))/*check length*/
    {error=LONG_LINE;PRE_ASMBLR_EXIT}

    DELETE_SPACES
    if(checkCommand(token)!=-1 || checkDirective(token)!=-1)
    {error=LABEL_SAVE;PRE_ASMBLR_EXIT}

    for (i = 0; token[i] != '\n' && token[i] != '\t' && token[i] != '\0' && token[i] != ' '; i++)
        ;
    strncpy(label,token,i);
    label[i]=0;

    if(i>=LABEL_SIZE)
    {error=LONG_LABEL;PRE_ASMBLR_EXIT}
    
    token+=i;
    DELETE_SPACES
    if(token && (*token!='\n' && *token!='\0'))
    {error=END_ERROR;PRE_ASMBLR_EXIT}



    return 0;
}

int checkEndMacro(FILE * file,char * line,char * token,int lineNumber){

    int error=0;

    if (checkLineSize(line,file))/*check length*/
    {error=LONG_LINE;PRE_ASMBLR_EXIT}


    return 0;
}

line_data checkLine(char line[LINE_SIZE+1],FILE * file,int lineNumber){

    int cmd,i,ok,labelPoint;
    char p[LINE_SIZE+1],*token,*param,skipper;
    line_data lineData;

    i=cmd=ok=labelPoint=skipper=0;
    param=p;
    COMMAND_TYPE=PARAM1_TYPE=PARAM2_TYPE=DATA_TYPE=NUMBER_OF_DATAS=ERROR=CMD_NUMBER=0;
    COMMAND_NAME[0]=COMMAND_LABEL[0]=PARAM1_LABEL[0]=PARAM2_LABEL[0]=0;
    DATA_PARAM[0]=0;
    STRING_PARAM[0]=0;

    token=line;

    if (checkLineSize(line,file)){/*check length*/
        ERROR=LONG_LINE;EXIT
    }

    DELETE_SPACES

    if ( !*token || isspace(*token)){
        EXIT
    }

    if (*token==COMMENT_SIGN){/*check comment*/
        EXIT
    }

    for (i = 0; token[i] != '\n' && token[i] != '\t' && token[i] != '\0' && token[i] != ' '; i++)
        if(token[i]==':'){
	    if(i>=LABEL_SIZE)
    		{ERROR=LONG_LABEL;EXIT}
            labelPoint=i;
	}
    if(labelPoint){
        if(i>=LABEL_SIZE)
            {ERROR=LONG_LABEL;EXIT}
        strncpy(COMMAND_LABEL,token,labelPoint);
	COMMAND_LABEL[labelPoint]=0;
	if(!(ok=checkLabelParam(token)) || ok==-1)
            {ERROR=ILLEGAL_LABEL;EXIT} 
        if(checkCommand(COMMAND_LABEL)!=-1 || checkDirective(COMMAND_LABEL)!=-1)
            {ERROR=LABEL_SAVE;EXIT}
        COMMAND_LABEL[labelPoint]=0;
        token+=(labelPoint+1);
        DELETE_SPACES
        for (i = 0; token[i] != '\n' && token[i] != '\t' && token[i] != '\0' && token[i] != ' '; i++)
            ;
    }

    if(*token=='.') {/*if directive*/
        COMMAND_TYPE=2;
        token++;
        if(!token || isspace(*token))
        {ERROR=MISSING_COMMAND;EXIT}
        if((cmd=checkDirective(token))==-1)
        {ERROR=ILLEGAL_COMMAND;EXIT}
        strcpy(COMMAND_NAME,DIRECTIVE_LIST[cmd-16]);
        token+=i;
        DELETE_SPACES
    }
    else {
        if(!token || isspace(*token))
        {ERROR=MISSING_COMMAND;EXIT}
        if ((cmd = CMD_NUMBER = checkCommand(token)) == -1) {
            {ERROR = ILLEGAL_COMMAND;EXIT}
        }
        if ((checkDirective(token)) != -1)
        {ERROR = MISSING_POINT;EXIT}
        strcpy(COMMAND_NAME, COMMAND_LIST[cmd]);
        token += i;
        COMMAND_TYPE = 1;
        DELETE_SPACES
    }
    ok=0;
    switch (cmd) {
        case -2:
            ERROR=MISSING_COMMAND;
            EXIT
        case -1:
            ERROR=ILLEGAL_COMMAND;
            EXIT
        case 0:case 2:case 3:/*1,3,5-3,5*/
            /*1*/
        CHECK_PARAM
            TAKE_PARAM
            CHECK_NUMBER
            if (ok)
            {PARAM1_TYPE=1;PARAM1_NUMBER=strtol(param,NULL,10);ok=0;}
            else if(!PARAM1_TYPE)
            {CHECK_REGISTER}
            if(ok)
            {PARAM1_TYPE=5;PARAM1_REG=param[2]-'0';ok=0;}
            else if(!PARAM1_TYPE)
            {CHECK_LABEL}
            if (ok)
            {PARAM1_TYPE=3;strcpy(PARAM1_LABEL,param);ok=0;}
            if(!PARAM1_TYPE)
            {ERROR=ILLEGAL_PARAM;EXIT}
            DELETE_SPACES
            CHECK_COMMA
            DELETE_SPACES
            /*2*/
            CHECK_PARAM
            TAKE_PARAM
            CHECK_REGISTER
            if(ok)
            {PARAM2_TYPE=5;PARAM2_REG=param[2]-'0';ok=0;}
            else if(!PARAM2_TYPE)
            {CHECK_LABEL}
            if (ok)
            {PARAM2_TYPE=3;strcpy(PARAM2_LABEL,param);ok=0;}
            if(!PARAM2_TYPE)
            {ERROR=ILLEGAL_PARAM;EXIT}
            DELETE_SPACES
            CHECK_END
            NUMBER_OF_PARAMS=2;
            break;
        case 6:/*3-3,5*/
            /*1*/
            CHECK_PARAM
            TAKE_PARAM
            CHECK_LABEL
            if (ok)
            {PARAM1_TYPE=3;strcpy(PARAM1_LABEL,param);ok=0;}
            if(!PARAM1_TYPE)
            {ERROR=ILLEGAL_PARAM;EXIT}
            DELETE_SPACES
            CHECK_COMMA
            DELETE_SPACES
            /*2*/
            CHECK_PARAM
            TAKE_PARAM
            CHECK_REGISTER
            if(ok)
            {PARAM2_TYPE=5;PARAM2_REG=param[2]-'0';ok=0;}
            else if(!PARAM2_TYPE)
            {CHECK_LABEL}
            if (ok)
            {PARAM2_TYPE=3;strcpy(PARAM2_LABEL,param);ok=0;}
            if(!PARAM2_TYPE)
            {ERROR=ILLEGAL_PARAM;EXIT}
            DELETE_SPACES
            CHECK_END
            NUMBER_OF_PARAMS=2;
            break;
        case 4:case 5:case 7:case 8:case 9:case 10:case 11:case 13:/*0-3,5*/
            CHECK_PARAM
            TAKE_PARAM
            CHECK_REGISTER
            if(ok)
            {PARAM2_TYPE=5;PARAM2_REG=param[2]-'0';ok=0;}
            else if(!PARAM2_TYPE)
            {CHECK_LABEL}
            if (ok)
            {PARAM2_TYPE=3;strcpy(PARAM2_LABEL,param);ok=0;}
            if(!PARAM2_TYPE)
            {ERROR=ILLEGAL_PARAM;EXIT}
            DELETE_SPACES
            CHECK_END
            NUMBER_OF_PARAMS=1;
            break;
        case 1:/*1,3,5-1,3,5*/
            /*1*/
            CHECK_PARAM
            TAKE_PARAM
            CHECK_NUMBER
            if (ok)
            {PARAM1_TYPE=1;PARAM1_NUMBER=strtol(param,NULL,10);ok=0;}
            else if(!PARAM1_TYPE)
            {CHECK_REGISTER}
            if(ok)
            {PARAM1_TYPE=5;PARAM1_REG=param[2]-'0';ok=0;}
            else if(!PARAM1_TYPE)
            {CHECK_LABEL}
            if (ok)
            {PARAM1_TYPE=3;strcpy(PARAM1_LABEL,param);ok=0;}
            if(!PARAM1_TYPE)
            {ERROR=ILLEGAL_PARAM;EXIT}
            DELETE_SPACES
            CHECK_COMMA
            DELETE_SPACES
            /*2*/
            CHECK_PARAM
            TAKE_PARAM
            CHECK_NUMBER
            if (ok)
            {PARAM2_TYPE=1;PARAM2_NUMBER=strtol(param,NULL,10);ok=0;}
            else if(!PARAM2_TYPE)
            {CHECK_REGISTER}
            if(ok)
            {PARAM2_TYPE=5;PARAM2_REG=param[2]-'0';ok=0;}
            else if(!PARAM2_TYPE)
            {CHECK_LABEL}
            if (ok)
            {PARAM2_TYPE=3;strcpy(PARAM2_LABEL,param);ok=0;}
            if(!PARAM2_TYPE)
            {ERROR=ILLEGAL_PARAM;EXIT}
            DELETE_SPACES
            CHECK_END
            NUMBER_OF_PARAMS=2;
            break;
        case 12:/*0-1,3,5*/
            CHECK_PARAM
            TAKE_PARAM
            CHECK_NUMBER
            if (ok)
            {PARAM2_TYPE=1;PARAM2_NUMBER=strtol(param,NULL,10);ok=0;}
            else if(!PARAM2_TYPE)
            {CHECK_REGISTER}
            if(ok)
            {PARAM2_TYPE=5;PARAM2_REG=param[2]-'0';ok=0;}
            else if(!PARAM2_TYPE)
            {CHECK_LABEL}
            if (ok)
            {PARAM2_TYPE=3;strcpy(PARAM2_LABEL,param);ok=0;}
            if(!PARAM2_TYPE)
            {ERROR=ILLEGAL_PARAM;EXIT}
            DELETE_SPACES
            CHECK_END
            NUMBER_OF_PARAMS=1;
            break;
        case 14:case 15:/*0-0*/
            DELETE_SPACES
            CHECK_END
            NUMBER_OF_PARAMS=0;
            break;
        case 16:/*data*/
	    if(COMMAND_LABEL[0]==0)
		{printf("warnning: in line %d tha data unreachable\n",lineNumber);COMMAND_TYPE=0;break;}
            DATA_TYPE=1;
            CHECK_PARAM
            TAKE_PARAM
            CHECK_NUMBER
            /*DATA_PARAM = (int *)malloc(sizeof(int));*/
            if (ok)
            {DATA_PARAM[NUMBER_OF_DATAS]=strtol(param,NULL,10);}
            else
            {ERROR = ILLEGAL_PARAM;EXIT}
            DELETE_SPACES

            while (token && !isspace(*token) && (*token != '\0')) {
                NUMBER_OF_DATAS++;
                CHECK_COMMA
                DELETE_SPACES
                CHECK_PARAM
                TAKE_PARAM
                CHECK_NUMBER
                if (ok)
                {DATA_PARAM[NUMBER_OF_DATAS]=strtol(param,NULL,10);}
                else
                {ERROR = ILLEGAL_PARAM;EXIT}
                DELETE_SPACES
            }
            CHECK_END
            break;
        case 17:/*string*/
	    if(COMMAND_LABEL[0]==0)
	        {printf("warnning: in line %d tha string unreachable\n",lineNumber);COMMAND_TYPE=0;break;}
	    DELETE_SPACES
            DATA_TYPE=2;
            i=0;
            CHECK_PARAM
            CHECK_APOSTROPHES
            token++;
            for (i=0;token[i]!='"' && token[i] && token[i]!='\n';i++);
            CHECK_APOSTROPHES
            strncpy(STRING_PARAM,token,i);
            STRING_PARAM[i]=0;
            token+=i+1;
            DELETE_SPACES
            CHECK_END
            break;
        case 18:case 19:/*entry/extern*/
            ENTEX_TYPE=cmd;
            COMMAND_TYPE=3;
            CHECK_PARAM
            TAKE_PARAM 
            CHECK_LABEL
            if (ok)
                strcpy(ENTEX_PARAM,param);
            else
            {ERROR = ILLEGAL_PARAM;EXIT}
            DELETE_SPACES
            CHECK_END
            if(*COMMAND_LABEL) {
                *COMMAND_LABEL = 0;
                printf("warning: Defining a label at the beginning of a entry or extern is meaningless\n");
            }
            break;
        default:
            break;
    }
    return lineData;
}

int errorSwitch(int error,int lineNumber){
    switch (error) {
        case LONG_LINE:
            printf("in line %d line too long\n",lineNumber);
            break;
        case ILLEGAL_LABEL:
            printf("in line %d illegal label\n",lineNumber);
            break;
	case LONG_LABEL:
            printf("in line %d label too long\n",lineNumber);
            break;
        case MISSING_COMMAND:
            printf("in line %d missing command\n",lineNumber);
            break;
        case ILLEGAL_COMMAND:
            printf("in line %d illegal command\n",lineNumber);
            break;
        case MISSING_PARAMETER:
            printf("in line %d missing parameter\n",lineNumber);
            break;
        case ILLEGAL_NUMBER:
            printf("in line %d illegal number\n",lineNumber);
            break;
        case ILLEGAL_REGISTER:
            printf("in line %d illegal register\n",lineNumber);
            break;
        case ILLEGAL_LABEL_PARAM:
            printf("in line %d illegal label in parameters\n",lineNumber);
            break;
        case MISSING_COMMA:
            printf("in line %d missing comma\n",lineNumber);
            break;
        case END_ERROR:
            printf("in line %d extraneous text after end of command\n",lineNumber);
            break;
        case ILLEGAL_PARAM:
            printf("in line %d illegal parameter\n",lineNumber);
            break;
        case LABEL_SAVE:
            printf("in line %d a label cannot be defined as a saved word\n",lineNumber);
            break;
        case MISSING_POINT :
            printf("in line %d missing point\n",lineNumber);
            break;
        case MISSING_APOSTROPHES:
            printf("in line %d missing Apostrophes\n",lineNumber);
            break;
	case TO_MANY_PARAMETERS:
            printf("in line %d to many parameters\n",lineNumber);
            break;
        default:
            break;
    }
    return 0;
}

short checkCommand(char * token) {
    short i=0;
    int len=0;
    while (COMMAND_LIST[i] != NULL) {
        if (strncmp(token, COMMAND_LIST[i], (len=(int)strlen(COMMAND_LIST[i])))==0)
            if(isspace(token[len]) || !token[len])
                return i;
        i++;
    }
    return -1;
}

int checkDirective(char * token){
    int i=0;
    int len=0;
    while (DIRECTIVE_LIST[i]!=NULL){
        if (strncmp(token,DIRECTIVE_LIST[i],(len=(int)strlen(DIRECTIVE_LIST[i])))==0)
            if(isspace(token[len]) || !token[len])
                return i+16;
        i++;
    }
    return -1;
}

int checkNumber (char ** token) {

    if (isdigit(**token) || **token == '-' || **token == '+'){
        if ((**token == '-' || **token == '+') && !isdigit(*(*token+1)))
            return ILLEGAL_NUMBER;
        (*token)++;
        while (**token != ',' && **token != '\0' && !isspace(**token)){
            if (!isdigit(**token))
            {return ILLEGAL_NUMBER;}
            (*token)++;}
        return 1;
    }
    return 0;
}

int checkLabelParam(char *token) { /*if label ok return j if not label return 0 if error return -1*/
    int i, j;
    i = 0;
    if (isalpha(token[0])) {
        while (token[i] != ',' && !isspace(token[i]) && token[i] != '\0' && token[i] != ':')
            {i++;}
        if(i>=LABEL_SIZE)
            return -2;
        j = i;
        while (--i)
            if (!isalpha(token[i]) && !isdigit(token[i]))
                return -1;
        return j;
    }
    return 0;
}
int takeParam(const char *token,char *param){
    int i;
    for (i=0;token[i]!=' ' && token[i]!=',' && token[i]!='\n' && token[i]!='\t' && token[i]; i++)
        param[i]=token[i];
    param[i]=0;

    return 0;
}

int checkLineSize(char * line,FILE * file){
    char skipper;
    if (strlen(line) > LINE_SIZE){/*check length*/
        if(line[LINE_SIZE]!='\n')
            while ((skipper=fgetc(file))!=EOF && skipper!='\n')
                ;/*skip to the end of the line*/
	return 1;
    }
    return 0;

}



