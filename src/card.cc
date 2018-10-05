/*******************************
 * Name: Neel Verma            *
 * Project: Casino CPP         *
 * Class: CMPS 366-01          *
 * Due Date: 10/2/18           *
 *******************************/

#include "card.h"

/**
 * Description: Constructor used for taking in serialized game data.
 * Parameters: const std::string& symbol: The symbolic form of a card.
 * Returns: Nothing.
 */

Card::Card(const std::string& symbol) {
  switch (symbol[0]) {
    case 'H':
      suit_ = kHearts;
      break;
    case 'S':
      suit_ = kSpades;
      break;
    case 'C':
      suit_ = kClubs;
      break;
    case 'D':
      suit_ = kDiamonds;
      break;
  }

  switch (symbol[1]) {
    case 'A':
      value_ = kAceOne;
      break;
    case '2':
      value_ = kTwo;
      break;
    case '3':
      value_ = kThree;
      break;
    case '4':
      value_ = kFour;
      break;
    case '5':
      value_ = kFive;
      break;
    case '6':
      value_ = kSix;
      break;
    case '7':
      value_ = kSeven;
      break;
    case '8':
      value_ = kEight;
      break;
    case '9':
      value_ = kNine;
      break;
    case 'X':
      value_ = kTen;
      break;
    case 'J':
      value_ = kJack;
      break;
    case 'Q':
      value_ = kQueen;
      break;
    case 'K':
      value_ = kKing;
      break;
  }

  if (value_ == kAceOne) {
    SetIsAce(true);
  } else {
    SetIsAce(false);
  }
}

/**
 * Description: Converts a card to its symbolic notation.
 * Parameters: None.
 * Returns: The symbolic form of a card.
 */

std::string Card::ToString() const {
  char suit;
  char value;

  switch (suit_) {
    case kHearts:
      suit = 'H';
      break;
    case kSpades:
      suit = 'S';
      break;
    case kClubs:
      suit = 'C';
      break;
    case kDiamonds:
      suit = 'D';
      break;
  }

  switch (value_) {
    case kAceOne:
      value = 'A';
      break;
    case kAceTwo:
      value = 'A';
      break;
    case kTwo:
      value = '2';
      break;
    case kThree:
      value = '3';
      break;
    case kFour:
      value = '4';
      break;
    case kFive:
      value = '5';
      break;
    case kSix:
      value = '6';
      break;
    case kSeven:
      value = '7';
      break;
    case kEight:
      value = '8';
      break;
    case kNine:
      value = '9';
      break;
    case kTen:
      value = 'X';
      break;
    case kJack:
      value = 'J';
      break;
    case kQueen:
      value = 'Q';
      break;
    case kKing:
      value = 'K';
      break;
  }

  std::string symbol(1, suit);
  symbol += value;

  return symbol;
}