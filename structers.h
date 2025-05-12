#ifndef STRUCTERS_H
#define STRUCTERS_H

#include "data.h"


typedef struct waiting{
    int machineLineNum;
    char label[LABEL_SIZE];
}waiting;

typedef struct {
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

struct Trie{
    int endOfWord;
    char *macro;
    short address;
    int ext;

    struct Trie * next[80];
};

typedef struct Trie labels_trie;

int insert(const char * ,char * ,short,int ,labels_trie *);
labels_trie * trieSearch(char * ,int,labels_trie * );
int freeTrie(labels_trie *);

#endif
