#include "Application.hpp"
#include <iostream>

int main(int argc, const char **argv) {

  lg::Application app;

  if(!(app.init())) {
    std::cout << "Initialisation failed - exiting..." << std::endl;
    return 1;
  }

  app.run();
  return 0;
}
