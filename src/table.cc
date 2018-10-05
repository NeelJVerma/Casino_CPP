#include <algorithm>
#include <unordered_set>
#include "table.h"
#include "inputhandler.h"

/**
 * Description: Add the cards dealt at the beginning of a round.
 * Parameters: const std::vector<std::shared_ptr<Card>>& dealt:
 *     The dealt cards.
 * Returns: Nothing.
 */

void Table::AddDealtCards(const std::vector<std::shared_ptr<Card>>& dealt) {
  for (unsigned i = 0; i < dealt.size(); i++) {
    loose_cards_.push_back(dealt[i]);
  }
}

/**
 * Description: Checks if a value matches a loose card on the table.
 * Parameters: const unsigned& card_value: The card value.
 * Returns: Whether or not there was a match.
 */

bool Table::ValueMatchesLooseCard(const unsigned& card_value) const {
  for (unsigned i = 0; i < loose_cards_.size(); i++) {
    if (card_value == loose_cards_[i]->GetValue()) {
      return true;
    }
  }

  return false;
}

/**
 * Description: Checks if a value matches a build on the table.
 * Parameters: const unsigned& card_value: The card value.
 * const unsigned& player_number: The player number.
 * Returns: Whether or not there was a match.
 */

bool Table::ValueMatchesBuild(
    const unsigned& card_value, const unsigned& player_number) const {
  for (unsigned i = 0; i < current_builds_.size(); i++) {
    if (current_builds_[i]->GetOwnerIndex() != player_number) {
      continue;
    }

    if (current_builds_[i]->GetBuildSum() == card_value) {
      return true;
    }
  }

  return false;
}

/**
 * Description: Checks if a player owns any builds.
 * Parameters: const unsigned& player_number: The current player number.
 * Returns: Whether or not there was a match.
 */

bool Table::PlayerOwnsAnyBuilds(const unsigned& player_number) const {
  for (unsigned i = 0; i < current_builds_.size(); i++) {
    if (current_builds_[i]->GetOwnerIndex() == player_number) {
      return true;
    }
  }

  return false;
}

/**
 * Description: Checks the given set of cards is on the table.
 * Parameters: const std::vector<std::string>& cards: The serialized cards.
 * std::vector<unsigned>& card_indices: An input parameter to fill out the
 *     positions of the cards found.
 * Returns: Whether or not the cards are found.
 */

bool Table::CardsOnTable(
    const std::vector<std::string>& cards,
    std::vector<unsigned>& card_indices) const {
  for (unsigned i = 0; i < loose_cards_.size(); i++) {
    auto card = loose_cards_[i];

    if (find(cards.begin(), cards.end(), card->ToString()) != cards.end()) {
      card_indices.push_back(i);
    }
  }

  return card_indices.size() == cards.size();
}

/**
 * Description: Checks the given set of cards are a valid build.
 * Parameters: const std::vector<std::string>& cards: The serialized cards.
 * unsigned& build_index: An input parameter to get the position of the build
 *     found.
 * Returns: Whether or not the build was found.
 */

bool Table::CardsInBuild(
    const std::vector<std::string>& cards, unsigned& build_index) const {
  std::unordered_set<std::string> seen;

  for (unsigned i = 0; i < cards.size(); i++) {
    seen.insert(cards[i]);
  }

  for (unsigned i = 0; i < current_builds_.size(); i++) {
    std::unordered_set<std::string> seen_on_table;
    auto current_build = current_builds_[i];

    for (unsigned j = 0; j < current_build->GetBuildSize(); j++) {
      for (unsigned k = 0; k < current_build->GetBuildAt(j).size(); k++) {
        std::string card_str = current_build->GetBuildAt(j)[k]->ToString();

        if (seen.find(card_str) == seen.end()) {
          seen_on_table.clear();
          break;
        }

        seen_on_table.insert(card_str);
      }
    }

    if (seen_on_table.size() == seen.size()) {
      build_index = i;
      return true;
    }
  }

  return false;
}

/**
 * Description: Removes the loose cards at the given indices.
 * Parameters: const std::vector<unsigned>& indices: The indices of loose cards.
 * Returns: Nothing.
 */

void Table::RemoveLooseCards(const std::vector<unsigned>& indices) {
  std::vector<std::shared_ptr<Card>> temp;

  for (unsigned i = 0; i < loose_cards_.size(); i++) {
    if (find(indices.begin(), indices.end(), i) == indices.end()) {
      temp.push_back(loose_cards_[i]);
    }
  }

  loose_cards_.clear();

  for (unsigned i = 0; i < temp.size(); i++) {
    loose_cards_.push_back(temp[i]);
  }
}

/**
 * Description: Removes the builds at the given indices.
 * Parameters: const std::vector<unsigned>& indices: The indices of builds.
 * Returns: Nothing.
 */

void Table::RemoveMultipleBuilds(const std::vector<unsigned>& indices) {
  std::vector<std::shared_ptr<Build>> temp;

  for (unsigned i = 0; i < current_builds_.size(); i++) {
    if (find(indices.begin(), indices.end(), i) == indices.end()) {
      temp.push_back(current_builds_[i]);
    }
  }

  current_builds_.clear();

  for (unsigned i = 0; i < temp.size(); i++) {
    current_builds_.push_back(temp[i]);
  }
}

/**
 * Description: Deconstructs a given build.
 * Parameters: const unsigned& index: The build index.
 * Returns: A vector of the cards that were removed.
 */

std::vector<std::shared_ptr<Card>> Table::DeconstructBuild(
    const unsigned& index) {
  std::vector<std::shared_ptr<Card>> cards;

  for (unsigned i = 0; i < current_builds_[index]->GetBuildSize(); i++) {
    for (auto card : current_builds_[index]->GetBuildAt(i)) {
      cards.push_back(card);
    }
  }

  return cards;
}

/**
 * Description: Checks whether or not the table has aces.
 * Parameters: None.
 * Returns: Whether or not the table has aces.
 */

bool Table::HasAces() const {
  for (unsigned i = 0; i < loose_cards_.size(); i++) {
    if (loose_cards_[i]->IsAce()) {
      return true;
    }
  }

  return false;
}

/**
 * Description: Clears the table.
 * Parameters: None.
 * Returns: A vector of cards that were cleared.
 */

std::vector<std::shared_ptr<Card>> Table::ClearTable() {
  std::vector<std::shared_ptr<Card>> cards;

  for (unsigned i = 0; i < loose_cards_.size(); i++) {
    cards.push_back(loose_cards_[i]);
  }

  loose_cards_.clear();

  return cards;
}

/**
 * Description: Serializes the table into a string.
 * Parameters: None.
 * Returns: The serialized table string.
 */

std::string Table::ToString() const {
  std::string table = "Table: ";

  for (unsigned i = 0; i < current_builds_.size(); i++) {
    table += current_builds_[i]->ToString();
  }

  for (unsigned i = 0; i < loose_cards_.size(); i++) {
    table += loose_cards_[i]->ToString() + " ";
  }

  return table;
}