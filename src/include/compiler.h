#ifndef KAVA_COMPILER_H
#define KAVA_COMPILER_H

#include "scanner.h"
#include "common.h"
#include "chunk.h"

typedef struct {
    Chunk *emitting_chunk;
    Scanner *scanner;
    Token current, previous;
    bool had_error, panic_mode;
} Parser;

typedef enum {
    prec_none,
    prec_assignment,
    prec_or,
    prec_and,
    prec_equ,
    prec_comp,
    prec_term,
    prec_fact,
    prec_unary,
    prec_call,
    prec_primary
} Precedence;

typedef void (*ParseFn)();

typedef struct {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
} ParseRule;

bool compile(const char *, Chunk *);

static void expression(Parser *);
static void parse_precedence(Parser *, Precedence);
static void binary(Parser *);
static void grouping(Parser *);
static void unary(Parser *);
static void number(Parser *);
static void emit_byte(Parser *, uint8_t);
static void emit_bytes(Parser *, uint8_t, uint8_t);
static void emit_return(Parser *);
static void emit_constant(Parser *, Value);
static uint8_t make_constant(Parser *, Value);
static void parser_advance(Parser *);
static void consume(Parser *, TokenType, const char *);
static void err_at_current(Parser *, const char *);
static void err_at(Parser *,Token *, const char *);
static void err(Parser *, const char *);

#endif