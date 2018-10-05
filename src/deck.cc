/*******************************
 * Name: Neel Verma            *
 * Project: Casino CPP         *
 * Class: CMPS 366-01          *
 * Due Date: 10/2/18           *
 *******************************/

#include <cmath>
#include <cstdlib>
#include "deck.h"

/**
 * Description: Default constructor to initialize a randomly shuffled deck.
 * Parameters: None.
 * Returns: Nothing.
 */

Deck::Deck() {
  for (unsigned suit = 1; suit <= Card::kNumSuits; suit++) {
    for (unsigned value = 1; value <= Card::kNumValues; value++) {
      std::shared_ptr<Card> card(new Card(suit, value));

      if (value == Card::kAceOne) {
        card->SetIsAce(true);
      }

      deck_.push_back(card);
    }
  }

  Shuffle();
}

/**
 * Description: Constructor to take in serialized deck data.
 * Parameters: const std::vector<std::string>& symbols: Symbolic forms of cards.
 * Returns: Nothing.
 */

Deck::Deck(const std::vector<std::string>& symbols) {
  for (unsigned i = 0; i < symbols.size(); i++) {
    deck_.push_back(std::shared_ptr<Card>(new Card(symbols[i])));
  }
}

/**
 * Description: Deals the next four cards from the deck.
 * Parameters: None.
 * Returns: A vector of the four most recent cards from the deck.
 */

std::vector<std::shared_ptr<Card>> Deck::DealNext() {
  std::vector<std::shared_ptr<Card>> return_cards;
  unsigned num_removed = 0;

  while ((num_removed++) < kDealSize) {
    return_cards.push_back(deck_.back());
    deck_.pop_back();
  }

  return return_cards;
}

/**
 * Description: Randomly shuffles the deck. std::random_shuffle is depracated.
 * Parameters: None.
 * Returns: Nothing.
 */

void Deck::Shuffle() {
  for (unsigned i = kMaxDeckSize - 1; i >= 1; i--) {
    unsigned random_index = ((i + 1) * ((double) rand() / (RAND_MAX)));
    std::shared_ptr<Card> temp = deck_[i];
    deck_[i] = deck_[random_index];
    deck_[random_index] = temp;
  }
}

/**
 * Description: Serializes the deck to a string.
 * Parameters: None.
 * Returns: The serialized deck string.
 */

std::string Deck::ToString() const {
  std::string deck = "Deck: ";

  for (unsigned i = 0; i < deck_.size(); i++) {
    deck.insert(6, deck_[i]->ToString() + ' ');
  }

  return deck;
}