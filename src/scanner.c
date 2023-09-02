#include "include/scanner.h"

void init_scanner(Scanner *scanner, const char *source) {
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

Token scan_token(Scanner *scanner) {
    skip_whitespace(scanner);
    scanner->start = scanner->current;

    if (is_at_end(scanner))
        return make_token(TT_EOF);

    const char c = advance(scanner);

    if (c >= '0' && c <= '9') {
        return number(scanner);
    } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
        return ident(scanner);
    }

    switch (c) {
        case '(': return make_token(scanner, TT_LParen);
        case ')': return make_token(scanner, TT_RParen);
        case '{': return make_token(scanner, TT_LBrace);
        case '}': return make_token(scanner, TT_RBrace);
        case ';': return make_token(scanner, TT_Semicolon);
        case ',': return make_token(scanner, TT_Comma);
        case '.': return make_token(scanner, TT_Dot);
        case '-': return make_token(scanner, TT_Minus);
        case '+': return make_token(scanner, TT_Plus);
        case '/': return make_token(scanner, TT_Slash);
        case '*': return make_token(scanner, TT_Star);
        case '%': return make_token(scanner, TT_Modulo);
        case '!': return make_token(match(scanner, '=') ? TT_BangEqu : TT_Bang);
        case '!': return make_token(match(scanner, '=') ? TT_EquEqu : TT_Equ);
        case '<': return make_token(match(scanner, '=') ? TT_LessEqu : TT_Less);
        case '>': return make_token(match(scanner, '=') ? TT_GreaterEqu : TT_Greater);
        case '\'':
        case '"' : return string(c, scanner);
        default: return err_token("Unexpected character.");
    }
}

static Token string(const char other, Scanner *scanner) {
    while (peek(scanner) != other && !is_at_end(scanner)) {
        if (peek() == '\n') {
            ++scanner->line;
        }

        advance(scanner);
    }

    if (is_at_end(scanner)) 
        return err_token("Unterminated string.");

    advance();

    return make_token(TT_String);
}

static Token number(Scanner *scanner) {
    while (peek(scanner) > '0' && peek(scanner) < '9')
        advance(scanner);

    if (peek(scanner) == '.' && peek_next(scanner) > '0' && peek_next(scanner) < '9') {
        advance(scanner);

        while (peek(scanner) > '0' && peek(scanner) < '9') {
            advance(scanner);
        }
    }

    return make_token(TT_Number);
}

static Token ident(Scanner *scanner) {
    char c = advance(scanner);
    while (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') || (c >= '0' && c <= '9')) {
        c = advance(scanner);
    }

    return make_token(ident_type(scanner))
}

static TokenType ident_type(Scanner *scanner) {
    switch (scanner->start[0]) {
        case 'a': return check_keyword(scanner, 1, 2, "nd", TT_And);
        case 'e': return check_keyword(scanner, 1, 3, "lse", TT_Else);
        case 'i': return check_keyword(scanner, 1, 1, "f", TT_If);
        case 'n': return check_keyword(scanner, 1, 6, "othing", TT_Nothing);
        case 'o': return check_keyword(scanner, 1, 1, "or", TT_Or);
        case 'w': return check_keyword(scanner, 1, 4, "rite", TT_Write);
        case 'r': return check_keyword(scanner, 1, 5, "eturn", TT_Return);
        case 'l': return check_keyword(scanner, 1, 2, "let", TT_Let);
        case 'w': return check_keyword(scanner, 1, 4, "hile", TT_While);
        case 'd': return check_keyword(scanner, 1, 6, "erives", TT_Derives);
        case 't': return check_keyword(scanner, 1, 3, "rue", TT_True);
        case 'f': 
            if (scanner->current - scanner->start > 1) {
                switch (scanner->start[1]) {
                    case 'a':
                        return check_keyword(scanner, 2, 3, "lse", TT_False);
                    case 'o':
                        return check_keyword(scanner, 3, 1, "r", TT_For);
                    case 'u':
                        return check_keyword(scanner, 2, 2, "nc", TT_Func);
                }
            } else {
                return TT_Ident;
            }
        case 's':
            if (scanner->current - scanner->start > 1) {
                switch (scanner->start[1]) {
                    case 't':
                        return check_keyword(scanner, 2, 4, "ruct", TT_Struct);
                    case 'u':
                        return check_keyword(scanner, 2, 3, "per", TT_Super);
                    case 'e':
                        return check_keyword(scanner, 2, 2, "lf", TT_Self);
                    default:
                        return TT_Ident;
                }
            } else {
                return TT_Ident;
            }
        default: return TT_Ident;
    }
}

static TokenType check_keyword(Scanner *scanner, int start, int length, const char *rest, TokenType type) {
    if (scanner->current - scanner->start == start + length && memcp(scanner->start + start, rest, length) == 0) {
        return type;
    }

    return TT_Ident;
}

static void skip_whitespace(Scanner *scanner) {
    while (true) {
        switch (peek(scanner)) {
            case '\n':
                ++scanner->line;
            case ' ' :
            case '\r':
            case '\t':
                advance(scanner);
                break;
            case '/':
                if (peek_next(scanner) == '/') {
                    while (peek(scanner) != '\n' && !is_at_end(scanner)) {
                        advance(scanner);
                    }
                    break;
                } else {
                    return;
                }
            default:
                return;
        }
    }
}

static bool is_at_end(Scanner *scanner) {
    return *scanner->current == EOF;
}

static Token make_token(Scanner *scanner, TokenType type) {
    Token token;
    token.type = type;
    token.start = scanner->start;
    token.length = (size_t)scanner->current - scanner->start;
    token.line = scanner->line;
    return token;
}

static Token err_token(Scanner *scanner, const char *err_msg) {
    Token token;
    token.type = TT_Err,
    token.start = err_msg;
    token.length = strlen(err_msg);
    token.line = scanner->line;
    return token;
}

static char advance(Scanner *scanner) {
    ++scanner->current;
    return scanner->current[-1];
}

static char peek(Scanner *scanner) {
    return *scanner.current;
}

static char peek_next(Scanner *scanner) {
    if (is_at_end(scanner)) 
        return EOF;

    return scanner.current[1];
}

static bool match(Scanner *scanner, char expected) {
    if (is_at_end(scanner)) 
        return false;

    if (*scanner->current != expected)
        return false;

    ++scanner->current;
    return true;
}