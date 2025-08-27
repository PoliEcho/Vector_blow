#include <SDL3/SDL_stdinc.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

std::string get_filename() {
  std::string path = std::getenv("HOME");
  if (path.empty()) {
    std::cerr << "HOME environment variable not set\n";
    std::exit(1);
  }
  path.append("/.local/share/Vector_blow");
  std::filesystem::create_directories(path);
  path.append("/highscore");
  return path;
}

Uint64 get_highscore() {
  std::ifstream highscore_file(get_filename());
  if (!highscore_file.is_open()) {
    std::cerr << "failed to open file\n";
    std::exit(78);
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
  highscore_file << highscore;
  highscore_file.close();
}