#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "lib.h"

#define GREEN "\033[0;32m"
#define WHITE "\033[0;37m"

#define CHECK "✓"
#define CROSS "✗"

funcCmp_t *getCompareFunction(type_t t);
funcClone_t *getCloneFunction(type_t t);
funcDelete_t *getDeleteFunction(type_t t);
funcPrint_t *getPrintFunction(type_t t);

void testlogTitle(char *title)
{
    printf("\n   %s\n   ", title);
    while (*(title++))
        printf("=");
    printf("\n\n");
}

void testlogSeparator()
{
    printf("    ");
    for (int i = 0; i < 100; i++)
    {
        printf("_");
    }
    printf("\n\n");
}

void testlogAfirm(char *type, int num, char *description)
{
    printf(GREEN "   %s ", CHECK);
    if (num < 10)
    {
        printf(WHITE " 0%i - %s - %s \n", num, type, description);
    }
    else
    {
        printf(WHITE " %i - %s - %s \n", num, type, description);
    }
}

/*---------------------------------- AUXILIARES -------------------------------------------*/

void cardDeleteC(card_t *c)
{
    strDelete(c->suit);
    intDelete(c->number);
    listDelete(c->stacked);
    free(c);
}

void arrayDeleteC(array_t *a)
{
    funcDelete_t *delete = getDeleteFunction(a->type);
    for (int i = 0; i < a->size; i++)
    {
        delete (a->data[i]);
    }
    free(a->data);
    free(a);
}

void strDeleteC(char *str)
{
    free(str);
}

/*---------------------------------- INT TEST -------------------------------------------*/

void test01IntCmpWithFirstElementGreaterThanSecond(char *testType, int testNum, int *number, int *number_2)
{

    assert(intCmp(number, number_2) == -1);
    testlogAfirm(testType, testNum, "When the first argument is greater than the second one, IntCmp returns -1");
}

void test02IntCmpWithFirstElementLessThanSecond(char *testType, int testNum, int *number, int *number_2)
{

    assert(intCmp(number, number_2) == 1);
    testlogAfirm(testType, testNum, "When the first argument is lower than the second one, IntCmp returns -1");
}

void test03IntCmpWithEqualNumbers(char *testType, int testNum, int *number)
{

    testlogAfirm(testType, testNum, "Given two equal numbers, IntCmp devuelve 0");
    assert(intCmp(number, number) == 0);
}

void test04IntPrint(char *testType, int testNum, int *number)
{
    FILE *file = fopen("outInt.txt", "w");

    intPrint(number, file);
    fclose(file);

    file = fopen("outInt.txt", "r");
    int numberInStream;
    fscanf(file, "%d", &numberInStream);

    assert(intCmp(number, &numberInStream) == 0);

    testlogAfirm(testType, testNum, "Given an integer, intPrint prints it correctly in the given stream.");
    fclose(file);
}

void test05IntClone(char *testType, int testNum, int *number)
{
    int *copy = intClone(number);
    assert(intCmp(number, copy) == 0);
    assert(copy != number);
    testlogAfirm(testType, testNum, "Given an integer, IntClone returns the same number located in a different memory address");
    intDelete(copy);
}

/*---------------------------------- LIST TEST -------------------------------------------*/

void test01ListNewWithoutElements(char *testType, int testNum)
{
    list_t *list = listNew(0);

    assert(list->size == 0);
    assert(list->first == NULL);
    assert(list->last == NULL);

    testlogAfirm(testType, testNum, "An empty list is created and it's initialized correctly");

    listDelete(list);
}

void test02ListAddFirstInEmptyList(char *testType, int testNum)
{
    list_t *list = listNew(1);
    int new_element = 5;

    listAddFirst(list, &new_element);
    assert(list->size == 1);
    assert(intCmp(list->first->data, &new_element) == 0);
    assert(intCmp(list->last->data, &new_element) == 0);
    assert(&new_element != (list->first->data));
    testlogAfirm(testType, testNum, "An element is added to the beginning of the list successfully");
    listDelete(list);
}

void test03ListAddFirstConsecutivesInEmptyList(char *testType, int testNum)
{
    list_t *list = listNew(1);
    int element = 10;
    int element2 = 20;
    int element3 = 30;

    listAddFirst(list, &element);
    listAddFirst(list, &element2);
    listAddFirst(list, &element3);
    assert(list->size == 3);
    assert(intCmp(list->first->data, &element3) == 0);
    assert(intCmp(list->first->next->data, &element2) == 0);
    assert(intCmp(list->last->data, &element) == 0);

    testlogAfirm(testType, testNum, "Various elements are added to the first position of the list successfully");
    listDelete(list);
}

void test04ListGetSizeWithEmptyList(char *testType, int testNum)
{
    list_t *list = listNew(1);

    assert(listGetSize(list) == 0);
    testlogAfirm(testType, testNum, "Given an empty list, listGetSize returns 0");
    listDelete(list);
}

void test05ListGetSizeListWithSomeElements(char *testType, int testNum)
{
    list_t *list = listNew(1);
    int element = 10;
    int element2 = 20;
    int element3 = 30;

    listAddFirst(list, &element);
    listAddFirst(list, &element2);
    listAddFirst(list, &element3);
    assert(listGetSize(list) == 3);
    testlogAfirm(testType, testNum, "Given a list with some elements, listGetSize returns the correct amount");
    listDelete(list);
}

void test06ListGetWithSomeElements(char *testType, int testNum)
{
    list_t *list = listNew(1);
    int element = 10;
    int element2 = 20;
    int element3 = 30;

    listAddFirst(list, &element);
    listAddFirst(list, &element2);
    listAddFirst(list, &element3);
    assert(intCmp(listGet(list, 1), &element2) == 0);
    testlogAfirm(testType, testNum, "Given a list with some elements, the element in the given position is obtained successfully");
    listDelete(list);
}

void test07ListAddLastWithEmptyList(char *testType, int testNum)
{
    list_t *list = listNew(1);
    int new_element = 5;

    listAddLast(list, &new_element);
    assert(list->size == 1);
    assert(intCmp(listGet(list, 0), &new_element) == 0);
    assert(&new_element != (list->first->data));
    testlogAfirm(testType, testNum, "Given an empty list, an element is added to the last position successfully");
    listDelete(list);
}

void test08ListRemoveWithEmptyList(char *testType, int testNum)
{
    list_t *list = listNew(1);

    assert(listRemove(list, 10) == 0);
    testlogAfirm(testType, testNum, "Given an empty list, removing the element in the position 10 returns 0");
    listDelete(list);
}

void test09ListRemoveListWithSomeElements(char *testType, int testNum)
{
    list_t *list = listNew(1);
    int element = 10;
    int element2 = 20;
    int element3 = 30;

    listAddLast(list, &element);
    listAddLast(list, &element2);
    listAddLast(list, &element3);

    void *obtained_element = listRemove(list, 1);
    assert(list->size == 2);
    assert(intCmp(listGet(list, 1), &element3) == 0);
    assert(intCmp(obtained_element, &element2) == 0);
    intDelete(obtained_element);
    testlogAfirm(testType, testNum, "Given a list with some elements, the element in the given position is removed successfully");
    listDelete(list);
}

void test10ListSwapWithEmptyList(char *testType, int testNum)
{
    list_t *list = listNew(1);

    listSwap(list, 10, 100);
    testlogAfirm(testType, testNum, "Given an empty list, swapping two elements does nothing");
    listDelete(list);
}

void test11ListSwapListWithOneElement(char *testType, int testNum)
{
    list_t *list = listNew(1);
    int element = 10;
    listAddFirst(list, &element);

    listSwap(list, 0, 100);
    testlogAfirm(testType, testNum, "Given a list with one element, swapping two elements does nothing");
    listDelete(list);
}

void test12ListSwapListWithSomeElements(char *testType, int testNum)
{
    list_t *list = listNew(1);
    int element = 10;
    int element2 = 20;
    int element3 = 30;
    int element4 = 50;

    listAddLast(list, &element);
    listAddLast(list, &element2);
    listAddLast(list, &element3);
    listAddLast(list, &element4);

    listSwap(list, 0, 3);
    assert(intCmp(listGet(list, 0), &element4) == 0);
    assert(intCmp(listGet(list, 3), &element) == 0);
    testlogAfirm(testType, testNum, "Given a list with some elements, swapping two elements in valid positions works successfully");
    listDelete(list);
}

void test13ListCloneEmptyList(char *testType, int testNum)
{
    list_t *list = listNew(1);

    list_t *newList = listClone(list);

    assert(newList->first == NULL);
    assert(newList->type == list->type);
    assert(newList->last == NULL);
    testlogAfirm(testType, testNum, "Cloning an empty list returns an empty list");
    listDelete(list);
    listDelete(newList);
}

void test14ListCloneListWithOneElement(char *testType, int testNum)
{
    list_t *list = listNew(1);
    int element = 10;

    listAddLast(list, &element);

    list_t *newList = listClone(list);

    assert(intCmp(list->first->data, newList->first->data) == 0);
    assert(intCmp(list->last->data, newList->last->data) == 0);
    assert(newList->type == list->type);
    assert(newList->size == list->size);
    testlogAfirm(testType, testNum, "A list with one element clones correctly");
    listDelete(list);
    listDelete(newList);
}

void test15ListCloneListWithSomeElements(char *testType, int testNum)
{
    list_t *list = listNew(1);
    int element = 10;
    int element2 = 20;
    int element3 = 30;
    int element4 = 50;

    listAddLast(list, &element);
    listAddLast(list, &element2);
    listAddLast(list, &element3);
    listAddLast(list, &element4);

    list_t *newList = listClone(list);

    assert(intCmp(list->first->data, newList->first->data) == 0);
    assert(intCmp(listGet(list, 1), listGet(newList, 1)) == 0);
    assert(intCmp(listGet(list, 2), listGet(newList, 2)) == 0);
    assert(intCmp(list->last->data, newList->last->data) == 0);
    assert(newList->type == list->type);
    assert(newList->size == list->size);

    testlogAfirm(testType, testNum, "A list with some elements clones correctly");

    listDelete(list);
    listDelete(newList);
}

void test16ListPrintEmptyList(char *testType, int testNum)
{
    FILE *file = fopen("outEmptyList.txt", "w");
    list_t *list = listNew(1);
    int counter = 0;
    char ch;

    listPrint(list, file);
    fclose(file);

    file = fopen("outEmptyList.txt", "r");

    ch = fgetc(file);
    assert(ch == '[');
    ch = fgetc(file);
    assert(ch == ']');

    testlogAfirm(testType, testNum, "Given an empty list, listPrint generates the corresponding file printing the expected: '[]'");
    fclose(file);
    listDelete(list);
}

void test17ListPrintListWithOneElement(char *testType, int testNum)
{
    FILE *file = fopen("outListWithOneElement.txt", "w");
    list_t *list = listNew(1);
    int element = 10;
    listAddLast(list, &element);

    listPrint(list, file);
    fclose(file);

    file = fopen("outListWithOneElement.txt", "r");
    int elementOnStream;
    fscanf(file, "[%d]", &elementOnStream);

    assert(intCmp(listGet(list, 0), &elementOnStream) == 0);

    testlogAfirm(testType, testNum, "Given a list with one element, listPrint prints the element in the stream passed by argument");
    fclose(file);
    listDelete(list);
}

void test18ListPrintListWithSomeElements(char *testType, int testNum)
{
    FILE *file = fopen("outlist.txt", "w");
    list_t *list = listNew(1);
    int element = 10;
    int element2 = 20;
    int element3 = 30;
    int element4 = 50;

    listAddLast(list, &element);
    listAddLast(list, &element2);
    listAddLast(list, &element3);
    listAddLast(list, &element4);

    listPrint(list, file);
    fclose(file);

    file = fopen("outList.txt", "r");

    fscanf(file, "[%d, %d, %d, %d]", &element, &element2, &element3, &element4);

    assert(intCmp(listGet(list, 0), &element) == 0);
    assert(intCmp(listGet(list, 1), &element2) == 0);
    assert(intCmp(listGet(list, 2), &element3) == 0);
    assert(intCmp(listGet(list, 3), &element4) == 0);

    testlogAfirm(testType, testNum, "Given a list with some elements, listPrint prints the same elements on the stream passed by argument");
    fclose(file);
    listDelete(list);
}

/*---------------------------------- STR TEST -------------------------------------------*/

void test01StrDeleteEmpty(char *testType, int testNum)
{
    char *str = malloc(10 * sizeof(char));
    testlogAfirm(testType, testNum, "Given an empty string, strDelete deletes it");
    strDelete(str);
}

void test02StrDeleteNotEmpty(char *testType, int testNum)
{
    char *str = malloc(20 * sizeof(char));
    strcpy(str, "Hello World");
    testlogAfirm(testType, testNum, "Given a not empty string, strDelete deletes it");
    strDelete(str);
}

void test03StrLenEmpty(char *testType, int testNum)
{
    char string[] = "";
    int len = strLen(string);
    assert(len == 0);
    testlogAfirm(testType, testNum, "strLen = 0 with an empty string");
}

void test04StrLen(char *testType, int testNum)
{
    char string[] = "helloo";
    int len = strLen(string);
    assert(len == 6);
    testlogAfirm(testType, testNum, "strLen with a not empty string");
}

void test05StrPrint(char *testType, int testNum)
{
    char str[12] = "Hello World";
    FILE *file = fopen("outStr.txt", "w");
    strPrint(str, file);
    fclose(file);

    int bufferLength = 12;
    char buffer[bufferLength];
    file = fopen("outStr.txt", "r");
    fgets(buffer, bufferLength, file);
    assert(strcmp(buffer, "Hello World") == 0);
    fclose(file);
    testlogAfirm(testType, testNum, "Given a str, strPrint prints it in a file passed by argument");
}

void test06StrPrintEmpty(char *testType, int testNum)
{
    char str[10] = "";
    FILE *file = fopen("outStrEmpty.txt", "w");
    strPrint(str, file);
    fclose(file);

    int bufferLength = 5;
    char buffer[bufferLength];
    file = fopen("outStrEmpty.txt", "r");
    fgets(buffer, bufferLength, file);
    assert(strcmp(buffer, "NULL") == 0);
    fclose(file);
    testlogAfirm(testType, testNum, "Given an empty string, strPrint prints NULL in a file passed by argument");
}

void test07StrCloneStringWithSomeElements(char *testType, int testNum)
{
    char *string = "VAMOSSS RIVERRR";
    char *copy = strClone(string);
    assert(strcmp(string, copy) == 0);
    assert(string != copy);
    testlogAfirm(testType, testNum, "strClone of string with some elements");
    free(copy);
}

void test08StrCloneEmptyString(char *testType, int testNum)
{
    char *string = "";
    char *copy = strClone(string);
    assert(strcmp(string, copy) == 0);
    assert(string != copy);
    testlogAfirm(testType, testNum, "strClone of empty string");
    free(copy);
}

void test09StrCmpOfTwoStringWithSomeElements(char *testType, int testNum)
{
    char string[] = "aaaaa";
    char otro[] = "helloo";
    int result = 0;
    result = strCmp(string, otro);
    assert(result == 1);

    testlogAfirm(testType, testNum, "strCmp of two string with some elements");
}

void test10StrCmpOfTwoEmptyStrings(char *testType, int testNum)
{
    char string[] = "";
    char another[] = "";
    int result = 0;
    result = strCmp(string, another);
    assert(result == 0);

    testlogAfirm(testType, testNum, "strCmp of two empty strings");
}

void test11StrCmpWithAnEmptyString(char *testType, int testNum)
{
    char string[] = "helloo";
    char another[] = "";
    int result = 0;
    result = strCmp(string, another);
    assert(result == -1);

    testlogAfirm(testType, testNum, "strCmp with empty string");
}

/*---------------------------------- ARRAY TEST -------------------------------------------*/

void test01ArrayNew(char *testType, int testNum)
{
    array_t *a = arrayNew(1, 3);
    assert(a->type == 1);
    assert(a->capacity == 3);
    testlogAfirm(testType, testNum, "arrayNew");
    arrayDeleteC(a);
}

void test02ArrayAddLastWithOneElement(char *testType, int testNum)
{
    array_t *a = arrayNew(1, 10);
    int num = 3;
    arrayAddLast(a, &num);
    assert(*(int *)(a->data[0]) == 3);
    assert(a->size == 1);
    testlogAfirm(testType, testNum, "An element is added at the end of an empty array");
    arrayDelete(a);
}

void test03ArrayAddLastWithSomeElements(char *testType, int testNum)
{
    array_t *a = arrayNew(1, 10);
    int element = 10;
    int element2 = 20;
    int element3 = 30;
    arrayAddLast(a, &element);
    arrayAddLast(a, &element2);
    arrayAddLast(a, &element3);
    assert(*(int *)(a->data[0]) == 10);
    assert(*(int *)(a->data[1]) == 20);
    assert(*(int *)(a->data[2]) == 30);
    assert(a->size == 3);
    testlogAfirm(testType, testNum, "An element is added at the end of an array with some elements");
    arrayDelete(a);
}

void test04ArrayAddLastOverflow(char *testType, int testNum)
{
    array_t *a = arrayNew(1, 2);
    int element = 10;
    int element2 = 20;
    int element3 = 30;
    arrayAddLast(a, &element);
    arrayAddLast(a, &element2);
    arrayAddLast(a, &element3);
    assert(*(int *)(a->data[0]) == 10);
    assert(*(int *)(a->data[1]) == 20);
    assert(a->size == 2);
    testlogAfirm(testType, testNum, "Can't add an element to a full array");
    arrayDelete(a);
}

void test05ArrayRemoveOneElement(char *testType, int testNum)
{
    array_t *a = arrayNew(1, 3);
    int32_t num = 3;
    arrayAddLast(a, &num);
    int32_t *elementRemoved = arrayRemove(a, 0);
    assert(*elementRemoved == num);
    assert(a->size == 0);
    testlogAfirm(testType, testNum, "The only element of an array is removed");
    arrayDeleteC(a);
    intDelete(elementRemoved);
}

void test06ArrayRemoveElementInTheMiddle(char *testType, int testNum)
{
    array_t *a = arrayNew(1, 3);
    int32_t num = 3;
    int32_t num2 = 5;
    int32_t num3 = 6;
    arrayAddLast(a, &num);
    arrayAddLast(a, &num2);
    arrayAddLast(a, &num3);
    int32_t *elementRemoved = arrayRemove(a, 1);
    assert(*elementRemoved == num2);
    assert(a->size == 2);
    testlogAfirm(testType, testNum, "An element in the middle of the array is removed");
    arrayDeleteC(a);
    intDelete(elementRemoved);
}

void test07ArrayRemoveLastElement(char *testType, int testNum)
{
    array_t *a = arrayNew(1, 3);
    int32_t num = 3;
    int32_t num2 = 5;
    int32_t num3 = 6;
    arrayAddLast(a, &num);
    arrayAddLast(a, &num2);
    arrayAddLast(a, &num3);
    int32_t *elementRemoved = arrayRemove(a, 2);
    assert(*elementRemoved == num3);
    assert(a->size == 2);
    testlogAfirm(testType, testNum, "The last element of an array is removed");
    arrayDeleteC(a);
    intDelete(elementRemoved);
}

void test08ArrayRemoveOutOfBounds(char *testType, int testNum)
{
    array_t *a = arrayNew(1, 3);
    int num = 3;
    int num2 = 5;
    int num3 = 6;
    arrayAddLast(a, &num);
    arrayAddLast(a, &num2);
    arrayAddLast(a, &num3);
    int *elementRemoved = arrayRemove(a, 3);
    assert(elementRemoved == NULL);
    assert(a->size == 3);
    testlogAfirm(testType, testNum, "Can't remove an element out of bounds");
    arrayDeleteC(a);
}

void test09ArrayRemoveArrayWithTwoElements(char *testType, int testNum)
{
    array_t *a = arrayNew(1, 3);
    int num = 3;
    int num1 = 2;
    arrayAddLast(a, &num);
    arrayAddLast(a, &num1);
    uint8_t i = 0;
    int *resultado = arrayRemove(a, i);
    assert(*resultado == num);
    assert(a->size == 1);
    testlogAfirm(testType, testNum, "arrayRemove first element of an array with two elements");
    intDelete(resultado);
    arrayDelete(a);
}

void test10ArrayRemoveDeArrayWithOneElement(char *testType, int testNum)
{
    array_t *a = arrayNew(1, 3);
    int num = 3;
    arrayAddLast(a, &num);
    uint8_t i = 0;
    int *result= arrayRemove(a, i);
    assert(*result == num);
    assert(a->size == 0);
    testlogAfirm(testType, testNum, "arrayRemove on an array with one element");
    intDelete(result);
    arrayDelete(a);
}

void test11ArrayDeleteEmptyArray(char *testType, int testNum)
{
    array_t *a = arrayNew(1, 10);
    arrayDelete(a);
    testlogAfirm(testType, testNum, "An empty array is deleted");
}

void test12ArrayDeleteArrayWithSomeElements(char *testType, int testNum)
{
    array_t *a = arrayNew(1, 10);
    int element = 10;
    int element2 = 20;
    arrayAddLast(a, &element);
    arrayAddLast(a, &element2);
    assert(a->size == 2);
    arrayDelete(a);
    testlogAfirm(testType, testNum, "An array with some elements is deleted");
}

void test13ArrayGetSizeEmpty(char *testType, int testNum)
{
    array_t *a = arrayNew(1, 10);
    uint8_t size = arrayGetSize(a);
    assert(size == 0);
    testlogAfirm(testType, testNum, "The size of an empty array is obtained");
    arrayDelete(a);
}

void test14ArrayGetSizeWithOneElement(char *testType, int testNum)
{
    array_t *a = arrayNew(1, 10);
    int num = 3;
    arrayAddLast(a, &num);
    uint8_t size = arrayGetSize(a);
    assert(size == 1);
    testlogAfirm(testType, testNum, "The size of an array with one element is obtained");
    arrayDelete(a);
}

void test15ArrayGetSizeWithSomeElements(char *testType, int testNum)
{
    array_t *a = arrayNew(1, 10);
    int num = 3;
    arrayAddLast(a, &num);
    arrayAddLast(a, &num);
    arrayAddLast(a, &num);
    uint8_t size = arrayGetSize(a);
    assert(size == 3);
    testlogAfirm(testType, testNum, "The size of an array with some elements is obtained");
    arrayDelete(a);
}

void test16ArraySwap(char *testType, int testNum)
{
    array_t *a = arrayNew(1, 2);
    int element = 10;
    int element2 = 20;
    arrayAddLast(a, &element);
    arrayAddLast(a, &element2);
    assert(*(int *)(a->data[0]) == 10);
    assert(*(int *)(a->data[1]) == 20);
    arraySwap(a, 0, 1);
    assert(*(int *)(a->data[0]) == 20);
    assert(*(int *)(a->data[1]) == 10);
    testlogAfirm(testType, testNum, "Two elements of an array are swaped");
    arrayDelete(a);
}

void test17ArraySwapOutOfBounds(char *testType, int testNum)
{
    array_t *a = arrayNew(1, 2);
    int element = 10;
    int element2 = 20;
    arrayAddLast(a, &element);
    arrayAddLast(a, &element2);
    assert(*(int *)(a->data[0]) == 10);
    assert(*(int *)(a->data[1]) == 20);
    arraySwap(a, 3, 1);
    assert(*(int *)(a->data[0]) == 10);
    assert(*(int *)(a->data[1]) == 20);
    arraySwap(a, 0, 3);
    assert(*(int *)(a->data[0]) == 10);
    assert(*(int *)(a->data[1]) == 20);
    testlogAfirm(testType, testNum, "Out of bounds directions can´t be swapped");
    arrayDelete(a);
}

void test18ArrayPrintEmptyArray(char *testType, int testNum)
{
    FILE *file = fopen("outEmptyArray.txt", "w");
    array_t *a = arrayNew(1, 5);
    char ch;

    arrayPrint(a, file);
    fclose(file);

    file = fopen("outEmptyArray.txt", "r");

    ch = fgetc(file);
    assert(ch == '[');
    ch = fgetc(file);
    assert(ch == ']');

    testlogAfirm(testType, testNum, "Given an empty array, arrayPrint generates the corresponding file printing the expected: '[]'");
    fclose(file);
    arrayDelete(a);
}

void test19ArrayPrintArrayWithOneElement(char *testType, int testNum)
{
    array_t *a = arrayNew(1, 5);
    int element = 10;
    arrayAddLast(a, &element);

    FILE *file = fopen("outArray.txt", "w");
    arrayPrint(a, file);
    fclose(file);

    file = fopen("outArray.txt", "r");

    char ch;
    ch = fgetc(file);
    assert(ch == '[');
    ch = fgetc(file);
    assert(ch == '1');
    ch = fgetc(file);
    assert(ch == '0');
    ch = fgetc(file);
    assert(ch == ']');

    testlogAfirm(testType, testNum, "Given an array with one element, arrayPrint generates the corresponding file printing the expected");
    fclose(file);

    arrayDelete(a);
}

void test20ArrayPrintArrayWithSomeElements(char *testType, int testNum)
{
    array_t *a = arrayNew(1, 5);
    int element = 10;
    int element2 = 20;
    arrayAddLast(a, &element);
    arrayAddLast(a, &element2);

    FILE *file = fopen("outArraySomeElements.txt", "w");
    arrayPrint(a, file);
    fclose(file);

    file = fopen("outArraySomeElements.txt", "r");

    char ch;
    ch = fgetc(file);
    assert(ch == '[');
    ch = fgetc(file);
    assert(ch == '1');
    ch = fgetc(file);
    assert(ch == '0');
    ch = fgetc(file);
    assert(ch == ',');
    ch = fgetc(file);
    assert(ch == '2');
    ch = fgetc(file);
    assert(ch == '0');
    ch = fgetc(file);
    assert(ch == ']');

    testlogAfirm(testType, testNum, "Given an array with some elements, arrayPrint generates the corresponding file printing the expected");
    fclose(file);

    arrayDelete(a);
}

void test21ArrayGetValidPosition(char *testType, int testNum)
{
    array_t *a = arrayNew(1, 3);
    int num = 3;
    int num1 = 2;
    int num2 = 5;
    arrayAddLast(a, &num);
    arrayAddLast(a, &num1);
    arrayAddLast(a, &num2);
    uint8_t i = 1;
    int *result = arrayGet(a, i);
    assert(*result == num1);
    testlogAfirm(testType, testNum, "arrayGet valid position");

    arrayDelete(a);
}

void test22ArrayGetInvalidPosition(char *testType, int testNum)
{
    array_t *a = arrayNew(1, 3);
    int num = 3;
    int num1 = 2;
    int num2 = 5;
    arrayAddLast(a, &num);
    arrayAddLast(a, &num1);
    arrayAddLast(a, &num2);
    uint8_t i = -1;
    int *result = arrayGet(a, i);
    assert(result == NULL);
    testlogAfirm(testType, testNum, "arrayGet invalid position");

    arrayDelete(a);
}

/*---------------------------------- CARD TEST -------------------------------------------*/

void test01CardNew(char *testType, int testNum)
{
    int32_t number = 12;
    char str[6] = "swords";
    card_t *card = cardNew(str, &number);
    assert(strcmp(card->suit, str) == 0);
    assert(*(card->number) == 12);
    assert((card->stacked->type) == 3);
    assert((card->stacked->size) == 0);
    testlogAfirm(testType, testNum, "cardNew creates a card successfully");
    cardDeleteC(card);
}

void test02CardAddStacked(char *testType, int testNum)
{
    int32_t number = 12;
    char str[] = "swords";
    card_t *card = cardNew(str, &number);

    int32_t number2 = 4;
    char str2[] = "coins";
    card_t *card2 = cardNew(str2, &number2);

    cardAddStacked(card, card2);

    card_t *cardStacked = listGet(card->stacked, 0);

    assert((card->stacked->size) == 1);
    assert(*(cardStacked->number) == number2);
    assert(strcmp(cardStacked->suit, str2) == 0);

    testlogAfirm(testType, testNum, "cardAddStacked adds a cart to the stacked of another one");
    cardDeleteC(card);
    cardDeleteC(card2);
}

void test03CardDelete(char *testType, int testNum)
{
    int32_t *number = malloc(sizeof(int32_t));
    *number = 10;
    card_t *card = cardNew("swords", number);

    cardDelete(card);
    free(number);
    testlogAfirm(testType, testNum, "cardDelete of a card, empty stacked");
}

void test04CardDeleteWithStacked(char *testType, int testNum)
{
    int32_t *number = malloc(sizeof(int32_t));
    *number = 10;
    card_t *card = cardNew("swords", number);
    int32_t number2 = 4;
    char str2[6] = "coins";
    card_t *card2 = cardNew(str2, &number2);

    cardAddStacked(card, card2);

    cardDelete(card);
    cardDelete(card2);
    free(number);
    testlogAfirm(testType, testNum, "cardDelete of a card, stacked not empty");
}

void test05CardGetNumber(char *testType, int testNum)
{
    int32_t number = 12;
    char str[6] = "swords";
    card_t *card = cardNew(str, &number);

    int32_t *cardNumber = cardGetNumber(card);
    assert(*cardNumber == number);
    testlogAfirm(testType, testNum, "cardGetNumber obtains correctly the number of a card");

    cardDelete(card);
}

void test06CardGetStackedEmpty(char *testType, int testNum)
{
    int32_t number = 12;
    char str[6] = "swords";
    card_t *card = cardNew(str, &number);

    list_t *stacked = cardGetStacked(card);

    assert(stacked->size == 0);
    testlogAfirm(testType, testNum, "cardGetStacked obtains the empty stacked");

    cardDelete(card);
}

void test07CardGetStacked(char *testType, int testNum)
{
    int32_t number = 12;
    char str[] = "swords";
    card_t *card = cardNew(str, &number);

    int32_t number2 = 4;
    char str2[] = "coins";
    card_t *card2 = cardNew(str, &number2);

    cardAddStacked(card, card2);

    card_t *cardStacked = listGet(cardGetStacked(card), 0);
    assert(*(cardStacked->number) == number2);
    assert(strcmp(cardStacked->suit, str2));
    testlogAfirm(testType, testNum, "cardGetStacked gets the stacked with one card");

    cardDelete(card);
    cardDelete(card2);
}

void test08CardGetSuitCardWithNotEmptySuit(char *testType, int testNum)
{
    int32_t number = 10;
    card_t *card = cardNew("swords", &number);
    char *suit = cardGetSuit(card);
    assert(strcmp("swords", suit) == 0);
    cardDelete(card);
    testlogAfirm(testType, testNum, "cardGetSuit card with suit with some elements");
}

void test09CardGetSuitCardWithEmptySuit(char *testType, int testNum)
{
    int32_t number = 10;
    card_t *card = cardNew("", &number);
    char *suit = cardGetSuit(card);
    assert(strcmp("", suit) == 0);
    cardDelete(card);
    testlogAfirm(testType, testNum, "cardGetSuit card with empty suit");
}

void test10CardCmpEqualCards(char *testType, int testNum)
{
    int32_t number = 15;
    int32_t number2 = 15;
    card_t *card = cardNew("swords", &number);
    card_t *card2 = cardNew("swords", &number2);

    assert(cardCmp(card, card2) == 0);
    testlogAfirm(testType, testNum, "cardCmp two cards with same suit and number");
    cardDelete(card);
    cardDelete(card2);
}

void test11CardCmpCardWithDifferentSuit(char *testType, int testNum)
{
    int32_t number = 15;
    int32_t number2 = 15;
    card_t *card = cardNew("cups", &number);
    card_t *card2 = cardNew("swords", &number2);

    assert(cardCmp(card, card2) == 1);
    testlogAfirm(testType, testNum, "cardCmp two cards with different suit");
    cardDelete(card);
    cardDelete(card2);
}

void test12CardCmpCardsWithDifferentNumber(char *testType, int testNum)
{
    int32_t number = 24;
    int32_t number2 = 15;
    card_t *card = cardNew("cups", &number);
    card_t *card2 = cardNew("cups", &number2);

    assert(cardCmp(card, card2) == -1);
    testlogAfirm(testType, testNum, "cardCmp two cards with different number");
    cardDelete(card);
    cardDelete(card2);
}

void test13CardClone(char *testType, int testNum)
{
    int32_t number = 10;
    card_t *card = cardNew("swords", &number);
    card_t *cloned = cardClone(card);

    assert(cardCmp(card, cloned) == 0);
    testlogAfirm(testType, testNum, "cardClone");
    cardDelete(cloned);
    cardDelete(card);
}

void test14CardPrint(char *testType, int testNum)
{
    int32_t number = 12;
    char str[6] = "swords";
    card_t *card = cardNew(str, &number);
    FILE *frame = fopen("outCard.txt", "w");

    cardPrint(card, frame);
    testlogAfirm(testType, testNum, "cardPrint");
    fclose(frame);
    cardDelete(card);
}

/*---------------------------------- EXTRA TEST -------------------------------------------*/

void testExtraArray(card_t **cards, uint8_t size)
{
    array_t *array = arrayNew(3, 5);

    for (uint8_t i = 0; i < size; i++)
    {
        arrayAddLast(array, cards[i]);
    }

    FILE *file = fopen("outExtraArray.txt", "w");

    arrayPrint(array, file);

    card_t *cardDest = arrayGet(array, 1);
    card_t *cardSource = arrayGet(array, 3);

    cardAddStacked(cardDest, cardSource);
    card_t *c = arrayRemove(array, 3);

    arrayPrint(array, file);

    arrayDelete(array);
    cardDelete(c);
    fclose(file);
    testlogAfirm("Extra", 1, "Extra tests for array were completed");
}

void testExtraList(card_t **cards, uint8_t size)
{
    list_t *list = listNew(3);

    for (uint8_t i = 0; i < size; i++)
    {
        listAddLast(list, cards[i]);
    }

    FILE *file = fopen("outExtraList.txt", "w");
    listPrint(list, file);

    card_t *cardDest = listGet(list, 1);
    card_t *cardSource = listGet(list, 3);

    cardAddStacked(cardDest, cardSource);
    card_t *c = listRemove(list, 3);

    listPrint(list, file);

    listDelete(list);
    cardDelete(c);
    fclose(file);
    testlogAfirm("Extra", 2, "Extra tests for list were completed");
}

/*-------------------------  TESTS  -------------------------*/

void testInt()
{
    char *type = "Int";

    int32_t *number = malloc(sizeof(int32_t));
    *number = 10;
    int32_t *number_2 = malloc(sizeof(int32_t));
    *number_2 = 8;

    test01IntCmpWithFirstElementGreaterThanSecond(type, 1, number, number_2);

    test02IntCmpWithFirstElementLessThanSecond(type, 2, number_2, number);
    test03IntCmpWithEqualNumbers(type, 3, number);

    test04IntPrint(type, 4, number);

    test05IntClone(type, 5, number);

    intDelete(number);
    intDelete(number_2);
}

void testList()
{

    char *type = "List";

    test01ListNewWithoutElements(type, 1);

    test02ListAddFirstInEmptyList(type, 2);
    test03ListAddFirstConsecutivesInEmptyList(type, 3);

    test04ListGetSizeWithEmptyList(type, 4);
    test05ListGetSizeListWithSomeElements(type, 5);

    test06ListGetWithSomeElements(type, 6);

    test07ListAddLastWithEmptyList(type, 7);

    test08ListRemoveWithEmptyList(type, 8);
    test09ListRemoveListWithSomeElements(type, 9);

    test10ListSwapWithEmptyList(type, 10);
    test11ListSwapListWithOneElement(type, 11);
    test12ListSwapListWithSomeElements(type, 12);

    test13ListCloneEmptyList(type, 13);
    test14ListCloneListWithOneElement(type, 14);
    test15ListCloneListWithSomeElements(type, 15);

    test16ListPrintEmptyList(type, 16);
    test17ListPrintListWithOneElement(type, 17);
    test18ListPrintListWithSomeElements(type, 18);
}

void testString()
{
    char type[] = "String";

    test01StrDeleteEmpty(type, 1);
    test02StrDeleteNotEmpty(type, 2);

    test03StrLenEmpty(type, 3);
    test04StrLen(type, 4);

    test05StrPrint(type, 5);
    test06StrPrintEmpty(type, 6);

    test07StrCloneStringWithSomeElements(type, 7);
    test08StrCloneEmptyString(type, 8);

    test09StrCmpOfTwoStringWithSomeElements(type, 9);
    test10StrCmpOfTwoEmptyStrings(type, 10);
    test11StrCmpWithAnEmptyString(type, 11);
}

void testArray()
{
    char type[] = "Array";

    test01ArrayNew(type, 1);

    test02ArrayAddLastWithOneElement(type, 2);
    test03ArrayAddLastWithSomeElements(type, 3);
    test04ArrayAddLastOverflow(type, 4);

    test05ArrayRemoveOneElement(type, 5);
    test06ArrayRemoveElementInTheMiddle(type, 6);
    test07ArrayRemoveLastElement(type, 7);
    test08ArrayRemoveOutOfBounds(type, 8);
    test09ArrayRemoveArrayWithTwoElements(type, 9);
    test10ArrayRemoveDeArrayWithOneElement(type, 10);

    test11ArrayDeleteEmptyArray(type, 11);
    test12ArrayDeleteArrayWithSomeElements(type, 12);

    test13ArrayGetSizeEmpty(type, 13);
    test14ArrayGetSizeWithOneElement(type, 14);
    test15ArrayGetSizeWithSomeElements(type, 15);

    test16ArraySwap(type, 16);
    test17ArraySwapOutOfBounds(type, 17);

    test18ArrayPrintEmptyArray(type, 18);
    test19ArrayPrintArrayWithOneElement(type, 19);
    test20ArrayPrintArrayWithSomeElements(type, 20);

    test21ArrayGetValidPosition(type, 21);
    test22ArrayGetInvalidPosition(type, 22);
}

void testCard()
{

    char *type = "Card";

    test01CardNew(type, 1);

    test02CardAddStacked(type, 2);

    test03CardDelete(type, 3);
    test04CardDeleteWithStacked(type, 4);

    test05CardGetNumber(type, 5);
    test06CardGetStackedEmpty(type, 6);
    test07CardGetStacked(type, 7);
    test08CardGetSuitCardWithNotEmptySuit(type, 8);
    test09CardGetSuitCardWithEmptySuit(type, 9);

    test10CardCmpEqualCards(type, 10);
    test11CardCmpCardWithDifferentSuit(type, 11);
    test12CardCmpCardsWithDifferentNumber(type, 12);

    test13CardClone(type, 13);

    test14CardPrint(type, 14);
}

void testExtra()
{
    int32_t num1 = 12;
    int32_t num2 = 1;
    int32_t num3 = 4;
    int32_t num4 = 6;
    int32_t num5 = 7;

    card_t *card1 = cardNew("swords", &num1);
    card_t *card2 = cardNew("clubs", &num2);
    card_t *card3 = cardNew("coins", &num3);
    card_t *card4 = cardNew("coins", &num4);
    card_t *card5 = cardNew("cups", &num5);

    card_t *cards[5] = {card1, card2, card3, card4, card5};

    testExtraArray(cards, 5);
    testExtraList(cards, 5);

    for (int i = 0; i < 5; i++)
    {
        cardDelete(cards[i]);
    }
}

/*-----------------------------------------------------------------*/

int main(void)
{
    testlogTitle("Test made for implementations in C");
    testInt();
    testlogSeparator();
    testList();

    testlogTitle("Test made for implementations in asm");
    testString();
    testlogSeparator();
    testArray();
    testlogSeparator();
    testCard();

    testlogTitle("Extra tests");
    testExtra();
    testlogSeparator();
}
