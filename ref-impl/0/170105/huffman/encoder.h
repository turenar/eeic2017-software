#pragma once

#include <stdio.h>
#include "attribute.h"

struct encoder_state;
typedef struct encoder_state EncoderState;

/**
 * インスタンスの作成
 * @param fp 出力先ファイルポインタ
 */
EncoderState* encoder_new(FILE* fp) __attribute_malloc__;
/**
 * ハフマン符号を作成するためにバイト統計情報を更新する
 *
 * 原理上は、pass2で出現するバイトが全て統計情報としてあれば問題ないことにはなっているが、
 * そうする特段の理由がなければpass2に渡す全データをpass1でも使うべきである。
 * @param input 入力データ
 * @param len データ長
 */
void encoder_update_pass1(EncoderState*, unsigned char* input, size_t len);
/**
 * ハフマン符号を実際に作成し、ファイルポインタに出力する
 * @param input 入力データ
 * @param len データ長
 */
void encoder_update_pass2(EncoderState*, unsigned char* input, size_t len);
/** インスタンスの破棄 */
void encoder_close(EncoderState*);
