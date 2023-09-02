#include "inlcude/common.h"
#include "include/chunk.h"
#include "include/debug.h"
#include "include/vm.h"

int main(int argc, const char **argv) {
    Vm virtual_machine = new_vm(virtual_machine);
    
    switch (argc) {
    case 1:
        run_repl(&virtual_machine);
        break;
    case 2:
        run_file(&virtual_machine, argc[1]);
        break;
    default:
        fprintf(stderr, "Usage: kava <?path>\n");
        exit(64);
    }

    free_vm(&virtual_machine);

    return EXIT_SUCCESS;
}