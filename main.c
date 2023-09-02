#include "inlcude/common.h"
#include "include/chunk.h"
#include "include/debug.h"

int main() {
    Chunk chunk = new_chunk(chunk);
    size_t constant = add_constant(&chunk, 1.2);

    write_chunk(&chunk, op_constant);
    write_chunk(&chunk, op_return);

    disassemble_chunk(&chunk, "test chunk");

    free_chunk(&chunk);

    return EXIT_SUCCESS;
}