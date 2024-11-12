#include "GameApp.hpp"

#include <iostream>

int main(int argc, const char **argv) {

  GameApp app("LearnGL", 800, 800);

  if (!app.Init()) {
    std::cout << "GameApp: failed to initialise" << std::endl;
    return 1;
  }

  app.Run();

  return 0;
}
