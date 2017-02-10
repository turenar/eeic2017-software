#pragma once

#include <stdbool.h>
#include <stdio.h>
#include "attribute.h"

struct decoder_state;
typedef struct decoder_state DecoderState;

/**
 * インスタンスの作成
 * @param fp 出力先ファイルポインタ
 */
DecoderState* decoder_new(FILE* fp) __attribute_malloc__;
/**
 * デコーダーにバイト列を通し、ファイルポインタに出力する
 *
 * バイト列が不完全な場合、適切に処理される。
 * @param input 入力バイト列
 * @param len バイト列長
 * @return 圧縮データが終了した場合trueが返る
 */
bool decoder_update(DecoderState*, unsigned char* input, size_t len);
/**
 * デコーダーを破棄する
 */
void decoder_close(DecoderState*);
