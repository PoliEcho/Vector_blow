#include <SDL3/SDL_stdinc.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

std::string get_filename() {
#ifdef _WIN32
  std::string path = std::getenv("USERPROFILE");
  if (path.empty()) {
    std::cerr << "USERPROFILE environment variable not set\n";
    std::exit(1);
  }
  path.append("/AppData/Local/Vector_blow");
#else
  std::string path = std::getenv("HOME");
  if (path.empty()) {
    std::cerr << "HOME environment variable not set\n";
    return "";
  }
  path.append("/.local/share/Vector_blow");
#endif
  std::filesystem::create_directories(path);
  path.append("/highscore");
  return path;
}

Uint64 get_highscore() {
  std::ifstream highscore_file(get_filename());
  if (!highscore_file.is_open()) {
    std::cerr << "failed to get highscore\n";
    return 0;
  }

  std::string highscore_str((std::istreambuf_iterator<char>(highscore_file)),
                            std::istreambuf_iterator<char>());
  highscore_file.close();
  try {

    return std::stoull(highscore_str);
  } catch (...) {
    return 0;
  }
}

void set_highscore(Uint64 highscore) {
  std::ofstream highscore_file(get_filename());
  if (!highscore_file.is_open()) {
    std::cerr << "failed to set highscore\n";
    return;
  }
  highscore_file << highscore;
  highscore_file.close();
}