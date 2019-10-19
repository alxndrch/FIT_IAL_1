
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2019
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
    // inicializace
    L->First = NULL;
    L->Last = NULL;
    L->Act = NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/

    if(L != NULL){    
	L->Act = L->First;

	while(L->First != L->Last){
	    tDLElemPtr del = L->Act->rptr;

	    L->Act->rptr = del->rptr;

	    if(del == L->Last) L->Last = L->Act;
	    else del->rptr->lptr = L->Act;

	    free(del);
	}

	if(L->Act != NULL) free(L->Act);

	// uvedeni seznamu do puvodniho stavu
	L->First = NULL;
	L->Last = NULL;
	L->Act = NULL;
    }

}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

    struct tDLElem *new = (struct tDLElem*) malloc(sizeof(struct tDLElem));

    if(new == NULL) DLError();
    else{
	// inicializace noveho prvku 
	new->data = val;
	new->lptr = NULL;
	new->rptr = L->First;

	//pokud existuje prvni prvek, stava se druhym
	// jinak je v seznamu pouze jeden prvek
	if(L->First != NULL) L->First->lptr = new;
	else L->Last = new;

	L->First = new; 
    }

}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/ 	
    
    struct tDLElem *new = (struct tDLElem*) malloc(sizeof(struct tDLElem));

    if(new == NULL) DLError();
    else{
	// inicializace noveho prvku
	new->data = val;
	new->lptr = L->Last;
	new->rptr = NULL;

	// pokud existuje posledni prvek, stava se predposlednim
	// jinak je v seznamu pouze jeden prvek
	if(L->Last != NULL) L->Last->rptr = new;
	else L->First = new;

	L->Last = new; 
    }

}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/

    L->Act = L->First; // prvni prvek je aktivni

}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/

    L->Act = L->Last; // posledni prvek je aktivni

}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
    // seznam nesmi byt prazdny
    if(L == NULL || L->First == NULL) DLError();
    else *val = L->First->data;
    
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

    // seznam nesmi byt prazdy
    if(L == NULL || L->Last == NULL) DLError();
    else *val = L->Last->data;

}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/

    if(L->First != NULL){
	// musi existovat prvni prvek

	tDLElemPtr temp = L->First; // ulozeni prvniho prvku
    
	if(L->First == L->Act) L->Act = NULL; // odstraneni aktivity, pokud byla

	if(L->First == L->Last){
	    // pokud byl v seznamu jeden prvek
	    L->First = NULL;
	    L->Last = NULL;
	}else{
	    // druhy prvek se stava prvnim 
	    L->First = L->First->rptr;
	    L->First->lptr = NULL;
	}

	free(temp); // dealokace ulozeneho prvku
    }

}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/ 
	
    if(L->Last != NULL){
	//musi existovat posledni prvek
    
	tDLElemPtr temp = L->Last; // ulozeni posledniho prvku

	if(L->Last == L->Act) L->Act = NULL; // odstraneni aktivity, pokud byla
    
	if(L->Last == L->First){
	    // pokud byl v seznamu jeden prvek
	    L->First = NULL;
	    L->Last = NULL;	
	}else{
	    // predposledni je ted poslednim a nema nasledovnika
	    L->Last = L->Last->lptr;
	    L->Last->rptr = NULL;
	}

	free(temp); // dealokace ulozeneho prvku
    }


}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/

    if(L->Act != NULL && L->Act != L->Last){
	// musi existovat aktivni prvek, a nesmi byt posledni

	tDLElemPtr del = L->Act->rptr; // ulozeni prvku k odstraneni

	L->Act->rptr = del->rptr; // presunuti nasledovniku

	//aktivni prvek se stava poslednim pokud byl predposledni
	if(del == L->Last) L->Last = L->Act;
	else del->rptr->lptr = L->Act; // L->Act<---[del->rptr]

	free(del); // dealokace prvku
    }

}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/

    if(L->Act != NULL && L->Act != L->First){
	// musi existovat aktivni prvek, a nesmi byt prvni

	tDLElemPtr del = L->Act->lptr; // ulozeni prvku k odstraneni

	L->Act->lptr = del->lptr; // presunuti predchudcu
    
	// aktivni prvek se stava prvnim, pokud byl druhy
	if(del == L->First) L->First = L->Act;
	else del->lptr->rptr = L->Act; // [del->lptr]--->L->Act
    
	free(del); // dealokace prvku
    }
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

    if(L->Act != NULL){
	// provede se pokud existuje aktivni prvek

	struct tDLElem* new = (struct tDLElem*) malloc(sizeof(struct tDLElem));

	if(new != NULL){
	    // inicialzace noveho prvku
	    new->data = val;
	    new->lptr = L->Act;
	    new->rptr = L->Act->rptr;

	    // pripojeni noveho prvku k aktivnimu
	    L->Act->rptr = new;

	    // novy prvek se stava poslednim, pokud je aktivita na konci
	    // jinak naslednovnik aktivniho [L->Act->rtpr] sousedi zleva s novym
	    if(L->Act == L->Last) L->Last = new;
	    else L->Act->rptr->lptr = new;

	}else{
	    DLError();
	}

    }

}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

    if(L->Act != NULL){
	// provede se pokud existuje aktivni prvek

	struct tDLElem* new = (struct tDLElem*) malloc(sizeof(struct tDLElem));

	if(new != NULL){
	    // inicializace noveho prvku
	    new->data = val;
	    new->lptr = L->Act->lptr;
	    new->rptr = L->Act;


	    //pripojeni noveho prvku k aktivnimu
	    L->Act->lptr = new;

	    // novy prvek se stava prvnim, pokud je aktivita na zacatku
	    // jinak predchudce aktivniho [L->Act->ltpr] sousedi zprava s novym
	    if(L->Act == L->First) L->First = new; 
	    else L->Act->lptr->rptr = new;

	}else{
	    DLError();
	}

    }
	
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
		
    if(L->Act != NULL) *val = L->Act->data; // vrati hodnotu aktivniho prvku
    else DLError();	
	
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/

    if(L->Act != NULL) L->Act->data = val;
	
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/

    if(L->Act != NULL) L->Act = L->Act->rptr; // presunuti aktivity na pravy prvek
	
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
	
    if(L->Act != NULL) L->Act = L->Act->lptr; // presunuti aktivity na levy prvek 

}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/

    return L->Act != NULL? 1 : 0; // pokud je seznam aktivni vraci 1, jinak 0

}

/* Konec c206.c*/
