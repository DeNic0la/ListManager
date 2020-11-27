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
	Those Things here
*/
ListElement* createLinkedList(int listSize);
void fillData(DataElement* toFill);
void deleteList(ListElement* pToDelete);


/*
	@autor Nicola
	@autor --
*/
int main() {
	
}

/*
	This is one of the 5 Base Functionality Functions
	@autor Nicola
	The list is Built Back to Front
*/
ListElement* createLinkedList(int listSize) {
	ListElement* pPreviousElement = NULL;
	for (int i = 0; i < listSize; i++) {
		ListElement* pListElement = (ListElement*)malloc(sizeof(ListElement));
		DataElement* pDataElement = (DataElement*)malloc(sizeof(DataElement));
		if (pListElement == NULL||pDataElement == NULL) return NULL;
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