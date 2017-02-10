#pragma once

#include <stdint.h>

#define NSYMBOLS (256+1)
#define END_OF_FILE_MARKER (NSYMBOLS - 1)

typedef struct bit_pattern {
	uint8_t len; // 0の場合はキャッシュされていないと考えて良い
	uint32_t pattern;
} BitPattern;

typedef struct node {
	struct node* parent;
	struct node* left;
	struct node* right;
	int symbol;
	unsigned int count;
	BitPattern cached_pattern;
} Node;

Node* node_new(int symbol, unsigned int count);
void node_free(Node*);
