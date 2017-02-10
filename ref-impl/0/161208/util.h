#pragma once

#include <stdint.h>

#define UNUSED_ARG(var) ((void) var)

/**
 * 空白までをトークンとし、一つのトークンを返す。
 *
 * \code
 * char s[] = "hoge fuga";
 * char* p = s;
 * char* x = tokenize(p, &p); // p -> "fuga"
 * char* y = tokenize(p, &p); // p -> ""
 * char* z = tokenize(p, &p);
 * // x == "hoge" y == "fuga" z == NULL
 * // s == "hoge\0fuga"
 * \endcode
 *
 * @param in 文字列。破壊されるので書き込み可能でなければならない。
 * @param next この次のトークンを得るために使用する文字列。
 * @return NULLでなければ、トークン文字列。
 */
char* tokenize(char* in, char** next);

void remove_newline(char*);

static inline uint8_t get_r(uint32_t);
static inline uint8_t get_g(uint32_t);
static inline uint8_t get_b(uint32_t);

static inline int max(int, int);
static inline int max3(int, int, int);
static inline int min(int, int);
static inline int min3(int, int, int);


static inline uint8_t get_r(uint32_t c) {
	return (uint8_t) ((c & 0xff0000) >> 16);
}

static inline uint8_t get_g(uint32_t c) {
	return (uint8_t) ((c & 0x00ff00) >> 8);
}

static inline uint8_t get_b(uint32_t c) {
	return (uint8_t) ((c & 0x0000ff));
}

static inline int max(const int a, const int b) {
	return (a > b) ? a : b;
}

static inline int max3(const int a, const int b, const int c) {
	return max(a, max(b, c));
}

static inline int min(const int a, const int b) {
	return (a < b) ? a : b;
}

static inline int min3(const int a, const int b, const int c) {
	return min(a, min(b, c));
}
