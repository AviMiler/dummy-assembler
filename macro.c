#include "macro.h"


#define DELETE_SPACES while (token && (*token==' ' || *token=='\t')) token+=1;

char* trimBefore(char * line){
    char * resulte=line;
    while ((*resulte==' ' ||  *resulte=='\t') && resulte!=NULL && *resulte!=0){
        resulte++;
    }
    return resulte;
}

int setMacros(void * labelsTrie,char * fileName,int fileNameLengthe){

    char line[100],tokenP[100],*token,*macro,*macroP,label[33], *newFile,*newFileP;
    int lineNumber,isMacro,mcrIndex,NFIndex,errorFlag,macroSize,newFileSize,i,endMacroFlag,thereIsMacro;
    FILE *asFile,*amFile;
    void * lt;

    token=tokenP;
    lineNumber=endMacroFlag=isMacro=mcrIndex=NFIndex=errorFlag=macroSize=newFileSize=thereIsMacro=0;
    *label=0;
    asFile=amFile=NULL;
    labelsTrie=NULL;
    

    strcat(fileName,".as");
    asFile=fopen(fileName,"r");
    if (asFile==NULL)
    {printf("open file error\n");return 2;}
    *(fileName+fileNameLengthe)=0;

    if((macro=malloc(1*sizeof(char)))==NULL)
    {printf("malloc feild\n");return 2;}
    if((newFile=malloc(1*sizeof(char)))==NULL)
    {printf("malloc feild\n");return 2;}

    while (fgets(line,LINE_SIZE+2, asFile) != NULL){
        endMacroFlag=0;
        lineNumber++;
        macroSize=0;
        lt=NULL;

        token=line;

        DELETE_SPACES

        if(!(strncmp("mcro",token,4)) && (isspace(*(token+=4)) || token==0)){/*if start macro definition*/
            if(!checkMacroLabel(token,line,lineNumber,asFile,label))/*check end put the label at label variable*/
                isMacro=1;
            else
                errorFlag=1;
            while (isMacro && fgets(line,100,asFile)!=NULL){
                token=trimBefore(line);
                if (strncmp(token,"endmcro",7)==0){
                    if(checkEndMacro(asFile,line,token,lineNumber))
                        errorFlag=1;
                    isMacro=0;
                    endMacroFlag=1;
                }
                else{
                    macroSize = (macroSize+(int)strlen(line));
                    if((macroP = realloc(macro, (macroSize+5) * sizeof(char)))==NULL)
                    {printf("realloc faild\n");return -1;}
                    macro=macroP;
                    strcpy(macro+mcrIndex,line);
                    mcrIndex=macroSize;
                    macro[mcrIndex]='\0';
                }
            }
            mcrIndex=0;
            if(label!=NULL && !errorFlag){
                if(insert(label,macro,0,0,&labelsTrie)){;
                    printf("#at preassembler: in line %d the label %s already defined\n",lineNumber,label);
                }
                thereIsMacro=1;
            }
        }
        token=trimBefore(line);/*take the line to check if there is macro calling*/
        for (i = 0; token[i] != '\n' && token[i] != '\t' && token[i] != '\0' && token[i] != ' '; i++)
            ;
        if(thereIsMacro)
            lt=trieSearch(token,i,labelsTrie);
        if(lt!=NULL){  /*if found macro name*/

            newFileSize += strlen(getMacro(lt));
            if((newFileP = realloc(newFile, (newFileSize+1) * sizeof(char)))!=NULL)
                newFile=newFileP;
            strcpy(newFile+NFIndex,getMacro(lt));
            NFIndex=newFileSize;
            *(newFile+NFIndex)='\0';

        }
        else if (!endMacroFlag){/*if not a macro or definition of macro*/
            newFileSize += strlen(line);
            if((newFileP = realloc(newFile, (newFileSize+1) * sizeof(char)))!=NULL)
                newFile=newFileP;
            strcpy(newFile+NFIndex,line);
            NFIndex=newFileSize;
            newFile[NFIndex]='\0';
        }
    }

    if(!errorFlag){/*create the am file end print*/
        newFile[NFIndex]='\0';
        strcat(fileName,".am");
        if((amFile = fopen(fileName,"w"))==NULL)
            return -1;
        fprintf(amFile,"%s",newFile);

    }

    free(macro);
    free(newFile);
    if (amFile!=NULL) fclose(amFile);
    if (asFile!=NULL) fclose(asFile);

    return 0;
}

















