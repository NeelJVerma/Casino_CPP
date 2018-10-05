#ifndef _APP_H_
#define _APP_H_

#include <fstream>
#include "tournament.h"

class App {
 public:
  // Delete copy constructor and assignment operator
  App(const App& app) = delete;
  App& operator=(const App& app) = delete;

  // Constructor
  App() = default;

  // Public utils
  void Start();

 private:
  // Private enums
  enum LoadOption {
    kLoad = 1,
    kNew
  };

  std::shared_ptr<Tournament> tournament_;

  // Private utils
  void SetPlayerDataFromFile(std::ifstream& in_file);
};

#endif