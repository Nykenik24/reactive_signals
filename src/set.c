#include "resig/signal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void update_dependents(signal_t *signal) {
  for (size_t i = 0; i < signal->dependent_count; i++) {
    computed_signal_t *dependent = signal->dependents[i];
    if (dependent) {
      dependent->value = dependent->func();
      for (size_t i = 0; i < dependent->effect_count; i++) {
        dependent->effects[i](dependent);
      }
    }
  }
}

void set_signal_int(signal_t *signal, int value) {
  signal->value.i = value;
  update_dependents(signal);
}

void set_signal_float(signal_t *signal, float value) {
  signal->value.f = value;
  update_dependents(signal);
}

void set_signal_uint(signal_t *signal, uint value) {
  signal->value.ui = value;
  update_dependents(signal);
}

void set_signal_str(signal_t *signal, const char *value, size_t value_len) {
  if (value_len >= SIGNAL_STR_MAX) {
    fprintf(stderr, "Error: string value too long\n");
    exit(1);
  }

  if (signal->value.str) {
    free(signal->value.str);
  }

  signal->value.str = malloc(value_len + 1);
  if (!signal->value.str) {
    fprintf(stderr, "Error: string malloc failed\n");
    exit(1);
  }

  strncpy(signal->value.str, value, value_len);
  signal->value.str[value_len] = '\0';

  update_dependents(signal);
}

void set_signal_ulong(signal_t *signal, ulong value) {
  signal->value.ul = value;
  update_dependents(signal);
}
