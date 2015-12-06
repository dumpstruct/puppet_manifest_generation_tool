#ifndef _LIB_OPTION_H
#define _LIB_OPTION_H 1
/*
 * 共通ライブラリ　オプション
 *
 * 1995-2002 K.H
 * 2014 K.H  pmgt 向け最小
 */

#include "libcommon.h"

/* オプション取得処理
 *
 * 1.オプションが見つかった場合、それ以降のパラメータを詰める
 * 2.オプションが重複して指定されているとき、一番最後の指定を取得する。
 *     またこのとき、同じオプションはすべて削除する。
 *     戻り値にオプションがいくつ見つかったか返す。
 *     エラーとするか、それとも最後のみ有効な仕様にするか、受け取り側で判断
 *   すること
 * 3.オプションに値があるとき value を指定し、ないとき NULL を指定すること
 * 4."--" をオプション終了と見なす
 * 5.value に NULL 以外を指定したとき、その値は NULL になることは無い
 *   (最初に *value = ""; で初期化する)
 */
int  /* <0: パラメータエラー, 0:無し, >0:見つかった数 */
Option_get(
  int *argc,  /* IO */
  char **argv,  /* IO */
  char *name,  /* I */
  char **value  /* IO: NULL:値なし, 他:オプション値 */
);

#define OPTION_MAX_VALUES 512  /* １つのオプションの最大指定数 */

/* オプション群取得処理
 *
 * 1.オプションが見つかった場合、それ以降のパラメータを詰める。
 * 2.オプションが重複して指定されているとき、各値を values に設定する。
 * 3.オプションに値があるとき values を指定し、ないとき NULL を指定すること。
 *   値の終了を示す NULL を最後に設定する。
 * 4."--" をオプション終了と見なす。
 */
bool_t  /* TRUE:あり, FALSE:なし */
Option_gets(
  int *argc,  /* IO */
  char **argv,  /* IO */
  char *name,  /* I */
  char *values[OPTION_MAX_VALUES + 1]  /* IO: NULL:なし, 他:オプション値群 */
);

/* オプション有無検査処理
 *
 * 1.'-'文字から始まる文字列をオプションと見なす。
 */
bool_t
Option_exist(
  int argc,  /* I */
  char **argv  /* I */
);

#if 0  /* ! pmgt */
/* 数字以外 オプション有無検査処理
 *
 * 1.'-'文字から始まる文字列をオプションと見なす。
 *   ただし、- 直後に数字"0"から"9"のときは例外とする
 */
bool_t
Option_existNoNumber(
  int argc,  /* I */
  char **argv  /* I */
);

/* オプション削除処理 */
void
Option_deleteOption(
  int *argc,  /* IO */
  char **argv,  /* IO */
  char *option  /* I */
);

/* オプション数値取得処理
 *
 * 1.オプション取得処理(Option_get)により値を取得する。
 *   続いて、値の数値化を行い指定された範囲比較を行う。
 * 2.戻り値が FALSE(オプションがない)のとき、stringValue 以外のパラメータは
 *   変化しない。
 * 2.当該関数の使用手順例は下記のようになる。
 *
 *   int argc;
 *   char **argv;
 *   char *value;
 *   bool_t isRange;
 *   Options *options;
 *
 *   if (Option_getInt(&argc, argv, "-option", &value, &isRange,
 *         &options->ma, 1, 100))
 *     {
 *       if (! isRange)
 *         {
 *           CommandError_print(
 *             "オプションが有効範囲外です -- %s", value);
 *
 *           return ABNORMAL;
 *         }
 *     }
 *
 *   ＊範囲外の値をエラーメッセージで利用するときなどは、文字列(stringValue)を
 *     使用すること(数字以外のとき intValue に値を返せないため)
 */
bool_t
Option_getInt(
  int *argc,  /* IO */
  char **argv,  /* IO */
  char *name,  /* I */
  char **stringValue,  /* O */
  bool_t *isRange,  /* O */
  int *intValue,  /* O */
  int mi,
  int ma
);

/* 不明オプションメッセージ処理
 *
 * 1.パラメータに含まれているすべてのオプションに対して、下記のメッセージを
 *   表示する。
 *
 *       不明なオプション(オプション文字列)です
 *
 * 2.当該処理はすべてのオプションを取得した後に呼び出し、残るオプションを
 *   不明なオプションとしてメッセージ表示するために使用する
 */
bool_t  /* TRUE:オプションあり, FALSE:オプションなし */
Option_unknown(
  int *argc,
  char **argv  /* I */
);

/* オプション一意処理
 *
 * 1.パラメータを一意にする。
 * 2.当該関数は文字列を検査対象とする。そのため同じファイルを一意にすることは
 *   できない。ファイルを一意にしたいとき Option_uniqFiles を使用すること。
 */
status_t  /* パラメータ誤りのみエラー */
Option_uniq(
  int *argc,  /* IO */
  char **argv  /* IO */
);

/* パスオプション一意処理
 *
 * 1.すべてのパラメータにファイル、またはディレクトリを指定していると仮定し、
 *   シンボリック・リンクを含め一意にする。
 * 2.同一のファイルがシンボリック・リンク・ファイルと実体ファイルであるとき、
 *   実体ファイルのパスを残す。
 *     両方のファイルがシンボリック・リンク・ファイルのとき、パラメータ順で
 *   先のパスが残る。
 * 3.ファイルの実在検査は行わない。
 * 4.文字列最後の / 記号は削除する。
 */
status_t  /* パラメータ誤りのみエラー */
Option_uniqPath(
  int *argc,  /* IO */
  char **argv  /* IO */
);

/* トリムオプション取得処理
 *
 * 1.文字列のトリムオプションを取得する。
 *
 *   　仕様は下記の固定となる。
 *
 *        -オプション {1 | 2 | T}[,{1 | 2 | T}]
 *
 *            1 ... 半角空白を取り除きます。
 *            2 ... 全角空白を取り除きます。
 *            T ... タブを取り除きます。
 *
 *        複数の要素を同時に取り除く場合、下記例のように指定する。
 *
 *            -オプション 12T   ... 左右から半角/全角空白/タブを取り除く
 *            -オプション 2,1T  ... 左全角空白、右半角空白/タブを取り除く
 *
 * 2.取得した文字列は下記のように処理する。
 *
 *   if (trim[0][0])
 *     {
 *       /@ bool_t 削除有無は受け取らない @/
 *       (void)String_leftTrimChars(value, trim[0]);
 *     }
 *
 *   if (trim[1][0])
 *     {
 *       /@ bool_t 削除有無は受け取らない @/
 *       (void)String_rightTrimChars(value, trim[1]);
 *     }
 */
status_t
Option_getTrim(
  char trim[2][1 + 2 + 1 + 1],  /* O 半角スペース、全角スペース、タブ */
  char *value  /* I */
);

#endif  /* ! pmgt */

#endif /* _LIB_OPTION_H */
