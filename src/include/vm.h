#ifndef KAVA_VM_H
#define KAVA_VM_H

#include "chunk.h"
#include "common.h"
#include "debug.h"
#include "value.h"
#include "compiler.h"

#define new_vm(v) nil; init_vm(&v)
#define stack_max 256

typedef struct { 
    Chunk *chunk;
    uint8_t *ip;
    Value stack[stack_max];
    Value *stack_top;
} Vm;

typedef enum {
    int_ok,
    int_compile_error,
    int_runtime_error
} Result;

static void run_repl(Vm *);
static void run_file(Vm *, const char *);
static char *read_file(const char *);
void init_vm(Vm *);
void reset_stack(Vm *);
void free_vm(Vm *);
void run(Vm *);
void push(Vm *, Value);
Value pop(Vm *);
Result interpret(Vm *, const char *);

#endif