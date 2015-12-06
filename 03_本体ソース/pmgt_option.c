#include "pmgt.h"

static status_t
PO_check(
  Option_t *option  /* i */
){
  if (option->env[0] && option->module[0])
    {
      PCMsg_err("オプション「--env」と「--module」は同時に指定できません");

      return ABNORMAL;
    }

  return NORMAL;
}

status_t
PO_get(
  Option_t *option,  /* i */
  int *max,  /* io */
  char **params  /* io */
){
  char *name;
  char *value = NULL;  /* lint 対策 */
  int i;

  memset(option, 0x00, sizeof(*option));

  name = "--config";
  if (Option_get(max, params, name, &value))
    {
      snprintf(option->config, sizeof(option->config), "%s", value);
    }

  name = "--env";
  if (Option_get(max, params, name, &value))
    {
      snprintf(option->env, sizeof(option->env), "%s", value);
    }

  name = "--module";
  if (Option_get(max, params, name, &value))
    {
      snprintf(option->module, sizeof(option->module), "%s", value);
    }

  if (Option_exist(*max, params))
    {
      for (i = 0; i < *max; ++i)
        {
          if (params[i][0] != '-')
            {
              continue;
            }

          PCMsg_err("不明なパラメータがあります - %s", params[i]);
        }

      return ABNORMAL;
    }

  return PO_check(option);
}
