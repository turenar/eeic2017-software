#include "heap.h"
#include <assert.h>
#include <stdlib.h>

static const size_t ROOT = 1;

/** std::greaterと同等、a > b */
static inline bool node_compare(const Node* a, const Node* b);
static inline size_t left_child(size_t);
//static inline size_t right_child(size_t);
static inline size_t parent(size_t);
static inline void set_broken_head(Heap* h, bool);
/**
 * down heap処理を行う
 * @param p 1始まり要素番号でヒープ開始位置 (inclusive)
 * @param q 1始まり要素番号でヒープ終了位置 (inclusive)
 */
static void down_heap(Node**, size_t p, size_t q);

void heap_prepare(Heap*h ,Node** n, size_t len) {
	h->arr = n - 1; // heapの要素番号を1始まりにするため。
	h->len = len;
	set_broken_head(h, false);
	for (size_t p = parent(len); p >= ROOT; --p) {
		down_heap(h->arr, p, len);
	}
}

size_t heap_get_len(const Heap* h) {
	return h->len;
}

Node* heap_peek(const Heap* h) {
	assert(!h->_broken_head);
	return h->arr[ROOT];
}

Node* heap_pop(Heap* h) {
	assert(!h->_broken_head);
	Node* ret = h->arr[ROOT];
	h->arr[ROOT] = h->arr[h->len];
	--h->len;
	down_heap(h->arr, ROOT, h->len);
	return ret;
}

Node* heap_steal_root(Heap* h) {
	assert(!h->_broken_head);
	set_broken_head(h, true);
	return h->arr[ROOT];
}

void heap_place_root(Heap* h, Node* n) {
	assert(h->_broken_head);
	h->arr[ROOT] = n;
	down_heap(h->arr, ROOT, h->len);
	set_broken_head(h, false);
}

static inline bool node_compare(const Node* a, const Node* b) {
	return a->count > b->count;
}

static inline size_t left_child(size_t p) {
	return p * 2;
}

static inline size_t parent(size_t p) {
	return p / 2;
}

static void down_heap(Node** a, size_t p, size_t q) {
	// 参考: http://www.moon.sannet.ne.jp/okahisa/sort/node20.html
	Node* tmp = a[p];
	size_t c;
	while ((c = left_child(p)) <= q) {
		if (c < q && node_compare(a[c], a[c + 1])) {
			c++; // min(a[c], a[c+1]) == a[c+1]
		}
		if (!node_compare(tmp, a[c])) {
			// すでに下はheapになっている
			break;
		}
		a[p] = a[c]; // 小さいものは上に持っていく
		p = c;
	}
	a[p] = tmp; // 最後に
}

static inline void set_broken_head(Heap* h, bool val) {
#ifdef _DEBUG
	h->_broken_head = val;
#else
	(void) val;
	(void) h;
#endif
}

