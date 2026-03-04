# Reactive signals

[Skip to build instructions](README#Build).

Reactive signals allows you to declare _signals_, which can then be dependencies
for _computed signals_ which recompute their value each time one dependency
updates it's state.

For example, imagine you have this formula:

```c
y = x * 7;
```

In C you normally need to update the value of `y` manually everytime `x` changes:

```c
int x = 3;
int y = x * 7;
x = 5;
y = x * 7;
```

Which quickly becomes tedious.

With signals, you turn `x` into a signal and `y` into a computed signal and
Reactive Signals does the work for you:

```c
signal_t* x;

signal_value_t compute(void) {
    return new_value_int(x->value.i * 7); // Calculate `x * 7`
}

int main(void) {
    x = new_signal(SIGNAL_INT); // Initialize `x` as a signal with an integer value.
    set_signal_int(x, 3); // Set the initial value of `x` to 3

  // Note that this isn't the only way of declaring the dependency array.
  // You just need to declare a valid array of signal_t and pass it, how
  // you do doesn't matter.
  signal_t *dependencies[SIGNAL_MAX_DEPENDENTS] = {};
  dependencies[0] = x;
  computed_signal_t *y =
      new_computed_signal(SIGNAL_INT, compute, dependencies, 1);
      // Signal type, Compute function, dependencies, dependency count

  set_signal_int(x, 5); // x = 5; y = 5 * 7; (computes automatically)

  // Avoid memory leaks
  free_signal(x);
  free_computed_signal(y);
  return 0;
}
```

This might seem overkill, but through all the codebase you won't ever need to
recompute Y manually.

Also, computed signals can have various dependencies, meaning that values you
would need to manually change everytime which depend on other 3 automatically
recompute when one changes.

Have this formula?

```c
b = x / 9 * (y + z) - a
```

No problem, as reactive signals can automatically update `b` everytime `x`, `y`,
`z` or `a` change:

```c
signal_t* x;
signal_t* y;
signal_t* z;
signal_t* a;

signal_value_t compute_b() {
    return new_value_int(x->value.i / 9 * (y->value.i + z->value.i) - a->value.i);
}

int main(void) {
    // All of these default to 0
    x = new_signal(SIGNAL_INT);
    y = new_signal(SIGNAL_INT);
    z = new_signal(SIGNAL_INT);
    a = new_signal(SIGNAL_INT);

    computed_signal_t b =
        new_computed_signal(SIGNAL_INT, compute_b, (signal_t**){x, y, z, a}, 4);
}
```

And now `b` will recompute everytime any of the other signals updates.

## Build

Using `make` automatically builds both static and shared library files at `lib`.

Note that Reactive Signals does not support Windows, only Linux (and MacOS probably,
although not tested).
