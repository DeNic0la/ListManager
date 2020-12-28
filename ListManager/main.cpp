#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

/*
    @autor Nicola
*/
typedef struct Data {
    char Bez[4];
    int mapped;
    double Preis;
} DataElement;

/*
    @autor Nicola
*/
typedef struct Elm {
    struct Elm* pNext;
    struct Data* pData;
    // This may only be used for Quciksort
    struct Elm* pLast;
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
void N_MS_SortList(ListElement** firstElement);
void N_MS_Split(ListElement* source, ListElement** start, ListElement** mid);
ListElement* N_MS_SortedMerge(ListElement* a, ListElement* B);
int getShiftedChar(char* pStart);
void mapInt(ListElement* firstElement, int sortType);



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
        _strupr_s(userInput);
        if (strstr(userInput, "HELP")) {
            printf("Moegliche commands:\ncreateList - Erstellt eine Liste\nprintList - gibt die Liste aus\ndeleteList - Loescht die Liste\nexit - Beendet die Applikation\n");
        }
        else if (strstr(userInput, "CREATELIST")) {
            if (pStartOfTheList != NULL) {
                printf("Sie haben bereits eine erstellte Liste, moechten sie diese Loeschen [Y/N] ?\n");
                char delListAnswer[50] = { "empty" };
                fgets(delListAnswer, 50, stdin);
                if (isYes(delListAnswer)) {
                    deleteList(pStartOfTheList);
                    printf("Liste wurde geloescht\n");
                }
                else {
                    printf("Rueckkehr zum Menu\n");
                    continue;
                }
            }

            pStartOfTheList = createLinkedList(getIntFromUser("Wie viele elemente soll die Liste haben ?\n", false));
            printf("Liste wurde erstellt.\n");
            continue;

        }
        else if (strstr(userInput, "SORTLIST")) {
            int algorythm = 0;
            while (algorythm > 2 || algorythm < 1)
            {
                algorythm = getIntFromUser("Mit welchem Algorythmus moechten sie die Liste sortieren ?\n1 = Mergesort (Nicola)\n2 = Quicksort (Laurin)\n", false);
            }
            int sort = 0;
            while (sort > 4 || sort < 1)
            {
                sort = getIntFromUser("Wie Soll sortiert werden ? \n1 = Bez - A-Z\n2 = Bez - Z-A\n3 = Preis - Aufsteigend\n4 = Preis - Absteigend\n", false);
            }            
            clock_t startZeit = clock();
            mapInt(pStartOfTheList, sort);
            switch (algorythm) {
            case 1:
                N_MS_SortList(&pStartOfTheList);
                break;
            case 2:
                printf("Hey Laurin du chasch do inne 1 Function call mache, lueg wie ich s bi Case 1 gmacht han und machs s done glich");
                break;
            }            
            clock_t endZeit = clock();
            double dauer = ((double)endZeit - (double)startZeit) / (double)CLOCKS_PER_SEC;
            printf("Die Sortierung ist beendet und dauerte %.5lf Sekunden\n", dauer);

        }
        else if (strstr(userInput, "PRINTLIST")) {
            if (pStartOfTheList == NULL) {
                printf("Keine liste vorhanden.\n");
                continue;
            }
            printList(pStartOfTheList, getIntFromUser("Wie viele Elemente sollen auf einmal ausgegeben werden ? [-1 = alle] \n", true));
            continue;
        }
        else if (strstr(userInput, "DELETELIST")) {
            if (pStartOfTheList == NULL) {
                printf("Keine liste vorhanden.\n");
                continue;
            }
            deleteList(pStartOfTheList);
            printf("Liste wurde geloescht.\n");
            pStartOfTheList = NULL;
            continue;
        }
        else if (strstr(userInput, "EXIT")) {
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
        ListElement* pListElement = (ListElement*)malloc(sizeof(ListElement));
        DataElement* pDataElement = (DataElement*)malloc(sizeof(DataElement));
        if (pListElement == NULL || pDataElement == NULL) return NULL;
        fillData(pDataElement);
        pListElement->pData = pDataElement;
        pListElement->pNext = pPreviousElement;
        if (pPreviousElement != NULL) {
            pPreviousElement->pLast = pListElement;
        }
        pPreviousElement = pListElement;
    }
    return pPreviousElement;
}

/*
    @autor Nicola
*/
void mapInt(ListElement* firstElement, int sortType) { // TODO: Performance Test betwen - and ~
   
        switch (sortType)
        {
            /*
            * SortType
                1= Bez - A-Z
                2= Bez - Z-A
                3= Preis - Aufsteigend
                4= Preis - Absteigend
            */
        case 1:
            while (firstElement != NULL) {
                DataElement * element = firstElement->pData;
                element->mapped = -((element->Bez[0] << 16) + (element->Bez[1] << 8) + (element->Bez[2]));
                firstElement = firstElement->pNext;
            }
            break;
        case 2:
            while (firstElement != NULL) {
                DataElement* element = firstElement->pData;
                element->mapped = (element->Bez[0] << 16) + (element->Bez[1] << 8) + (element->Bez[2]);
                firstElement = firstElement->pNext;
            }
            break;
        case 3:
            while (firstElement != NULL) {
                DataElement* element = firstElement->pData;
                element->mapped = -((int)(element->Preis*10));
                firstElement = firstElement->pNext;
            }
            break;
        case 4:
            while (firstElement != NULL) {
                DataElement* element = firstElement->pData;
                element->mapped = ((int)(element->Preis * 10));
                firstElement = firstElement->pNext;
            }
            break;
        default:
            break;
        }       
    
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
    @Autor Laurin
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
            printf("|     %3s     |     %05.1f      |", data->Bez, data->Preis);
            //printf("     %5i      |", data->mapped);// this line is for insight in the Mapped int
            printf("\n");
        }
        printf("|-------------|----------------|\n");
        char result[10] = { 'N' };
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
    @autor Laurin
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
    @autor Laurin
*/
int getIntFromUser(const char* messageToUser, bool allowNegative) {
    while (true) {
        printf(messageToUser);
        char input[100];
        fgets(input, 100, stdin);
        int userInt = atoi(input);
        if (userInt != 0 && (allowNegative || userInt > 0))
            return userInt;

    }

}

/*
    @autor Nicola
*/
void N_MS_SortList(ListElement** firstElement) {

    ListElement* head = *firstElement;
    if ((head == NULL) || (head->pNext == NULL)) {
        return;
    }

    ListElement* a;
    ListElement* b;
    N_MS_Split(head, &a, &b);

    /*Recursive sort*/
    N_MS_SortList(&a);
    N_MS_SortList(&b);
    *firstElement = N_MS_SortedMerge(a, b);
}
/*
    @autor Nicola
*/
void N_MS_Split(ListElement* source,
    ListElement** start, ListElement** mid) {
    ListElement* fast;
    ListElement* slow;
    slow = source;
    fast = source->pNext;

    while (fast != NULL) {
        fast = fast->pNext;
        if (fast != NULL) {
            slow = slow->pNext;
            fast = fast->pNext;
        }
    }
    *start = source;
    *mid = slow->pNext;
    slow->pNext = NULL;
}

ListElement* N_MS_SortedMerge(ListElement* a, ListElement* b) {

    ListElement* result = NULL;


    if (a->pData->mapped > b->pData->mapped) {
        result = a;
        a = a->pNext;
    }
    else
    {
        result = b;
        b = b->pNext;
    }
    ListElement* current = result;

    while (a != NULL && b != NULL)
    {
        if (a->pData->mapped > b->pData->mapped) {

            current->pNext = a;
            current = a;
            a = a->pNext;
        }
        else
        {
            current->pNext = b;
            current = b;
            b = b->pNext;
        }
    }
    if (a == NULL)
        current->pNext = b;
    else if (b == NULL)
        current->pNext = a;

    return result;
}
