#if ! defined(_PMGT_H)
#define _PMGT_H 1

#include "pmgt_common.h"

#define MAX_LINE_BYTES (1024 * 10)
#define MAX_ENVS 100
#define MAX_ENV_BYTES 20
#define MAX_MODULE_BYTES 50

typedef struct
{
  char config[PATH_MAX + 1];
  char env[MAX_ENV_BYTES + 1];
  char module[MAX_MODULE_BYTES + 1];
} Option_t;

typedef struct
{
  char ***config;
  char ***brackets;

  char *puppet_conf_dir;
  char *manifests_dir;
  char *modules_dir;
  char *params_dir;
  char *params_envs;
  char *params_hosts;
  char *params_modules;
  char *params_consts;
  char *config_dir;
  char *config_brackets;
  char *templates_dir;
  char *templates_site_header;
  char *hosts_dir;
  char *hostname_max_bytes;
  char *consts_dir;
  char *test_dir;
} Config_t;

typedef struct
{
  char ***envs;
  char ***hosts;
  char ***modules;
  char ***consts;
} Base_t;

#include "pmgt_option.h"
#include "pmgt_hosts.h"
#include "pmgt_consts.h"
#include "pmgt_test.h"
#include "pmgt_gen.h"

#endif  /* _PMGT_H */
