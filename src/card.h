/*******************************
 * Name: Neel Verma            *
 * Project: Casino CPP         *
 * Class: CMPS 366-01          *
 * Due Date: 10/2/18           *
 *******************************/

#ifndef _CARD_H_
#define _CARD_H_

#include <string>

class Card {
 public:
  // Delete copy constructor and assignment operator
  Card(const Card& card) = delete;
  Card& operator=(const Card& card) = delete;
  
  // Public constants
  static const unsigned kNumSuits = 4;
  static const unsigned kNumValues = 13;

  // Public enums
  enum SuitType {
    kSpades = 1,
    kHearts,
    kClubs,
    kDiamonds,
  };

  enum CardValue {
    kAceOne = 1,
    kTwo,
    kThree,
    kFour,
    kFive,
    kSix,
    kSeven,
    kEight,
    kNine,
    kTen,
    kJack,
    kQueen,
    kKing,
    kAceTwo
  };

  // Use compiler generated default constructor
  Card() = default;

  // Constructors
  Card(const std::string& symbols);
  Card(unsigned suit, unsigned value) : suit_(suit), value_(value),
      is_ace_(false) {}

  // Accessors
  inline unsigned GetValue() const { return value_; }
  inline unsigned GetSuit() const { return suit_; }
  inline bool IsAce() const { return is_ace_; }

  // Mutators
  inline void SetValue(const unsigned& value) { value_ = value; }
  inline void SetSuit(const unsigned& suit) { suit_ = suit; }
  inline void SetIsAce(const bool& is_ace) { is_ace_ = is_ace; }

  // Public utils
  std::string ToString() const;

 private:
  unsigned suit_;
  unsigned value_;
  bool is_ace_;
};

#endif