/*******************************
 * Name: Neel Verma            *
 * Project: Casino CPP         *
 * Class: CMPS 366-01          *
 * Due Date: 10/2/18           *
 *******************************/

#ifndef _INPUT_HANDLER_H_
#define _INPUT_HANDLER_H_

#include <string>

class InputHandler {
 public:
  // Public utils
  static std::string GetCardInput();
  static unsigned GetActionInput();
  static unsigned GetBuildInput();
  static unsigned GetMultiBuildInput();
  static unsigned GetCaptureInput();
  static std::string GetDiffCardInput();
  static std::vector<std::string> GetBuildCardsInput();
  static std::vector<std::string> GetBuildAddOnInput();
  static std::vector<std::string> GetCaptureSetInput();
  static std::vector<std::string> GetCaptureBuildInput();
  static unsigned GetMenuInput();
  static unsigned GetDeckInput();
  static std::string GetFileInput();
  static unsigned GetCoinInput();
  static unsigned GetLoadChoiceInput();

 private:
  // Private utils
  static std::string GetStringInput();
};

#endif