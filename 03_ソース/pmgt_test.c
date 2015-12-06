#include "pmgt.h"

static status_t
_PT_genLine(
  Config_t *config,  /* i */
  FILE *stream,  /* o */
  char **h,  /* i */
  char **attribute,  /* i */
  char **m  /* i */
){
  int i, j;
  bool_t isIf = TRUE, isIf2 = FALSE;
  char envs[MAX_ENVS + 1][MAX_ENV_BYTES + 1];

  for (i = 1; m[i]; ++i)
    {
      if (m[i][0] == '\0') { continue; }

      if (attribute[i][0] == '\0') { continue; }
      if (attribute[i][0] == '@') { continue; }

/* temp
      _PM_getEnvs(envs, m[i], attribute[i]);
*/

#if 0  /* DEBUG */
      printf("m[%s], a[%s]\n", m[i], attribute[i]);
#endif

/* temp
      for (j = 0; envs[j][0]; ++j)
        {
          if (isIf)
            {
              fprintf(
                stream,
              "\n  if   ((($pmgt_hostname == '%s') and ($pmgt_env == '%s'))\n",
                h[i],
                envs[j]);
              isIf = FALSE;
            }
          else
            {
              fprintf(
                stream,
                "     or (($pmgt_hostname == '%s') and ($pmgt_env == '%s'))\n",
                h[i],
                envs[j]);
            }
        }
*/
    }

  if (isIf == FALSE) { fprintf(stream, "  ) {\n"); }

  for (i = 1; m[i]; ++i)
    {
      if (attribute[i][0] == '\0') { continue; }
      if (attribute[i][0] != '@') { break; }

      if (strcmp(attribute[i], "@pmgt_if") == 0 && m[i][0])
        {
          fprintf(stream, "    if %s {\n", m[i]);
          isIf2 = TRUE;
          break;
        }
    }

  fprintf(stream, "%s    %s {", isIf2 ? "  ": "", PM_a(attribute[0]));

  for (i = 1; m[i]; ++i)
    {
      if (attribute[i][0] == '\0') { continue; }
      if (attribute[i][0] != '@') { break; }

/*
      _PM_attribute(
        config,
        isIf2,
        stream,
        PM_a(attribute[0]),
        h[i],
        attribute[i],
        m[i],
        h[1]);
*/
    }

  fprintf(stream, "%s    }\n", isIf2 ? "  ": "");

  if (isIf2) { fprintf(stream, "    }\n"); }
  if (isIf == FALSE) { fprintf(stream, "  }\n"); }

  return NORMAL;
}

static status_t
_PT_genTest(
  Option_t *option,  /* i */
  Config_t *config,  /* i */
  Base_t *base,  /* i */
  char ***ms  /* i */
){
  status_t status;
  int i;
  char path[PATH_MAX + 1], **attribute = NULL;
  FILE *stream;

  PCMsg("gen test - %s", ms[0][1]);

#if 0  /* DEBUG */
  PCTab_debug(ms);
#endif  /* DEBUG */

  snprintf(path, sizeof(path), "%s", config->test_dir);

  if (mkdir(path, 0755) != 0)
    {
      if (errno != EEXIST)
        {
          PCMsg_err("mkdir - %s", path);

          return ABNORMAL;
        }
    }

  snprintf(path, sizeof(path), "%s/params", config->test_dir);

  if (mkdir(path, 0755) != 0)
    {
      if (errno != EEXIST)
        {
          PCMsg_err("mkdir - %s", path);

          return ABNORMAL;
        }
    }

  snprintf(path, sizeof(path), "%s/results", config->test_dir);

  if (mkdir(path, 0755) != 0)
    {
      if (errno != EEXIST)
        {
          PCMsg_err("mkdir - %s", path);

          return ABNORMAL;
        }
    }

  snprintf(
    path,
    sizeof(path),
    "%s/params/%s.tab",
    config->test_dir,
    ms[0][1]);
  PCMsg("  %s", path);
  stream = fopen(path, "w");

  if (! stream)
    {
      PCMsg_err("open - [%s]\n", path);

      return ABNORMAL;
    }

  status = NORMAL;

  for (i = 1; ms[i]; ++i)
    {
      if (ms[i][0][0] == '\0') { continue; }
      if (ms[i][0][0] == '#') { continue; }

      if (ms[i][0][0] == '@')
        {
          attribute = ms[i];

          continue;
        }

      status = _PT_genLine(config, stream, ms[0], attribute, ms[i]);
      if (status) { break; }
    }

  fclose(stream);

  return status;
}

status_t
PT_test(
  Option_t *option,  /* I */
  Config_t *config,  /* I */
  Base_t *base  /* I */
){
  status_t status;
  char ****mss;
  int i;

  PCMsg("gen test");

  mss = PM_loadModules(option, config, base->modules);
  if (! mss) { return ABNORMAL; }

  for (i = 0; mss[i]; ++i)
    {
      status = _PT_genTest(option, config, base, mss[i]);
      if (status) { return status; }
    }

  PPPP_free(mss);

  return NORMAL;
}
