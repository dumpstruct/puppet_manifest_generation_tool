#ifndef _LIB_BASE_H
#define _LIB_BASE_H 1

/*
 * 共通ライブラリ　ベース
 *
 * 1995-2002 K.H
 * 2014 K.H  pmgt 向け最小コピー
 */

#if ! defined(NORMAL)

typedef int status_t;

#define NORMAL 0
#define ABNORMAL 1

#endif  /* status_t */

#if ! defined(TRUE)

typedef enum
{
  TRUE = 1,
  FALSE = 0
} bool_t;

#endif  /* bool_t */

#endif  /* _LIB_BASE_H */
