#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

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
int getIntFromUser(const char* messageToUser, bool allowNegative);



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
            printf("Mögliche commands:\ncreateList - Erstellt eine Liste\nprintList - gibt die Liste aus\ndeleteList - Loescht die Liste\nexit - Beendet die Applikation\n");
        }
        else if (strcmp(userInput, "createList\n") ==0|| strcmp(userInput, "createList\n")==0) {
            if (pStartOfTheList != NULL) {
                printf("Sie haben bereits eine erstellte Liste, moechten sie diese Loeschen [Y/N] ?\n");
                char delListAnswer[50] = { "empty" };
                fgets(delListAnswer, 50, stdin);
                if (isYes) {
                    deleteList(pStartOfTheList);
                    printf("Liste wurde geloescht");
                }
                else {
                    printf("Rückkehr zum Menu");
                    continue;
                }
            }           
                       
            pStartOfTheList = createLinkedList(getIntFromUser("Wie viele elemente soll die Liste haben ?\n", false));
            printf("Liste wurde erstellt.\n");
            continue;           
            
        }
        else if (strcmp(userInput, "printList\n")==0 || strcmp(userInput, "printList\n")==0) {
            if (pStartOfTheList == NULL) {
                printf("Keine liste vorhanden.\n");
                continue;
            }            
            printList(pStartOfTheList, getIntFromUser("Wie viele Elemente sollen auf einmal ausgegeben werden ? [-1 = alle] \n", true));
            continue;
        }
        else if (strcmp(userInput, "deleteList\n") == 0 || strcmp(userInput, "DeleteList\n") == 0) {
            if (pStartOfTheList == NULL) {
                printf("Keine liste vorhanden.\n");
                continue;
            }
            deleteList(pStartOfTheList);
            printf("Liste wurde geloescht.\n");
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
    @Autor Laurin
	kinda necessary
*/
int generateRandomInt(int min, int max) {
 	return (rand() % (max - min + 1)) + min;
}


/*
	@Autor Laurin
*/
void fillData(DataElement* toFill) {
	char bezArray[4];

	for (int i = 0; i < 3; i++)
	{
		char c = generateRandomInt(65, 90);
		bezArray[i] = c;
	}

	bezArray[3] = '\0';

	strcpy_s(toFill->Bez, bezArray);
    toFill->Preis = generateRandomInt(100, 9999) / 10.0;
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
        printf("Daten werden Ausgegeben:\n");
        printf("| ### Bez ### | ### Nummer ### |\n");
        printf("|-------------|----------------|\n");
        for (int i = 0; i != ElementsToPrintPerIteration && firstElement->pNext != NULL; i++)
        {
            firstElement = firstElement->pNext;
            DataElement* data = firstElement->pData;
            //This Line is Preferences:
            //printf("|-------------|----------------|\n");// This line can be Comment or code, visual changes only
            printf("|     %3s     |     %05.1f      |\n", data->Bez, data->Preis);
            //printf("%s: %04.1f\n", data->Bez, preis);
        }
        printf("|-------------|----------------|\n");
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

/*
    @autor Nicola
*/
int getIntFromUser(const char* messageToUser, bool allowNegative) {
    while (true) {
        printf(messageToUser);
        char input[100];
        fgets(input, 100, stdin);
        int userInt = atoi(input);
        if (userInt != 0 && (allowNegative || userInt>0)) 
            return userInt;
        
    }
    
}