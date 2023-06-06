#include <exception>
#include <typeindex>
#include <string>
#include <sstream>

class InvalidTypeException : public std::exception {
public:
  InvalidTypeException(const std::type_index& expected, const std::type_index& given) {
    std::stringstream stream;
    stream << "Expected " << expected.name() << ", got " << given.name() << ".";
    _msg = stream.str();
  }

  const char* what() const throw() {
    return _msg.c_str();
  }
private:
  std::string _msg;
};
