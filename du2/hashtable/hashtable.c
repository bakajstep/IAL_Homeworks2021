/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {

    if(table == NULL){
        return;
    }
    for (int i = 0; i < HT_SIZE;++i) {
        (*table)[i] = NULL;
    }
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {

    if(table == NULL || key == NULL){ //kontrola zda jsme vubec dostali validni parametry
        return NULL;
    }

    ht_item_t *iterator;
    iterator = (*table)[get_hash(key)];

    while (iterator != NULL){
        if (!strcmp(iterator->key,key)) {
            return iterator;
        }
        iterator = iterator->next;
    }
    return NULL;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {

    if(table == NULL || key == NULL){ //kontrola zda jsme vubec dostali validni parametry
        return;
    }

    ht_item_t *iterator;
    iterator = ht_search(table,key);

    if (iterator){
        iterator->value = value; // shodnost klicu
    } else {
        ht_item_t *item;
        item = (ht_item_t *) malloc(sizeof(ht_item_t));
        if (item){
                item->key = key;
                item->value = value;
                item->next = NULL;
                iterator = (*table)[get_hash(key)];
                if (iterator){
                    item->next = iterator; // pripojeni noveho prvku
                }
                (*table)[get_hash(key)] = item; //vlozeni noveho upraveneho zaznamu
        } else{
            return; // malloc se nezdaril
        }
    }
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {

    if(table == NULL || key == NULL){ //kontrola zda jsme vubec dostali validni parametry
        return NULL;
    }

    ht_item_t *iterator;
    iterator = ht_search(table,key);
    while (iterator){
        if (!strcmp(iterator->key,key)) {
            return &iterator->value;
        }
        iterator = iterator->next;
    }
  return NULL;
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {

    ht_item_t *iterator;
    ht_item_t *prev;


    if(table == NULL || key == NULL){ //kontrola zda jsme vubec dostali validni parametry
        return;
    }

    iterator = (*table)[get_hash(key)];

    //kontrola zda to neni prvni prvek
    if (!strcmp(iterator->key,key)) {
        (*table)[get_hash(key)] = iterator->next;
        free(iterator);
        return;
    } else {
        while (iterator){
            if (!strcmp(iterator->key,key)){
                prev->next = iterator->next;
                free(iterator);
                return;
            }
            prev = iterator;
            iterator = iterator->next;
        }
    }
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {

    ht_item_t *iterator;

    if(table == NULL){ //kontrola zda jsme vubec dostali validni parametry
        return;
    }

    for (int i = 0; i < HT_SIZE;++i) {
        while ((*table)[i]){
            iterator = (*table)[i];
            (*table)[i] = (*table)[i]->next;
            free(iterator);
        }
        (*table)[i] = NULL; //uvedeni do stavu po initu
    }
}
