
global strClone
global strPrint
global strCmp
global strLen
global strDelete

global arrayNew
global arrayDelete
global arrayPrint
global arrayGetSize
global arrayAddLast
global arrayGet
global arrayRemove
global arraySwap

global cardCmp
global cardClone
global cardAddStacked
global cardDelete
global cardGetSuit
global cardGetNumber
global cardGetStacked
global cardPrint
global cardNew

section .rodata

format: db "%s", 0   ; strPrint string format
null_string: db "NULL", 0
open_sqrbracket: db "[", 0
close_sqrbracket: db "]", 0
comma: db ",", 0
open_bracket: db "{", 0
hyphen: db "-", 0
close_bracket: db "}", 0

section .text

extern malloc
extern free
extern fprintf
extern getCloneFunction
extern getPrintFunction
extern listPrint
extern intPrint
extern listClone
extern intClone
extern listNew
extern intCmp
extern listAddFirst
extern intDelete
extern listDelete
extern getDeleteFunction

%define PTR_SIZE 8

; ** String **
;char* strClone(char* a);
strClone:
    ; Prologue
    push rbp
    mov rbp, rsp
    push rbx
    sub rsp, 8

    mov rbx, rdi            ; Store a in rbx

    call strLen             ; Call strlen for a

    inc rax                 ; Increase return of strlen to consider null character
    mov rdi, rax            ; Set the argument for malloc (size of the string)

    call malloc             ; Call malloc for strlen(a) + 1

    xor rsi, rsi            ; Set iterator to 0
    xor ch, ch              ; Set comparison register to 0

    .loop:
        mov cl, [rbx + rsi] ; Load the next character from a
        mov [rax + rsi], cl ; Copy the character to returned ptr
        cmp [rbx + rsi], ch ; Compare if 0 with next character of a
        je .end_loop        ; if equal, end loop
        inc rsi             ; else increase iterator
        jmp .loop           ; and loop
    .end_loop:

    ; Epilogue
    add rsp, 8
    pop rbx
    pop rbp
    ret

;void strPrint(char* a, FILE* pFile)
strPrint:
    ; Prologue
    push rbp
    mov rbp, rsp

    xor cl, cl                  ; Set comparison register to 0

    mov rdx, rdi                ; Set a as 3rd argument for fprintf
    mov rdi, rsi                ; Set pFile as 1st argument for fprintf

    cmp [rdx], cl               ; Compare if a is empty
    je .empty

    mov rsi, format             ; if a is not empty, load str format as second argument for fprintf
    jmp .end

    .empty:
        mov rsi, null_string    ; if a is empty

    .end:
    call fprintf                ; fprintf(pFile, "null") or fprintf(pFile, "%s", a)

    ; Epilogue
    pop rbp
    ret

;uint32_t strLen(char* a);
strLen:
    xor rax, rax            ; Counter set to 0
    xor cl, cl              ; Set comparison register to 0

    .loop:
        cmp [rdi + rax], cl ; Compare the next character of a to 0
        je .end_loop        ; if equal end loop
        inc eax             ; else increase counter
        jmp .loop           ; and loop
    .end_loop:

    ret

;int32_t strCmp(char* a, char* b);
strCmp:
    xor rdx, rdx                ; Iterator
    xor ch, ch                  ; Set comparison register to 0

    .loop:
        mov cl, [rdi + rdx]     ; Load the next a char
        cmp [rsi + rdx], cl     ; Compare the next a char with the next b char
        je .condition_2         ; if equal evaluate second condition
        jmp .end_loop           ; else end loop
        .condition_2:
            cmp [rdi + rdx], ch ; compare next character of a with 0
            je .end_loop        ; if equal end loop
            inc rdx             ; else increment iterator
            jmp .loop           ; and loop
    .end_loop:

    xor eax, eax                ; Set return value to 0

    cmp cl, [rsi + rdx]         ; compare the a character and the b character
    je .end                     ; if equal return 0
    jl .lower                   ; if lower return 1
    jg .greater                 ; if greater return -1

    .lower:
        inc eax                 ; Is lower then increase return value
        jmp .end
    .greater:
        dec eax                 ; Is greater then decrease return value

    .end:
    ret

;void strDelete(char* a);
strDelete:
    push rbp
    mov rbp, rsp
    call free       ; free(a)
    pop rbp
    ret

; ** Array **

%define ARRAY_TYPE_OFFSET 0
%define ARRAY_SIZE_OFFSET 4
%define ARRAY_CAPACITY_OFFSET 5
%define ARRAY_DATA_OFFSET 8
%define ARRAY_SIZE 16

; uint8_t arrayGetSize(array_t* a)
arrayGetSize:
    mov al, [rdi + ARRAY_SIZE_OFFSET]   ; Set a size as return value
    ret

; void arrayAddLast(array_t* a, void* data)
arrayAddLast:
    ; Prologue
    push rbp
    mov rbp, rsp

    push rbx
    push r12
    push r13
    sub rsp, 8                              ; Align stack

    mov rbx, rdi                            ; Store a in rbx
    mov r12, rsi                            ; Store data in r12
    mov r13, [rdi + ARRAY_DATA_OFFSET]      ; Store a data in r13

    mov cl, [rbx + ARRAY_SIZE_OFFSET]       ; Store in cl a size
    cmp cl, [rbx + ARRAY_CAPACITY_OFFSET]   ; Compare if a is full
    je .end                                 ; if full end function

    mov edi, [rbx + ARRAY_TYPE_OFFSET]      ; Set first argument of getCloneFunction to a type
    call getCloneFunction                   ; getCloneFunction(a->type)

    mov rdi, r12                            ; Set first argument of the cloneFunction to data
    call rax                                ; Call the clone function

    xor rcx, rcx                            ; Empty rcx

    mov cl, [rbx + ARRAY_SIZE_OFFSET]       ; Store in cl a size
    mov [r13 + PTR_SIZE * rcx], rax         ; Store in data[size] the cloned value
    inc byte [rbx + ARRAY_SIZE_OFFSET]      ; increase array size

    ; Epilogue
    .end:
    add rsp, 8
    pop r13
    pop r12
    pop rbx
    pop rbp
    ret

; void* arrayGet(array_t* a, uint8_t i)
arrayGet:
    xor rax, rax                        ; Set return value to 0

    cmp sil, [rdi + ARRAY_SIZE_OFFSET]  ; Compare i with the size of a
    jae .end                            ; if above or equal end function

    mov rdi, [rdi + ARRAY_DATA_OFFSET]  ; Store in rdi a data
    mov rax, [rdi + PTR_SIZE * rsi]     ; Set return value to a->data[i]

    .end:
    ret

; array_t* arrayNew(type_t t, uint8_t capacity)
arrayNew:
    ; Prologue
    push rbp
    mov rbp, rsp

    push rbx
    push r12
    push r13
    push r14

    mov rbx, rdi                            ; Store t value in rbx
    xor r12, r12
    mov r12b, sil                           ; Store capacity value in r12b
    xor r13, r13                            ; r13 = 0

    mov rdi, ARRAY_SIZE                     ; Set ARRAY_SIZE as the first parameter for malloc
    call malloc                             ; malloc(ARRAY_SIZE)

    mov [rax + ARRAY_TYPE_OFFSET], rbx      ; Store t in the type of the array
    mov [rax + ARRAY_SIZE_OFFSET], r13b     ; Store 0 in the size of the array
    mov [rax + ARRAY_CAPACITY_OFFSET], r12b ; Store capacity in the capacity of the array

    mov r14, rax                            ; Store in r14 the ptr to the new array

    lea rdi, [PTR_SIZE * r12]               ; Set capacity * 8 as first parameter for malloc
    call malloc                             ; malloc(capacity * PTR_SIZE)

    mov [r14 + ARRAY_DATA_OFFSET], rax      ; Point the data of the new array to the allocated memory
    mov rax, r14                            ; Set return value to the new array

    ; Epilogue
    pop r14
    pop r13
    pop r12
    pop rbx
    pop rbp
    ret

; void* arrayRemove(array_t* a, uint8_t i)
arrayRemove:
    ; Prologue
    push rbp
    mov rbp, rsp

    xor rax, rax                        ; Return value = 0
    cmp sil, [rdi + ARRAY_SIZE_OFFSET]  ; compare i with size
    jae .end                            ; If above or equal return 0

    push rbx
    push r12
    push r13
    push r14

    mov rbx, rdi                        ; Store a in rbx
    mov r12, rsi                        ; Store i in r12

    call arrayGet                       ; arrayGet(a, i)
    mov r14, rax                        ; Store removed element in r14

    xor r13, r13                        ; Empty r13
    mov r13b, [rbx + ARRAY_SIZE_OFFSET] ; Store a size in r13b
    dec r13b

    ; Move all elements past the removed one to the left
    .loop:
        cmp r12, r13                    ; Compare i with a size
        jae .end_loop                   ; if above or equal end loop
        mov rdi, rbx                    ; else set a as first parameter for arraySwap
        mov rsi, r12                    ; set iterator as second parameter for arraySwap
        inc r12
        mov rdx, r12                    ; set iterator + 1 as third parameter for arraySwap
        dec r12                         ; restore the iterator
        call arraySwap                  ; arraySwap(a, i, i + 1)

        inc r12                         ; Increase iterator
        jmp .loop                       ; and loop
    .end_loop:

    dec byte [rbx + ARRAY_SIZE_OFFSET]  ; Decrement a size

    mov rax, r14                        ; Set return value to the removed element

    ; Epilogue
    pop r14
    pop r13
    pop r12
    pop rbx

    .end:
    pop rbp
    ret

; void arraySwap(array_t* a, uint8_t i, uint8_t j)
arraySwap:
    cmp sil, [rdi + ARRAY_SIZE_OFFSET]  ; compare i with size
    jae .end                            ; if equal or above end function
    cmp dl, [rdi + ARRAY_SIZE_OFFSET]   ; compare j with size
    jae .end                            ; if equal or above end function

    mov rdi, [rdi + ARRAY_DATA_OFFSET]  ; Store in rdi a data

    mov rcx, [rdi + PTR_SIZE * rsi]     ; Store a->data[i] in an auxiliary
    mov r8, [rdi + PTR_SIZE * rdx]      ; Store a->data[j] in an auxiliary
    mov [rdi + PTR_SIZE * rsi], r8      ; Store in a->data[i] the a->data[j] value
    mov [rdi + PTR_SIZE * rdx], rcx     ; Store in a->data[j] the a->data[i] value

    .end:
    ret

; void arrayDelete(array_t* a) {
arrayDelete:
    ; Prologue
    push rbp
    mov rbp, rsp

    push rbx
    push r12
    push r13
    push r14
    push r15
    sub rsp, 8                          ; Align stack

    mov rbx, rdi                        ; Store a in rbx
    xor r12, r12                        ; Empty r12
    mov r12b, [rbx + ARRAY_SIZE_OFFSET] ; store in r12b a size

    mov rdi, [rbx + ARRAY_TYPE_OFFSET]  ; Set a type as first parameter of getDeleteFunction
    call getDeleteFunction              ; getDeleteFunction(a->type)
    mov r13, rax                        ; Store the ptr to the deleter function in r13

    xor r14, r14                        ; Set iterator to 0
    .loop:
        cmp r14b, r12b                  ; Compare iterator with a size
        jae .end_loop                   ; if above or equal end loop

        mov rdi, rbx                    ; set a as first argument of arrayGet
        mov rsi, r14                    ; set iterator as second argument of arrayGet
        call arrayGet                   ; arrayGet(a, iterator)

        mov rdi, rax                    ; Set return value of arrayGet as first argument of the deleter function
        call r13                        ; Delete the value

        inc r14b                        ; Increase iterator
        jmp .loop                       ; and loop again
    .end_loop:

    mov rdi, [rbx + ARRAY_DATA_OFFSET]  ; Set a data as first argument of free
    call free                           ; free(a->data)

    mov rdi, rbx                        ; Set a as first argument of free
    call free                           ; free(a)

    add rsp, 8
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    pop rbp
    ret

;void arrayPrint(array_t* a, FILE* pFile)
arrayPrint:
    ; Prologue
    push rbp
    mov rbp, rsp

    push rbx
    push r12
    push r13
    push r14
    push r15
    sub rsp, 8

    mov rbx, rdi                            ; Store a in rbx
    mov r12, rsi                            ; Store pFile in r12

    mov edi, [rdi + ARRAY_TYPE_OFFSET]      ; Set a type as first argument of getPrintFunction
    call getPrintFunction                   ; getPrintFunction(a->type)
    mov r13, rax                            ; Store ptr to the print function in r13

    mov rdi, r12                            ; Set pfile as first argument of fprintf
    mov rsi, open_sqrbracket                ; Set open_sqrbracket as second argument of fprintf
    call fprintf                            ; fprintf(pFile, "[")

    xor r15, r15                            ; Empty r15
    mov r15b, [rbx + ARRAY_SIZE_OFFSET]     ; store array size in r15b
    xor r14, r14                            ; Set iterator to 0
    .loop:
        cmp r14b, r15b                      ; Compare iterator with the array size
        jae .end_loop                       ; if above or equal end loop

        mov rdi, rbx                        ; Set a as first argument of arrayGet
        xor rsi, rsi                        ; Empty rsi
        mov sil, r14b                       ; Set iterator as second argument of arrayGet
        call arrayGet                       ; arrayGet(a, iterator)

        mov rdi, rax                        ; Set the return value of arrayGet as first argument of the printer function
        mov rsi, r12                        ; Set pFile as second argument of the printer function
        call r13                            ; printFunction(arrayGet(a, iterator))

        dec r15b                            ; Set r15 to a size - 1
        cmp r14b, r15b                      ; Compare iterator with a size - 1
        je .end_if                          ; if equal doesn't enter if

        .if:
            mov rdi, r12                    ; Set pFile as first argument of fprintf
            mov rsi, comma                  ; Set comma as second argument of fprintf
            call fprintf                    ; fprintf(pFile, ",")
        .end_if:

        inc r15b                            ; Restore to size
        inc r14b                            ; Increment iterator
        jmp .loop                           ; and loop
    .end_loop:

    mov rdi, r12                            ; Set pFile as first argument for fprintf
    mov rsi, close_sqrbracket               ; set close_sqrbracker as second argument for fprintf
    call fprintf                            ; fprintf(pFile, "]")

    ; Epilogue
    add rsp, 8
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    pop rbp
    ret

; ** Card **

%define CARD_SIZE 24
%define CARD_SUIT_OFFSET 0
%define CARD_NUMBER_OFFSET 8
%define CARD_STACKED_OFFSET 16
%define CARD_TYPE 3

; card_t* cardNew(char* suit, int32_t* number)
cardNew:
    ; Prologue
    push rbp
    mov rbp, rsp

    push rbx
    push r12
    push r13
    sub rsp, 8                              ; Align stack

    mov rbx, rdi                            ; Store suit in rbx
    mov r12, rsi                            ; Store number in r12

    mov rdi, CARD_SIZE                      ; Set CARD_SIZE as first argument of malloc
    call malloc

    mov r13, rax                            ; Store ptr to new card in r13

    mov rdi, rbx                            ; Set suit as first argument of strClone
    call strClone                           ; strClone(suit)
    mov [r13 + CARD_SUIT_OFFSET], rax       ; Store the return value of strClone as the suit of the new card

    mov rdi, r12                            ; Set number as first argument of intClone
    call intClone                           ; strClone(number)
    mov [r13 + CARD_NUMBER_OFFSET], rax     ; Store the return value of strClone as the number of the new card

    mov rdi, CARD_TYPE                      ; Set CARD_TYPE as first argument of listNew
    call listNew                            ; listNew(CARD_TYPE)
    mov [r13 + CARD_STACKED_OFFSET], rax    ; Set the return value of listNew to the stacked of the new card

    mov rax, r13                            ; Set the new card as the return value

    add rsp, 8
    pop r13
    pop r12
    pop rbx
    pop rbp
    ret

;char* cardGetSuit(card_t* c)
cardGetSuit:
    mov rax, [rdi + CARD_SUIT_OFFSET]   ; Set as return value the c suit
    ret

;int32_t* cardGetNumber(card_t* c)
cardGetNumber:
    mov rax, [rdi + CARD_NUMBER_OFFSET] ; Set as return value the c number
    ret

;list_t* cardGetStacked(card_t* c)
cardGetStacked:
    mov rax, [rdi + CARD_STACKED_OFFSET]    ; Set as return value the c stacked
    ret


;void cardPrint(card_t* c, FILE* pFile)
cardPrint:
    push rbp
    mov rbp, rsp
    push rbx
    push r12

    mov rbx, rdi                            ; Store c in rbx
    mov r12, rsi                            ; Store pFile in r12

    mov rdi, r12                            ; Set pFile as first argument of fprintf
    mov rsi, open_bracket                   ; Set open_bracket as second argument of fprintf
    call fprintf                            ; fprintf(pFile, "{")

    mov rdi, [rbx + CARD_SUIT_OFFSET]       ; Set c suit as first argument of strPrint
    mov rsi, r12                            ; Set pFile as second argument of strPrint
    call strPrint                           ; strPrint(c->suit, pFile)

    mov rdi, r12                            ; Set pFile as first argument of fprintf
    mov rsi, hyphen                         ; Set hyphen as second argument of fprintf
    call fprintf                            ; fprintf(pFile, "-")

    mov rdi, [rbx + CARD_NUMBER_OFFSET]     ; Set c number as first argument of intPrint
    mov rsi, r12                            ; Set pFile as second argument of fprintf
    call intPrint                           ; intPrint(c->number, pFile)

    mov rdi, r12                            ; Set pFile as first argument of fprintf
    mov rsi, hyphen                         ; Set hyphen as second argument of fprintf
    call fprintf                            ; fprintf(pFile, "-")

    mov rdi, [rbx + CARD_STACKED_OFFSET]    ; Set c stacked as first argument of listPrint
    mov rsi, r12                            ; Set pFile as second argument of listPrint
    call listPrint                          ; listPrint(c->stacked, pFile)

    mov rdi, r12                            ; Set pFile as first argument of fprintf
    mov rsi, close_bracket                  ; Set open_bracket as second argument of fprintf
    call fprintf                            ; fprintf(pFile, "}")

    pop r12
    pop rbx
    pop rbp
    ret


;int32_t cardCmp(card_t* a, card_t* b)
cardCmp:
    push rbp
    mov rbp, rsp
    push rbx
    push r12
    push r13
    sub rsp, 8                              ; Align stack

    mov rbx, rdi                            ; Store a in rbx
    mov r12, rsi                            ; Store b in r12
    xor r13, r13                            ; Empty r13

    mov rdi, [rbx + CARD_SUIT_OFFSET]       ; Set a suit as first argument of strCmp
    mov rsi, [r12 + CARD_SUIT_OFFSET]       ; Set b suit as second argument of strCmp
    call strCmp                             ; strCmp(a->suit, b->suit)

    cmp eax, r13d                           ; Compare strCmp return value with 0
    je .number_comparision                  ; If equal compare numbers
    jmp .end                                ; else return the return value of strCmp

    .number_comparision:
        mov rdi, [rbx + CARD_NUMBER_OFFSET] ; Set a number as first argument of intCmp
        mov rsi, [r12 + CARD_NUMBER_OFFSET] ; Set b number as second argument of intCmp
        call intCmp                         ; intCmp(a->number, b->number)

    ; The return value will be the return value of intCmp

    .end:
    add rsp, 8
    pop r13
    pop r12
    pop rbx
    pop rbp
    ret

;card_t* cardClone(card_t* c)
cardClone:
    push rbp
    mov rbp, rsp
    push rbx
    push r12

    mov rbx, rdi                            ; Store c in rbx

    mov rdi, [rbx + CARD_SUIT_OFFSET]       ; Set c suit as first argument of cardNew
    mov rsi, [rbx + CARD_NUMBER_OFFSET]     ; Set c number as second argument of cardNew
    call cardNew                            ; cardNew(c->suit, c->number)

    mov r12, rax                            ; Store ptr to new card in r12

    mov rdi, [r12 + CARD_STACKED_OFFSET]    ; Set new card stacked as first argument of free
    call free                               ; free(newCard->stacked)

    mov rdi, [rbx + CARD_STACKED_OFFSET]    ; Set c stacked as first argument of listClone
    call listClone                          ; listClone(c->stacked)

    mov [r12 + CARD_STACKED_OFFSET], rax    ; Store in new card stacked the return value of listClone

    mov rax, r12                            ; Set ptr to new card as return value

    pop r12
    pop rbx
    pop rbp
    ret

;void cardAddStacked(card_t* c, card_t* card)
cardAddStacked:
    push rbp
    mov rbp, rsp

    mov rdi, [rdi + CARD_STACKED_OFFSET]    ; Set c stacked as first argument of listAddFirst
    call listAddFirst                       ; listAddFirst(c->stacked, card)

    pop rbp
    ret

;void cardDelete(card_t* c)
cardDelete:
    push rbp
    mov rbp, rsp
    push rbx
    sub rsp, 8                              ; Align stack

    mov rbx, rdi                            ; Store c in rbx

    mov rdi, [rbx + CARD_SUIT_OFFSET]       ; Set c suit as first argument of strDelete
    call strDelete                          ; strDelete(c->suit)

    mov rdi, [rbx + CARD_NUMBER_OFFSET]     ; Set c number as first argument of intDelete
    call intDelete                          ; intDelete(c->number)

    mov rdi, [rbx + CARD_STACKED_OFFSET]    ; Set c stacked as first argument of listDelete
    call listDelete                         ; listDelete(c->stacked)

    mov rdi, rbx                            ; Set c as first argument of free
    call free                               ; free(c)

    add rsp, 8
    pop rbx
    pop rbp
    ret

