#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "attribute.h"

struct bit_list;
typedef struct bit_list BitList;

/**
 * BitListインスタンスを作成する。解放時は bitlist_free を使用する。
 */
BitList* bitlist_new() __attribute_malloc__;
/**
 * 解放する。NULLであってはならない。
 */
void bitlist_free(BitList*);

/**
 * 読み込みビット位置 (read ptr) を取得する。
 *
 * 取得したポインタは書き込みによって無効化される場合がある。間に挟まる操作が読み込みのみであることを条件に
 * bitlist_seek_read_ptr(BitList*, size_t) を使用することが出来る。
 *
 * もし仮に無効化されていた場合、abortするかSegmentation Faultするか、何事もなかったかのようにデータが壊れるかは場合による。
 * すべての処理は決定的であるとはいえ、見つけにくいバグになりやすいので注意すること。
 * @return 読み込み位置の生の値。この値を弄ってseekに使うことは推奨できない。
 */
size_t bitlist_tell_read_ptr(const BitList*);
/**
 * 読み込みビット位置 (read ptr)を設定する。
 *
 * 注意点については bitlist_tell_read_ptr を参照すること。
 *
 * @param ptr 読み込み位置の生の値。 bitlist_tell_read_ptr で取得した値を弄らずに渡すべきである。
 */
void bitlist_seek_read_ptr(BitList*, size_t ptr);
/**
 * 書き込みビット位置 (write ptr) を取得する。一度もpopされていないのみに限り、追加したビット数と一致する。
 *
 * 設定する関数は用意されていない。あたらしい書き込み自体がread/write ptrを無効にする場合があるのと、
 * そのような書き戻しに対応するのはput処理の呼び出し元が行うべきであるからである。
 * @return 書き込み位置の生の値。
 */
size_t bitlist_tell_write_ptr(const BitList*);

/**
 * BitListにビット列を追加する
 *
 * 一度でもpopしている場合は、この関数の呼び出しによってread/write ptrが無効になるかもしれない。
 *
 * @param bits 右詰めビット列。
 * @param bit_len ビット列の長さ。
 */
void bitlist_put(BitList*, uint32_t bits, uint8_t bit_len);
/**
 * BitListにバイト列を追加する
 *
 * ファイルからの読み込み等、8bit単位で行われる処理にはこちらを使うほうが効率が良い。
 * ただし、実装の効率化の都合上、write ptrが8bit単位になっていなければ使用できない。これを実現するためには、
 * すべての書き込みを8bit単位で行えばいいのである。
 * @param bytes バイト列
 * @param len バイト列の長さ
 */
void bitlist_put_bytes(BitList*, unsigned char* bytes, size_t len);
/**
 * 書き込みデータを8bit単位に揃える
 */
void bitlist_pad(BitList*);
/**
 * ビット列をバイト列として取得する
 *
 * read ptrが8bit単位になっていなければ使用できない。すなわちすべての読み込みを8bit単位で行えば問題ない。
 *
 * write ptrが8bit単位になっていない場合 (これまでに書き込まれたデータ全長が8bitになっていない場合)、
 * 8bit単位になってない部分は取得できない。必要があれば bitlist_pad を使用すること
 * @param buf バッファ
 * @param len バッファ長
 * @return バッファに書き込まれたバイト数
 */
size_t bitlist_pop_bytes(BitList*, unsigned char* buf, size_t len);
/**
 * まだ読み込まれていない部分のビット列を先頭から取得する
 * @param out 読み込み先。右詰めで格納される。
 * @param bit_len 読み込み
 * @return 必要な長さのビット列がBitListに書き込まれていなかった場合false。この場合はoutにはデータは書き込まれていない。
 */
bool bitlist_pop_bits(BitList*, uint32_t* out, uint8_t bit_len);
