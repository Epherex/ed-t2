#include "static_list.h"

#define NULLC -1

typedef struct node_t {
    void *val;
    int previous;
    int next;
} *Node;

typedef struct stlist_t {
    int size;
    int first;
    int last;
    int freeIndex;
    Node nodes;
} *StListPtr;

StList StList_Create(int size) {
    if(size <= 0)
        return NULL;
    
    StListPtr newStList = malloc(sizeof(struct stlist_t));
    if(newStList == NULL)
        return NULL;
    
    newStList->nodes = malloc(size * sizeof(struct node_t));
    if(newStList->nodes == NULL) {
        free(newStList);
        return NULL;
    }
    
    newStList->first = NULLC;
    newStList->last = NULLC;
    newStList->size = size;
    newStList->freeIndex = 0;
    for(int i = 0; i < size; i++) {
        newStList->nodes[i].next = i + 1;
    }
    newStList->nodes[size - 1].next = NULLC;
    return newStList;
}

void StList_Resize(StList staticListVoid, int size) {
    StListPtr list = (StListPtr) staticListVoid;
    if(size < list->size)
        return;
    Node nodes = realloc(list->nodes, size * sizeof(struct node_t));
    if(nodes == NULL)
        return;
    list->nodes = nodes;
    list->nodes[list->size].next = list->freeIndex;
    list->nodes[list->size].previous = 0;
    for(int i = list->size + 1; i < size; i++) {
        list->nodes[i].next = i - 1;
    }
    list->freeIndex = size - 1;
    list->size = size;
}

void StList_Destroy(StList staticListVoid, void destroyElement(void*)) {
    StListPtr list = (StListPtr) staticListVoid;
    for(int i = list->first; i != NULLC; i = list->nodes[i].next) {
        destroyElement(list->nodes[i].val);
    }
    free(list->nodes);
    free(list);
}

int _freeStackPop(StListPtr list) {
    int freeIndex = list->freeIndex;
    if(freeIndex != NULLC)
        list->freeIndex = list->nodes[freeIndex].next;
    return freeIndex;
}

void _freeStackPush(StListPtr list, int index) {
    list->nodes[index].next = list->freeIndex;
    list->freeIndex = index;
}

bool StList_Add(StList staticListVoid, void *element) {
    StListPtr list = (StListPtr) staticListVoid;
    if(StList_IsFull(staticListVoid))
        return false;
    int freeIndex = _freeStackPop(list);
    list->nodes[freeIndex].next = NULLC;
    list->nodes[freeIndex].val = element;
    if(list->first == NULLC) {
        list->nodes[freeIndex].previous = NULLC;
        list->first = freeIndex;
    } else {
        int last = list->last;
        list->nodes[last].next = freeIndex;
        list->nodes[freeIndex].previous = last;
        list->last = freeIndex;
    }
    list->last = freeIndex;
    return true;
}

void *StList_Remove(StList staticListVoid, bool compFunc(void*, void*), void *comparingField) {
    StListPtr list = (StListPtr) staticListVoid;
    for(int i = list->first; i != NULLC; i = list->nodes[i].next) {
        if(compFunc(list->nodes[i].val, comparingField)) {
            int previous = list->nodes[i].previous;
            int next = list->nodes[i].next;
            if(previous != NULLC)
                list->nodes[previous].next = next;
            else
                list->first = list->nodes[i].next;
            if(next != NULLC)
                list->nodes[next].previous = previous;
            else
                list->last = previous;
            void *removedElement = list->nodes[i].val;
            _freeStackPush(list, i);
            return removedElement;
        }
    }
    return NULL;
}

void *StList_RemoveLast(StList staticListVoid) {
    StListPtr list = (StListPtr) staticListVoid;
    if(StList_IsEmpty(list))
        return NULL;
    int last = list->last;
    int previous = list->nodes[last].previous;
    if(previous != NULLC)
        list->nodes[previous].next = NULLC;
    else
        list->first = NULLC;
    void *removedElement = list->nodes[last].val;
    list->last = previous;
    _freeStackPush(list, last);
    return removedElement;
}

bool StList_IsEmpty(StList staticListVoid) {
    StListPtr list = (StListPtr) staticListVoid;
    return list->first == NULLC;
}

bool StList_IsFull(StList staticListVoid) {
    StListPtr list = (StListPtr) staticListVoid;
    return list->freeIndex == NULLC;
}

void *StList_GetFirst(StList staticListVoid) {
    StListPtr list = (StListPtr) staticListVoid;
    if(StList_IsEmpty(list))
        return NULL;
    else
        return list->nodes[list->first].val;
}

void *StList_Find(StList staticListVoid, bool compFunc(void*, void*), void *comparingField) {
    StListPtr list = (StListPtr) staticListVoid;
    for(int i = list->first; i != NULLC; i = list->nodes[i].next) {
        if(compFunc(list->nodes[i].val, comparingField))
            return list->nodes[i].val;
    }
    return NULL;
}

void StList_Execute(StList staticListVoid, void f(void*, void*), void *parameter) {
    StListPtr list = (StListPtr) staticListVoid;
    for(int i = list->first; i != NULLC; i = list->nodes[i].next) {
        f(list->nodes[i].val, parameter);
    }
}
