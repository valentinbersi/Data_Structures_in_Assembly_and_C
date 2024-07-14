#include "lib.h"

funcCmp_t *getCompareFunction(type_t t)
{
    switch (t)
    {
    case TypeInt:
        return (funcCmp_t *)&intCmp;
        break;
    case TypeString:
        return (funcCmp_t *)&strCmp;
        break;
    case TypeCard:
        return (funcCmp_t *)&cardCmp;
        break;
    default:
        break;
    }
    return 0;
}
funcClone_t *getCloneFunction(type_t t)
{
    switch (t)
    {
    case TypeInt:
        return (funcClone_t *)&intClone;
        break;
    case TypeString:
        return (funcClone_t *)&strClone;
        break;
    case TypeCard:
        return (funcClone_t *)&cardClone;
        break;
    default:
        break;
    }
    return 0;
}
funcDelete_t *getDeleteFunction(type_t t)
{
    switch (t)
    {
    case TypeInt:
        return (funcDelete_t *)&intDelete;
        break;
    case TypeString:
        return (funcDelete_t *)&strDelete;
        break;
    case TypeCard:
        return (funcDelete_t *)&cardDelete;
        break;
    default:
        break;
    }
    return 0;
}
funcPrint_t *getPrintFunction(type_t t)
{
    switch (t)
    {
    case TypeInt:
        return (funcPrint_t *)&intPrint;
        break;
    case TypeString:
        return (funcPrint_t *)&strPrint;
        break;
    case TypeCard:
        return (funcPrint_t *)&cardPrint;
        break;
    default:
        break;
    }
    return 0;
}

/** Int **/

int32_t intCmp(int32_t *a, int32_t *b)
{
    return *a == *b ? 0 : (*a < *b ? 1 : -1);
}

void intDelete(int32_t *a)
{
    free(a);
}

void intPrint(int32_t *a, FILE *pFile)
{
    fprintf(pFile, "%i", *a);
}

int32_t *intClone(int32_t *a)
{
    int32_t *aux = malloc(sizeof(int32_t));
    *aux = *a;
    return aux;
}

/** Lista **/

list_t *listNew(type_t t)
{
    list_t *list = malloc(sizeof(list_t));

    list->type = t;
    list->size = 0;
    list->first = NULL;
    list->last = NULL;

    return list;
}

uint8_t listGetSize(list_t *l)
{
    return l->size;
}

// Aux function
listElem_t *listGetElem(list_t *l, uint8_t i)
{
    if (i >= l->size) return NULL;

    listElem_t *aux = l->first;
    for (uint8_t _ = 0; _ < i; ++_) aux = aux->next;
    return aux;
}

void *listGet(list_t *l, uint8_t i)
{
    listElem_t *aux = listGetElem(l, i);
    if (aux == NULL) return 0;
    return aux->data;
}

// Aux function
void initializeList(list_t *l)
{
    l->first = malloc(sizeof(listElem_t));
    l->last = l->first;
    l->first->data = NULL;
    l->first->next = NULL;
    l->first->prev = NULL;
    ++l->size;
}

void listAddFirst(list_t *l, void *data)
{
    funcClone_t *clone = getCloneFunction(l->type);

    if (!(l->size)) {
        initializeList(l);
        l->first->data = clone(data);
        return;
    }

    listElem_t *new_node = malloc(sizeof(listElem_t));
    new_node->data = clone(data);
    new_node->next = l->first;
    new_node->prev = NULL;

    l->first->prev = new_node;
    l->first = new_node;
    ++l->size;
}

void listAddLast(list_t *l, void *data)
{
    funcClone_t *clone = getCloneFunction(l->type);

    if (!l->size){
        initializeList(l);
        l->last->data = clone(data);
        return;
    }

    listElem_t *new_node = malloc(sizeof(listElem_t));
    new_node->data = clone(data);
    new_node->next = NULL;
    new_node->prev = l->last;

    l->last->next = new_node;
    l->last = new_node;
    ++l->size;
}

list_t *listClone(list_t *l)
{
    list_t *new_list = listNew(l->type);

    for (int i = 0; i < l->size; ++i) listAddLast(new_list, listGet(l, i));

    return new_list;
}

void *listRemove(list_t *l, uint8_t i)
{
    listElem_t *removed = listGetElem(l, i);
    if (removed == NULL) return 0;
    void *data = removed->data;

    if (l->size > 1) {
        if (i == 0){
            l->first->next->prev = NULL;
            l->first = l->first->next;


        } else if (i == l->size - 1) {
            l->last->prev->next = NULL;
            l->last = l->last->prev;

        } else {
            removed->next->prev = removed->prev;
            removed->prev->next = removed->next;
        }
    } else {
        l->first = NULL;
        l->last = NULL;
    }

    --l->size;
    free(removed);
    return data;
}

void listSwap(list_t *l, uint8_t i, uint8_t j)
{
    listElem_t *elem_i = listGetElem(l, i);
    listElem_t *elem_j = listGetElem(l, j);

    if (elem_i == NULL || elem_j == NULL) return;

    void *aux = elem_i->data;
    elem_i->data = elem_j->data;
    elem_j->data = aux;
}

void listDelete(list_t *l)
{
    funcDelete_t *deleter = getDeleteFunction(l->type);

    while (l->size) deleter(listRemove(l, l->size - 1));

    free(l);
}

void listPrint(list_t *l, FILE *pFile)
{
    funcPrint_t *printer = getPrintFunction(l->type);

    fprintf(pFile, "[");
    for(uint8_t i = 0; i < l->size; ++i) {
        printer(listGet(l, i), pFile);
        if (i != l->size - 1) fprintf(pFile, ",");
    }
    fprintf(pFile, "]");
}

/** Game **/

game_t *gameNew(void *cardDeck, funcGet_t *funcGet, funcRemove_t *funcRemove, funcSize_t *funcSize, funcPrint_t *funcPrint, funcDelete_t *funcDelete)
{
    game_t *game = (game_t *)malloc(sizeof(game_t));
    game->cardDeck = cardDeck;
    game->funcGet = funcGet;
    game->funcRemove = funcRemove;
    game->funcSize = funcSize;
    game->funcPrint = funcPrint;
    game->funcDelete = funcDelete;
    return game;
}
int gamePlayStep(game_t *g)
{
    int applied = 0;
    uint8_t i = 0;
    while (applied == 0 && i + 2 < g->funcSize(g->cardDeck))
    {
        card_t *a = g->funcGet(g->cardDeck, i);
        card_t *b = g->funcGet(g->cardDeck, i + 1);
        card_t *c = g->funcGet(g->cardDeck, i + 2);
        if (strCmp(cardGetSuit(a), cardGetSuit(c)) == 0 || intCmp(cardGetNumber(a), cardGetNumber(c)) == 0)
        {
            card_t *removed = g->funcRemove(g->cardDeck, i);
            cardAddStacked(b, removed);
            cardDelete(removed);
            applied = 1;
        }
        i++;
    }
    return applied;
}
uint8_t gameGetCardDeckSize(game_t *g)
{
    return g->funcSize(g->cardDeck);
}
void gameDelete(game_t *g)
{
    g->funcDelete(g->cardDeck);
    free(g);
}
void gamePrint(game_t *g, FILE *pFile)
{
    g->funcPrint(g->cardDeck, pFile);
}
