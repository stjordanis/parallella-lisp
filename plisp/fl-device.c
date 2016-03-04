#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>

#if EPIPHANY
#include "e-lib.h"
#endif

#include "common.h"
#include "proto.h"

#define BUF_ADDRESS 0x8f000000

//
// Add items to the history
//
void pr(node *cell) {
    atl(&history, cell);
}

void addInt(long long i) {
    pr(integer(i));
}

void addString(char *s) {
    pr(sym(s));
}

void addValue(char *s, long long i) {
    addString(s);
    addInt(i);
}

//
// local version of strcpy
//
char *scpy(char *s1, const char *s2) {
    char *s = s1;
    while ((*s++ = *s2++) isnt '\0')
        ;
    *s = '\0';
    return (s1);
}

//
// local version of atoi
//
long long stoi(const char *c)
{
    long long value = 0;
    int sign = 1;
    if ( *c == '+' or *c == '-' ) {
        if ( *c == '-' ) {
            sign = -1;
        }
        c++;
    }
    while (isdigit(*c)) {
        value *= 10;
        value += (int) (*c-'0');
        c++;
    }
    return (value * sign);
}

//
// length of a string
//
int slen(char *s) {
    int c = 0;
    while (*(s+c)) {
        c++;
    }
    return c;
}

//
// Save global variables
//
void saveGlobals(char *message) {
    edata *data = &memory->data[id];
    scpy(data->message, message);
    data->id = id;
    data->ememory_size = sizeof(ememory);
    data->node_size = sizeof(node);
    data->nnodes = nnodes;
    data->nodemem = nodemem;
    data->nnames = nnames;
    data->namemem = namemem;
    data->nstrings = nstrings;
    data->stringmem = stringmem;
}

#if EPIPHANY

//
// get the core ID
//
int coreID(unsigned int *row, unsigned int *col) {
    e_coreid_t coreid;
    coreid = e_get_coreid();
    coreid = coreid - e_group_config.group_id;
    *row = (coreid >> 6) & 0x3f;
    *col = coreid & 0x3f;
    return ((*row * 4) + *col);
}

//
// Initilaize core memory
//
void coreInit(int argc, char *argv[]) {
    memory = (ememory *)(BUF_ADDRESS);
    freeStringArray = &memory->data[id].freeStringArray[0];
    freeNodeArray = &memory->data[id].freeNodeArray[0];
    freeNameArray = &memory->data[id].freeNameArray[0];
    freelist = freeNodeArray;
    stringfreelist = freeStringArray;
    namefreelist = freeNameArray;
}

//
// Print memory stats and store a pointer to the history list.
// Put the processor in idle mode
//
void setflag(char *message) {
    unsigned *d;
    saveGlobals(message);
    memory->data[id].NULLPTR = NULLPTR;
    memory->data[id].history = history;
    memory->data[id].finished = 1;
    d = (unsigned *) 0x7000;
    (*(d)) = 0x00000001;
    __asm__ __volatile__("idle");
}

#else

//
// Read a text file
//
char *readFile(char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (not file) {
        fprintf(stderr, "%s\n", "file not found");
        exit(-1);
    }
    string *code;
    size_t n = 0;
    int c;
    if (file == NULL) {
        return NULL;
    }
    code = smalloc();
    while ((c = fgetc(file)) isnt EOF) {
        code->s[n++] = (char)c;
    }
    code->s[n] = '\0';
    return code->s;
}

//
// Create the freelist
//
void createFreelist(ememory *memory, int rows, int cols) {
    int id, k;
    node *freeNodeArray;
    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++) {
            id = (cols * i) + j;
            freeNodeArray = memory->data[id].freeNodeArray;
            for (k = 0; k < FREEOBJECT - 1; k++) {
                freeNodeArray[k].next = &freeNodeArray[k + 1];
                freeNodeArray[k].type = FREE;
            }
            freeNodeArray[FREEOBJECT - 1].type = FREE;
            freeNodeArray[FREEOBJECT - 1].next = NULL;
        }
    }
}

void createStringFreelist(ememory *memory, int rows, int cols) {
    int id, k;
    string *freeStringArray;
    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++) {
            id = (cols * i) + j;
            freeStringArray = memory->data[id].freeStringArray;
            for (k = 0; k < FREESTRING - 1; k++) {
                freeStringArray[k].next = &freeStringArray[k + 1];
            }
            freeStringArray[FREESTRING - 1].next = NULL;
        }
    }
}

void createNameFreelist(ememory *memory, int rows, int cols) {
    int id, k;
    namestr *freeNameArray;
    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++) {
            id = (cols * i) + j;
            freeNameArray = memory->data[id].freeNameArray;
            for (k = 0; k < FREENAME - 1; k++) {
                freeNameArray[k].next = &freeNameArray[k + 1];
            }
            freeNameArray[FREENAME - 1].next = NULL;
        }
    }
}

//
// Generate a core ID for testing
//
int coreID(unsigned int *row, unsigned int *col) {
    *row = 1;
    *col = 1;
    return ((*row * 4) + *col);
}

//
// Initialize globals
//
void coreInit(int argc, char *argv[]) {
    char *code;
    memory = (ememory *)calloc(1, sizeof(ememory));
    if (not memory) {
        fprintf(stderr, "%s\n", "out of memory in init_ememory");
        exit(-1);
    }
    freeStringArray = &memory->data[id].freeStringArray[0];
    freeNodeArray = &memory->data[id].freeNodeArray[0];
    freeNameArray = &memory->data[id].freeNameArray[0];
    freelist = freeNodeArray;
    stringfreelist = freeStringArray;
    namefreelist = freeNameArray;
    if (argc == 2) {
        code = readFile(argv[1]);
    }
    else {
        code = readFile("code/p2.lisp");
    }
    scpy(memory->data[id].code, code);
    createFreelist(memory, 4, 4);
    createNameFreelist(memory, 4, 4);
    createStringFreelist(memory, 4, 4);
}

//
// Printing routines
//

void nl(void) {
    printf("\n");
}

void prpair(node *l) {
    printf("%s", "(");
    print(car(l));
    printf("%s", ".");
    print(cdr(l));
    printf("%s", ")");
}

void print(node *l) {
    if (nullp(l)) {
        printf(" NULL ");
    }
    else if (teep(l)) {
        printf(" t ");
    }
    else if (nilp(l)) {
        printf(" nil ");
    }
    else if (symp(l)) { // symbol
        printf(" %s ", name(l));
    }
    else if (intp(l)) { // integer
        printf(" %lld ", ival(l));
    }
    else if (lambdap(l)) { // lambda expression
        printf(" #lambda ");
        print(largs(l));
        print(lbody(l));
    } else if (subrp(l)) {
        printf(" subr ");
    }
    else if (fsubrp(l)) {
        printf(" fsubr ");
    }
    else if (pairp(l)) { // pair
        prpair(l);
    }
    else if (consp(l)) {
        if (not nullp(cdr(l)) and not consp(cdr(l))) { // untyped dotted pair
            prpair(l);
        }
        else { // list
            printf("( ");
            for (node *ptr = l; ptr isnt NULLPTR; ptr = cdr(ptr)) {
                print(car(ptr));
            }
            printf(" )");
        }
    } else {
        printf(" Something went wrong \n");
    }
}

//
// Print out global variables
//
void prGlobals(ememory *memory, int id) {
    edata *data = &memory->data[id];
    printf("\n");
    printf("processor id: \t\t%d\n", data->id);
    printf("memory: \t\t%d\n", data->ememory_size);
    printf("node size: \t\t%d\n", data->node_size);
    printf("nnodes: \t\t%d\n", data->nnodes);
    printf("nodemem: \t\t%d\n", data->nodemem);
    printf("nnames: \t\t%d\n", data->nnames);
    printf("namemem: \t\t%d\n", data->namemem);
    printf("nstrings: \t\t%d\n", data->nstrings);
    printf("stringmem: \t\t%d\n", data->stringmem);
    printf("setflag message: \t%s\n", data->message);
}

//
// Print out the history list and exit
//
void setflag(char *message) {
    saveGlobals(message);
    int n = 1;
    if (nnodes < FREEOBJECT and nnames < FREENAME) {
        forlist (ptr in history) {
            if (n) {
                printf("> ");
            }
            n = not n;
            print(car(ptr));
            printf("\n\n");
        }
    }
    prGlobals(memory, id);
    exit(0);
}

#endif

// LISP Code
#include "lisp.c"
// End of LISP Code

//
// test on the host - simulate the info for a single core
//
int main(int argc, char *argv[]) {
    unsigned int row, col;
    //
    // get the core id
    //
    id = coreID(&row, &col);
    //
    // Initialize the core
    //
    coreInit(argc, argv);
    //
    // load the code
    //
    input = &memory->data[id].code[0];
    //
    // Read, Eval and Print
    //
    REPL(input);
    //
    // Print stats and exit
    //
    setflag("Exited normally!");
    return 0;
}
