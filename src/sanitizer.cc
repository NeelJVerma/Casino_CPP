#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <unordered_set>
#include "sanitizer.h"

// Static constants

static const char kSuits[Card::kNumSuits] = {'H', 'S', 'C', 'D'};
static const char kValues[Card::kNumValues] =
    {'A', '2', '3', '4', '5', '6', '7', '8', '9', 'X', 'J', 'Q', 'K'};
static const unsigned kNumMoveDirectives = 3;
static const unsigned kMaxMenuChoices = 4;
static const unsigned kNumBuildChoices = 3;
static const unsigned kNumCaptureChoices = 3;
static const unsigned kNumDeckChoices = 2;
static const unsigned kNumLoadChoices = 2;
static const std::string kMoveDirectives[kNumMoveDirectives] = {"1", "2", "3"};
static const std::string kMenuChoices[kMaxMenuChoices] = {"1", "2", "3", "4"};
static const std::string kBuildChoices[kNumBuildChoices] = {"1", "2", "3"};
static const std::string kCaptureChoices[kNumCaptureChoices] = {"1", "2", "3"};
static const std::string kDeckChoices[kNumDeckChoices] = {"1", "2"};
static const std::string kLoadChoices[kNumLoadChoices] = {"1", "2"};

/**
 * Description: Verifies if the menu directive is valid.
 * Parameters: const std::string& choice: The choice.
 * Returns: Whether or not it was valid.
 */

bool Sanitizer::MenuChoiceValid(const std::string& choice) {
  std::vector<std::string> tokens = TokenizeInput(choice);

  if (tokens.size() != 1) {
    return false;
  }

  for (unsigned i = 0; i < kMaxMenuChoices; i++) {
    if (tokens[0] == kMenuChoices[i]) {
      return true;
    }
  }

  return false;
}

/**
 * Description: Verifies if the card choice is valid.
 * Parameters: const std::string& choice: The choice.
 * Returns: Whether or not it was valid.
 */

bool Sanitizer::CardValid(const std::string& choice) {
  if (choice.length() != 2) {
    return false;
  }

  bool suit_valid = false;

  for (unsigned i = 0; i < Card::kNumSuits; i++) {
    if (choice[0] == kSuits[i]) {
      suit_valid = true;
    }
  }

  if (!suit_valid) {
    return false;
  }

  for (unsigned i = 0; i < Card::kNumValues; i++) {
    if (choice[1] == kValues[i]) {
      return true;
    }
  }

  return false;
}

/**
 * Description: Tokenizes input.
 * Parameters: const std::string& input: The input string.
 * Returns: The tokenized input.
 */

std::vector<std::string> Sanitizer::TokenizeInput(const std::string& input) {
  std::vector<std::string> tokens;
  std::istringstream stream(input);

  std::copy(
      std::istream_iterator<std::string>(stream),
      std::istream_iterator<std::string>(), std::back_inserter(tokens));

  return tokens;
}

/**
 * Description: Verifies if the card choice is valid
 *     (Generalized for many cards).
 * Parameters: const std::string& choice: The choice.
 * Returns: Whether or not it was valid.
 */

bool Sanitizer::CardChoiceValid(const std::string& choice) {
  std::vector<std::string> tokens = TokenizeInput(choice);

  if (tokens.size() != 1) {
    return false;
  }

  return CardValid(tokens[0]);
}

/**
 * Description: Verifies if the action directive is valid.
 * Parameters: const std::string& choice: The choice.
 * Returns: Whether or not it was valid.
 */

bool Sanitizer::ActionChoiceValid(const std::string& choice) {
  std::vector<std::string> tokens = TokenizeInput(choice);

  if (tokens.size() != 1) {
    return false;
  }

  for (unsigned i = 0; i < kNumMoveDirectives; i++) {
    if (tokens[0] == kMoveDirectives[i]) {
      return true;
    }
  }

  return false;
}

/**
 * Description: Verifies if the build directive is valid.
 * Parameters: const std::string& option: The choice.
 * Returns: Whether or not it was valid.
 */

bool Sanitizer::BuildOptionValid(const std::string& option) {
  std::vector<std::string> tokens = TokenizeInput(option);

  if (tokens.size() != 1) {
    return false;
  }

  for (unsigned i = 0; i < kNumBuildChoices; i++) {
    if (tokens[0] == kBuildChoices[i]) {
      return true;
    }
  }

  return false;
}

/**
 * Description: Verifies if the coin choice is valid.
 * Parameters: const std::string& choice: The choice.
 * Returns: Whether or not it was valid.
 */

bool Sanitizer::CoinChoiceValid(const std::string& choice) {
  std::vector<std::string> tokens = TokenizeInput(choice);

  if (tokens.size() != 1) {
    return false;
  }

  if (tokens[0] != "1" && tokens[0] != "0") {
    return false;
  }

  return true;
}

/**
 * Description: Verifies if the cards are valid.
 * Parameters: const std::string& choice: The choice
 * std::vector<std::string& cards: An input parameter for the cards.
 * Returns: Whether or not it was valid.
 */

bool Sanitizer::CardsValid(
    const std::string& choice, std::vector<std::string>& cards) {
  std::vector<std::string> tokens = TokenizeInput(choice);

  if (tokens.empty()) {
    return true;
  }

  std::unordered_set<std::string> seen;

  for (unsigned i = 0; i < tokens.size(); i++) {
    if (seen.find(tokens[i]) != seen.end()) {
      return false;
    }

    seen.insert(tokens[i]);
  }

  for (std::string token : tokens) {
    if (!CardValid(token)) {
      return false;
    }
  }

  cards = tokens;

  return true;
}

/**
 * Description: Verifies if the capture directive is valid.
 * Parameters: const std::string& choice: The choice.
 * Returns: Whether or not it was valid.
 */

bool Sanitizer::CaptureChoiceValid(const std::string& choice) {
  std::vector<std::string> tokens = TokenizeInput(choice);

  if (tokens.size() != 1) {
    return false;
  }

  for (unsigned i = 0; i < kNumCaptureChoices; i++) {
    if (tokens[0] == kCaptureChoices[i]) {
      return true;
    }
  }

  return false;
}

/**
 * Description: Verifies if the file name is valid.
 * Parameters: const std::string& choice: The choice.
 * Returns: Whether or not it was valid.
 */

bool Sanitizer::FileValid(const std::string& file_name) {
  std::vector<std::string> tokens = TokenizeInput(file_name);

  if (tokens.size() != 1) {
    return false;
  }

  unsigned index = file_name.find('.');

  if (index == std::string::npos) {
    return false;
  }

  if (file_name.substr(index + 1) != "txt") {
    return false;
  }

  return true;
}

/**
 * Description: Verifies if the load deck directive is valid.
 * Parameters: const std::string& choice: The choice.
 * Returns: Whether or not it was valid.
 */

bool Sanitizer::DeckChoiceValid(const std::string& choice) {
  std::vector<std::string> tokens = TokenizeInput(choice);

  if (tokens.size() != 1) {
    return false;
  }

  for (unsigned i = 0; i < kNumDeckChoices; i++) {
    if (tokens[0] == kDeckChoices[i]) {
      return true;
    }
  }

  return false;
}

/**
 * Description: Verifies if the load directive is valid.
 * Parameters: const std::string& choice: The choice.
 * Returns: Whether or not it was valid.
 */

bool Sanitizer::LoadChoiceValid(const std::string& choice) {
  std::vector<std::string> tokens = TokenizeInput(choice);

  if (tokens.size() != 1) {
    return false;
  }

  for (unsigned i = 0; i < kNumLoadChoices; i++) {
    if (tokens[0] == kLoadChoices[i]) {
      return true;
    }
  }

  return false;
}