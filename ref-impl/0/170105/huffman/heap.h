#pragma once

#include <stdbool.h>
#include <stddef.h>
#include "node.h"

typedef struct heap_tag {
	Node** arr;
	size_t len;
#ifdef _DEBUG
	bool _broken_head;
#endif
} Heap;

/**
 * nodeをcountの小さい順に並び替えることでヒープ構造を作成する
 */
void heap_prepare(Heap*, Node**, size_t);

/**
 * ヒープの長さを取得する (heap->lenでも取得できる)
 */
size_t heap_get_len(const Heap*);
/**
 * 最もcountが小さいnodeを取得する
 */
Node* heap_peek(const Heap*);
/**
 * 最もcountが小さいnodeをpopする
 */
Node* heap_pop(Heap*);
/**
 * 最もcountが小さいnodeをpopするが、並び替えない (ヒープを壊した状態にする)
 *
 * 次の操作は heap_place_root に限る
 */
Node* heap_steal_root(Heap*);
/**
 * heap_steal_root で壊したヒープを、指定したノードを追加して正しいヒープのあり方に戻す。
 *
 * pop→pushするよりはsteal→placeしたほうがヒープの更新回数が一回分減るので良い。
 * ただし、pop→pushよりも操作の順番に敏感なので注意
 * @param n 追加するノード
 */
void heap_place_root(Heap*, Node* n);
