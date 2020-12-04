#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
    @autor Nicola
*/
typedef struct Data {
    char Bez[50];
    double Preis;
} DataElement;

/*
    @autor Nicola
*/
typedef struct Elm {
    struct Elm* pNext;
    struct Data* pData;
} ListElement;

/*
    Prototypes
*/
ListElement* createLinkedList(int listSize);
void fillData(DataElement* toFill);
void deleteList(ListElement* pToDelete);
void printList(ListElement* firstElement, int ElementsToPrintPerIteration);


/*
    @autor Nicola
    @autor --
*/
int main() {
    ListElement* List = createLinkedList(30);
    printList(List, 100);
    deleteList(List);
    printList(List, -1);
}

/*
    This is one of the 5 Base Functionality Functions
    @autor Nicola
    The list is Built Back to Front
*/
ListElement* createLinkedList(int listSize) {

    ListElement* pPreviousElement = NULL;
    for (int i = 0; i < listSize; i++) {
        //if (pList == NULL) { pList = pPreviousElement; }
        ListElement* pListElement = (ListElement*)malloc(sizeof(ListElement));
        DataElement* pDataElement = (DataElement*)malloc(sizeof(DataElement));
        if (pListElement == NULL || pDataElement == NULL) return NULL;
        fillData(pDataElement);
        pListElement->pData = pDataElement;
        pListElement->pNext = pPreviousElement;
        pPreviousElement = pListElement;

    }
    return pPreviousElement;
}

/*
    TODO
*/
void fillData(DataElement* toFill) {
    strcpy_s(toFill->Bez, "THIS IS STILL HARDCODED");
    toFill->Preis = 420.69;
}

/*
    @Autor Nicola
    Antoher one of The 5 Core Functionalty Functions
*/
void deleteList(ListElement* pToDelete) {
    ListElement* pTemporary = pToDelete;
    while (pTemporary != NULL) {
        pToDelete = pTemporary;
        pTemporary = pToDelete->pNext;
        free(pToDelete->pData);
        free(pToDelete);
    }
}

void printList(ListElement* firstElement, int ElementsToPrintPerIteration) {
    bool exit = false;
    while (!exit)
    {
    for (int i = 1; i != ElementsToPrintPerIteration && firstElement != NULL; i++, firstElement->pNext)
    {
        DataElement* data = firstElement->pData;
        double preis = data->Preis;
        printf("Data: %lf\n", preis);
    }
    exit = true;
}
}