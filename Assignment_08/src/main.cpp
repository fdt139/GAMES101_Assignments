#include "CGL/CGL.h"
#include "CGL/viewer.h"

#include "application.h"

#include <iostream>

using namespace std;
using namespace CGL;

void usage(const char *binaryName) {
  printf("Usage: %s [options] <scenefile>\n", binaryName);
  printf("Program Options:\n");
  printf("  -m  <FLOAT>            Mass per node\n");
  printf("  -g  <FLOAT> <FLOAT>    Gravity vector (x, y)\n");
  printf("  -s  <INT>              Number of steps per simulation frame\n");
  printf("\n");
}

int main(int argc, char **argv) {
  AppConfig config;

  // Simple portable argument parsing (replaces POSIX getopt for Windows builds).
  for (int i = 1; i < argc; ++i) {
    string arg = argv[i];
    if (arg == "-m" && i + 1 < argc) {
      config.mass = (float)atof(argv[++i]);
    } else if (arg == "-g" && i + 2 < argc) {
      config.gravity = Vector2D((float)atof(argv[++i]), (float)atof(argv[++i]));
    } else if (arg == "-s" && i + 1 < argc) {
      config.steps_per_frame = atoi(argv[++i]);
    } else if (arg == "-h" || arg == "--help") {
      usage(argv[0]);
      return 0;
    }
  }

  // create application
  Application *app = new Application(config);

  // create viewer
  Viewer viewer = Viewer();

  // set renderer
  viewer.set_renderer(app);

  // init viewer
  viewer.init();

  // start viewer
  viewer.start();

  return 0;
}
