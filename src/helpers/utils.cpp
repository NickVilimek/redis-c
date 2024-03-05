#include <iostream>

void msg(const char *msg) {
  fprintf(stderr, "%s\n", msg);
}

void die(const char *msg) {
  int err = errno;
  fprintf(stderr, "[%d] %s\n", err, msg);
  abort();
}

void printMessage(const char *message) {
  std::cout << message << " " << std::endl;
}

void printMessages(const char **messages) {
  while (*messages != nullptr) {
    std::cout << *messages << " ";;
    ++messages;
  }
  std::cout << std:: endl;
}

void log(const char* cxt, const char *message) {
  std::cout << "[" << cxt << "] - " << message << " " << std::endl;
}
