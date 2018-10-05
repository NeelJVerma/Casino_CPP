#include <cstdlib>
#include "app.h"

int main() {
  srand(time(NULL));
  std::shared_ptr<App> app(new App);
  app->Start();

  return 0;
}