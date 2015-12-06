#if ! defined(_PMGT_COMMON_H)
#define _PMGT_COMMON_H 1

#include "libcommon.h"

#define PUPPET_HOME "/etc/puppet"
#define PMGT_HOME "/etc/puppet/PMGT"

#define PMGT_CONFIG_DIR "/etc/puppet/PMGT/config"
#define PMGT_CONFIG_FILE "config.tab"

#define MAX_MESSAGE_BYTES 1024
#define MAX_ATTRIBUTE_BYTES 1024
#define MAX_REPLACE_FIELDS 256


char *
PC_fgets(
  char *s,  /* o */
  int size,
  FILE *stream  /* i */
);

/* libfile から移植 */
char *  /* ベース名 */
PCFile_basename(
  char *path  /* IO:分割後はディレクトリ名となる */
);

/* ファイルモード取得処理
 *
 * 1.S_ISFIFO(mode), S_ISCHR(mode), S_ISDIR(mode), S_ISBLK(mode),
 *   S_ISREG(mode), S_ISSOCK(mode), S_ISDOOR(mode)
 *
 *   S_ISLNK() によるシンボリックリンクファイルのモードを取得したいときは、
 *   File_getSymbolicLinkMode() を使用すること。
 *
 * 2.シンボリックリンクはリンク先のモードを返す。
 *   リンク先が無効なときはエラーとなる。
 *
 *   if (File_getMode(path) == 0)  エラー、またはリンク先が無効
 *     {
 *       if (File_getSymbolicLinkMode(path) == 0)  エラー
 *         {
 *         }
 *       else  リンク先無効
 */
mode_t  /* 0:異常, 他:ファイルモード */
File_getMode(
  char *path  /* I */
);

/* 通常ファイル確認処理
 *
 * 1.通常ファイル、またはリンク先が通常ファイルのとき TRUE を返す。
 * 2.エラーのときは FALSE を返す。
 */
bool_t  /* TRUE:通常ファイル, FALSE:非通常ファイル */
File_isRegular(
  char *path  /* I */
);

/* その他 */
void
PCMsg_setHeader(
  char *header  /* i */
);

void
PCMsg_err(
  char *format,  /* i */
  ...
);

void
PCMsg(
  char *format,  /* i */
  ...
);

void
PCTab_debug(
  char ***tabs  /* i */
);

char ***
PCTab_loader(
  char *dir,  /* i: dir or path */
  char *file,  /* i: file or NULL */
  char *ex  /* i: ex or NULL */
);

void
_PPP_free(
  char ***ppp  /* io */
);
#define PPP_free(ppp_) \
  _PPP_free(ppp_); \
  ppp_ = NULL

void
_PPPP_free(
  char ****pppp  /* io */
);
#define PPPP_free(pppp_) \
  _PPPP_free(pppp_); \
  pppp_ = NULL

char *
PC_a(
  char *attribute  /* i */
);

status_t
PCTab_search2(
  char ***tabs,  /* i */
  char *key,  /* i */
  char **value  /* o: NULL or value */
);

status_t
PCStream_cat(
  FILE *istream,  /* i */
  FILE *ostream  /* o */
);

void
PC_brackets(
  char *type,  /* i */
  char *attribute,  /* i */
  char **l,  /* o */
  char **r  /* o */
);

bool_t
String_replace(
  char *string,  /* io */
  char *search,  /* i */
  char *replace  /* i */
);

void
String_replaces(
  char *string,  /* io */
  char *search,  /* i */
  char *replace  /* i */
);

void
String_toUpper(
  char *s  /* io */
);

#endif  /* _PMGT_COMMON_H */
