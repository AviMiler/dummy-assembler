#ifndef MAIN_C
#define MAIN_C
#include "checkers.h"
#include "structures.h"
#include "macro.h"
#include "data.h"
#endif

#define CHECK_MEMORY if(mlc>=MEMORY_SIZE-100) {printf("Memory exception error\n");return -2;}

int printAs64Base (FILE * ,short * ,int);
short enterParam (int,int,int,int,int,char * ,void *);

int main(int argc, char *argv[]) {

    FILE *file, *ent, *ext, *obFile, *amFile;
    void *labelsTrie,*labelsTrieP;
    line_data *linesDataArray, *linesCommandArray, *entExArray;
    void *waitingToAddress;
    int i, j, k, lineNumber, errorFlag, dc, ic, lc, mlc, wc, ec, fileNameLengthe,wmlc;/*ic = instruction counter dc = line data counter mlc = machine line counter wc = 										waiting counter ec =entry,extern cnt*/
    short machineWord[MEMORY_SIZE], machineWordMask;
    char line[LINE_SIZE + 3], *fileName,*fileNameP,*label;
    line_data lineData;
   

    for (k = 1; k < argc; k++) {/*for every file*/

        fileNameLengthe = strlen(argv[k]);

 	linesCommandArray=linesDataArray=entExArray=NULL;
        labelsTrie=labelsTrieP=waitingToAddress=NULL;

        if ((fileNameP = malloc((fileNameLengthe+5) * sizeof(char))) == NULL) {
            printf("malloc failed\n");
            return -1;
        }

        fileName = fileNameP;
        strcpy(fileName,argv[k]);


        i = lineNumber = errorFlag = ic = dc = j = lc = mlc = wc = ec = 0;
        obFile = ext = ent = amFile = NULL;

        setMacros(labelsTrie,fileName,fileNameLengthe);

        fileName[fileNameLengthe]=0;/*open the am file*/
        strcat(fileName,".am");

        if((file = fopen(fileName, "r"))==NULL)
        {printf("open file feild\n");
            return -1;}

        fileName[fileNameLengthe]=0;


        while (fgets(line, LINE_SIZE + 2, file) != NULL) {/*read line from the file */

            lineNumber++;

            lineData = checkLine(line, file, lineNumber);/*check for syntax errors end return the data of the line*/

            if (!errorFlag)
                errorFlag = ERROR;

            /*build arrays of data commands datas end labels for entry or extern*/
            if (!errorFlag) {
                if (COMMAND_TYPE == 1 || COMMAND_TYPE == 3) {
                    linesCommandArray = insertInformationArray (linesCommandArray,lineData,ic);
                    if(linesCommandArray==NULL)
                        return -1;
                    ic++;
                }
                if (COMMAND_TYPE == 2) {
                    linesDataArray = insertInformationArray (linesDataArray,lineData,dc);
                    if(linesDataArray==NULL)
                        return-1;
                    dc++;
                }
                if (COMMAND_TYPE == 3) {
                    entExArray = insertInformationArray (entExArray,lineData,ec);
                    if(entExArray==NULL)
                        return-1;
                    ec++;
                }
            }

        }/*end of reding from file*/


/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/


        mlc = SYSTEM_MEMORY;


/*read data from the Information arrays end translate to machine words*/

        for (i = 0; i < ic; ++i) {/*read data from the Information arrays end translate to machine words*/
            machineWord[mlc] = 0;
            lineData = getInformationArray(linesCommandArray,i);
            if (*COMMAND_LABEL)
                if (insert(COMMAND_LABEL, NULL, (short) mlc, 0,&labelsTrie)) {/*insert to the labels trie ,if already exist print error*/
                    printf("the label %s has conflicting declarations\n", COMMAND_LABEL);
                    errorFlag = 1;
                }
            if (!errorFlag) {
                /*build the command machine word*/
                if (COMMAND_TYPE == 1) {
                    machineWordMask = CMD_NUMBER;
                    machineWord[mlc] |= (machineWordMask = (machineWordMask << 5));

                    machineWordMask = PARAM1_TYPE;
                    machineWord[mlc] |= (machineWordMask = (machineWordMask << 9));

                    machineWordMask = PARAM2_TYPE;
                    machineWord[mlc] |= (machineWordMask = (machineWordMask << 2));

                    mlc++;
                    CHECK_MEMORY
                    machineWord[mlc] = 0;

                    if (NUMBER_OF_PARAMS) {/*if there is parameters */
                        if (NUMBER_OF_PARAMS == 2) {
                            machineWord[mlc] = enterParam(PARAM1_NUMBER, PARAM1_REG, PARAM1_TYPE, wc, mlc,PARAM1_LABEL,labelsTrie);
                            if ((short) machineWord[mlc] == -1) {
                                waitingToAddress=insertInWaitingArray(&waitingToAddress,mlc,PARAM1_LABEL,wc);
                                machineWord[mlc] = 0;
                                wc++;
                            }
                            if (PARAM1_TYPE == 5)
                                machineWord[mlc] = (machineWord[mlc] << 5);
                            mlc++;
                        }
                        machineWord[mlc] = enterParam(PARAM2_NUMBER, PARAM2_REG, PARAM2_TYPE, wc, mlc, PARAM2_LABEL,labelsTrie);

                        if (PARAM1_TYPE == 5 && 5 == PARAM2_TYPE) {
                            machineWordMask = machineWord[mlc];
                            mlc--;
                            machineWord[mlc] |= machineWordMask;
                        }
                        CHECK_MEMORY
                        if ((short) machineWord[mlc] == -1) {
                            waitingToAddress=insertInWaitingArray(&waitingToAddress,mlc,PARAM2_LABEL,wc);
                            machineWord[mlc] = 0;
                            wc++;
                        }
                        mlc++;
                        CHECK_MEMORY
                    }
                }
            }
        }/*End of inserting the instructions*/

        ic = mlc - 100;

        for (i = 0; i < dc; ++i) {

            lineData = getInformationArray(linesDataArray,i);
	    if(*COMMAND_LABEL!=0)
            if (insert(COMMAND_LABEL, NULL, (short) mlc, 0, &labelsTrie)) {
                printf("the label %s has conflicting declarations\n", COMMAND_LABEL);
                errorFlag = 1;
            }

            if (!errorFlag) {

                if (DATA_TYPE == 1) {/*data*/
                    j = 0;
                    while (j <= NUMBER_OF_DATAS) {
                        machineWord[mlc] = DATA_PARAM[j];
                        mlc++;
                        CHECK_MEMORY
                        j++;
                    }
                }

                if (DATA_TYPE == 2) {/*string*/
                    j = 0;
                    while (STRING_PARAM[j]) {
                        machineWord[mlc] = (char) STRING_PARAM[j];
                        mlc++;
                        CHECK_MEMORY
                        j++;
                    }
                    machineWord[mlc] = (char) STRING_PARAM[j];
                    mlc++;
                    CHECK_MEMORY
                }
            }
        }/*End of inserting the datas*/

        dc = mlc - 100 - ic;

        for (i = 0; i < ec; ++i) {

            lineData = getInformationArray(entExArray,i);

            if (ENTEX_TYPE == 18) {/*entry if the label defined print the addres to file.ent*/
                if (ent == NULL) {
                    fileName[fileNameLengthe] = 0;
                    strcat(fileName,".ent");
                    ent = fopen(fileName, "w");
                }
                if ((labelsTrieP = trieSearch(ENTEX_PARAM, (int) strlen(ENTEX_PARAM), labelsTrie)) == NULL) {
                    printf("the label %s didn't found for entry\n", ENTEX_PARAM);
                    errorFlag = 1;
                } else if (!errorFlag)
                    fprintf(ent, "%-32s %d\n",ENTEX_PARAM,getAddres(labelsTrieP));
            }

            if (ENTEX_TYPE == 19) {/*if there is extern labels insert to labelsTrie*/
                if (insert(ENTEX_PARAM, NULL, 0, 1, &labelsTrie)) {
                    printf("the label %s to extern already exist in the program\n", ENTEX_PARAM);
                    errorFlag = 1;
                }
            }
        }


        ic = mlc - 100 - dc;

        for (i = 0; i < wc; ++i) {/*set the waiting labels and if label is extern print to file.ext*/

            label=getWaitingLabel(waitingToAddress,i);
            wmlc=getWaitingMlc(waitingToAddress,i);

            if ((labelsTrieP = trieSearch(label , strlen(label) , labelsTrie)) != NULL) {
                if (getExt(trieSearch(label , strlen(label) , labelsTrie))) {
                    if (ext == NULL) {
                        fileName[fileNameLengthe] = 0;
                        strcat(fileName,".ext");
                        ext = fopen(fileName, "w");
                    }
                    machineWord[wmlc] = 1;
                    fprintf(ext, "%-32s %d\n", label , getWaitingMlc(waitingToAddress,i));
                } else
                    machineWord[wmlc] = enterParam(0, 0, 3, wc, 0,label,labelsTrie);
            } else {
                printf("the label %s dose not initialized\n",label);
                errorFlag = 1;
            }
        }/*end of creating the machine words*/



/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/


/*create and print the ob file*/
        if (!errorFlag && mlc>100) {
            fileName[fileNameLengthe]=0;
            strcat(fileName,".ob");
            obFile = fopen(fileName, "w");
            fprintf(obFile, "%d %d\n", dc, ic);
            printAs64Base(obFile,machineWord, mlc);
        }

	if(errorFlag){
	    fileName[fileNameLengthe]=0;
	    if (ent != NULL) remove(strcat(fileName,".ent"));
	    fileName[fileNameLengthe]=0;
	    if (ext != NULL) remove(strcat(fileName,".ext"));
	
	}


/*free and close everything*/
        if (waitingToAddress != NULL) free(waitingToAddress);
        if (labelsTrie != NULL) freeTrie(labelsTrie);
        if (linesDataArray != NULL) free(linesDataArray);
        if (linesCommandArray != NULL) free(linesCommandArray);
        if (entExArray != NULL) free(entExArray);
        if (fileNameP != NULL) free(fileNameP);

        if (obFile != NULL) fclose(obFile);
        if (ent != NULL) fclose(ent);
        if (ext != NULL) fclose(ext);
        if (file != NULL) fclose(file);
        if (amFile != NULL) fclose(amFile);	

    }
    return 0;
}





short enterParam (int num,int reg,int paramType,int wc,int mlc,char * label,void * labelsTrie) {
    unsigned short machineWord = 0;
    unsigned short machineWordMask;
    switch (paramType) {
        case 1:
            machineWordMask = (short) num;
            machineWord |= (machineWordMask = (machineWordMask << 2));
            break;
        case 3:
            if ((labelsTrie = (trieSearch(label, (int) strlen(label),labelsTrie))) != NULL) {
                machineWordMask = getAddres(labelsTrie);
                machineWord |= (machineWordMask = (machineWordMask << 2));
                machineWord |= (machineWordMask = 2);/*ARE*/
            } else {
                return -1;
            }
            break;
        case 5:
            machineWordMask = (short) reg;
            machineWord |= (machineWordMask = (machineWordMask << 2));
            break;
        default:
            break;
    }
    return (short)machineWord;
}
int printAs64Base(FILE * obFile,short * machineWords,int mlc){
    int i;
    short left,right;
    char base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    for(i=100;i<mlc;i++) {
        left = ((machineWords[i]) >> 6);
        left &= 63;
        right = ((machineWords[i]) & 63);

        fprintf(obFile,"%c%c\n",base64[left], base64[right]);
    }
    return 0;
}
