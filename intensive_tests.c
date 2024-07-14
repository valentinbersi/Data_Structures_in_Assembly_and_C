#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>

#include "lib.h"
#include "optparser.h"

void test_int();
void test_string();
void test_array();
void test_list();
void test_card();
void test_game();

FILE *pfile;

int main(int argc, char **argv)
{
    parse_cli(argc, argv);

    if (test_run.file)
    {
        remove(test_run.file);
        pfile = fopen(test_run.file, "w");
    }
    else
    {
        pfile = stdout;
    }

    setvbuf(pfile, NULL, _IONBF, 0); // Desactiva el buffer de salida

    srand(12345);

    if (test_run.testcases[INT_TEST])
    {
        test_int();
    }
    if (test_run.testcases[STRING_TEST])
    {
        test_string();
    }
    if (test_run.testcases[ARRAY_TEST])
    {
        test_array();
    }
    if (test_run.testcases[LIST_TEST])
    {
        test_list();
    }
    if (test_run.testcases[CARD_TEST])
    {
        test_card();
    }
    if (test_run.testcases[GAME_TEST])
    {
        test_game();
    }

    fclose(pfile);
    return 0;
}

char *randomString(uint32_t l)
{
    // 32 to 126 = valid characters
    char *s = malloc(l + 1);
    for (uint32_t i = 0; i < (l + 1); i++)
        s[i] = (char)(33 + (rand() % (126 - 33)));
    s[l] = 0;
    return s;
}

char *randomHexString(uint32_t l)
{
    char *s = malloc(l + 1);
    for (uint32_t i = 0; i < (l + 1); i++)
    {
        do
        {
            s[i] = (char)(rand() % 256);
        } while (s[i] == 0);
    }
    s[l] = 0;
    return s;
}

char *strings[10] = {"aa", "bb", "dd", "ff", "00", "zz", "cc", "ee", "gg", "hh"};

void test_array()
{
    array_t *a;
    void *data;

    fprintf(pfile, "== Array ==\n\n");

    // Agregar
    fprintf(pfile, "Add\n\n");

    a = arrayNew(TypeString, 20);
    fprintf(pfile, "%i - ", arrayGetSize(a));
    arrayPrint(a, pfile);
    fprintf(pfile, "\n");
    arrayAddLast(a, strings[0]);
    fprintf(pfile, "%i - ", arrayGetSize(a));
    arrayPrint(a, pfile);
    fprintf(pfile, "\n");
    arrayAddLast(a, strings[1]);
    fprintf(pfile, "%i - ", arrayGetSize(a));
    arrayPrint(a, pfile);
    fprintf(pfile, "\n");
    arrayAddLast(a, strings[2]);
    fprintf(pfile, "%i - ", arrayGetSize(a));
    arrayPrint(a, pfile);
    fprintf(pfile, "\n");
    fprintf(pfile, "\n");
    // Get
    fprintf(pfile, "Get\n");
    fprintf(pfile, "\n");
    fprintf(pfile, "%i - ", arrayGetSize(a));
    arrayPrint(a, pfile);
    fprintf(pfile, "\n");
    data = arrayGet(a, 0);
    fprintf(pfile, "%s\n", (char *)data);
    data = arrayGet(a, 1);
    fprintf(pfile, "%s\n", (char *)data);
    data = arrayGet(a, 2);
    fprintf(pfile, "%s\n", (char *)data);
    data = arrayGet(a, 3);
    fprintf(pfile, "%s\n", (char *)data);
    fprintf(pfile, "\n");

    // Quitar
    fprintf(pfile, "Remove\n");
    fprintf(pfile, "\n");
    data = arrayRemove(a, 0);
    fprintf(pfile, "%i - ", arrayGetSize(a));
    arrayPrint(a, pfile);
    fprintf(pfile, " - %s\n", (char *)data);
    strDelete((char *)data);
    data = arrayRemove(a, 1);
    fprintf(pfile, "%i - ", arrayGetSize(a));
    arrayPrint(a, pfile);
    fprintf(pfile, " - %s\n", (char *)data);
    strDelete((char *)data);
    data = arrayRemove(a, 0);
    fprintf(pfile, "%i - ", arrayGetSize(a));
    arrayPrint(a, pfile);
    fprintf(pfile, " - %s\n", (char *)data);
    strDelete((char *)data);
    fprintf(pfile, "\n");
    // Swap
    fprintf(pfile, "Swap\n");
    fprintf(pfile, "\n");
    arrayAddLast(a, strings[0]);
    arrayAddLast(a, strings[1]);
    arrayAddLast(a, strings[2]);
    fprintf(pfile, "%i - ", arrayGetSize(a));
    arrayPrint(a, pfile);
    fprintf(pfile, "\n");
    arraySwap(a, 0, 0);
    fprintf(pfile, "%i - ", arrayGetSize(a));
    arrayPrint(a, pfile);
    fprintf(pfile, "\n");
    arraySwap(a, 1, 1);
    fprintf(pfile, "%i - ", arrayGetSize(a));
    arrayPrint(a, pfile);
    fprintf(pfile, "\n");
    arraySwap(a, 2, 2);
    fprintf(pfile, "%i - ", arrayGetSize(a));
    arrayPrint(a, pfile);
    fprintf(pfile, "\n");
    arraySwap(a, 0, 2);
    fprintf(pfile, "%i - ", arrayGetSize(a));
    arrayPrint(a, pfile);
    fprintf(pfile, "\n");
    arraySwap(a, 2, 0);
    fprintf(pfile, "%i - ", arrayGetSize(a));
    arrayPrint(a, pfile);
    fprintf(pfile, "\n");
    arraySwap(a, 0, 1);
    arraySwap(a, 1, 2);
    fprintf(pfile, "%i - ", arrayGetSize(a));
    arrayPrint(a, pfile);
    fprintf(pfile, "\n");
    fprintf(pfile, "\n");
    // Agregar de mas
    fprintf(pfile, "Adding too much\n");
    fprintf(pfile, "\n");
    fprintf(pfile, "%i - ", arrayGetSize(a));
    arrayPrint(a, pfile);
    fprintf(pfile, "\n");
    for (int j = 0; j < 3; j++)
    {
        for (int i = 0; i < 10; i++)
        {
            arrayAddLast(a, strings[i]);
            fprintf(pfile, "%i - ", arrayGetSize(a));
            arrayPrint(a, pfile);
            fprintf(pfile, "\n");
        }
    }
    fprintf(pfile, "\n");
    // Quitar de mas
    fprintf(pfile, "Remove too much\n");
    fprintf(pfile, "\n");
    fprintf(pfile, "%i - ", arrayGetSize(a));
    arrayPrint(a, pfile);
    fprintf(pfile, "\n");
    for (int j = 0; j < 3; j++)
    {
        for (int i = 0; i < 10; i++)
        {
            data = arrayRemove(a, i);
            fprintf(pfile, "%i - ", arrayGetSize(a));
            arrayPrint(a, pfile);
            fprintf(pfile, " - %s\n", (char *)data);
            strDelete((char *)data);
        }
    }
    data = arrayRemove(a, 0);
    fprintf(pfile, "%i - ", arrayGetSize(a));
    arrayPrint(a, pfile);
    fprintf(pfile, " - %s\n", (char *)data);
    strDelete((char *)data);
    data = arrayRemove(a, 0);
    fprintf(pfile, "%i - ", arrayGetSize(a));
    arrayPrint(a, pfile);
    fprintf(pfile, " - %s\n", (char *)data);
    strDelete((char *)data);
    data = arrayRemove(a, 10);
    fprintf(pfile, "%i - ", arrayGetSize(a));
    arrayPrint(a, pfile);
    fprintf(pfile, " - %s\n", (char *)data);
    strDelete((char *)data);
    fprintf(pfile, "\n");
    // Delete
    fprintf(pfile, "Delete\n");
    fprintf(pfile, "\n");
    fprintf(pfile, "%i - ", arrayGetSize(a));
    arrayPrint(a, pfile);
    fprintf(pfile, "\n");
    for (int i = 0; i < 10; i++)
    {
        arrayAddLast(a, strings[i]);
    }
    fprintf(pfile, "%i - ", arrayGetSize(a));
    arrayPrint(a, pfile);
    fprintf(pfile, "\n");
    arrayDelete(a);
    fprintf(pfile, "\n");
}

void test_list()
{
    list_t *a, *b;
    void *data;

    fprintf(pfile, "== List ==\n");
    fprintf(pfile, "\n");

    // Agregar
    fprintf(pfile, "Add\n");
    fprintf(pfile, "\n");
    a = listNew(TypeString);
    fprintf(pfile, "%i - ", listGetSize(a));
    listPrint(a, pfile);
    fprintf(pfile, "\n");
    listAddLast(a, strings[0]);
    fprintf(pfile, "%i - ", listGetSize(a));
    listPrint(a, pfile);
    fprintf(pfile, "\n");
    listAddLast(a, strings[1]);
    fprintf(pfile, "%i - ", listGetSize(a));
    listPrint(a, pfile);
    fprintf(pfile, "\n");
    listAddLast(a, strings[2]);
    fprintf(pfile, "%i - ", listGetSize(a));
    listPrint(a, pfile);
    fprintf(pfile, "\n");

    b = listNew(TypeInt);
    fprintf(pfile, "%i - ", listGetSize(b));
    listPrint(b, pfile);
    fprintf(pfile, "\n");

    for (int i = 0; i < 20; i++)
    {
        listAddFirst(b, &i);
        fprintf(pfile, "%i - ", listGetSize(b));
        listPrint(b, pfile);
        fprintf(pfile, "\n");
    }
    fprintf(pfile, "\n");
    // Get
    fprintf(pfile, "Get\n");
    fprintf(pfile, "\n");
    fprintf(pfile, "%i - ", listGetSize(a));
    listPrint(a, pfile);
    fprintf(pfile, "\n");
    data = listGet(a, 0);
    fprintf(pfile, "%s\n", (char *)data);
    data = listGet(a, 1);
    fprintf(pfile, "%s\n", (char *)data);
    data = listGet(a, 2);
    fprintf(pfile, "%s\n", (char *)data);
    data = listGet(a, 3);
    fprintf(pfile, "%s\n", (char *)data);
    fprintf(pfile, "\n");

    // Quitar
    fprintf(pfile, "Remove\n");
    fprintf(pfile, "\n");
    data = listRemove(a, 0);
    fprintf(pfile, "%i - ", listGetSize(a));
    listPrint(a, pfile);
    fprintf(pfile, " - %s\n", (char *)data);
    strDelete((char *)data);
    data = listRemove(a, 1);
    fprintf(pfile, "%i - ", listGetSize(a));
    listPrint(a, pfile);
    fprintf(pfile, " - %s\n", (char *)data);
    strDelete((char *)data);
    data = listRemove(a, 0);
    fprintf(pfile, "%i - ", listGetSize(a));
    listPrint(a, pfile);
    fprintf(pfile, " - %s\n", (char *)data);
    strDelete((char *)data);
    fprintf(pfile, "\n");
    // Swap
    fprintf(pfile, "Swap\n");
    fprintf(pfile, "\n");
    listAddLast(a, strings[0]);
    listAddLast(a, strings[1]);
    listAddLast(a, strings[2]);
    fprintf(pfile, "%i - ", listGetSize(a));
    listPrint(a, pfile);
    fprintf(pfile, "\n");
    listSwap(a, 0, 0);
    fprintf(pfile, "%i - ", listGetSize(a));
    listPrint(a, pfile);
    fprintf(pfile, "\n");
    listSwap(a, 1, 1);
    fprintf(pfile, "%i - ", listGetSize(a));
    listPrint(a, pfile);
    fprintf(pfile, "\n");
    listSwap(a, 2, 2);
    fprintf(pfile, "%i - ", listGetSize(a));
    listPrint(a, pfile);
    fprintf(pfile, "\n");
    listSwap(a, 0, 2);
    fprintf(pfile, "%i - ", listGetSize(a));
    listPrint(a, pfile);
    fprintf(pfile, "\n");
    listSwap(a, 2, 0);
    fprintf(pfile, "%i - ", listGetSize(a));
    listPrint(a, pfile);
    fprintf(pfile, "\n");
    listSwap(a, 0, 1);
    listSwap(a, 1, 2);
    fprintf(pfile, "%i - ", listGetSize(a));
    listPrint(a, pfile);
    fprintf(pfile, "\n");
    fprintf(pfile, "\n");
    // Agregar de mas
    fprintf(pfile, "Adding 2\n");
    fprintf(pfile, "\n");
    fprintf(pfile, "%i - ", listGetSize(a));
    listPrint(a, pfile);
    fprintf(pfile, "\n");
    for (int j = 0; j < 3; j++)
    {
        for (int i = 0; i < 10; i++)
        {
            listAddLast(a, strings[i]);
            fprintf(pfile, "%i - ", listGetSize(a));
            listPrint(a, pfile);
            fprintf(pfile, "\n");
        }
    }
    fprintf(pfile, "\n");
    // Quitar de mas
    fprintf(pfile, "Removing too much\n");
    fprintf(pfile, "\n");
    fprintf(pfile, "%i - ", listGetSize(a));
    listPrint(a, pfile);
    fprintf(pfile, "\n");
    for (int j = 0; j < 3; j++)
    {
        for (int i = 0; i < 10; i++)
        {
            data = listRemove(a, i);
            fprintf(pfile, "%i - ", listGetSize(a));
            listPrint(a, pfile);
            fprintf(pfile, " - %s\n", (char *)data);
            strDelete((char *)data);
        }
    }

    for (int i = 0; i < 10; i++)
    {
        data = listRemove(a, 0);
        fprintf(pfile, "%i - ", listGetSize(a));
        listPrint(a, pfile);
        fprintf(pfile, " - %s\n", (char *)data);
        strDelete((char *)data);
    }
    fprintf(pfile, "\n");
    // Delete
    fprintf(pfile, "Delete\n");
    fprintf(pfile, "\n");
    fprintf(pfile, "%i - ", listGetSize(a));
    listPrint(a, pfile);
    fprintf(pfile, "\n");
    for (int i = 0; i < 10; i++)
    {
        listAddLast(a, strings[i]);
    }
    fprintf(pfile, "%i - ", listGetSize(a));
    listPrint(a, pfile);
    fprintf(pfile, "\n");
    listDelete(a);
    listDelete(b);
    fprintf(pfile, "\n");
}

void test_int()
{
    int32_t ia = 13, ib = 0, ic = -20;
    int32_t *a = &ia;
    int32_t *b = &ib;
    int32_t *c = &ic;

    fprintf(pfile, "== Int ==\n");
    fprintf(pfile, "\n");

    // Clonar
    fprintf(pfile, "Clone\n");
    fprintf(pfile, "\n");
    int32_t *ac = intClone(a);
    assert(a != ac);
    assert(*a == *ac);
    intPrint(ac, pfile);
    fprintf(pfile, "\n");
    int32_t *bc = intClone(b);
    assert(b != bc);
    assert(*b == *bc);
    intPrint(bc, pfile);
    fprintf(pfile, "\n");
    int32_t *cc = intClone(c);
    assert(c != cc);
    assert(*c == *cc);
    intPrint(cc, pfile);
    fprintf(pfile, "\n");
    fprintf(pfile, "\n");

    // Borrar

    intDelete(ac);
    intDelete(bc);
    intDelete(cc);

    // Comparar
    fprintf(pfile, "Compare\n");
    fprintf(pfile, "\n");
    fprintf(pfile, "intCmp(%d,%d) --> %d\n", *a, *a, intCmp(a, a));
    fprintf(pfile, "intCmp(%d,%d) --> %d\n", *b, *b, intCmp(b, b));
    fprintf(pfile, "intCmp(%d,%d) --> %d\n", *b, *a, intCmp(b, a));
    fprintf(pfile, "intCmp(%d,%d) --> %d\n", *a, *b, intCmp(a, b));
    fprintf(pfile, "intCmp(%d,%d) --> %d\n", *c, *a, intCmp(c, a));
    fprintf(pfile, "intCmp(%d,%d) --> %d\n", *b, *c, intCmp(b, c));
    fprintf(pfile, "intCmp(%d,%d) --> %d\n", *c, *b, intCmp(c, b));
    fprintf(pfile, "\n");
}

void test_string()
{
    char *a = "Omega 4";
    char *b = "Palaven";
    char *c = "Feros";
    char *n = "";

    fprintf(pfile, "== String ==\n");
    fprintf(pfile, "\n");

    // Clonar
    fprintf(pfile, "Clone\n");
    fprintf(pfile, "\n");
    char *ac = strClone(a);
    assert(a != ac);
    assert(strCmp(a, ac) == 0);
    fprintf(pfile, "%i - ", strLen(ac));
    strPrint(ac, pfile);
    fprintf(pfile, "\n");
    char *nc = strClone(n);
    assert(n != nc);
    assert(strCmp(n, nc) == 0);
    fprintf(pfile, "%i - ", strLen(nc));
    strPrint(nc, pfile);
    fprintf(pfile, "\n");
    fprintf(pfile, "\n");

    // Borrar
    strDelete(ac);
    strDelete(nc);

    // Comparar
    fprintf(pfile, "Compare\n");
    fprintf(pfile, "\n");
    fprintf(pfile, "strCmp(%s,%s) --> %d\n", a, a, strCmp(a, a));
    fprintf(pfile, "strCmp(%s,%s) --> %d\n", n, n, strCmp(n, n));
    fprintf(pfile, "strCmp(%s,%s) --> %d\n", b, a, strCmp(b, a));
    fprintf(pfile, "strCmp(%s,%s) --> %d\n", a, b, strCmp(a, b));
    fprintf(pfile, "strCmp(%s,%s) --> %d\n", c, a, strCmp(c, a));
    fprintf(pfile, "strCmp(%s,%s) --> %d\n", b, c, strCmp(b, c));
    fprintf(pfile, "strCmp(%s,%s) --> %d\n", c, b, strCmp(c, b));

    char *texts[9] = {"sar", "23", "taaa", "tbb", "tix", "taaab", "taa0", "tbb", ""};
    for (int i = 0; i < 9; i++)
    {
        fprintf(pfile, "strLen(%s) --> %d\n", texts[i], strLen(texts[i]));
        for (int j = 0; j < 9; j++)
        {
            fprintf(pfile, "strCmp(%s,%s) --> %d\n", texts[i], texts[j], strCmp(texts[i], texts[j]));
        }
    }
    fprintf(pfile, "\n");
}

void test_card()
{
    fprintf(pfile, "== Card ==\n");
    fprintf(pfile, "\n");

    fprintf(pfile, "New\n");
    fprintf(pfile, "\n");
    int32_t v = 1;
    int32_t *one = &v;
    card_t *oneOfSwords = cardNew("swords", one);
    assert(intCmp(cardGetNumber(oneOfSwords), one) == 0);
    assert(strCmp(cardGetSuit(oneOfSwords), "swords") == 0);
    assert(listGetSize(cardGetStacked(oneOfSwords)) == 0);

    cardPrint(oneOfSwords, pfile);
    fprintf(pfile, "\n");
    fprintf(pfile, "Compare\n");
    fprintf(pfile, "\n");

    card_t *clone = cardClone(oneOfSwords);
    assert(cardCmp(clone, oneOfSwords) == 0);
    cardDelete(oneOfSwords);
    cardDelete(clone);

    char *suits[] = {"swords", "clubs", "cups", "coins"};
    array_t *deckArray = arrayNew(TypeCard, 48);
    list_t *deckList = listNew(TypeCard);

    for (int i = 0; i < 4; ++i)
    {
        for (int32_t j = 1; j < 13; ++j)
        {
            card_t *card = cardNew(suits[i], &j);
            arrayAddLast(deckArray, (void *)card);
            listAddLast(deckList, (void *)card);
            arrayPrint(deckArray, pfile);
            fprintf(pfile, "\n");
            listPrint(deckList, pfile);
            fprintf(pfile, "\n");
            cardDelete(card);
        }
    }
    fprintf(pfile, "\n");

    for (uint8_t i = 0; i < 48; ++i)
    {
        card_t *cardi = (card_t *)arrayGet(deckArray, i);
        for (uint8_t j = 0; j < 48; ++j)
        {
            card_t *cardj = (card_t *)arrayGet(deckArray, j);
            cardPrint(cardi, pfile);
            fprintf(pfile, " cmp ");
            cardPrint(cardj, pfile);
            fprintf(pfile, " -> %d", cardCmp(cardi, cardj));
            fprintf(pfile, "\n");
        }
    }

    fprintf(pfile, "\n");

    fprintf(pfile, "Stacking\n");
    fprintf(pfile, "\n");

    card_t *newOneOfSwords = cardNew("swords", one);

    for (int32_t j = 1; j < 13; ++j)
    {
        card_t *card = arrayRemove(deckArray, j);
        cardAddStacked(newOneOfSwords, card);
        cardPrint(newOneOfSwords, pfile);
        fprintf(pfile, "\n");
        cardDelete(card);
    }

    arrayDelete(deckArray);
    fprintf(pfile, "\n");

    for (int32_t j = 13; j < 26; ++j)
    {
        card_t *card = listRemove(deckList, j);
        cardAddStacked(newOneOfSwords, card);
        cardPrint(newOneOfSwords, pfile);
        fprintf(pfile, "\n");
        cardDelete(card);
    }

    listDelete(deckList);

    list_t *stacked = cardGetStacked(newOneOfSwords);
    cardPrint(newOneOfSwords, pfile);
    fprintf(pfile, "\n");

    while (listGetSize(stacked) != 0)
    {

        card_t *card = listRemove(stacked, 0);
        cardPrint(card, pfile);
        cardPrint(newOneOfSwords, pfile);
        fprintf(pfile, "\n");
        cardDelete(card);
    }
    cardPrint(newOneOfSwords, pfile);
    fprintf(pfile, "\n");
    cardDelete(newOneOfSwords);
    fprintf(pfile, "\n");
}

#define TESTING
#include "game.c"

void test_game()
{
    array_t *a;
    void *data;

    game_t *game;

    for (int i = 0; i < 10; i++)
    {
        fprintf(pfile, "== Game Array ==\n");
        fprintf(pfile, "\n");
        game = buildGameWithArray(i);
        playGame(game, pfile);
        fprintf(pfile, "\n");
        gameDelete(game);

        fprintf(pfile, "== Game List ==\n");
        fprintf(pfile, "\n");
        game = buildGameWithList(i);
        playGame(game, pfile);
        fprintf(pfile, "\n");
        gameDelete(game);
    }
}
