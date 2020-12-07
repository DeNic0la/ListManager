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
bool isYes(char* inputstring);


/*
    @autor Nicola
    @autor --
*/
int main() {
    printf("Das Programm List-Manager wurde gestartet. Fuer Hilfe benuetzen sie den Command 'help'\n");
    ListElement* pStartOfTheList = NULL;
    bool closeTheApplication = false;
    while (!closeTheApplication) {
        char userInput[50] = { "empty" };
        fgets(userInput, 50, stdin);
        if (strcmp(userInput, "help\n") == 0|| strcmp(userInput, "Help\n")==0) {
            printf("Mögliche commands:\ncreateList - Erstellt eine Liste\nprintList - gibt die Liste aus\ndeleteList - Löscht die Liste\n");
        }
        else if (strcmp(userInput, "createList\n") ==0|| strcmp(userInput, "createList\n")==0) {
            if (pStartOfTheList != NULL) {
                printf("Sie haben bereits eine erstellte Liste, moechten sie diese Loeschen [Y/N] ?\n");
                char delListAnswer[50] = { "empty" };
                fgets(delListAnswer, 50, stdin);
                if (isYes) {
                    deleteList(pStartOfTheList);
                    printf("Liste wurde gelöscht");
                }
                else {
                    printf("Rückkehr zum Menu");
                    continue;
                }
            }           
            
            int anzahlElementeDerListe = 0;
            while (anzahlElementeDerListe <= 0) {
                printf("Wie viele elemente soll die Liste haben ?\n");
                scanf_s("%d", &anzahlElementeDerListe);
            }
            pStartOfTheList = createLinkedList(anzahlElementeDerListe);
            printf("Liste wurde erstellt.\n");
            continue;           
            
        }
        else if (strcmp(userInput, "printList\n")==0 || strcmp(userInput, "printList\n")==0) {
            int elementeProIteration = 0;
            while (elementeProIteration != -1 || elementeProIteration > 0) {
                printf("Wie viele Elemente sollen auf einmal ausgegeben werden ? [-1 = alle] \n");
                scanf_s("%d", &elementeProIteration);
            }
            printList(pStartOfTheList, elementeProIteration);
            continue;
        }
        else if (strcmp(userInput, "exit\n") == 0 || strcmp(userInput, "Exit\n") == 0) {
            return 0;
        }
    }
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
/*
    @autor Nicola
*/
void printList(ListElement* firstElement, int ElementsToPrintPerIteration) {
    bool exit = false;
    while (!exit)
    {
        for (int i = 0; i != ElementsToPrintPerIteration && firstElement->pNext != NULL; i++)
        {
            firstElement = firstElement->pNext;
            DataElement* data = firstElement->pData;
            double preis = data->Preis;
            printf("Data: %lf\n", preis);
        }
        char result[10] = {'N'};
        if (firstElement->pNext == NULL) {
            printf("Es Wurden alle Elemente der Liste ausgegeben. Klicken sie eine Belibige Taste um zum Menu zurueckzukehren\n");
            system("pause");
        }
        else
        {
            printf("Es wurden %i Elemente ausgegeben. Möchten sie eie weiter Iteration an Elementen ausgeben ? [Y/N]\n", ElementsToPrintPerIteration);
            fgets(result, 10, stdin);
        }
        
        exit = !isYes(result);
    }
}

/*
    @autor Nicola
*/
bool isYes(char* inputstring) {
    if (*inputstring == 'Y' ||
        *inputstring == 'J' ||
        *inputstring == 'y' ||
        *inputstring == 'j') {
        return true;
    }
    return false;
}