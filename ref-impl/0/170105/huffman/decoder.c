#include "decoder.h"
#include <assert.h>
#include <stdlib.h>
#include "bitlist.h"
#include "logger.h"
#include "node.h"

struct decoder_state {
	Node* root;
	BitList* buffer;
	FILE* out_fp;
	bool encounter_eof;
};

typedef struct decoder_state DecoderState;

/**
 * データから二分木を作成する。データ不足の場合はstateは変更されない
 */
static void build_tree(DecoderState*);
/**
 * ビット列から二分木を作成する。
 * @return データ不足の場合はNULL
 */
static Node* build_subtree(BitList*);
/**
 * データからデコードする
 */
static void decode(DecoderState*);
/**
 * ビット列から元のバイトを作成する
 * @return 元のバイト or END_OF_FILE_MARKER or -1 (データ不足)
 */
static int decode_byte(DecoderState*);

#ifdef _DEBUG
static void debug_show_bit_patterns(Node* root);
static unsigned char get_readable_char(int symbol);
static void debug_show_bit_patterns0(Node* node, char* buf, char* endptr);
#endif

DecoderState* decoder_new(FILE* fp) {
	DecoderState* ret = (DecoderState*) malloc(sizeof(DecoderState));
	ret->root = NULL;
	ret->buffer = bitlist_new();
	ret->out_fp = fp;
	ret->encounter_eof = false;
	return ret;
}

bool decoder_update(DecoderState* state, unsigned char* input, size_t len) {
	if (state->encounter_eof) {
		return true;
	}
	bitlist_put_bytes(state->buffer, input, len);
	if (!state->root) {
		build_tree(state);
	}
	if (state->root) {
		decode(state);
	}
	return state->encounter_eof;
}

void decoder_close(DecoderState* state) {
	node_free(state->root);
	bitlist_free(state->buffer);
	free(state);
}

static void build_tree(DecoderState* state) {
	size_t saved_ptr = bitlist_tell_read_ptr(state->buffer);
	Node* tree = build_subtree(state->buffer);
	if (tree) {
		state->root = tree;
#ifdef _DEBUG
		debug_show_bit_patterns(tree);
#endif
	} else {
		bitlist_seek_read_ptr(state->buffer, saved_ptr);
	}
}

static Node* build_subtree(BitList* list) {
	uint32_t bits;
	if (!bitlist_pop_bits(list, &bits, 1)) {
		return NULL;
	}

	// 二分木を復元する
	if (bits) {
		// 1<subtree><subtree>
		Node* left = build_subtree(list);
		if (!left) {
			return NULL;
		}
		Node* right = build_subtree(list);
		if (!right) {
			node_free(left);
			return NULL;
		}
		Node* ret = node_new(-1, 0);
		ret->left = left;
		ret->right = right;
		return ret;
	} else {
		// 0<bit pattern>
		if (!bitlist_pop_bits(list, &bits, 8)) {
			return NULL;
		}
		if (bits == 0u) {
			// 0000 0000 0 == '\0'
			// 0000 0000 1 == EOF
			if (!bitlist_pop_bits(list, &bits, 1)) {
				return NULL;
			}
			bits = bits ? END_OF_FILE_MARKER : '\0';
		}
		// それ以外のbit patternはsymbolそのまま
		Node* ret = node_new((int) bits, 0);
		ret->left = NULL;
		ret->right = NULL;
		return ret;
	}
}

static void decode(DecoderState* state) {
	while (true) {
		int decoded = decode_byte(state);
		if (decoded == END_OF_FILE_MARKER) {
			state->encounter_eof = true;
			return;
		} else if (decoded < 0) {
			// lacking data
			return;
		}
		assert(0 <= decoded && decoded < END_OF_FILE_MARKER);
		fputc(decoded, state->out_fp);
	}
}

static int decode_byte(DecoderState* state) {
	size_t saved_ptr = bitlist_tell_read_ptr(state->buffer);
	// 二分木で作成しているので空間局所性が低い
	// 深さ8くらいまでヒープ構造で管理しても良かったのかな？
	Node* node = state->root;
	while (node->left != NULL) {
		uint32_t bit;
		if (!bitlist_pop_bits(state->buffer, &bit, 1)) {
			bitlist_seek_read_ptr(state->buffer, saved_ptr);
			return -1;
		}
		node = bit ? node->right : node->left;
	}
	return node->symbol;
}

#ifdef _DEBUG

static void debug_show_bit_patterns(Node* root) {
	char buf[NSYMBOLS];
	debug_show_bit_patterns0(root, buf, buf);
}

static void debug_show_bit_patterns0(Node* node, char* buf, char* lastptr) {
	if (node->left == NULL) {
		*lastptr = '\0';
		if (node->symbol == (NSYMBOLS - 1)) {
			LOG(debug, "EOF  : %s", buf);
		} else {
			LOG(debug, "%02x(%c): %s", node->symbol, get_readable_char(node->symbol), buf);
		}
	} else {
		*lastptr = '0';
		debug_show_bit_patterns0(node->left, buf, lastptr + 1);
		*lastptr = '1';
		debug_show_bit_patterns0(node->right, buf, lastptr + 1);
	}
}

static unsigned char get_readable_char(int symbol) {
	return (unsigned char) ((0x20 <= symbol && symbol <= 0x7e) ? symbol : '.');
}

#endif
