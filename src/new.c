#include "resig/signal.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

signal_value_t default_value(signal_type_t type) {
  switch (type) {
  case SIGNAL_INT:
    return (signal_value_t){.i = 0};
  case SIGNAL_FLOAT:
    return (signal_value_t){.f = 0};
  case SIGNAL_UINT:
    return (signal_value_t){.ui = 0};
  case SIGNAL_STR:
    return (signal_value_t){.str = ""};
  case SIGNAL_ULONG:
    return (signal_value_t){.ul = 0};
  }
}

signal_t *new_signal(signal_type_t type) {
  signal_t *signal = malloc(sizeof(signal_t));
  signal->value = default_value(type);
  signal->type = type;

  signal->dependent_count = 0;

  for (size_t i = 0; i < SIGNAL_MAX_DEPENDENTS; i++) {
    signal->dependents[i] = NULL;
  }

  return signal;
}

computed_signal_t *new_computed_signal(signal_type_t type,
                                       computed_signal_func_t func,
                                       signal_t **depends_on,
                                       int dependency_count) {

  computed_signal_t *comp_signal = malloc(sizeof(computed_signal_t));
  comp_signal->type = type;
  comp_signal->effect_count = 0;

  for (size_t i = 0; i < SIGNAL_MAX_EFFECTS; i++) {
    comp_signal->effects[i] = NULL;
  }

  for (size_t i = 0; i < (size_t)dependency_count; i++) {
    if (depends_on[i]) {
      signal_t *dep = depends_on[i];
      if (dep->dependent_count < SIGNAL_MAX_DEPENDENTS) {
        dep->dependents[dep->dependent_count] = comp_signal;
        dep->dependent_count++;
      } else {
        printf("Error: too many dependents (%zu)\n", dep->dependent_count);
        exit(1);
      }
    }
  }

  comp_signal->func = func;
  comp_signal->value = comp_signal->func();

  return comp_signal;
}

signal_value_t new_value_int(int i) { return (signal_value_t){.i = i}; }
signal_value_t new_value_float(float f) { return (signal_value_t){.f = f}; }

signal_value_t new_value_str(const char *str, size_t len) {
  if (len >= SIGNAL_STR_MAX) {
    fprintf(stderr, "Error: string value too long\n");
    exit(1);
  }

  char *value_str = malloc(len + 1);
  if (!value_str) {
    fprintf(stderr, "Error: malloc failed\n");
    exit(1);
  }

  strncpy(value_str, str, len);
  value_str[len] = '\0';

  return (signal_value_t){.str = value_str};
}

signal_value_t new_value_ulong(ulong ul) { return (signal_value_t){.ul = ul}; }

void effect(computed_signal_t *comp_signal, effect_t effect_func) {
  comp_signal->effects[comp_signal->effect_count] = effect_func;
  comp_signal->effect_count++;
}
