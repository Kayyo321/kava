#ifndef KAVA_SCANNER_H
#define KAVA_SCANNER_H

typedef struct {
    const char *start;
    const char *current;
    size_t line;
} Scanner;

typedef enum {
    TT_LParen,
    TT_RParen,
    TT_LBrace,
    TT_RBrace,
    TT_Comma,
    TT_Dot,
    TT_Minus,
    TT_Plus,
    TT_Semicolon,
    TT_Slash,
    TT_Star,
    TT_Modulo,

    TT_Bang,
    TT_BangEqu,
    TT_Equ,
    TT_EquEqu,
    TT_Greater,
    TT_GreaterEqu,
    TT_Less,
    TT_LessEqu,

    TT_Ident,
    TT_String,
    TT_Number,

    TT_And,
    TT_Struct,
    TT_Else,
    TT_False,
    TT_For,
    TT_Func,
    TT_If,
    TT_Nothing,
    TT_Or,
    TT_Write,
    TT_Return,
    TT_Super,
    TT_Self,
    TT_True,
    TT_Let,
    TT_While,
    TT_Derives,
    TT_Err,
    TT_EOF
} TokenType;

typedef struct {
    TokenType type;
    const char *start;
    size_t length;
    size_t line;
} Token;

void init_scanner(Scanner *, const char *);
Token scan_token(Scanner *);
static Token string(const char, Scanner *);
static Token number(Scanner *);
static Token ident(Scanner *);
static TokenType ident_type(Scanner *);
static TokenType check_keyword(Scanner *, int, int, const char *, TokenType);
static void skip_whitespace(Scanner *);
static bool is_at_end(Scanner *);
static Token make_token(Scanner *, TokenType);
static Token err_token(Scanner *, const char *);
static char advance(Scanner *);
static char peek(Scanner *);
static char peek_next(Scanner *);
static bool match(Scanner *, char);

#endif