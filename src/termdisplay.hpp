#pragma once

#include <memory>
#include <stdarg.h>
#include <string.h>
#include <string>
#include <sys/ioctl.h> //ioctl() and TIOCGWINSZ
#include <utility>

namespace Display {
enum class Type : int {
  MUST_SHOW = 0,
  ERROR = 1,
  INFO = 2,
  LOG = 3,
  VERBOSE = 4,
  WARNING = 5,
  DEBUG = 6,
  EVERYTHING = 7,
};
static char *Typename[] = {
    [(int)Type::MUST_SHOW] = "MUST_SHOW",
    [(int)Type::ERROR] = "ERROR",
    [(int)Type::INFO] = "INFO",
    [(int)Type::LOG] = "LOG",
    [(int)Type::VERBOSE] = "VERBOSE",
    [(int)Type::WARNING] = "WARNING",
    [(int)Type::DEBUG] = "DEBUG",
    [(int)Type::EVERYTHING] = "EVERYTHING",
};

const static inline std::string string_format(const std::string fmt_str, ...) {
  int final_n,
      n = ((int)fmt_str.size()) *
          2; /* Reserve two times as much as the length of the fmt_str */
  std::unique_ptr<char[]> formatted;
  va_list ap;
  while (1) {
    formatted.reset(new char[n]); /* Wrap the plain char array into the
                                         unique_ptr */
    strcpy(&formatted[0], fmt_str.c_str());
    va_start(ap, fmt_str);
    final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
    va_end(ap);
    if (final_n < 0 || final_n >= n)
      n += abs(final_n - n + 1);
    else
      break;
  }
  return std::string(formatted.get());
}
std::string findAndReplaceAllT(std::string data, std::string toSearch,
                               std::string replaceStr) {
  auto pos = data.find(toSearch);

  while (pos != std::string::npos) {
    data.replace(pos, toSearch.size(), replaceStr);
    pos = data.find(toSearch, pos + replaceStr.size());
  }
  return data;
}
void findAndReplaceAll(std::string &data, std::string toSearch,
                       std::string replaceStr) {
  auto pos = data.find(toSearch);

  while (pos != std::string::npos) {
    data.replace(pos, toSearch.size(), replaceStr);
    pos = data.find(toSearch, pos + replaceStr.size());
  }
}
class SingleLineTermStatus {
  struct winsize tsize, oldtsize;
  struct Position {
    int x;
    int y;
  } sposition, eposition;

public:
  Type min_req = Type::INFO;

  bool dirty = false;
  void refresh_size() {
    oldtsize = tsize;
    ioctl(1, TIOCGWINSZ, &tsize);
    if (!dirty && oldtsize.ws_col != tsize.ws_col)
      // resized to a different width redraw everything
      dirty = true;
  }

public:
  SingleLineTermStatus() {
    dirty = true;
    refresh_size();
  }

  void display(std::string str, bool cleanup) {
    if (cleanup)
      return;
    refresh_size();

    findAndReplaceAll(str, "{<clean>}", "\033[0m");
    findAndReplaceAll(str, "{<green>}", "\033[32m");
    findAndReplaceAll(str, "{<red>}", "\033[31m");
    findAndReplaceAll(str, "{<magenta>}", "\033[35m");

    std::printf(
        (std::string(cleanup ? "\033[11;1H\033[J" : "") + "%s\n").c_str(),
        str.c_str());
    dirty = false;
  }

  template <typename... Args>
  void show(const Type type, std::string fmt, Args... a) {
    if (type > min_req)
      return;
    display(Display::string_format("[{<green>}%s{<clean>}] %s",
                                   Typename[(int)type],
                                   Display::string_format(fmt, a...).c_str()),
            false);
  }
  template <typename... Args>
  void show_c(const Type type, std::string fmt, Args... a) {
    if (type > min_req)
      return;
    display(Display::string_format("[{<green>}%s{<clean>}] %s",
                                   Typename[(int)type],
                                   Display::string_format(fmt, a...).c_str()),
            false);
  }
};
} // namespace Display
