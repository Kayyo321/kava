#ifndef KAVA_COMPILER_H
#define KAVA_COMPILER_H

#include "scanner.h"
#include "common.h"
#include "chunk.h"
#include "value.h"

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
  [TT_LParen]    = {grouping, nil,   prec_none},
  [TT_LParen]   = {nil,     nil,   prec_none},
  [TT_LBrace]    = {nil,     nil,   prec_none},
  [TT_RBrace]   = {nil,     nil,   prec_none},
  [TT_Comma]         = {nil,     nil,   prec_none},
  [TT_Dot]           = {nil,     nil,   prec_none},
  [TT_Minus]         = {unary,    binary, prec_term},
  [TT_Plus]          = {nil,     binary, prec_term},
  [TT_Semicolon]     = {nil,     nil,   prec_none},
  [TT_Slash]         = {nil,     binary, prec_fact},
  [TT_Star]          = {nil, binary, prec_fact},
  [TT_Bang]          = {unary,     nil,   prec_none},
  [TT_BangEqu]    = {nil,     binary,   prec_equ},
  [TT_Equ]         = {nil,     nil,   prec_none},
  [TT_EquEqu]   = {nil,     binary,   prec_equ},
  [TT_Greater]       = {nil,     binary,   prec_equ},
  [TT_GreaterEqu] = {nil,     binary,   prec_equ},
  [TT_Less]          = {nil,     binary,   prec_equ},
  [TT_LessEqu]    = {nil,     binary,   prec_equ},
  [TT_Ident]    = {nil,     nil,   prec_none},
  [TT_String]        = {nil,     nil,   prec_none},
  [TT_Number]        = {number,   nil,   prec_none},
  [TT_And]           = {nil,     nil,   prec_none},
  [TT_Struct]         = {nil,     nil,   prec_none},
  [TT_Else]          = {nil, nil,   prec_none},
  [TT_False]         = {literal,     nil,   prec_none},
  [TT_For]           = {nil,     nil,   prec_none},
  [TT_Func]           = {nil,     nil,   prec_none},
  [TT_If]            = {nil,     nil,   prec_none},
  [TT_Nothing]           = {literal,     nil,   prec_none},
  [TT_Or]            = {nil,     nil,   prec_none},
  [TT_Print]         = {nil,     nil,   prec_none},
  [TT_Return]        = {nil,     nil,   prec_none},
  [TT_Super]         = {nil,     nil,   prec_none},
  [TT_Self]          = {nil,     nil,   prec_none},
  [TT_True]          = {literal,     nil,   prec_none},
  [TT_Let]           = {nil,     nil,   prec_none},
  [TT_While]         = {nil,     nil,   prec_none},
  [TT_Err]         = {nil, nil,   prec_none},
  [TT_EOF]           = {nil,     nil,   prec_none},
};

bool compile(const char *, Chunk *);

static void expression(Parser *);
static void parse_precedence(Parser *, Precedence);
static ParseRule *get_rule(TokenType);
static void binary(Parser *);
static void grouping(Parser *);
static void unary(Parser *);
static void number(Parser *);
static void literal(Parser *);
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