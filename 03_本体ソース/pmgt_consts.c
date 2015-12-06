#include "pmgt.h"

status_t
PN_consts(
  Option_t *option,  /* I */
  Config_t *config,  /* I */
  Base_t *base  /* I */
){
  int i, j;
  char path[PATH_MAX + 1];
  FILE *stream, *stream2;
  char *clone;

  PCMsg("gen consts");

  if (! base->consts) { return NORMAL; }

  /* sh */
  for (i = 1; base->envs[i]; ++i)
    {
      snprintf(
        path,
        sizeof(path),
        "%s/%s_template.sh",
        config->consts_dir,
        base->envs[i][0]);
      PCMsg("  %s", path);

      stream = fopen(path, "w");
      if (! stream) { return ABNORMAL; }

      snprintf(
        path,
        sizeof(path),
        "%s/%s_clear.sh",
        config->consts_dir,
        base->envs[i][0]);
      PCMsg("  %s", path);

      stream2 = fopen(path, "w");
      if (! stream2) { return ABNORMAL; }

      for (j = 1; base->consts[j]; ++j)
        {
          clone = strdup(base->consts[j][2]);
          String_toUpper(clone);
          fprintf(
            stream,
            "\n"
            "# %s,%s,%s,%s\n"
            "#FACTER_%s='@'; export FACTER_%s\n",
            base->consts[j][0],
            base->consts[j][1],
            base->consts[j][2],
            base->consts[j][3],
            clone,
            clone);
          fprintf(
            stream2,
            "\n"
            "# %s,%s,%s,%s\n"
            "unset FACTER_%s\n",
            base->consts[j][0],
            base->consts[j][1],
            base->consts[j][2],
            base->consts[j][3],
            clone);
          free(clone);
        }

      fclose(stream);
      fclose(stream2);
    }

  /* csh */
  for (i = 1; base->envs[i]; ++i)
    {
      snprintf(
        path,
        sizeof(path),
        "%s/%s_template.csh",
        config->consts_dir,
        base->envs[i][0]);
      PCMsg("  %s", path);

      stream = fopen(path, "w");
      if (! stream) { return ABNORMAL; }

      snprintf(
        path,
        sizeof(path),
        "%s/%s_clear.csh",
        config->consts_dir,
        base->envs[i][0]);
      PCMsg("  %s", path);

      stream2 = fopen(path, "w");
      if (! stream2) { return ABNORMAL; }

      for (j = 1; base->consts[j]; ++j)
        {
          clone = strdup(base->consts[j][2]);
          String_toUpper(clone);
          fprintf(
            stream,
            "\n"
            "# %s,%s,%s,%s\n"
            "#setenv FACTER_%s '@'\n",
            base->consts[j][0],
            base->consts[j][1],
            base->consts[j][2],
            base->consts[j][3],
            clone);
          fprintf(
            stream2,
            "\n"
            "# %s,%s,%s,%s\n"
            "unsetenv FACTER_%s\n",
            base->consts[j][0],
            base->consts[j][1],
            base->consts[j][2],
            base->consts[j][3],
            clone);
          free(clone);
        }

      fclose(stream);
      fclose(stream2);
    }

  return NORMAL;
}
