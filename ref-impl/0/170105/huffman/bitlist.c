#include "bitlist.h"
#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "logger.h"
#include "utils.h"

#if CHAR_BIT != 8
#error "CHAR_BIT is not 8!"
#endif

// ちょっと小さすぎるかも
#define DEFAULT_ALLOCATE_BITS (8*1024)

typedef struct bit_list {
	size_t allocated_bits;
	size_t pushed_bits; /*!< 書き込まれたビット数 */
	size_t read_bits; /*!< 読み込まれたビット数 */
	uint8_t* data; /*!< packed data */
} BitList;

/**
 * @brief 必要な領域が確保されているかを確認し、必要があれば確保する。
 *
 * 呼び出しから返ると、listの内容はすべて変更されているかもしれない。この呼び出し前後で変数のスコープをまたがないこと。
 * @param list
 * @param bit_len 必要な書き込みビット数 (pushed_bytes+新規領域)
 */
static void ensure_capacity(BitList* list, size_t bit_len);

BitList* bitlist_new() {
	BitList* ret = (BitList*) malloc(sizeof(struct bit_list));
	ret->pushed_bits = 0;
	ret->read_bits = 0;
	ret->allocated_bits = DEFAULT_ALLOCATE_BITS;
	ret->data = (uint8_t*) calloc(1, sizeof(uint8_t) * (DEFAULT_ALLOCATE_BITS / 8));
	return ret;
}

void bitlist_free(BitList* list) {
	free(list->data);
	free(list);
}

size_t bitlist_tell_read_ptr(const BitList* list) {
	return list->read_bits;
}

void bitlist_seek_read_ptr(BitList* list, size_t ptr) {
	assert(ptr <= list->pushed_bits);
	list->read_bits = ptr;
}

size_t bitlist_tell_write_ptr(const BitList* list) {
	return list->pushed_bits;
}

void bitlist_put_bytes(BitList* list, unsigned char* bytes, size_t len) {
	assert(list->pushed_bits % 8 == 0);
	ensure_capacity(list, list->pushed_bits + len * 8);
	size_t pushed_bytes = list->pushed_bits / 8;
	memcpy(list->data + pushed_bytes, bytes, len);
	list->pushed_bits += len * 8;
}

void bitlist_put(BitList* list, uint32_t bits, const uint8_t bit_len) {
	assert(bit_len <= 32);
	ensure_capacity(list, list->pushed_bits + bit_len);

	size_t pushed_bytes = list->pushed_bits / 8;
	size_t fragment_bits = list->pushed_bits & 0x07; // 前回までに書き込まれたはしくれ部分の大きさ
	size_t copy_offset_bits = 0; // "bits"から何bitコピーしたか
	uint8_t* p = list->data + pushed_bytes; // 次の書き込みバイト先
	uint8_t remain_bits = bit_len; // 残りコピービット数
#if 0
	LOG(debug, "bitlist_put: input=0x%x (len=%u)", (bits & ((1u << bit_len) - 1u)), bit_len);
#endif
	while (remain_bits > 0) {
		// なにやっているのかさっぱりわからないがとりあえず動く・・・
		uint8_t copy_bits = (uint8_t) (min(remain_bits, 8 - fragment_bits)); // コピーするビット数
		uint8_t shift_bit = (uint8_t) (8 - fragment_bits - copy_bits); // いくつ左shiftをして保管するか
		uint8_t bit_mask = (uint8_t) ((1 << copy_bits) - 1); // 書き込みで余分なゴミがつかないように
		uint8_t bucket = (uint8_t) ((bits >> (remain_bits - copy_bits)) & bit_mask); // 書き込む実データ
		if (fragment_bits) {
			*p = (uint8_t) ((*p & ~bit_mask) | (bucket << shift_bit)); // はしくれが残っていたらそれを活かす
			// *p=1010 1010, fragment_bits=1, bits=0011 0011, bit_len=4だった場合、
			// copy_bits=4, shift_bit=3, bit_mask=0000 1111, bucket=0000 0011
			// よって*p=10[00 11]10となる。右端に10というゴミがついているが、次回以降の書き込みで修正され、読み出し時には無視される
		} else {
			// fragment_bitsがない場合。上の場合は*p=[0011] 1010となる。
			*p = (uint8_t) (bucket << shift_bit);
		}
#if 0
		LOG(debug, "data[%d] = 0x%x", (int) (p - list->data), (unsigned int) *p);
#endif
		remain_bits = (uint8_t) (remain_bits - copy_bits);
		copy_offset_bits += copy_bits;
		fragment_bits = 0;
		p++;
	}
	list->pushed_bits += bit_len;
}

void bitlist_pad(BitList* list) {
	size_t pushed_bytes = list->pushed_bits / 8;
	size_t fragment_bits = list->pushed_bits & 0x07; // if 6:
	if (fragment_bits == 0) {
		return;
	}
	uint8_t padding_bits = (uint8_t) (8 - fragment_bits); // 2
	uint8_t mask = (uint8_t) ((1u << padding_bits) - 1); // 0000 0011
	list->data[pushed_bytes] &= (uint8_t) ~mask; // データとして使用する部分のゴミを消去する 1000 1110 -> 1000 11[00]
	list->pushed_bits += padding_bits;
}

size_t bitlist_pop_bytes(BitList* list, unsigned char* buf, size_t len) {
	assert(list->read_bits % 8 == 0);
	size_t copy_len = min(len, (list->pushed_bits - list->read_bits) / 8); // 半端なビット列は切り捨て
	uint8_t* in_p = list->data + list->read_bits / 8;
	unsigned char* out_p = buf;
	for (size_t i = 0; i < copy_len; ++i) {
		*out_p++ = *in_p++;
	}
	list->read_bits += copy_len * 8;
	return copy_len;
}

bool bitlist_pop_bits(BitList* list, uint32_t* out_p, const uint8_t bit_len) {
	assert(bit_len <= 32);
	if (list->read_bits + bit_len > list->pushed_bits) {
		return false;
	}

	uint32_t out = 0;
	size_t remain_bits = bit_len;

	size_t read_bytes = list->read_bits / 8;
	size_t fragment_bits = list->read_bits - read_bytes * 8;
	if (fragment_bits > 0) { // if fragments_bits==1 or 7:
		size_t fragment_copyable_bits = 8 - fragment_bits; // 7 or 1
		uint32_t mask = (1u << fragment_copyable_bits) - 1u; // 0111 1111 or 0000 0001
		out = list->data[read_bytes++] & mask;
		if (remain_bits < fragment_copyable_bits) { // if remain_bits==2 or 4:
			out >>= (fragment_copyable_bits - remain_bits); // 0[11]1 1111 -> 0000 0011 or (dead)
			remain_bits = 0;
		} else {
			remain_bits -= fragment_copyable_bits;
		}
	}

	while (remain_bits >= 8) {
		out = (out << 8) | list->data[read_bytes++];
		remain_bits -= 8;
	}

	if (remain_bits > 0) {
		uint32_t mask = ~((1u << (8 - remain_bits)) - 1u); // 使う部分のマスク 0110 0000
		out = (out << remain_bits) | ((list->data[read_bytes] & mask) >> (8 - remain_bits));
	}

	list->read_bits += bit_len;
	*out_p = out;
	return true;
}


static void ensure_capacity(BitList* list, size_t bit_len) {
	if (bit_len > list->allocated_bits && bit_len < list->read_bits * 4) {
		// compaction
		size_t begin = list->read_bits / 8; // 切り捨て
		size_t end = (list->pushed_bits + 7) / 8; // 切り上げ
		LOG(debug, "compaction: begin=%zu, end=%zu", begin, end);
		memcpy(list->data, list->data + begin, end - begin);
		bit_len -= begin * 8u;
		list->read_bits -= begin * 8u;
		list->pushed_bits -= begin * 8u;
	}
	if (bit_len > list->allocated_bits) {
		// extend buffer if no
		size_t alloc_bits = max(bit_len, 2 * list->allocated_bits);
		size_t alloc_bytes = alloc_bits / 8;
		LOG(debug, "reallocation: %zu bytes -> %zu bytes", list->allocated_bits / 8, alloc_bytes);
		uint8_t* realloced = (uint8_t*) realloc(list->data, alloc_bytes);
		if (!realloced) {
			LOG(error, "allocation failed: %zu bytes", alloc_bytes);
			abort();
		}
		list->data = realloced;
		list->allocated_bits = alloc_bits;
	}
}
