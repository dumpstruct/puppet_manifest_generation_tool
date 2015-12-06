#include "pmgt_common.h"

#define MAX_LINE_BYTES (1024 * 1024)


/* libstdlib から移植 */
char *
PC_fgets(
  char *s,  /* o */
  int size,
  FILE *stream  /* i */
){
  char *r;
  int bytes;

  s[0] = '\0';
  r = fgets(s, size, stream);

  if (r)
    {
      bytes = strlen(s);

      if (bytes > 0)
        {
          if (s[bytes - 1] == '\n')
            {
              s[bytes - 1] = '\0';
            }
        }
    }

  return r;
}

/* libstring から移植 */
char *  /* 末尾文字のアドレス */
String_getLastChar(
  char *string  /* I */
){
  char *now;

  if (! string) { return NULL; }  /* _LINT_IGNORE */
  if (! *string) { return string; }  /* _LINT_IGNORE */

#if 0  /* 遅い */
  { char *last = NULL;

  for (now = string; *now; ++now)
    {
      last = now;

      if (Char_is2Byte1(*now))
        {
          ++now;
        }
    }

  now = last;
  }
#else
  now = &string[strlen(string) - 1];  /* _LINT_IGNORE */

  /* Char_is2Byte2 は移植しないため、2 バイト文字はサポート対象外 */
#if 0
  if (Char_is2Byte2(*now))  /* _LINT_IGNORE */
    {
      --now;  /* _LINT_IGNORE */
    }
#endif
#endif

  return now;  /* _LINT_IGNORE */
}

/* libfile から移植 */
char *  /* ベース名 */
PCFile_basename(
  char *path  /* IO:分割後はディレクトリ名となる */
){
  char *basename;
  char *p;

  if (! path) { return NULL; }
  if (! *path) { return path; }

  basename = path;

  if (strcmp(path, "/") != 0)
    {
      p = String_getLastChar(path);

      if (*p == '/')
        {
          *p = '\0';
        }

      p = strrchr(path, (int)'/');

      if (p)
        {
          basename = p + 1;
          *p = '\0';
        }
    }

  return basename;
}

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
){
  struct stat s;

  if (! path) { return (mode_t)0; }
  if (! *path) { return (mode_t)0; }

  if (stat(path, &s) != 0) { return (mode_t)0; }

  return s.st_mode;
}

/* 通常ファイル確認処理
 *
 * 1.通常ファイル、またはリンク先が通常ファイルのとき TRUE を返す。
 * 2.エラーのときは FALSE を返す。
 */
bool_t  /* TRUE:通常ファイル, FALSE:非通常ファイル */
File_isRegular(
  char *path  /* I */
){
  bool_t is = FALSE;
  mode_t mode;

  if (! path) { return FALSE; }
  if (! *path) { return FALSE; }  /* _LINT_IGNORE */

  mode = File_getMode(path);

  if (S_ISREG(mode))
    {
      is = TRUE;
    }

  return is;
}

/* libbase から移植 */
static char _pmgt_msgHeader[MAX_MESSAGE_BYTES + 1];

void
PCMsg_setHeader(
  char *header  /* i */
){
  snprintf(_pmgt_msgHeader, sizeof(_pmgt_msgHeader), "%s", header);
}

void
PCMsg_err(
  char *format,  /* i */
  ...
){
  va_list ap;

  fprintf(stderr, "%s:ERR: ", _pmgt_msgHeader);

  va_start(ap, format);
  vfprintf(stderr, format, ap);
  va_end(ap);

  fprintf(stderr, "\n");
  fflush(stderr);
}

void
PCMsg(
  char *format,  /* i */
  ...
){
  va_list ap;

  fprintf(stdout, "%s: ", _pmgt_msgHeader);

  va_start(ap, format);
  vfprintf(stdout, format, ap);
  va_end(ap);

  fprintf(stdout, "\n");
  fflush(stdout);
}

/* 独自 */
static char **
_PCTL_tab2pp(
  char *line  /* io */
){
  char **r;
  int i, s, n = 0;
  char c;

  r = (char **)malloc(sizeof(char *));
  r[0] = NULL;

  for (i = 0, s = 0; ; ++i)
    {
      if (line[i] != '\t' && line[i] != '\0') { continue; }

      c = line[i];
      line[i] = '\0';
      ++n;
      r = (char **)realloc(r, sizeof(char **) * (n + 1));
      r[n - 1] = strdup(&line[s]);
      r[n] = NULL;

      if (c == '\0') { break; }

      s = i + 1;
    }

  return r;
}

static char ***
_PCTL_stream(
  FILE *stream  /* i */
){
  char ***r = NULL;
  char line[MAX_LINE_BYTES + 1];
  char *p, **pp;
  int n = 0;

  r = (char ***)malloc(sizeof(char **));
  r[0] = NULL;

  for (;;)
    {
      p = PC_fgets(line, sizeof(line), stream);
      if (! p && ! line[0]) { break; }

      ++n;

      pp = _PCTL_tab2pp(line);
      if (! pp) { return NULL; }

      r = (char ***)realloc(r, sizeof(char **) * (n + 1));
      r[n - 1] = pp;
      r[n] = NULL;

      if (! p) { break; }
    }

  return r;
}

static char ***  /* lines */
_PCTL(
  char *path  /* i */
){
  FILE *stream;
  char ***r;

  stream = fopen(path, "r");
  if (! stream) { return NULL; }

  r = _PCTL_stream(stream);
  fclose(stream);

  return r;
}

void
PCTab_debug(
  char ***tabs  /* i */
){
  int i, j;

  printf("PCTab_debug(0x%llu)\n", (unsigned long long)tabs);

  if (! tabs) { return; }

  for (i = 0; tabs[i]; ++i)
    {
      printf("  %d", i + 1);

      for (j = 0; tabs[i][j]; ++j)
        {
          printf("[%s]", tabs[i][j]);
        }

      printf("\n");
    }
}

char ***
PCTab_loader(
  char *dir,  /* i: dir or path */
  char *file,  /* i: file or NULL */
  char *ex  /* i: ex or NULL */
){
  char ***r;
  char path[PATH_MAX + 1];

  if (file)
    {
      snprintf(
        path,
        sizeof(path),
        "%s/%s%s%s",
        dir,
        file,
        ex ? ".": "",
        ex ? ex: "");
    }
  else
    {
      snprintf(
        path,
        sizeof(path),
        "%s%s%s",
        dir,
        ex ? ".": "",
        ex ? ex: "");
    }

  r = _PCTL(path);
  if (! r) { PCMsg_err("load - [%s]", path); }

  return r;
}

void
_PPP_free(
  char ***ppp  /* io */
){
  int i, j;

  if (! ppp) { return; }

  for (i = 0; ppp[i]; ++i)
    {
      for (j = 0; ppp[i][j]; ++j)
        {
          free(ppp[i][j]);
        }

      free(ppp[i]);
    }

  free(ppp);
}

void
_PPPP_free(
  char ****pppp  /* io */
){
  int i, j, k;

  if (! pppp) { return; }

  for (i = 0; pppp[i]; ++i)
    {
      for (j = 0; pppp[i][j]; ++j)
        {
          for (k = 0; pppp[i][j][k]; ++k)
            {
              free(pppp[i][j][k]);
            }
        }

      free(pppp[i]);
    }

  free(pppp);
}

status_t
PCStream_cat(
  FILE *istream,  /* i */
  FILE *ostream  /* o */
){
  int c, eof;

  while (c = getc(istream), c != EOF)
    {
      eof = putc(c, ostream);

      if (eof == EOF) { return ABNORMAL; }
    }

  return NORMAL;
}

status_t
PCTab_search2(
  char ***tabs,  /* i */
  char *key,  /* i */
  char **value  /* o: NULL or value */
){
  int i;

  *value = NULL;

  for (i = 0; tabs[i]; ++i)
    {
      if (! tabs[i][0]) { break; }

      if (strcmp(tabs[i][0], key) == 0)
        {
          *value = tabs[i][1];
          break;
        }
    }

  return NORMAL;
}

/* libstring から移植しない(フレームワーク切り離しが手間すぎ) */
bool_t
String_replace(
  char *string,  /* io */
  char *search,  /* i */
  char *replace  /* i */
){
  char *p;
  int sbytes, rbytes;

  p = strstr(string, search);
  if (! p) { return FALSE; }

  sbytes = strlen(search);
  rbytes = strlen(replace);

  if (rbytes <= sbytes)
    {
      memmove(p, replace, rbytes + 1);
      memmove(p + rbytes, p + sbytes, strlen(p + sbytes) + 1);
    }
  else
    {
      memmove(p + rbytes, p + sbytes, strlen(p + sbytes) + 1);
      memmove(p, replace, rbytes);
    }

  return TRUE;
}

void
String_replaces(
  char *string,  /* io */
  char *search,  /* i */
  char *replace  /* i */
){
  bool_t isReplace;

  do
    {
      isReplace = String_replace(string, search, replace);
    }
  while (isReplace);
}

void
String_toUpper(
  char *s  /* io */
){
  while (*s)
    {
      *s = (char)toupper((int)*s);
      ++s;
    }
}
