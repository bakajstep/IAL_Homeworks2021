/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {

    *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {

    // kontrola prazdneho stromu
    while (tree != NULL){
        if (tree->key == key) {
            *value = tree->value;
            return true;
        }
        if(tree->key < key){
            tree = tree->right;
        } else {
            tree = tree->left;
        }
    }
    return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {

    while (*tree != NULL){
        if((*tree)->key < key){
            tree = &(*tree)->right;
        } else if ((*tree)->key > key) {
            tree = &(*tree)->left;
        }else if((*tree)->key == key){
            (*tree)->value = value;
            return;
        }
    }
    (*tree) = (bst_node_t*)malloc(sizeof(bst_node_t));
    (*tree)->key = key;
    (*tree)->value = value;
    (*tree)->left = NULL;
    (*tree)->right = NULL;
    return;
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */

void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {

    bst_node_t *father = *tree;

    while((*tree)->right != NULL){ // dostaneme se na pozadovanej uzel
        father = *tree;
        tree = &(*tree)->right;
    }
    if ((*tree)->left){ //kontrola zda nema leveho potomka
        target->key = (*tree)->key;
        target->value = (*tree)->value;
        (*tree)->key = (*tree)->left->key;
        (*tree)->value = (*tree)->left->value;
        free((*tree)->left);
        (*tree)->left = NULL;
    }else {
        target->key = (*tree)->key;
        target->value = (*tree)->value;
        free(*tree);
        father->right = NULL;
    }
     return;
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
    bst_node_t *delete;

    while((*tree) != NULL){
        if ((*tree)->key < key){
            if ((*tree)->right && (*tree)->right->key == key){

                delete = (*tree)->right;

                if (delete->right && delete->left) { // dva podstromy
                    bst_replace_by_rightmost(delete,&(delete->left));
                    return;
                } else if (delete->right && !delete->left) { // pouze pravej
                    (*tree)->right = delete->right;
                } else if (!delete->right && delete->left) { // pouze levej
                    (*tree)->right = delete->left;
                } else { // ani jeden
                    (*tree)->right = NULL;
                }
                free(delete);
                return;
            }
            tree = &(*tree)->right;
        }
        else if ((*tree)->key > key){
            if ((*tree)->left && (*tree)->left->key == key){

                delete = (*tree)->left;

                if (delete->right && delete->left) { // dva podstromy
                    bst_replace_by_rightmost(delete,&(delete->left));
                    return;
                } else if (delete->right && !delete->left) { // pouze pravej
                    (*tree)->left = delete->right;
                } else if (!delete->right && delete->left) { // pouze levej
                    (*tree)->left = delete->left;
                } else { // ani jeden
                    (*tree)->left = NULL;
                }
                free(delete);
                return;
            }
            tree = &(*tree)->left;
        }
        else if ((*tree)->key == key) { // hledany klic je koren
            delete = *tree;

            if (delete->right && delete->left) { // dva podstromy
                bst_replace_by_rightmost(delete,&(delete->left));
                return;
            } else if (delete->right && !delete->left) { // pouze pravej
                *tree = delete->right;
            } else if (!delete->right && delete->left) { // pouze levej
                *tree = delete->left;
            } else { // ani jeden
                *tree = NULL;
            }
            free(delete);
        }
    }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
    stack_bst_t s; //zasobnik
    bst_node_t *delete; //mazanej uzel
    stack_bst_init(&s); // inicializace zasobniku
    do {
        if (*tree == NULL){
            if(!stack_bst_empty(&s)){
                *tree = stack_bst_top(&s);
                stack_bst_pop(&s);
            }
        } else {
            if ((*tree)->right != NULL){
                stack_bst_push(&s,(*tree)->right);
            }
            delete = *tree;
            *tree = (*tree)->left;
            free(delete);
        }
    } while ((*tree != NULL) || (!stack_bst_empty(&s)));

}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
    while (tree != NULL){
        stack_bst_push(to_visit,tree);
        bst_print_node(tree);
        tree = tree->left;
    }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
    stack_bst_t s; //zasobnik
    stack_bst_init(&s);
    bst_leftmost_preorder(tree,&s);
    while (!stack_bst_empty(&s)){
        tree = stack_bst_top(&s);
        stack_bst_pop(&s);
        bst_leftmost_preorder(tree->right,&s);
    }
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
    while (tree != NULL){
        stack_bst_push(to_visit,tree);
        tree = tree->left;
    }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
    stack_bst_t s; //zasobnik
    stack_bst_init(&s);
    bst_leftmost_inorder(tree,&s);
    while (!stack_bst_empty(&s)){
        tree = stack_bst_top(&s);
        stack_bst_pop(&s);
        bst_print_node(tree);
        bst_leftmost_inorder(tree->right,&s);
    }
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit, stack_bool_t *first_visit) {
    while (tree != NULL){
        stack_bst_push(to_visit,tree);
        stack_bool_push(first_visit,true);
        tree = tree->left;
    }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
    stack_bst_t s; //zasobnik uzlu
    stack_bool_t s1; // zasobnik boolu
    bool fromLeft;

    stack_bst_init(&s);
    stack_bool_init(&s1);
    bst_leftmost_postorder(tree,&s,&s1);
    while (!stack_bst_empty(&s)){
        tree = stack_bst_top(&s);
        fromLeft = stack_bool_top(&s1);
        stack_bool_pop(&s1);
        if (fromLeft) { // prichazi uzel z leva
            stack_bool_push(&s1,false);
            bst_leftmost_postorder(tree->right,&s,&s1);
        }else { // zprava, odstrani a zpracuje dal
            stack_bst_pop(&s);
            bst_print_node(tree);
        }
    }
}
