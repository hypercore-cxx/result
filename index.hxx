#include <variant>
#include <string>
#include <iostream>

namespace Hypercore {
  class Error {
    public:
      std::string message;
      size_t code;
      Error (const std::string& s) : message(s) {};
  };

  template<typename T, class E = Error>
  class Result {
    bool hasError;
    bool initialized = false;
    bool checked = false;
    T value;

    public:
      std::string type;
      E error;

      operator bool() {
        this->checked = true;
        return !this->hasError;
      }

      Result& operator= (const T& v) {
        this->initialized = true;
        this->value = v;
        this->hasError = false;
        return *this;
      }

      Result& operator= (const E& e) {
        this->initialized = true;
        this->error = e;
        this->hasError = true;
        return *this;
      }

      bool empty() {
        return this->initialized;
      }

      T unwrap() {
        if (!this->checked) {
          throw std::runtime_error(".unwrap() called before check");
        }

        return this->value;
      }

      Result () : error("") {};

      Result (const E& e) : error(e) {
        this->initialized = true;
        this->hasError = true;
        this->type = typeid(E).name();
      };

      Result (const T& value) : error("") {
        this->initialized = true;
        this->hasError = false;
        this->value = value;
      };
  };
} // namespace Hypercore
