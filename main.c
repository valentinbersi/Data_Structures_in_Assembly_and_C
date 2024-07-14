#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "lib.h"

void startTest();
void testFiveCardDeck(void *const deck,
                      funcAddLast_t *const addLast,
                      funcGet_t *const get,
                      funcDelete_t *const delete,
                      funcPrint_t *const print);

int main (void){
    startTest();
    return 0;
}

void startTest(){
    printf("Test making a deck with an array\n");
    testFiveCardDeck(arrayNew(TypeCard, 5),
                     (void (*)(void *, void *)) arrayAddLast,
                     (void *(*)(void *, uint8_t)) arrayGet,
                     (void (*)(void *)) arrayDelete,
                     (void (*)(void *, FILE *)) arrayPrint);

    printf("Test making a deck with a list\n");
    testFiveCardDeck(listNew(TypeCard),
                     (void (*)(void *, void *)) listAddLast,
                     (void *(*)(void *, uint8_t)) listGet,
                     (void (*)(void *)) listDelete,
                     (void (*)(void *, FILE *)) listPrint);
}

void testFiveCardDeck(void *const deck,
                      funcAddLast_t *const addLast,
                      funcGet_t *const get,
                      funcDelete_t *const delete,
                      funcPrint_t *const print){

    char *suit = "swords";
    int32_t numbers[5] = {1, 2, 3, 4, 5};
    card_t *cards[5];

    for (int i = 0; i < 5; i++) cards[i] = cardNew(suit, &numbers[i]);
    for (int i = 0; i < 5; i++) addLast(deck, cards[i]);
    for (int i = 0; i < 5; i++) cardDelete(cards[i]);
    for (int i = 0; i < 5; i++) cards[i] = get(deck, i);
    for (int i = 0; i < 5; i++) assert(*cardGetNumber(cards[i]) == numbers[i] && !strCmp(cardGetSuit(cards[0]), suit));

    print(deck, stdout);
    printf("\n");

    cardAddStacked(cards[0], cards[1]);

    list_t* stack = cardGetStacked(cards[0]);
    card_t* stackedCard = listGet(stack, 0);

    assert(*cardGetNumber(stackedCard) == numbers[1] && !strCmp(cardGetSuit(stackedCard), suit));

    print(deck, stdout);
    printf("\n");

    delete(deck);

    printf("The deck test finished successfuly\n");
}


