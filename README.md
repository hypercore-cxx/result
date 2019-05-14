# SYNOPSIS
A container type that holds either an error or value.

# MOTIVATION
Provide a single type that can either test true and contain a value of type T
or test false and contain an error of type E that provides a message property.

This is not a replacement for exceptions but a complementary type for when
errors are not truly exceptional (ie, cases where something is not a surprise).

# EXAMPLE
Usually a function will make a decision and the result will be either an error
or a value. In this case, if the size of `n` is less than 50, the container is
returned with the new value and will test true. If `n` is less than 50 the
container is returned with an error and will test false.

```c++
auto foo = [](size_t n) {
  Hypercore::Result<int> res;

  if (n < 50) {
    res = n;
  } else {
    res = Hypercore::Error("oops");
  }

  return res;
};

auto f = foo(25);

//
// if `.upwrap()` is called before `f` is checked, an error will be thrown.
//
if (f) {
  // f.unwrap() == 25
}

auto g = foo(75);

if (!g) {
  // g.error.message == "oops"
}
```
