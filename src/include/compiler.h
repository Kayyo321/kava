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

ParseRule rules[] = {
  [TT_LParen]    = {grouping, NULL,   PREC_NONE},
  [TT_LParen]   = {NULL,     NULL,   PREC_NONE},
  [TT_LBrace]    = {NULL,     NULL,   PREC_NONE},
  [TT_RBrace]   = {NULL,     NULL,   PREC_NONE},
  [TT_Comma]         = {NULL,     NULL,   PREC_NONE},
  [TT_Dot]           = {NULL,     NULL,   PREC_NONE},
  [TT_Minus]         = {unary,    binary, PREC_TERM},
  [TT_Plus]          = {NULL,     binary, PREC_TERM},
  [TT_Semicolon]     = {NULL,     NULL,   PREC_NONE},
  [TT_Slash]         = {NULL,     binary, PREC_FACTOR},
  [TT_Star]          = {NULL, binary, PREC_FACTOR},
  [TT_Bang]          = {NULL,     NULL,   PREC_NONE},
  [TT_BangEqu]    = {NULL,     NULL,   PREC_NONE},
  [TT_Equ]         = {NULL,     NULL,   PREC_NONE},
  [TT_EquEqu]   = {NULL,     NULL,   PREC_NONE},
  [TT_Greater]       = {NULL,     NULL,   PREC_NONE},
  [TT_GreaterEqu] = {NULL,     NULL,   PREC_NONE},
  [TT_Less]          = {NULL,     NULL,   PREC_NONE},
  [TT_LessEqu]    = {NULL,     NULL,   PREC_NONE},
  [TT_Ident]    = {NULL,     NULL,   PREC_NONE},
  [TT_String]        = {NULL,     NULL,   PREC_NONE},
  [TT_Number]        = {number,   NULL,   PREC_NONE},
  [TT_And]           = {NULL,     NULL,   PREC_NONE},
  [TT_Struct]         = {NULL,     NULL,   PREC_NONE},
  [TT_Else]          = {NULL, NULL,   PREC_NONE},
  [TT_False]         = {NULL,     NULL,   PREC_NONE},
  [TT_For]           = {NULL,     NULL,   PREC_NONE},
  [TT_Func]           = {NULL,     NULL,   PREC_NONE},
  [TT_If]            = {NULL,     NULL,   PREC_NONE},
  [TT_Nothing]           = {NULL,     NULL,   PREC_NONE},
  [TT_Or]            = {NULL,     NULL,   PREC_NONE},
  [TT_Print]         = {NULL,     NULL,   PREC_NONE},
  [TT_Return]        = {NULL,     NULL,   PREC_NONE},
  [TT_Super]         = {NULL,     NULL,   PREC_NONE},
  [TT_Self]          = {NULL,     NULL,   PREC_NONE},
  [TT_True]          = {NULL,     NULL,   PREC_NONE},
  [TT_Let]           = {NULL,     NULL,   PREC_NONE},
  [TT_While]         = {NULL,     NULL,   PREC_NONE},
  [TT_Err]         = {NULL, NULL,   PREC_NONE},
  [TT_EOF]           = {NULL,     NULL,   PREC_NONE},
};

bool compile(const char *, Chunk *);

static void expression(Parser *);
static void parse_precedence(Parser *, Precedence);
static ParseRule *get_rule(TokenType);
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