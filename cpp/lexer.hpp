#ifndef jppt
    #define jppt
    #define pi(I) printf("%d", I)
    #define px(I) printf("%02X", I)
    #define pf printf

    #ifdef __cplusplus
    #include <cstdint>
    #else
    #include <stdint.h>
    #endif
    #define i8 char
    #define i16 int16_t
    #define i32 int
    #define i64 long long
    #define i128 __int128_t
    #define u0 void
    #define u8 unsigned char
    #define u16 uint16_t
    #define u32 unsigned int
    #define u64 unsigned long long
    #define u128 __uint128_t
    #define I8 i8
    #define I16 i16
    #define I32 i32
    #define I64 i64
    #define I128 i128
    #define U0 u0
    #define U8 u8
    #define U16 u16
    #define U32 u32
    #define U64 u64
    #define U128 u128
    #define nil NULL
    #define ret return
    #define str i8*
    #define alloc(T, S) (T*)malloc(sizeof(T)*S)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma once

#ifndef LITERAL_CHAR
#define LITERAL_CHAR '\''
#endif

#ifndef LITERAL_ESC_CHAR
#define LITERAL_ESC_CHAR '\\'
#endif

enum class TokenType {
    LITERAL = 0,
    NUMERIC = 1,
    WORD = 2,
    START = 3,
    SYMBOL = 4,
};
struct Token {
    str value;
    u32 line;
    TokenType type;
    Token *next;
};
Token *token_new(str value, TokenType type, u32 line);
Token *token_append(Token *token, str value, TokenType type, u32 line);
u0 token_free(Token *token);
u0 token_freeall(Token *token);

str minmv(str val);
str allocs(i32 c);
Token *lexer(str code);