#ifndef STRUCTERS_H
#define STRUCTERS_H

#include "data.h"

typedef struct Trie labels_trie;
typedef struct Waiting Waiting;


typedef struct {/*line data structure*/
    int cmdType;
    int error;
    short cmdNumber;
    int numberOfParams;
    char name[10];
    char label[LABEL_SIZE];
    union {
        struct {
            int param1Type;
            union {
                int number;
                char label[LABEL_SIZE];
                int reg;
            }param1;
            int param2Type;
            union {
                int number;	
                char label[LABEL_SIZE];
                int reg;
            }param2;
        }command;
        struct{
            int dataType;
            union {
                struct {
                    int numberOfData;
                    int data[40];
                }data;
                char string[72];
            }dataParam;
        }instruction;
        struct {
            int entExType;
            char param[LABEL_SIZE];
        }entryExtern;
    }content;

}line_data;



line_data * insertInformationArray(line_data *,line_data,int);
line_data getInformationArray(line_data *,int);


void * insertInWaitingArray(void ** ,int ,char * ,int );
int getWaitingMlc(void * ,int);
char * getWaitingLabel(void * ,int);



/*labels_trie functions*/
int insert(const char * ,char * ,short,int ,void **);/*insert to labels_trie get label and */
labels_trie * trieSearch(char * ,int,void * );/*serche at labels_trie*/
int freeTrie(labels_trie *);
int getAddres(void *);
int getExt(void *);
char * getMacro(void*);

#endif
