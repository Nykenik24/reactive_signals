#ifndef _SIGNAL_H
#define _SIGNAL_H

#include <stddef.h>
#include <sys/types.h>

#define SIGNAL_STR_MAX 8192
#define SIGNAL_MAX_DEPENDENTS 256
#define SIGNAL_MAX_EFFECTS 256

typedef enum {
  SIGNAL_INT,
  SIGNAL_FLOAT,
  SIGNAL_UINT,
  SIGNAL_STR,
  SIGNAL_ULONG
} signal_type_t;

typedef union {
  int i;
  float f;
  uint ui;
  char *str;
  ulong ul;
} signal_value_t;

typedef signal_value_t (*computed_signal_func_t)(void);

struct computed_signal {
  signal_type_t type;
  signal_value_t value;
  computed_signal_func_t func;
  void (*effects[SIGNAL_MAX_EFFECTS])(struct computed_signal *);
  size_t effect_count;
};

typedef struct computed_signal computed_signal_t;
typedef void (*effect_t)(computed_signal_t *);

struct signal {
  signal_type_t type;
  signal_value_t value;
  computed_signal_t *dependents[SIGNAL_MAX_DEPENDENTS];
  size_t dependent_count;
};

typedef struct signal signal_t;

signal_t *new_signal(signal_type_t type);
computed_signal_t *new_computed_signal(signal_type_t type,
                                       computed_signal_func_t func,
                                       signal_t **depends_on,
                                       int dependency_count);

signal_value_t new_value_int(int i);
signal_value_t new_value_float(float f);
signal_value_t new_value_uint(uint ui);
signal_value_t new_value_str(const char *str, size_t len);
signal_value_t new_value_ulong(ulong ul);

void free_signal(signal_t *signal);
void free_computed_signal(computed_signal_t *signal);

void set_signal_int(signal_t *signal, int value);
void set_signal_float(signal_t *signal, float value);
void set_signal_uint(signal_t *signal, uint value);
void set_signal_str(signal_t *signal, const char *value, size_t value_len);
void set_signal_ulong(signal_t *signal, ulong value);

void effect(computed_signal_t *comp_signal, effect_t effect_func);

#endif
