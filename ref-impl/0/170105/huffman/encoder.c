#include "encoder.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "bitlist.h"
#include "heap.h"
#include "logger.h"

struct encoder_state {
	Node* root;
	BitList* buffer;
	FILE* out_fp;
	Node* symbol_cache[NSYMBOLS];
	unsigned int symbol_count[NSYMBOLS];
};
typedef struct encoder_state EncoderState;

/**
 * BitListからファイルに書き込む
 */
static void sync_buffer(EncoderState*);
/**
 * symbol_count を元にハフマン二分木を作成する
 * @return 作成した二分木の根
 */
static Node* build_tree(EncoderState*);
/**
 * 入力データから symbol_count を更新する
 * @param input 入力データ
 * @param len データ長
 */
static void count_symbols(EncoderState*, unsigned char* input, size_t len);
/**
 * 指定したノードを表すハフマン符号をBitListに追加する
 */
static void append_encoded_bits(EncoderState*, Node*);
/**
 * ビットパターンキャッシュを作成する
 * @param root 根
 * @param pattern ビットパターン
 * @param len ビット長
 */
static void cache_bit_pattern(Node* root, uint32_t pattern, uint32_t len);
/**
 * 木をBitListに出力する
 * @param node 根
 */
static void write_tree(EncoderState*, Node* node);

#ifdef _DEBUG
static void debug_show_bit_patterns(Node* root);
static unsigned char get_readable_char(int symbol);
static void debug_show_bit_patterns0(Node* node, char* buf, char* endptr);
#endif

EncoderState* encoder_new(FILE* fp) {
	EncoderState* ret = (EncoderState*) malloc(sizeof(EncoderState));
	ret->root = NULL;
	ret->out_fp = fp;
	ret->buffer = bitlist_new();
	memset(ret->symbol_count, 0, sizeof(ret->symbol_count));
	memset(ret->symbol_cache, 0, sizeof(ret->symbol_cache));
	return ret;
}

void encoder_update_pass1(EncoderState* state, unsigned char* input, size_t len) {
	assert(state->root == NULL);
	count_symbols(state, input, len);
}

void encoder_update_pass2(EncoderState* state, unsigned char* input, size_t len) {
	if (state->root) {
		sync_buffer(state);
	} else {
		state->root = build_tree(state);
		write_tree(state, state->root);
		LOG(debug, "tree size: %zu bits", bitlist_tell_write_ptr(state->buffer));
	}
	unsigned char* p = input;
	for (size_t i = 0; i < len; ++i) {
#if 0
		LOG(debug, "put char: %x(%c)", *p, get_readable_char(*p));
#endif
		append_encoded_bits(state, state->symbol_cache[*p++]);
	}
}

void sync_buffer(EncoderState* state) {
	unsigned char buf[65536];
	while (true) {
		size_t len = bitlist_pop_bytes(state->buffer, buf, sizeof(buf));
		if (len == 0) {
			break;
		}
		fwrite(buf, 1, len, state->out_fp);
	}
}


void encoder_close(EncoderState* state) {
	// EOF出力、ファイルへ書き込み
	append_encoded_bits(state, state->symbol_cache[END_OF_FILE_MARKER]);
	bitlist_pad(state->buffer);
	sync_buffer(state);

	bitlist_free(state->buffer);
	node_free(state->root);
	free(state);
}

static void count_symbols(EncoderState* state, unsigned char* input, size_t len) {
	unsigned char* p = input;
	for (size_t i = 0; i < len; ++i) {
		state->symbol_count[*p++]++;
	}

	state->symbol_count[END_OF_FILE_MARKER] = 1; // End of File
}

static Node* build_tree(EncoderState* state) {
	size_t n = 0;
	Node* nodep[NSYMBOLS];

	for (int i = 0; i < NSYMBOLS; i++) {
		if (state->symbol_count[i] == 0) {
			continue;
		}
		nodep[n] = node_new(i, state->symbol_count[i]);
		state->symbol_cache[i] = nodep[n];
		++n;
	}

	Heap heap;
	heap_prepare(&heap, nodep, n);

	while (heap_get_len(&heap) >= 2) {
		// 3回down_heap処理するところを必要な2回のみに抑えているので若干高速化が期待できる
		Node* node1 = heap_pop(&heap);
		Node* node2 = heap_steal_root(&heap);
		Node* parent = node_new(-1, node1->count + node2->count);
		parent->left = node1;
		node1->parent = parent;
		parent->right = node2;
		node2->parent = parent;
		heap_place_root(&heap, parent);
	}

	assert(heap_get_len(&heap) == 1);
	Node* root = heap_peek(&heap);

	cache_bit_pattern(root, 0u, 0u);
#ifdef _DEBUG
	debug_show_bit_patterns(root);
#endif

	return root;
}

static void cache_bit_pattern(Node* node, uint32_t pattern, uint32_t len) {
	if (node->left == NULL || len >= 30) {
		// 子nodeがないか、ビットパターンが長くなるのならば、そこで打ち切る
		node->cached_pattern.pattern = pattern;
		node->cached_pattern.len = (uint8_t) len;
	} else {
		pattern <<= 1; // 10 1001 -> 101 0010
		cache_bit_pattern(node->left, pattern, len + 1u);
		pattern |= 1; // 101 0010 -> 101 0011
		cache_bit_pattern(node->right, pattern, len + 1u);
	}
}

static void append_encoded_bits(EncoderState* state, Node* n) {
	if (n->cached_pattern.len == 0) {
		// (生成されたビットパターンが長いなどで) キャッシュが存在しなければ
		Node* parent = n->parent;
		if (parent) {
			// ビット列は親のほうが先
			append_encoded_bits(state, parent);
			if (parent->left == n) {
				bitlist_put(state->buffer, 0, 1);
			} else {
				assert(parent->right == n);
				bitlist_put(state->buffer, 1, 1);
			}
		}
		// 親が居ない (なんてことはキャッシュ処理を行うからないはずなんだけど) ときは、ビット列を出力しない (呼び出し元でビット列の作成)
	} else {
		// キャッシュが存在すればそれを出力するだけ
		bitlist_put(state->buffer, n->cached_pattern.pattern, (uint8_t) n->cached_pattern.len);
	}
}

static void write_tree(EncoderState* state, Node* node) {
	if (node->left == NULL) {
		// 0<bit pattern>
		// bit pattern := "0000 0000 0" if symbol == 0
		//                "0000 0000 1" if symbol == EOF
		//                symbol (otherwise)
		bitlist_put(state->buffer, 0, 1);
		if (node->symbol & 0xff) {
			bitlist_put(state->buffer, (uint32_t) node->symbol, 8);
		} else {
			// '\0' or EOF
			bitlist_put(state->buffer, 0, 8);
			bitlist_put(state->buffer, (uint32_t) (node->symbol >> 8u), 1);
		}
	} else {
		// 1<subtree><subtree>
		bitlist_put(state->buffer, 1, 1);
		write_tree(state, node->left);
		write_tree(state, node->right);
	}
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
			LOG(debug, "EOF  : [%4u] %s", node->count, buf);
		} else {
			LOG(debug, "%02x(%c): [%4u] %s", node->symbol, get_readable_char(node->symbol), node->count, buf);
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
