/*******************************
 * Name: Neel Verma            *
 * Project: Casino CPP         *
 * Class: CMPS 366-01          *
 * Due Date: 10/2/18           *
 *******************************/

#ifndef _DECK_H_
#define _DECK_H_

#include <vector>
#include <memory>
#include "card.h"

class Deck {
 public:
  // Delete copy constructor and assignment operator
  Deck(const Deck& deck) = delete;
  Deck& operator=(const Deck& deck) = delete;

  // Pulic constants
  static const unsigned kMaxDeckSize = 52;
  
  // Constructors
  Deck();
  Deck(std::vector<std::shared_ptr<Card>> deck) : deck_(deck) {}
  Deck(const std::vector<std::string>& symbols);

  // Public utils
  inline unsigned GetDeckSize() const { return deck_.size(); }
  inline bool IsEmpty() const { return deck_.empty(); }
  std::vector<std::shared_ptr<Card>> DealNext();
  std::string ToString() const;

 private:
  // Private constants
  static const unsigned kDealSize = 4;

  // Private utils
  void Shuffle();
  std::vector<std::shared_ptr<Card>> deck_;
};

#endif