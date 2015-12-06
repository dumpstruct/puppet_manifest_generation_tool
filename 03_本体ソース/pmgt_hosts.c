#include "pmgt.h"

static char *
PH_pm(
  Base_t *base,  /* I */
  char *env  /* I */
){
  int i;

  for (i = 1; base->envs[i]; ++i)
    {
      if (strcmp(env, base->envs[i][0]) == 0)
        {
          return base->envs[i][1];
        }
    }

  return NULL;
}

status_t
PH_hosts(
  Option_t *option,  /* I */
  Config_t *config,  /* I */
  Base_t *base  /* I */
){
  int i, j;
  char path[PATH_MAX + 1];
  FILE *stream;
  char *pm;

  PCMsg("gen hosts");

  /* all */
  for (j = 0; base->hosts[1][j]; ++j)
    {
      snprintf(
        path,
        sizeof(path),
        "%s/%s_all",
        config->hosts_dir,
        base->hosts[1][j]);
      PCMsg("  %s", path);

      stream = fopen(path, "w");
      if (! stream) { return ABNORMAL; }

      for (i = 2; base->hosts[i]; ++i)
        {
          if (base->hosts[i][j][0])
            {
              fprintf(stream, "%s\n", base->hosts[i][j]);
            }
        }

      fclose(stream);
    }

  /* all minus puppetmaster */
  for (j = 0; base->hosts[1][j]; ++j)
    {
      snprintf(
        path,
        sizeof(path),
        "%s/%s_all_minus_pm",
        config->hosts_dir,
        base->hosts[1][j]);
      PCMsg("  %s", path);

      stream = fopen(path, "w");
      if (! stream) { return ABNORMAL; }

      for (i = 2; base->hosts[i]; ++i)
        {
          if (base->hosts[i][j][0])
            {
              pm = PH_pm(base, base->hosts[1][j]);

              if (! pm)
                {
                  PCMsg_err("bad envs or hosts");
                  fclose(stream);

                  return ABNORMAL;
                }

              if (strcmp(base->hosts[i][j], pm) == 0) { continue; }

              fprintf(stream, "%s\n", base->hosts[i][j]);
            }
        }

      fclose(stream);
    }

  return NORMAL;
}
