#include "resig/signal.h"
#include <stdint.h>
#include <stdlib.h>

void free_signal(signal_t *signal) {
  if (signal == NULL)
    return;

  if (signal->type == SIGNAL_STR && signal->value.str)
    free(signal->value.str);
  free(signal);
}

void free_computed_signal(computed_signal_t *signal) {
  if (signal == NULL)
    return;

  if (signal->type == SIGNAL_STR && signal->value.str)
    free(signal->value.str);
  free(signal);
}
