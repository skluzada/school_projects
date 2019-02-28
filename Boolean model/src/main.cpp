#include "WebApp.h"
#include <memory>

int main(int argc, char ** argv){
  return Wt::WRun(argc, argv, [](const Wt::WEnvironment & env){
    return std::make_unique<WebApp>(env);
  });
}
