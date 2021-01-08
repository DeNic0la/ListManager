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
void L_QS_Swap(Data* a, Data* b);
ListElement* L_QS_LastELement(ListElement* root);
ListElement* L_QS_Partition(ListElement* low, ListElement* high);
void L_QS__quickSort(ListElement* low, ListElement* high);
void L_QS_quickSort(ListElement* head);
void N_MS_SortList(ListElement** firstElement);
void N_MS_Split(ListElement* source, ListElement** start, ListElement** mid);
ListElement* N_MS_SortedMerge(ListElement* a, ListElement* B);
void mapInt(ListElement* firstElement, int sortType);



/*
    @autor Nicola
    @autor (Laurin)
*/
int main() {
    // Anfangsmessage
    printf("Das Programm List-Manager wurde gestartet. Fuer Hilfe benuetzen sie den Command 'help'\n");
    ListElement* pStartOfTheList = NULL; // Variable Erstellen damit man in der gesamten main Funktion darauf zugreiffen kann
    // Diese Schlaufe wird solag ausgeführt bis diese Variable true ist. Dann wird di Applikation beendet
    bool closeTheApplication = false;
    while (!closeTheApplication) {
        // Eingabe vom Benutzer entgegennehmen und anhand dieser den Entsprechenden Code Teil im If/Else IF Konstrukt aufrufen
        // Wenn die Eingabe nicht zugeordnet werden kann oder der Entsprechende Code Teil abgeschlossen wurde wird die erneut eine 
        // Benutzereingabe eintgegengenommen
        char userInput[50] = { "empty" };
        fgets(userInput, 50, stdin);
        _strupr_s(userInput);
        if (strstr(userInput, "HELP")) {
            // Auflistung der Befehle für den Benuzter
            printf("Moegliche commands:\ncreateList - Erstellt eine Liste\nprintList - gibt die Liste aus\ndeleteList - Loescht die Liste\nexit - Beendet die Applikation\n");
        }
        else if (strstr(userInput, "CREATELIST")) {
            // Listenerstellungsprozess
            if (pStartOfTheList != NULL) { // Wenn eine Liste Existier wird gefragt ob diese Gelöscht werden soll
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
            // Liste erstellen
            pStartOfTheList = createLinkedList(getIntFromUser("Wie viele elemente soll die Liste haben ?\n", false));
            if (pStartOfTheList == NULL) { // Liste war Fehlerhaft -> Message
                printf("Fehler beim Erstellen der Liste\n");
                continue;
            }
            // Message
            printf("Liste wurde erstellt.\n");
            continue;

        }
        else if (strstr(userInput, "SORTLIST")) {
            // Benuztereingabe zwischen 1 und 2 entgegennehmen für Sortieralgorythmuss
            int algorythm = 0;
            while (algorythm > 2 || algorythm < 1)
            {
                algorythm = getIntFromUser("Mit welchem Algorythmus moechten sie die Liste sortieren ?\n1 = Mergesort (Nicola)\n2 = Quicksort (Laurin)\n", false);
            }
            // Benuzereingabe für die Sortierart
            int sort = 0;
            while (sort > 4 || sort < 1)
            {
                sort = getIntFromUser("Wie Soll sortiert werden ? \n1 = Bez - A-Z\n2 = Bez - Z-A\n3 = Preis - Aufsteigend\n4 = Preis - Absteigend\n", false);
            }
            // Vor dem Start des Sortierprozess clock starten
            clock_t startZeit = clock();
            // mappedInt wert in jedem Feld anhand des sort werts setzen, Dies ist auch teil des Sortiervorgangs
            mapInt(pStartOfTheList, sort);
            switch (algorythm) { // Ruft die Entsprechende Funktion
            case 1:
                N_MS_SortList(&pStartOfTheList);
                break;
            case 2:
                L_QS_quickSort(pStartOfTheList);
                break;
            }
            // Zeitmessung beenden und ausgeben
            clock_t endZeit = clock();
            double dauer = ((double)endZeit - (double)startZeit) / (double)CLOCKS_PER_SEC;
            printf("Die Sortierung ist beendet und dauerte %.5lf Sekunden\n", dauer);

        }
        else if (strstr(userInput, "PRINTLIST")) {  // Gibt liste aus wenn vorhanden
            if (pStartOfTheList == NULL) {
                printf("Keine liste vorhanden.\n");
                continue;
            }
            printList(pStartOfTheList, getIntFromUser("Wie viele Elemente sollen auf einmal ausgegeben werden ? [-1 = alle] \n", true));
            continue;
        }
        else if (strstr(userInput, "DELETELIST")) { // Liste Löschen wenn vorhanden
            if (pStartOfTheList == NULL) {
                printf("Keine liste vorhanden.\n");
                continue;
            }
            deleteList(pStartOfTheList);
            printf("Liste wurde geloescht.\n");
            pStartOfTheList = NULL;
            continue;
        }
        else if (strstr(userInput, "EXIT")) { // Beendet die Applikation
            return 0;
        }
    }
}

/*
    @autor Nicola
    The list is Built Back to Front
    listSize ist die Anzahl Elemente welche die Liste haben soll
*/
ListElement* createLinkedList(int listSize) {
    ListElement* pPreviousElement = NULL; // Für zugriff auserhalb der Schlaufe
    for (int i = 0; i < listSize; i++) { // Iteration für Anzahl Elemente welche mitgegeben wurde
        // Elemente "Erstellen"
        ListElement* pListElement = (ListElement*)malloc(sizeof(ListElement));
        DataElement* pDataElement = (DataElement*)malloc(sizeof(DataElement));
        if (pListElement == NULL || pDataElement == NULL) { // Wenn eines der Beiden NULL ist bedeudet dass das der Speicherzugriff Fehlgeschlagen ist
            // Da pListElement nicht Existiert ist Hier der Letzte Punkt anwelchem auf die Liste zugegriffen werden Kann, die Liste wird
            // Hier Gelöscht damit der Speicher wieder Freigegeben werden kann.
            deleteList(pPreviousElement);
            return NULL;
        }
        fillData(pDataElement); // Feld wird befüllt
        pListElement->pData = pDataElement; // Pointer auf Data Setzen
        pListElement->pNext = pPreviousElement; // Pointer auf "Next" Setzen (Liste wird von Hinten nach vorne aufgebaut)
        if (pPreviousElement != NULL) { // Double Linking
            pPreviousElement->pLast = pListElement;
        }
        pPreviousElement = pListElement;
    }
    pPreviousElement->pLast = NULL;
    return pPreviousElement;
}

/*
    @autor Nicola
    Sort Type für sortierart, firstElement als Refferenz auf die Liste Für eine Umgekehrte Sortierung wird einfach der mappedInt vor dem Speichern invertiert
*/
void mapInt(ListElement* firstElement, int sortType) {

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
        // In Jedem Element wird die Bezeichnung mit Bit-Shift in einen Integer wert Umgewandelt welcher Invertiert wird und im Data Element abgespeichert werden
        while (firstElement != NULL) {
            DataElement* element = firstElement->pData;
            element->mapped = -((element->Bez[0] << 16) + (element->Bez[1] << 8) + (element->Bez[2]));
            firstElement = firstElement->pNext;
        }
        break;
    case 2:
        // In Jedem Element wird die Bezeichnung mit Bit-Shift in einen Integer wert Umgewandelt und im Data Element abgespeichert werden
        while (firstElement != NULL) {
            DataElement* element = firstElement->pData;
            element->mapped = (element->Bez[0] << 16) + (element->Bez[1] << 8) + (element->Bez[2]);
            firstElement = firstElement->pNext;
        }
        break;
    case 3:
        // Der wert von Preis wird verlustfrei zu einem Int verwandelt und Invertiert
        while (firstElement != NULL) {
            DataElement* element = firstElement->pData;
            element->mapped = -((int)(element->Preis * 10));
            firstElement = firstElement->pNext;
        }
        break;
    case 4:
        // Der wert von Preis wird verlustfrei zu einem Int verwandelt
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
    Ausgabe der Liste
*/
void printList(ListElement* firstElement, int ElementsToPrintPerIteration) {
    bool exit = false;
    while (!exit)
    {
        //Beginn ausgabe
        printf("Daten werden Ausgegeben:\n");
        printf("| ### Bez ### | ### Nummer ### |\n");
        printf("|-------------|----------------|\n");
        for (int i = 0; i != ElementsToPrintPerIteration && firstElement->pNext != NULL; i++) // For Schleife für Anzahl elemente 
        {
            firstElement = firstElement->pNext;
            DataElement* data = firstElement->pData;
            // Kosmetische Option:
            //printf("|-------------|----------------|\n");// This line can be Comment or code, visual changes only
            printf("|     %3s     |     %05.1f      |", data->Bez, data->Preis);
            //printf("     %5i      |", data->mapped);// Zusatzfeature für Tiefer einsicht in die Sortierung
            printf("\n");
        }
        printf("|-------------|----------------|\n");
        // Lässt den Benuzer sofern nicht die Komplette liste ausgegeben wurde eine weiter Iteration ausgeben
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

        exit = !isYes(result); // Entscheidet ob die Schlaufe weiter läuft
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
    @autor Nicola
    Hilfsfunktion um einen Int-Wert vom Benuzer zu erhalten (Scanf hat wegen \n nicht funktioniert)
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
    @autor Laurin
*/
void L_QS_Swap(Data** a, Data** b)
{
    Data* temp = *a;
    *a = *b;
    *b = temp;
}

/*
    @autor Laurin
*/
ListElement* L_QS_LastELement(ListElement* root)
{
    while (root && root->pNext)
        root = root->pNext;
    return root;
}

/*
    @autor Laurin
*/
ListElement* L_QS_Partition(ListElement* low, ListElement* high)
{
    int  pivot = high->pData->mapped;

    ListElement* iteratror = low->pLast;

    for (ListElement* j = low; j != high; j = j->pNext)
    {
        // If current element is smaller than the pivot 
        if (j->pData->mapped <= pivot)
        {
            iteratror = (iteratror == NULL) ? low : iteratror->pNext;
            L_QS_Swap(&(iteratror->pData), &(j->pData));
        }
    }
    iteratror = (iteratror == NULL) ? low : iteratror->pNext;
    L_QS_Swap(&(iteratror->pData), &(high->pData));
    return iteratror;
}

/*
    @autor Laurin
*/
void L_QS__quickSort(ListElement* low, ListElement* high)
{
    if (high != NULL && low != high && low != high->pNext)
    {
        ListElement* p = L_QS_Partition(low, high);
        L_QS__quickSort(low, p->pLast);
        L_QS__quickSort(p->pNext, high);
    }
}

/*
    @autor Laurin
*/
void L_QS_quickSort(ListElement* head) {
    ListElement* h = L_QS_LastELement(head);

    L_QS__quickSort(head, h);
}

/*
    @autor Nicola
    Main funktion des Mergesort algorythmus, Pointer Pointer auf den Anfang der Liste damit dieser auch verlegt werden kann
*/
void N_MS_SortList(ListElement** firstElement) {

    ListElement* head = *firstElement;
    if ((head == NULL) || (head->pNext == NULL)) { // Falls eine Liste mit 0 oder 1 Element mitgegeben wurde ist diese Funktion fertig
        return; // (Die Funktion wird Rekursiv aufgerufen)
    }

    // Halbiere die Liste
    ListElement* a;
    ListElement* b;
    N_MS_Split(head, &a, &b);

    /*Recursive sort*/
    N_MS_SortList(&a);
    N_MS_SortList(&b);
    *firstElement = N_MS_SortedMerge(a, b); // "Rückgabe" übertrschreibt den Pointer
}
/*
    @autor Nicola
    Halbiert die Liste
*/
void N_MS_Split(ListElement* source,
    ListElement** start, ListElement** mid) {
    ListElement* fast;
    ListElement* slow;
    slow = source;
    fast = source->pNext;
    // Fast ist Doppelt so Schnell, wenn Fast mit der Liste fertig ist, ist Slow genau in der Mitte
    while (fast != NULL) {
        fast = fast->pNext;
        if (fast != NULL) {
            slow = slow->pNext;
            fast = fast->pNext;
        }
    }
    // "Returne die Werte" und Halbier die Liste durch "Löschen" eines Verweiss
    *start = source;
    *mid = slow->pNext;
    slow->pNext = NULL;
}

/*
  @autor Nicola
*/
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
