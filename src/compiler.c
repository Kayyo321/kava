#include "include/compiler.h"

bool compile(const char *source, Chunk *chunk) {
    Scanner scanner;    
    init_scanner(&scanner, source);

    Parser parser;
    parser.scanner = &scanner;
    parser.emitting_chunk = chunk;

    parse(&parser);
    // expression();

    // consume(parser, TT_EOF, "Expect end of expression.");

    return !parser->had_error;
}

static void expression(Parser *parser) {
    parse_precedence(parser, prec_assignment);
}

static void parse_precedence(Parser *parser, Precedence precedence) {
    parser_advance(parser);
    ParseFn prefix_rule = get_rule(parser->previous.type)->prefix;
    if (prefix_rule == nil) {
        err("Expect expression.");
        return;
    }
    prefix_rule();
    ParseFn infixRule;
    while (precedence <= get_rule(parser->current.type)->precedence) {
        parser_advance(parser);
        infix_rule = get_rule(parser->previous.type);
        infix_rule();
    }
}

static ParseRule *get_rule(TokenType type) {
    return &rules[type];
}

static void binary(Parser *parser) {
    TokenType operator_type = parser->previous.type;

    ParseRule *rule = get_rule(operator_type);

    parse_precedence((Precedence)(rule->precedence+1));

    switch (operator_type) {
        case TT_BangEqu:
            emit_bytes(parser, op_equal, op_not);
            break;
        case TT_EquEqu:
            emit_byte(parser, op_equal);
            break;
        case TT_Greater:
            emit_byte(parser, op_greater);
            break;
        case TT_GreaterEqu:
            emit_bytes(parser, op_less, op_not);
            break;
        case TT_Less:
            emit_byte(parser, op_less);
            break;
        case TT_LessEqu:
            emit_bytes(parser, op_greater, op_not);
            break;
        case TT_Plus:
            emit_byte(parser, op_add);
            break;
        case TT_Minus:
            emit_byte(parser, op_sub);
            break;
        case TT_Star:
            emit_byte(parser, op_mul);
            break;
        case TT_Slash:
            emit_byte(parser, op_div);
            break;
        case TT_Modulo:
            emit_byte(parser, op_mod);
            break;
    }
}

static void grouping(Parser *parser) {
    expression(parser);

    consume(parser, TT_RParen, "Expect `)` after expression.");
}

static void unary(Parser *parser) {
    TokenType operator_type = parser->previous.type;

    expression(parser);

    parse_precedence(parser, prec_unary);

    switch (operator_type) {
        case TT_Minus:
            emit_byte(parser, op_negate);
            break;
        case TT_Bang:
            emit_byte(parser, op_not);
        default:
            return;
    }
}

static void number(Parser *parser) {
    double value = strtod(parser->previous.start, nil);
    emit_constant(as_number(value));
}

static void literal(Parser *parser) {
    switch (parser->previous.type) {
        case TT_False:
            emit_byte(op_false);
            break;
        case TT_True:
            emit_byte(op_true);
            break;
        case TT_Nothing:
            emit_byte(op_nothing);
            break;
        default:
            return;
    }
}

static void emit_byte(Parser *parser, uint8_t byte) {
    write_chunk(parser->emitting_chunk, byte, parser->previous.line);
}

static void emit_bytes(Parser *parser, uint8_t byte0, uint8_t byte1) {
    emit_byte(parser, byte0);
    emit_byte(parser, byte1);
}

static void emit_return(Parser *parser) {
    emit_byte(parser, op_return);
}

static void emit_constant(Parser *parser, Value value) {
    emit_bytes(op_constant, make_constant(value));
}

static uint8_t make_constant(Parser *parser, Value value) {
    size_t constant = add_constant(parser->emitting_chunk, value);
    if (constant > UINT8_MAX) {
        err("Too many constants in one chunk.");
        return 0;
    }

    return (uint8_t)constant;
}

static void parser_advance(Parser *parser) {
    parser->previous = parser->current;

    while (true) {
        parser->current = scan_token(parser->scanner);
        if (parser->current.type != TT_Err)
            break;

        err_at_current(parser, parser->current->start);
    }
}

static void consume(Parser *parser, TokenType type, const char *err_msg) {
    if (parser->current.type == type) {
        parser_advance(parser);
        return;
    }

    err_at_current(err_msg);
}

static void err_at_current(Parser *parser, const char *err_msg) {
    err_at(parser, &parser->current, err_msg);
}

static void err(Parser *parser, const char *err_msg) {
    err_at(parser, &parser->previous, err_msg);
}

static void err_at(Parser *parser, Token *token, const char *err_msg) {
    if (parser->panic_mode)
        return;

    parser->panic_mode = true;

    fprintf(stderr, "[line %d] Error", token->line);

    if (token->type == TT_EOF) {
        fprintf(stderr, " at end") ;
    } else if (token->type != TT_Err) {
        fprintf(stderr, " at `%.*s`", token->length, token->start);
    }

    fprintf(stderr, ": %s\n", err_msg);
    parser->had_error = true;
}