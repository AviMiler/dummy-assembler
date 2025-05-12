#include "structers.h"

int insert(const char * label,char * data,short address,int ext,labels_trie * lt){
    int i=0;
    labels_trie * nextLt;
    if(lt->next[((int) label[0]) - '/']==NULL) {
        nextLt = calloc(1, sizeof(labels_trie));
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
    if(data!=NULL) {
        nextLt->macro = malloc(strlen(data) * sizeof(char));
        strcpy(nextLt->macro,data);
    }
    nextLt->ext=ext;
    nextLt->address=address;
    nextLt->endOfWord=1;

    return 0;
}

labels_trie * trieSearch(char * label,int end,labels_trie * head){
    int i=0;
    if(!label[i])
        return NULL;
    head=head->next[label[i]-'/'];
    while (head!=NULL) {
        if (i+1 == end) {
            if (head->endOfWord)
                return head;
            else
                return NULL;
        }
        i++;
        head=head->next[label[i]-'/'];
    }
    return NULL;
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

return 0;printf("ss:");
}







