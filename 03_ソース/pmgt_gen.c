#include "pmgt.h"

status_t
PM_loadConfig(
  Option_t *option,  /* i */
  Config_t *config  /* o */
){
  char path[PATH_MAX + 1], *env;

  PCMsg("load config");

  memset(config, 0x00, sizeof(*config));

  if (option->config[0])
    {
      snprintf(path, sizeof(path), "%s", option->config);
    }
  else
    {
      env = getenv("PMGT_CONFIG");

      if (env)
        {
          snprintf(path, sizeof(path), "%s", env);
        }
      else
        {
          snprintf(
            path,
            sizeof(path),
            "%s/%s",
            PMGT_CONFIG_DIR,
            PMGT_CONFIG_FILE);
        }
    }

  PCMsg("  %s", path);
  config->config = PCTab_loader(path, NULL, NULL);

#if 0  /* DEBUG */
  PCTab_debug(config->config);
#endif  /* DEBUG */

  if (! config->config)
    {
      PCMsg_err("not exist config path - %s", path);

      return ABNORMAL;
    }

  PCTab_search2(config->config, "config_dir", &config->config_dir);

  if (! config->config_dir)
    {
      PCMsg_err("not exist config 'config_dir'");

      return ABNORMAL;
    }

  PCTab_search2(config->config, "config_brackets", &config->config_brackets);

  if (! config->config_brackets)
    {
      PCMsg_err("not exist config 'config_brackets'");

      return ABNORMAL;
    }

  PCMsg("  %s/%s", config->config_dir, config->config_brackets);
  config->brackets = PCTab_loader(
    config->config_dir,
    config->config_brackets,
    NULL);

#if 0  /* DEBUG */
  PCTab_debug(config->brackets);
#endif  /* DEBUG */

  if (! config->brackets)
    {
      PCMsg_err(
        "not exist config brackets path - %s/%s",
        config->config_dir,
        config->config_brackets);

      return ABNORMAL;
    }

  PCTab_search2(config->config, "puppet_conf_dir", &config->puppet_conf_dir);

  if (! config->puppet_conf_dir)
    {
      PCMsg_err("not exist config 'puppet_conf_dir'");

      return ABNORMAL;
    }

  PCTab_search2(config->config, "manifests_dir", &config->manifests_dir);

  if (! config->manifests_dir)
    {
      PCMsg_err("not exist config 'manifests_dir'");

      return ABNORMAL;
    }

  PCTab_search2(config->config, "modules_dir", &config->modules_dir);

  if (! config->modules_dir)
    {
      PCMsg_err("not exist config 'modules_dir'");

      return ABNORMAL;
    }

  PCTab_search2(config->config, "params_dir", &config->params_dir);

  if (! config->params_dir)
    {
      PCMsg_err("not exist config 'params_dir'");

      return ABNORMAL;
    }

  PCTab_search2(config->config, "params_envs", &config->params_envs);

  if (! config->params_envs)
    {
      PCMsg_err("not exist config 'params_envs'");

      return ABNORMAL;
    }

  PCTab_search2(config->config, "params_hosts", &config->params_hosts);

  if (! config->params_hosts)
    {
      PCMsg_err("not exist config 'params_hosts'");

      return ABNORMAL;
    }

  PCTab_search2(config->config, "params_modules", &config->params_modules);

  if (! config->params_modules)
    {
      PCMsg_err("not exist config 'params_modules'");

      return ABNORMAL;
    }

  PCTab_search2(config->config, "params_consts", &config->params_consts);

  if (! config->params_consts)
    {
      PCMsg_err("not exist config 'params_consts'");

      return ABNORMAL;
    }

  PCTab_search2(config->config, "templates_dir", &config->templates_dir);

  if (! config->templates_dir)
    {
      PCMsg_err("not exist config 'templates_dir'");

      return ABNORMAL;
    }

  PCTab_search2(
    config->config,
    "templates_site_header",
    &config->templates_site_header);

  if (! config->templates_site_header)
    {
      PCMsg_err("not exist config 'templates_site_header'");

      return ABNORMAL;
    }

  PCTab_search2(config->config, "hosts_dir", &config->hosts_dir);

  if (! config->hosts_dir)
    {
      PCMsg_err("not exist config 'hosts_dir'");

      return ABNORMAL;
    }

  PCTab_search2(
    config->config,
    "hostname_max_bytes",
    &config->hostname_max_bytes);

  if (! config->hostname_max_bytes)
    {
      PCMsg_err("not exist config 'hostname_max_bytes'");

      return ABNORMAL;
    }

  PCTab_search2(config->config, "consts_dir", &config->consts_dir);

  if (! config->consts_dir)
    {
      PCMsg_err("not exist config 'consts_dir'");

      return ABNORMAL;
    }

  PCTab_search2(config->config, "test_dir", &config->test_dir);

  if (! config->test_dir)
    {
      PCMsg_err("not exist config 'test_dir'");

      return ABNORMAL;
    }

  return NORMAL;
}

char *
PM_a(
  char *attribute  /* i */
){
  return &attribute[1];
}

void
PCBrackets_search(
  Config_t *config,  /* i */
  char *type,  /* i */
  char *attribute,  /* i */
  char **l,  /* o */
  char **r  /* o */
){
  int i;

  *l = "\"";
  *r = "\"";

  for (i = 0; config->brackets[i]; ++i)
    {
      if (config->brackets[i][0][0] == '*' ||
          strcmp(config->brackets[i][0], type) == 0)
        {
          if (config->brackets[i][1][0] == '*' ||
              strcmp(config->brackets[i][1], attribute) == 0)
            {
              *l = config->brackets[i][2];
              *r = config->brackets[i][3];

              break;
            }
        }
    }

#if 0  /* DEBUG */
  fprintf(stderr, "PC_brackets: [%s,%s,%s,%s]\n", type, attribute, *l, *r);
#endif  /* DEBUG */
}

status_t
PM_loadBase(
  Option_t *option,  /* i */
  Config_t *config,  /* i */
  Base_t *base  /* o */
){
  char path[PATH_MAX + 1];

  PCMsg("load base");

  memset(base, 0x00, sizeof(*base));
  PCMsg("  %s/%s", config->params_dir, config->params_envs);
  base->envs = PCTab_loader(config->params_dir, config->params_envs, NULL);

#if 0  /* DEBUG */
  PCTab_debug(base->envs);
#endif  /* DEBUG */

  if (! base->envs)
    {
      PCMsg_err(
        "not exist base envs path - %s/%s",
        config->params_dir,
        config->params_envs);

      return ABNORMAL;
    }

  PCMsg("  %s/%s", config->params_dir, config->params_hosts);
  base->hosts = PCTab_loader(config->params_dir, config->params_hosts, NULL);

#if 0  /* DEBUG */
  PCTab_debug(base->hosts);
#endif  /* DEBUG */

  if (! base->hosts)
    {
      PCMsg_err(
        "not exist base hosts path - %s/%s",
        config->params_dir,
        config->params_hosts);

      return ABNORMAL;
    }

  PCMsg("  %s/%s", config->params_dir, config->params_modules);
  base->modules = PCTab_loader(
    config->params_dir,
    config->params_modules,
    NULL);

#if 0  /* DEBUG */
  PCTab_debug(base->modules);
#endif  /* DEBUG */

  if (! base->modules)
    {
      PCMsg_err(
        "not exist base modules path - %s/%s",
        config->params_dir,
        config->params_modules);

      return ABNORMAL;
    }

  snprintf(
    path,
    sizeof(path),
    "%s/%s",
    config->params_dir,
    config->params_consts);

  if (File_isRegular(path))
    {
      PCMsg("  %s", path);
      base->consts = PCTab_loader(
        config->params_dir,
        config->params_consts,
        NULL);

#if 0  /* DEBUG */
      PCTab_debug(base->consts);
#endif  /* DEBUG */

      if (! base->consts)
        {
          PCMsg_err(
            "not exist base consts path - %s/%s",
            config->params_dir,
            config->params_consts);

          return ABNORMAL;
        }
    }

  return NORMAL;
}

static void
_PMGPC_replace(
  char *line,  /* io */
  char **envs  /* i */
){
  String_replaces(line, "@{PMGT_PuppetMaster}", envs[1]);
  String_replaces(line, "@{PMGT_env}", envs[0]);
}

static status_t
_PM_genPuppetConf(
  FILE *istream,  /* i */
  FILE *ostream,  /* o */
  char **envs  /* i */
){
  char line[MAX_LINE_BYTES + 1];
  char *p;
  int s;

  for (;;)
    {
      p = PC_fgets(line, sizeof(line), istream);
      if (! p && ! line[0]) { break; }

      _PMGPC_replace(line, envs);
      s = fprintf(ostream, "%s\n", line);
      if (s < 1) { return ABNORMAL; }
    }

  return NORMAL;
}

static status_t
_PM_genPuppetConfs(
  Option_t *option,  /* i */
  Config_t *config,  /* i */
  Base_t *base  /* i */
){
  status_t status;
  int i;
  char path[PATH_MAX + 1];
  FILE *istream, *ostream;

  PCMsg("gen puppet.conf");

  for (i = 1; base->envs[i]; ++i)
    {
      if (option->env[0])
        {
          if (strcmp(option->env, base->envs[i][0]) != 0) { continue; }
        }

      snprintf(
        path,
        sizeof(path),
        "%s/puppet.conf",
        config->templates_dir);
      istream = fopen(path, "r");

      if (! istream)
        {
          PCMsg_err("open - [%s]", path);

          return ABNORMAL;
        }

      snprintf(
        path,
        sizeof(path),
        "%s/puppet.conf_%s",
        config->puppet_conf_dir,
        base->envs[i][0]);
      PCMsg("  %s", path);
      ostream = fopen(path, "w");

      if (! ostream)
        {
          PCMsg_err("open - [%s]", path);

          return ABNORMAL;
        }

      status = _PM_genPuppetConf(istream, ostream, base->envs[i]);
      fclose(ostream);
      fclose(istream);

      if (status) { return status; }
    }

  return NORMAL;
}

static status_t
_PM_genAutosignConf(
  Option_t *option,  /* i */
  Config_t *config,  /* i */
  Base_t *base  /* i */
){
  status_t status;
  char path[PATH_MAX + 1];
  FILE *istream, *ostream;

  PCMsg("gen autosign.conf");

  snprintf(
    path,
    sizeof(path),
    "%s/autosign.conf",
    config->templates_dir);
  istream = fopen(path, "r");

  if (! istream)
    {
      PCMsg_err("open - [%s]", path);

      return ABNORMAL;
    }

  snprintf(
    path,
    sizeof(path),
    "%s/autosign.conf",
    config->puppet_conf_dir);
  PCMsg("  %s", path);
  ostream = fopen(path, "w");

  if (! ostream)
    {
      PCMsg_err("open - [%s]", path);

      return ABNORMAL;
    }

  status = PCStream_cat(istream, ostream);
  fclose(ostream);
  fclose(istream);

  if (status) { return status; }

  return NORMAL;
}

char ****
PM_loadModules(
  Option_t *option,  /* i */
  Config_t *config,  /* i */
  char ***modules  /* i */
){
  char ****r;
  int i, n = 0;

  PCMsg("load modules");

  r = (char ****)malloc(sizeof(char ***));
  r[0] = NULL;

  for (i = 1; modules[i]; ++i)
    {
      if (modules[i][0][0] == '\0' || modules[i][0][0] == '#') { continue; }

      ++n;
      r = (char ****)realloc(r, sizeof(char ***) * (n + 1));
      PCMsg("  %s", modules[i][0]);
      r[n - 1] = PCTab_loader(config->params_dir, modules[i][0], "tab");
      r[n] = NULL;

      if (! r[n - 1])
        {
          PCMsg_err("load module - [%s]", modules[i][0]);

          return NULL;
        }

#if 0  /* DEBUG */
      PCTab_debug(r[n - 1]);
#endif  /* DEBUG */
    }

  return r;
}

static status_t
_PM_siteHeader(
  Option_t *option,  /* i */
  Config_t *config,  /* i */
  char *env,  /* i */
  char ***consts,  /* i */
  FILE *ostream  /* o */
){
  status_t status;
  char path[PATH_MAX + 1];
  FILE *istream;
  int i;

  snprintf(
    path,
    sizeof(path),
    "%s/%s",
    config->templates_dir,
    config->templates_site_header);
  istream = fopen(path, "r");

  if (! istream)
    {
      PCMsg_err("open - [%s]", path);

      return ABNORMAL;
    }

  status = PCStream_cat(istream, ostream);

  if (fclose(istream) != 0)
    {
      if (status == NORMAL) { status = ABNORMAL; }
    }

  if (status != NORMAL) { return status; }

  (void)fprintf(
    ostream,
    "\n"
    "##########\n"
    "# pmgt consts\n"
    "\n"
    "if ($pmgt_env == \"\") {\n"
    "  $pmgt_env = '%s'\n"
    "}\n",
    env);

  (void)fprintf(ostream, "\n");
  (void)fprintf(ostream, "if ($pmgt_hostname == \"\") {\n");

  if (config->hostname_max_bytes[0] == '0')
    {
      (void)fprintf(ostream, "  $pmgt_hostname = $hostname\n");
    }
  else
    {
      (void)fprintf(
        ostream,
        "  $pmgt_hostname = sprintf(\"%%.%ss\", $hostname)\n",
        config->hostname_max_bytes);
    }

  (void)fprintf(ostream, "}\n");

  (void)fprintf(
    ostream,
    "\n"
    "##########\n"
    "# user consts\n");

  if (consts)
    {
      for (i = 1; consts[i]; ++i)
        {
          if (strcmp(consts[i][0], "*") == 0)
            {
              if (strcmp(consts[i][1], "*") == 0)
                {
                  (void)fprintf(
                    ostream,
                    "\n"
                    "if ($%s == \"\") {\n"
                    "  $%s = %s\n"
                    "}\n",
                    consts[i][2],
                    consts[i][2],
                    consts[i][3]);
                }
              else
                {
                  (void)fprintf(
                    ostream,
                    "\n"
                    "if ($pmgt_hostname == '%s') {\n"
                    "  if ($%s == \"\") {\n"
                    "    $%s = %s\n"
                    "  }\n"
                    "}",
                    consts[i][1],
                    consts[i][2],
                    consts[i][2],
                    consts[i][3]);

                  if (consts[i][4][0] == '\0')
                    {
                      (void)fprintf(ostream, "\n");
                    }
                  else
                    {
                      (void)fprintf(
                        ostream,
                        " else {\n"
                        "  if ($%s == \"\") {\n"
                        "    $%s = %s\n"
                        "  }\n"
                        "}\n",
                        consts[i][2],
                        consts[i][2],
                        consts[i][4]);
                    }
                }
            }
          else
            {
              if (strcmp(consts[i][1], "*") == 0)
                {
                  (void)fprintf(
                    ostream,
                    "\n"
                    "if ($pmgt_env == '%s') {\n"
                    "  if ($%s == \"\") {\n"
                    "    $%s = %s\n"
                    "  }\n"
                    "}",
                    consts[i][0],
                    consts[i][2],
                    consts[i][2],
                    consts[i][3]);
                }
              else
                {
                  (void)fprintf(
                    ostream,
                    "\n"
                    "if ($pmgt_env == '%s') and ($pmgt_hostname == '%s') {\n"
                    "  if ($%s == \"\") {\n"
                    "    $%s = %s\n"
                    "  }\n"
                    "}",
                    consts[i][0],
                    consts[i][1],
                    consts[i][2],
                    consts[i][2],
                    consts[i][3]);
                }

              if (consts[i][4][0] == '\0')
                {
                  (void)fprintf(ostream, "\n");
                }
              else
                {
                  (void)fprintf(
                    ostream,
                    " else {\n"
                    "  if ($%s == \"\") {\n"
                    "    $%s = %s\n"
                    "  }\n"
                    "}\n",
                    consts[i][2],
                    consts[i][2],
                    consts[i][4]);
                }
            }
        }
    }

  return NORMAL;
}

static status_t
_PM_genSite(
  Option_t *option,  /* i */
  Config_t *config,  /* i */
  Base_t *base  /* i */
){
  status_t status = ABNORMAL;
  int i, j;
  char path[PATH_MAX + 1];
  FILE *stream;

  PCMsg("gen site");

  for (i = 1; base->envs[i]; ++i)
    {
      if (option->env[0])
        {
          if (strcmp(option->env, base->envs[i][0]) != 0) { continue; }
        }

      snprintf(
        path,
        sizeof(path),
        "%s/site_%s.pp",
        config->manifests_dir,
        base->envs[i][0]);
      PCMsg("  %s", path);
      stream = fopen(path, "w");

      if (! stream)
        {
          PCMsg_err("open - [%s]", path);

          return ABNORMAL;
        }

      status = _PM_siteHeader(
        option, config, base->envs[i][0], base->consts, stream);
      if (status) { break; }

      (void)fprintf(
        stream,
        "\n"
        "##########\n"
        "# modules\n");

      for (j = 1; base->modules[j]; ++j)
        {
          fprintf(stream, "\nclass { \"%s\":\n", base->modules[j][0]);

          if (j > 1)
            {
              fprintf(
                stream,
                "  require => Class['%s'],\n",
                base->modules[j - 1][0]);
            }

          fprintf(stream, "}\n");
        }
    }

  fclose(stream);

  return NORMAL;
}

static status_t
_PM_attribute(
  Config_t *config,  /* i */
  bool_t isIf2,
  FILE *stream,  /* o */
  char *type,  /* i */
  char *h,  /* i */
  char *attribute,  /* i */
  char *m,  /* i */
  char *module  /* i */
){
  char *l, *r;

  if (! *m) { return NORMAL;}

  if (strcmp(attribute, "@pmgt_title") == 0)
    {
      fprintf(stream, " \"%s\":", m);
    }
  else if (strncmp(attribute, "@pmgt_replace,", 14) == 0)
    {
      return NORMAL;
    }
  else if (strcmp(attribute, "@pmgt_template") == 0)
    {
      fprintf(
        stream,
        "%s      content => template(\"%s/%s.erb\"),",
        isIf2 ? "  ": "",
        module,
        m);
    }
  else if (strcmp(attribute, "@pmgt_if") == 0)
    {
      return NORMAL;
    }
  else
    {
      PCBrackets_search(config, type, PM_a(attribute), &l, &r);
      fprintf(
        stream,
        "%s      %s => %s%s%s,",
        isIf2 ? "  ": "",
        PM_a(attribute),
        l,
        m,
        r);
    }

  fprintf(stream, "\n");

  return NORMAL;
}

static void
_PM_getEnvs(
  char envs[MAX_ENVS + 1][MAX_ENV_BYTES + 1],
  char *m,  /* i */
  char *attribute  /* i */
){
  char *envsM[MAX_ENVS + 1], *envsA[MAX_ENVS + 1];
  int i, j, e;
  char *cloneM, *cloneA;
  char *nowM, *nowA;

  cloneM = strdup(m);

  for (nowM = strtok(cloneM, ","), i = 0; nowM; nowM = strtok(NULL, ","), ++i)
    {
      envsM[i] = nowM;
    }

  envsM[i] = NULL;

  cloneA = strdup(attribute);

  for (nowA = strtok(cloneA, ","), i = 0; nowA; nowA = strtok(NULL, ","), ++i)
    {
      envsA[i] = nowA;
    }

  envsA[i] = NULL;

  e = 0;

  for (i = 0; envsM[i]; ++i)
    {
      if (strcmp(envsM[i], "*") == 0)
        {
          for (j = 0; envsA[j]; ++j)
            {
              snprintf(envs[e], sizeof(envs[0]), "%s", envsA[j]);
              ++e;
            }
        }
      else
        {
          snprintf(envs[e], sizeof(envs[0]), "%s", envsM[i]);
          ++e;
        }
    }

  envs[e][0] = '\0';

  free(cloneM);
  free(cloneA);
}

static status_t
_PM_init(
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

      _PM_getEnvs(envs, m[i], attribute[i]);

#if 0  /* DEBUG */
      printf("m[%s], a[%s]\n", m[i], attribute[i]);
#endif

#if 0  /* not simple */
      if (isIf)
        {
          fprintf(
            stream,
            "\n  if   (($pmgt_hostname == '%s') and (",
            h[i]);

          for (j = 0; envs[j][0]; ++j)
            {
              if (j)
                {
                  fprintf(stream, "\n          or ");
                }
              else
                {
                  fprintf(stream, "\n             ");
                }

              fprintf(stream, "($pmgt_env == '%s')", envs[j]);
            }

          fprintf(stream, ")\n");
          isIf = FALSE;
        }
      else
        {
          fprintf(
            stream,
            "    or (($pmgt_hostname == '%s') and (",
            h[i]);

          for (j = 0; envs[j][0]; ++j)
            {
              if (j)
                {
                  fprintf(stream, "\n          or ");
                }
              else
                {
                  fprintf(stream, "\n             ");
                }

              fprintf(stream, "($pmgt_env == '%s')", envs[j]);
            }

          fprintf(stream, "))\n");
        }
#else  /* simple */
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
#endif
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

      _PM_attribute(
        config,
        isIf2,
        stream,
        PM_a(attribute[0]),
        h[i],
        attribute[i],
        m[i],
        h[1]);
    }

  fprintf(stream, "%s    }\n", isIf2 ? "  ": "");

  if (isIf2) { fprintf(stream, "    }\n"); }
  if (isIf == FALSE) { fprintf(stream, "  }\n"); }

  return NORMAL;
}

static status_t
_PM_genModule(
  Option_t *option,  /* i */
  Config_t *config,  /* i */
  Base_t *base,  /* i */
  char ***ms  /* i */
){
  status_t status;
  int i;
  char path[PATH_MAX + 1], **attribute = NULL;
  FILE *stream;

  PCMsg("gen module - %s", ms[0][1]);

#if 0  /* DEBUG */
  PCTab_debug(ms);
#endif  /* DEBUG */

  snprintf(path, sizeof(path), "%s/gen/%s", config->modules_dir, ms[0][1]);

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
    "%s/gen/%s/manifests",
    config->modules_dir,
    ms[0][1]);
  (void)mkdir(path, 0755);

  snprintf(
    path,
    sizeof(path),
    "%s/gen/%s/templates",
    config->modules_dir,
    ms[0][1]);
  (void)mkdir(path, 0755);

  snprintf(
    path,
    sizeof(path),
    "%s/gen/%s/files",
    config->modules_dir,
    ms[0][1]);
  (void)mkdir(path, 0755);

  snprintf(
    path,
    sizeof(path),
    "%s/gen/%s/manifests/init.pp",
    config->modules_dir,
    ms[0][1]);
  PCMsg("  %s", path);
  stream = fopen(path, "w");

  if (! stream)
    {
      PCMsg_err("open - [%s]\n", path);

      return ABNORMAL;
    }

  fprintf(stream, "class %s\n{\n", ms[0][1]);

  status = ABNORMAL;

  for (i = 1; ms[i]; ++i)
    {
      if (ms[i][0][0] == '\0') { continue; }
      if (ms[i][0][0] == '#') { continue; }

      if (ms[i][0][0] == '@')
        {
          attribute = ms[i];

          continue;
        }

      status = _PM_init(config, stream, ms[0], attribute, ms[i]);
      if (status) { break; }
    }

  fprintf(stream, "}\n");
  fclose(stream);

  return status;
}

static status_t
_PM_genModules(
  Option_t *option,  /* i */
  Config_t *config,  /* i */
  Base_t *base,  /* i */
  char ****mss  /* i */
){
  status_t status;
  char path[PATH_MAX + 1];
  int i;

  PCMsg("gen modules");

  snprintf(path, sizeof(path), "%s", config->modules_dir);

  if (mkdir(path, 0755) != 0)
    {
      if (errno != EEXIST)
        {
          PCMsg_err("mkdir - %s", path);

          return ABNORMAL;
        }
    }

  snprintf(path, sizeof(path), "%s/nogen", config->modules_dir);

  if (mkdir(path, 0755) != 0)
    {
      if (errno != EEXIST)
        {
          PCMsg_err("mkdir - %s", path);

          return ABNORMAL;
        }
    }

  snprintf(path, sizeof(path), "%s/gen", config->modules_dir);

  if (mkdir(path, 0755) != 0)
    {
      if (errno != EEXIST)
        {
          PCMsg_err("mkdir - %s", path);

          return ABNORMAL;
        }
    }

  for (i = 0; mss[i]; ++i)
    {
      if (option->module[0])
        {
          if (strcmp(option->module, mss[i][0][1]) != 0) { continue; }
        }

      status = _PM_genModule(option, config, base, mss[i]);
      if (status) { return status; }
    }

  return NORMAL;
}

static status_t
_PM_genManifests(
  Option_t *option,  /* i */
  Config_t *config,  /* i */
  Base_t *base  /* i */
){
  status_t status;
  char ****mss;

  mss = PM_loadModules(option, config, base->modules);
  if (! mss) { return ABNORMAL; }

  if (! option->module[0])
    {
      status = _PM_genSite(option, config, base);
      if (status) { PPPP_free(mss); return status; }
    }

  status = _PM_genModules(option, config, base, mss);
  if (status) { PPPP_free(mss); return status; }

  PPPP_free(mss);

  return NORMAL;
}

status_t
PM_gen(
  Option_t *option,  /* I */
  Config_t *config,  /* I */
  Base_t *base  /* I */
){
  status_t status;

  if (! option->module[0])
    {
      status = _PM_genPuppetConfs(option, config, base);
      if (status) { return status; }

      status = _PM_genAutosignConf(option, config, base);
      if (status) { return status; }
    }

  if (! option->env[0])
    {
      status = _PM_genManifests(option, config, base);
      if (status) { return status; }
    }

  return NORMAL;
}
