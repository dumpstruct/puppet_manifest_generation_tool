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
){
  int count = 0;
  int i, j;
  int argcSave;

  if (value) { *value = ""; }
  if (! argc) { return -1; }
  if (*argc <= 0) { return FALSE; }
  if (! argv) { return -1; }
  if (! name) { return -1; }
  if (! *name) { return -1; }

  argcSave = *argc;

LABEL_DUPLICATION:;  /* 重複した指定は一番最後が有効 */

  for (i = 0; i < *argc; ++i)
    {
      if (strcmp(argv[i], "--") == 0)
        {
          break;
        }

      if (strcmp(argv[i], name) == 0)
        {
          count += 1;
          --(*argc);

          for (j = i; j < *argc; ++j)
            {
              argv[j] = argv[j + 1];
            }

          if (value)
            {
              if (i < *argc)
                {
                  *value = argv[i];
                  --(*argc);

                  for (j = i; j < *argc; ++j)
                    {
                      argv[j] = argv[j + 1];
                    }
                }
            }

          break;
        }
    }

  if (count)
    {
      if (argcSave != *argc)
        {
          argcSave = *argc;

          goto LABEL_DUPLICATION;
        }

      if (value && ! *value)
        {
          *value = "";
        }
    }

  return count;
}

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
){
  bool_t exist = FALSE;
  int i, j;
  int argcSave;
  int valuesIndex = 0;

  if (values) { values[0] = NULL; }
  if (! argc) { return FALSE; }
  if (*argc <= 0) { return FALSE; }
  if (! argv) { return FALSE; }
  if (! name) { return FALSE; }
  if (! *name) { return FALSE; }

  argcSave = *argc;

LABEL_DUPLICATION:;  /* 重複した指定は一番最後が有効 */

  for (i = 0; i < *argc; ++i)
    {
      if (strcmp(argv[i], "--") == 0)
        {
          break;
        }

      if (strcmp(argv[i], name) == 0)
        {
          exist = TRUE;
          --(*argc);

          for (j = i; j < *argc; ++j)
            {
              argv[j] = argv[j + 1];
            }

          if (values)
            {
              if (i < *argc)
                {
                  values[valuesIndex] = argv[i];
                  ++valuesIndex;
                  --(*argc);

                  for (j = i; j < *argc; ++j)
                    {
                      argv[j] = argv[j + 1];
                    }
                }
            }

          break;
        }
    }

  if (exist)
    {
      if (argcSave != *argc)
        {
          argcSave = *argc;

          goto LABEL_DUPLICATION;
        }
    }

  if (values)
    {
      values[valuesIndex] = NULL;
    }

  return exist;
}

/* オプション有無検査処理
 *
 * 1.'-'文字から始まる文字列をオプションと見なす。
 */
bool_t
Option_exist(
  int argc,  /* I */
  char **argv  /* I */
){
  bool_t exist = FALSE;
  int i;

  if (argc < 0) { return FALSE; }
  if (! argv) { return FALSE; }

  for (i = 0; i < argc; ++i)
    {
      if (strcmp(argv[i], "--") == 0)
        {
          break;
        }

      if (argv[i][0] == '-' && argv[i][1])
        {
          exist = TRUE;
          break;
        }
    }

  return exist;
}

/* 不明オプションメッセージ処理
 *
 * 1.パラメータに含まれているすべてのオプションに対して、下記のメッセージを
 *   表示する。
 *
 *       不明なオプション(オプション文字列)です
 *
 * 2.当該処理はすべてのオプションを取得した後に呼び出し、残るオプションを
 *   不明なオプションとしてメッセージ表示するために使用する。
 * 3.-- をオプション終了と見なし、これを削除する。
 */
bool_t  /* TRUE:オプションあり, FALSE:オプションなし */
Option_unknown(
  int *argc,
  char **argv  /* I */
){
  bool_t exist = FALSE;
  int i, j;

  if (! argc) { return FALSE; }
  if (*argc <= 0) { return FALSE; }
  if (! argv) { return FALSE; }

  for (i = 0; i < *argc; ++i)
    {
      if (argv[i][0] != '-')
        {
          continue;
        }

      if (strcmp(argv[i], "--") == 0)
        {
          --(*argc);

          for (j = i; j < *argc; ++j)
            {
              argv[j] = argv[j + 1];
            }

          break;
        }

#if 0  /* ! pmgt */
      CommandError_print("不明なオプション(%s)です", argv[i]);
#else
      fprintf(stderr, "pmgt:ERR: 不明なオプション(%s)です", argv[i]);
#endif

      exist = TRUE;
    }

  return exist;
}
