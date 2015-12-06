#if ! defined(_PMGT_GEN_H)
#define _PMGT_GEN_H 1

char *
PM_a(
  char *attribute  /* i */
);

status_t
PM_loadConfig(
  Option_t *option,  /* i */
  Config_t *config  /* o */
);

status_t
PM_loadBase(
  Option_t *option,  /* i */
  Config_t *config,  /* i */
  Base_t *base  /* o */
);

status_t
PM_gen(
  Option_t *option,  /* I */
  Config_t *config,  /* I */
  Base_t *base  /* I */
);

char ****
PM_loadModules(
  Option_t *option,  /* i */
  Config_t *config,  /* i */
  char ***modules  /* i */
);

#endif  /* _PMGT_GEN_H */
