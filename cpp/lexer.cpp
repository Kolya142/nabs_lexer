#include "lexer.hpp"
#
Token *token_new(str value, TokenType type, u32 line) {
    Token *token = alloc(Token, 1);
    token->value = value;
    token->type = type;
    token->next = NULL;
    token->line = line;
    ret token;
}
Token* token_append(Token *token, str value, TokenType type, u32 line) {
    token->next = token_new(value, type, line);
    ret token->next;
}
u0 token_free(Token *token) {
    free(token->value);
    free(token);
}
u0 token_freeall(Token *token) {
    Token *next = NULL;
    for (;;) {
        next = token->next;
        token_free(token);
        token = next;
        if( token == NULL ){
            break;
        }
    }
}
str minmv(str val) {
    u32 l = strlen(val);
    str o = alloc(i8, l);
    memcpy(o, val, l);
    free(val);
    ret o;
}
str allocs(i32 c) {
    str s = alloc(i8, c);
    memset(s, 0, c);
    ret s;
}
Token *lexer(str code) {
    U32 code_size = strlen(code);
    U8 context = 0 ;
    Token* first = token_new(NULL, TokenType::START, 0);
    Token* last = first;
    U32 line = 0;
    for (U32 i = 0; i < code_size; i++) {
        i8 c = code[i];
        if( c == '\n' ){
            line++;
            continue;
        }
        if( context != 3 ){
            switch( c ){
                case '0' ... '9':
                    if( context == 0 ){
                        context = 1;
                        last = token_append(last, allocs(30), TokenType::NUMERIC, line);
                        last->value[0] = c;
                        continue;
                    }
                    if( context == 1 || context == 2 ){
                        last->value[strlen(last->value)] = c;
                        continue;
                    }
                    break;
                case ' ':
                    if( context != 3 ){
                        context = 0;
                        last->value = minmv(last->value);
                    }
                    break;
                case '=':
                case '+':
                case '-':
                case '*':
                case '/':
                case '(':
                case ')':
                case '{':
                case '}':
                case ';':
                case ',':
                    if( context != 0 ){
                        context = 0;
                        last->value = minmv(last->value);
                    }
                    last = token_append(last, allocs(2), TokenType::SYMBOL, line);
                    last->value[0] = c;
                    break;
                case 'A' ... 'Z': case 'a' ... 'z': case '_':
                    if( context == 1 ){
                        context = 0;
                        last->value = minmv(last->value);
                    }
                    if( context == 0 ){
                        context = 2;
                        last = token_append(last, allocs(50), TokenType::WORD, line);
                        last->value[0] = c;
                        continue;
                    }
                    last->value[strlen(last->value)] = c;
                    break;
                case LITERAL_CHAR:
                    if( i == 0 || code[i-1] != LITERAL_ESC_CHAR ){
                        if( context == 0 ){
                            context = 3;
                            last = token_append(last, allocs(100), TokenType::LITERAL, line);
                        }
                        else if( context == 3 ){
                            context = 0;
                            last->value = minmv(last->value);
                        }
                    }
                    else {
                        last->value[strlen(last->value)] = c;
                    }
                    break;
                default:
                    if( context == 1 ){
                        context = 0;
                        last->value = minmv(last->value);
                        continue;
                    }
                    last->value[strlen(last->value)] = c;
            }
        }
        else {
            switch( c ){
                case LITERAL_CHAR:
                    if( i == 0 || code[i-1] != LITERAL_ESC_CHAR ){
                        if( context == 0 ){
                            context = 3;
                            last = token_append(last, allocs(100), TokenType::LITERAL, line);
                        }
                        if( context == 3 ){
                            context = 0;
                            last->value = minmv(last->value);
                        }
                    }
                    else {
                        last->value[strlen(last->value)] = LITERAL_CHAR;
                    }
                    break;
                case LITERAL_ESC_CHAR:
                    if( i == code_size - 1 || code[i+1] != LITERAL_CHAR ){
                        last->value[strlen(last->value)] = c;
                    }
                    break;
                default:
                    last->value[strlen(last->value)] = c;
            }
        }
    }
    return first->next;
}
