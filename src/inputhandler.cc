/*******************************
 * Name: Neel Verma            *
 * Project: Casino CPP         *
 * Class: CMPS 366-01          *
 * Due Date: 10/2/18           *
 *******************************/

#include <iostream>
#include "gui.h"
#include "sanitizer.h"
#include "inputhandler.h"
#include "serializer.h"

/**
 * Description: Gets a string from stdin.
 * Parameters: None.
 * Returns: The entered string.
 */

std::string InputHandler::GetStringInput() {
  std::string choice;
  getline(std::cin, choice);

  return choice;
}

/**
 * Description: Gets a chosen card from stdin.
 * Parameters: None.
 * Returns: The card chosen.
 */

std::string InputHandler::GetCardInput() {
  std::string choice;

  while (true) {
    GUI::DisplayCardPlayMessage();
    choice = InputHandler::GetStringInput();

    if (Sanitizer::CardChoiceValid(choice)) {
      break;
    }

    GUI::DisplayInvalidChoiceMessage();
  }

  return choice;
}

/**
 * Description: Gets a chosen action from stdin.
 * Parameters: None.
 * Returns: The action chosen.
 */

unsigned InputHandler::GetActionInput() {
  std::string choice;

  while (true) {
    GUI::DisplayActionMenu();
    choice = InputHandler::GetStringInput();

    if (Sanitizer::ActionChoiceValid(choice)) {
      break;
    }

    GUI::DisplayInvalidChoiceMessage();
  }

  return Serializer::GetIntFromString(choice);
}

/**
 * Description: Gets a chosen build directive from stdin.
 * Parameters: None.
 * Returns: The directive chosen.
 */

unsigned InputHandler::GetBuildInput() {
  std::string choice;

  while (true) {
    GUI::DisplayBuildMenu();
    choice = InputHandler::GetStringInput();

    if (Sanitizer::BuildOptionValid(choice)) {
      break;
    }

    GUI::DisplayInvalidChoiceMessage();
  }

  return Serializer::GetIntFromString(choice);
}

/**
 * Description: Gets a chosen capture directive from stdin.
 * Parameters: None.
 * Returns: The directive chosen.
 */

unsigned InputHandler::GetCaptureInput() {
  std::string choice;

  while (true) {
    GUI::DisplayCaptureMenu();
    choice = InputHandler::GetStringInput();

    if (Sanitizer::CaptureChoiceValid(choice)) {
      break;
    }

    GUI::DisplayInvalidChoiceMessage();
  }

  return Serializer::GetIntFromString(choice);
}

/**
 * Description: Gets a chosen build from stdin.
 * Parameters: None.
 * Returns: The build chosen.
 */

std::vector<std::string> InputHandler::GetBuildCardsInput() {
  std::string choice;
  std::vector<std::string> str_cards;

  while (true) {
    GUI::DisplayBuildCardsMessage();
    choice = InputHandler::GetStringInput();

    if (Sanitizer::CardsValid(choice, str_cards)) {
      break;
    }

    GUI::DisplayInvalidChoiceMessage();
  }

  return str_cards;
}

/**
 * Description: Gets a chosen build from stdin.
 * Parameters: None.
 * Returns: The build chosen.
 */

std::vector<std::string> InputHandler::GetBuildAddOnInput() {
  std::string choice;
  std::vector<std::string> str_cards;

  while (true) {
    GUI::DisplayBuildAddOnMessage();
    choice = InputHandler::GetStringInput();

    if (Sanitizer::CardsValid(choice, str_cards)) {
      if (!str_cards.empty()) {
        break;
      }
    }

    GUI::DisplayInvalidChoiceMessage();
  }

  return str_cards;
}

/**
 * Description: Gets a chosen set from stdin.
 * Parameters: None.
 * Returns: The set chosen.
 */

std::vector<std::string> InputHandler::GetCaptureSetInput() {
  std::string choice;
  std::vector<std::string> str_cards;

  while (true) {
    GUI::DisplayCaptureSetMessage();
    choice = InputHandler::GetStringInput();

    if (Sanitizer::CardsValid(choice, str_cards)) {
      if (!str_cards.empty()) {
        break;
      }
    }

    GUI::DisplayInvalidChoiceMessage();
  }

  return str_cards;
}

/**
 * Description: Gets a chosen build from stdin.
 * Parameters: None.
 * Returns: The build chosen.
 */

std::vector<std::string> InputHandler::GetCaptureBuildInput() {
  std::string choice;
  std::vector<std::string> str_cards;

  while (true) {
    GUI::DisplayCaptureBuildMessage();
    choice = InputHandler::GetStringInput();

    if (Sanitizer::CardsValid(choice, str_cards)) {
      if (!str_cards.empty()) {
        break;
      }
    }

    GUI::DisplayInvalidChoiceMessage();
  }

  return str_cards;
}

/**
 * Description: Gets a chosen card from stdin.
 * Parameters: None.
 * Returns: The card chosen.
 */

std::string InputHandler::GetDiffCardInput() {
  GUI::DisplayPickDifferentCardMessage();

  return InputHandler::GetStringInput();
}

/**
 * Description: Gets a chosen menu directive from stdin.
 * Parameters: None.
 * Returns: The directive chosen.
 */

unsigned InputHandler::GetMenuInput() {
  std::string choice;

  while (true) {
    GUI::DisplayMenu();
    choice = InputHandler::GetStringInput();

    if (Sanitizer::MenuChoiceValid(choice)) {
      break;
    }

    GUI::DisplayInvalidChoiceMessage();
  }

  return Serializer::GetIntFromString(choice);
}

/**
 * Description: Gets a file name from stdin.
 * Parameters: None.
 * Returns: The file name chosen.
 */

std::string InputHandler::GetFileInput() {
  std::string file_name;

  while (true) {
    GUI::DisplayFileMessage();
    file_name = InputHandler::GetStringInput();

    if (Sanitizer::FileValid(file_name)) {
      break;
    }

    GUI::DisplayInvalidChoiceMessage();
  }

  return file_name;
}

/**
 * Description: Gets a load deck directive from stdin.
 * Parameters: None.
 * Returns: The directive chosen.
 */

unsigned InputHandler::GetDeckInput() {
  std::string choice;

  while (true) {
    GUI::DisplayDeckMenu();
    choice = InputHandler::GetStringInput();

    if (Sanitizer::DeckChoiceValid(choice)) {
      break;
    }

    GUI::DisplayInvalidChoiceMessage();
  }

  return Serializer::GetIntFromString(choice);
}

/**
 * Description: Gets a chosen coin flip from stdin.
 * Parameters: None.
 * Returns: The coin chosen.
 */

unsigned InputHandler::GetCoinInput() {
  std::string choice;

  while (true) {
    GUI::DisplayCoinMessage();
    choice = InputHandler::GetStringInput();

    if (Sanitizer::CoinChoiceValid(choice)) {
      break;
    }

    GUI::DisplayInvalidChoiceMessage();
  }

  return Serializer::GetIntFromString(choice);
}

/**
 * Description: Gets a load directive from stdin.
 * Parameters: None.
 * Returns: The directive chosen.
 */

unsigned InputHandler::GetLoadChoiceInput() {
  std::string choice;

  while (true) {
    GUI::DisplayLoadMenu();
    choice = InputHandler::GetStringInput();

    if (Sanitizer::LoadChoiceValid(choice)) {
      break;
    }

    GUI::DisplayInvalidChoiceMessage();
  }

  return Serializer::GetIntFromString(choice);
}