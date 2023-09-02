#include "include/vm.h"

static void run_repl(Vm *vm) {
    char line[1024];

    while (true) {
        printf("> ");

        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }

        interpret(vm, line);
    }
}

static void run_file(Vm *vm, const char *path) {
    char *source = read_file(path);
    Result result = interpret(vm, source);
    free(source);

    switch (result) {
    case int_compile_error:
        exit(65);
    case int_runtime_error:
        exit(70);
    default:
        return;
    }
}

static char *read_file(const char *path) {
    FILE *file = fopen(path, "rb");
    if (file == nil) {
        fprintf(stderr, "Could not open file \"%s\".\n", path);
        exit(74);
    }

    fseek(file, 0L, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    char *buffer = (char*)malloc(file_size+1);
    if (buffer == nil) {
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
        exit(74);
    }

    size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
    if (bytes_read < file_size) {
        fprintf(stderr, "Could not read file \"%s\".\n", path);
        exit(74);
    }

    buffer[bytes_read] = '\0';

    fclose(file);
    return buffer;
}

void init_vm(Vm *vm) {
    reset_stack(vm);
}

void reset_stack(Vm *vm) {
    vm->stack_top = vm->stack;
}

void free_vm(Vm *vm) {

}

void run(Vm *vm) {
#define read_byte (*vm->ip++)
#define read_constant (vm->chunk->constants.values[read_byte])
#define binary_op(op)\
do {\
    local0 = pop();\
    local1 = pop();\
    push(local0 op local1);\
} while (false)

    uint8_t instruction;
    Value constant;

    double local0;
    double local1;

    while (true) {
#ifdef debug_trace_execution
        printf(" ");
        
        for (Value *slot = vm->stack; slot < vm->stack_top; ++slot) {
            printf("[ ");
            print_value(*slot);
            printf(" ]");
        }

        printf("\n");
        disassemble_instruction(vm->chunk, (int)(vm->ip - vm->chunk->code));
#endif
        switch(instruction = read_byte) {
            case op_constant:
                push(read_constant);
                break;
            case op_add:
                binary_op(+);
                break;
            case op_sub:
                binary_op(-);
                break;
            case op_mul:
                binary_op(*);
                break;
            case op_div:
                binary_op(/);
                break;
            case op_mod:
                binary_op(%);
            case op_negate:
                push(-pop());
                break;
            case op_return:
                print_value(pop());
                printf("\n");
                return int_ok;
        }
    }

#undef read_byte
#undef read_constant
}

Result interpret(Vm *vm, const char *source) {
    Chunk chunk = new_chunk(chunk);

    if (!compile(source, &chunk)) {
        free_chunk(&chunk);
        return int_compile_error;
    }

    vm->chunk = &chunk;
    cm->ip = vm->chunk->code;

    Result result = run(vm);

    free_chunk(&chunk);

    return result;
}

void push(Vm *vm, Value value) {
    *vm->stack_top = value;
    ++vm->stack_top;
}

Value pop(Vm *vm) {
    --vm->stack_top;
    return *vm->stack_top;
}