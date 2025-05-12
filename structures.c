#include "structures.h"

struct Trie{
    int endOfWord;
    char *macro;
    short address;
    int ext;

    struct Trie * next[80];
};

typedef struct Trie labels_trie;

int insert(const char * label,char * macro,short address,int ext,void ** labelsTrie){

    int i,j;
    labels_trie * nextLt,*lt;
    i=j=0;
    if(*labelsTrie==NULL){
        if ((lt = calloc(1, sizeof(labels_trie))) == NULL) {
            printf("malloc fails\n");
            return -1;
        }
        for(j=0;j<80;j++)
            lt->next[j]=NULL;
        *labelsTrie=lt;
    }

    lt=*labelsTrie;

    if(lt->next[((int) label[0]) - '/']==NULL) {
        nextLt = calloc(1, sizeof(labels_trie));
        for(j=0;j<80;j++)
            nextLt->next[j]=NULL;
        lt->next[((int) label[0]) - '/'] = nextLt;
    } else
        nextLt=lt->next[((int) label[0]) - '/'];
    while (label[i+1]){
        i++;
        if(nextLt->next[((int) label[i]) - '/']==NULL)
            nextLt->next[label[i]- '/'] = calloc(1,sizeof(labels_trie));
        nextLt=nextLt->next[label[i]- '/'];
    }
    if(nextLt->endOfWord)
        return 1;
    if(macro!=NULL) {
        nextLt->macro = malloc((strlen(macro)+1) * sizeof(char));
        strcpy(nextLt->macro,macro);
    }
    nextLt->ext=ext;
    nextLt->address=address;
    nextLt->endOfWord=1;

    *labelsTrie=lt;

    return 0;
}

labels_trie * trieSearch(char * label,int end,void * head){

    labels_trie * lt;
    int i;
    i=0;
    lt=head;
    if(!label[i])
        return NULL;
    lt=lt->next[label[i]-'/'];
    while (lt != NULL) {
        if (i+1 == end) {
            if (lt->endOfWord)
                return lt;
            else
                return NULL;
        }
        i++;
        lt=lt->next[label[i]-'/'];
    }
    return NULL;
}

int getAddres(void * labelsTrie){
    labels_trie * lt = labelsTrie;
    return lt->address;
}
int getExt(void * labelsTrie){
    labels_trie * lt = labelsTrie;
    return lt->ext;
}
char * getMacro(void * labelsTrie){
    labels_trie * lt = labelsTrie;
    return lt->macro;
}


int freeTrie(labels_trie *head) {

    int i;
    if (head == NULL) {
        return 0;
    }
    for (i = 0; i < 80; i++) {
        freeTrie(head->next[i]);
    }
    if(head->macro!=NULL)
        free(head->macro);
    if(head!=NULL)
        free(head);

    return 0;
}

struct Waiting{/*structure for labels that waiting for addres*/
    int machineLineNum;
    char label[LABEL_SIZE];
};
typedef struct Waiting waiting;
void * insertInWaitingArray(void ** waitingHade,int machineLineNum,char * label,int index){

    waiting * waitingP;
    if(*waitingHade==NULL){
        if ((*waitingHade = malloc(sizeof(waiting))) == NULL) {
            printf("malloc fails\n");
            return NULL;
        }
    }

    if ((waitingP = realloc(*waitingHade, ((index + 1) * sizeof(waiting)))) == NULL) {
        printf("reallocate fails\n");
        return NULL;
    }

    waitingP[index].machineLineNum=machineLineNum;
    strcpy(waitingP[index].label,label);

    *waitingHade=waitingP;

    return *waitingHade;
}

int getWaitingMlc(void * waitingHade,int index){
    waiting * waitingP;
    waitingP = waitingHade;
    return waitingP[index].machineLineNum;
}

char * getWaitingLabel(void * waitingHade,int index){
    waiting * waitingP;
    waitingP = waitingHade;
    return waitingP[index].label;
}

line_data * insertInformationArray(line_data * Structer, line_data lineData,int index){
    line_data * lineDataP;

    if(Structer==NULL){
        if ((Structer = malloc(sizeof(lineData))) == NULL) {
            printf("malloc fails\n");
            return NULL;
        }
    }

    if ((lineDataP = realloc(Structer, ((index + 1) * sizeof(lineData)))) == NULL) {
        printf("reallocate fails\n");
        return NULL;
    }
    Structer = lineDataP;
    Structer[index] = lineData;


    return Structer;
}

line_data getInformationArray(line_data * Structer,int index){

    return Structer[index];

}

