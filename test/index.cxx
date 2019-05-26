#include "../index.hxx"
#include "../deps/heapwolf/cxx-tap/index.hxx"

#include <iostream>

using Error = Hyper::Util::Error;

template<typename T>
  using Result = Hyper::Util::Result<T>;

int main() {
  TAP::Test t;

  t.test("sanity", [](auto t) {
    t->ok(true, "true is true");
    t->end();
  });

  t.test("unwrap", [](auto t) {
    Result<int> res(100);

    if (res) {
      t->equal(res.unwrap(), 100, "unwrapped");
    }

    t->end();
  });

  t.test("contains nothing", [](auto t) {
    Result<int> res;

    t->ok(!res.empty(), "result does not contain a value or error");
    t->end();
  });

  t.test("unchecked unwrap", [](auto t) {
    Result<int> res(100);

    try {
      t->equal(res.unwrap(), 100);
    } catch (const std::runtime_error& err) {
      const std::string expected = ".unwrap() called before check";
      std::string actual = static_cast<std::string>(err.what());
      t->equal(expected, actual, "a runtime error was thrown");
    }

    t->end();
  });

  t.test("error", [](auto t) {
    const std::string message = "oops, something went wrong";

    Error error(message);
    Result<int> res(error);

    if (res) {
      t->fail("constructed with error but bool operator tested true");
    }

    if (!res) {
      t->equal(message, res.error.message, "error messages match");
    }

    t->end();
  });

  t.test("either", [](auto t) {
    auto foo = [](size_t n) {
      Result<int> res;

      if (n < 50) {
        res = n;
      } else {
        res = Error("oops");
      }

      return res;
    };

    auto f = foo(25);

    if (f) {
      t->equal(f.unwrap(), 25);
    }

    auto g = foo(75);

    if (!g) {
      t->equal(g.error.message, "oops");
    }

    t->end();
  });
}
