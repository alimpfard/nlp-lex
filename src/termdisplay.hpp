#pragma once

#include <memory>
#include <stdarg.h>
#include <string.h>
#include <string>
#include <sys/ioctl.h> //ioctl() and TIOCGWINSZ
#include <utility>

namespace Display {
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

  void display(std::string str) {
    return;
    refresh_size();

    findAndReplaceAll(str, "{<clean>}", "\033[0m");
    findAndReplaceAll(str, "{<green>}", "\033[32m");
    findAndReplaceAll(str, "{<red>}", "\033[31m");
    findAndReplaceAll(str, "{<magenta>}", "\033[35m");

    std::printf("\033[11;1H\033[J%s\n", str.c_str());
    dirty = false;
  }

  template <typename... Args> void show(std::string fmt, Args... a) {
    display(Display::string_format(fmt, a...));
  }
};
} // namespace Display
