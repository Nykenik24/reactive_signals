#include "resig/signal.h"
#include <stdio.h>

signal_t *x;

signal_value_t compute_mul7(void) {
  signal_value_t value;
  value.i = x->value.i * 7;
  return value;
}

void print_sig_value(computed_signal_t *comp_signal) {
  if (comp_signal->type == SIGNAL_INT) {
    printf("Computed signal's value: %d\n", comp_signal->value.i);
  }
}

int main(void) {
  printf("Reactive signals demo\n");

  x = new_signal(SIGNAL_INT);
  set_signal_int(x, 3);

  signal_t *dependencies[SIGNAL_MAX_DEPENDENTS] = {};
  dependencies[0] = x;
  computed_signal_t *y =
      new_computed_signal(SIGNAL_INT, compute_mul7, dependencies, 1);
  printf("Y's initial value: %d\n", y->value.i);
  effect(y, print_sig_value);

  set_signal_int(x, 5);
  set_signal_int(x, 8);

  free_signal(x);
  free_computed_signal(y);
  return 0;
}
