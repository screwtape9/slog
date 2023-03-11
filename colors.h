#ifndef __SLOG_COLORS_
#define __SLOG_COLORS_

#include <string>
#include <array>

enum class Color {
  black,  red,     green,  yellow,
  blue,   purple,  cyan,   white,
  // the following are the non-standard "bright" colors, hence, the prefixed 'b'
  bblack, bred,    bgreen, byellow,
  bblue,  bpurple, bcyan,  bwhite,
  num_colors
};

std::array<std::string, static_cast<int>(Color::num_colors)> colorNames = {
  "black",  "red",  "green",  "yellow",  "blue",  "purple",  "cyan",  "white",
  "bblack", "bred", "bgreen", "byellow", "bblue", "bpurple", "bcyan", "bwhite"
};

std::array<std::string, static_cast<int>(Color::num_colors)> colorCodes = {
  "\033[0;30m", "\033[0;31m", "\033[0;32m", "\033[0;33m",
  "\033[0;34m", "\033[0;35m", "\033[0;36m", "\033[0;37m",
  "\033[0;90m", "\033[0;91m", "\033[0;92m", "\033[0;93m",
  "\033[0;94m", "\033[0;95m", "\033[0;96m", "\033[0;97m"
};

#endif // __SLOG_COLORS_
