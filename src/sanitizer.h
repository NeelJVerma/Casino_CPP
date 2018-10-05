#ifndef _SANITIZER_H_
#define _SANITIZER_H_

#include <string>
#include "card.h"
#include <vector>

class Sanitizer {
 public:
  // Public utils
  static bool MenuChoiceValid(const std::string& choice);
  static bool CardChoiceValid(const std::string& choice);
  static bool BuildOptionValid(const std::string& option);
  static bool ActionChoiceValid(const std::string& action);
  static bool CoinChoiceValid(const std::string& choice);
  static bool CardsValid(const std::string& choice,
      std::vector<std::string>& cards);
      
  static bool CaptureChoiceValid(const std::string& choice);
  static bool FileValid(const std::string& file_name);
  static bool DeckChoiceValid(const std::string& choice);
  static bool LoadChoiceValid(const std::string& choice);
  static std::vector<std::string> TokenizeInput(const std::string& input);

 private:
  // Private utils
  static bool CardValid(const std::string& choice);
};

#endif