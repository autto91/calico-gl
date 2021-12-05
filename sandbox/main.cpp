#include "calico.h"

int main(int argc, char *argv[]) {
  const auto app = new calico::application("Test Application", 800, 600);
  app->run();

  delete app;

  return 0;
}
