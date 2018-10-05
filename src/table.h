#ifndef _TABLE_H_
#define _TABLE_H_

#include "build.h"

class Table {
 public:
  // Delete copy constructor and assignment operator
  Table(const Table& table) = delete;
  Table& operator=(const Table& table) = delete;

  // Constructors
  Table() = default;

  // Accessors
  inline std::vector<std::shared_ptr<Card>> GetLooseCards() const {
    return loose_cards_;
  }

  inline std::vector<std::shared_ptr<Build>> GetCurrentBuilds() const {
    return current_builds_;
  }

  inline unsigned GetLastCapturedIndex() const { return last_captured_index_; }

  // Mutators
  inline void SetLooseCards(
      const std::vector<std::shared_ptr<Card>>& loose_cards)  {
    loose_cards_ = loose_cards;
  }

  inline void SetCurrentBuilds(
      const std::vector<std::shared_ptr<Build>>& current_builds)  {
    current_builds_ = current_builds;
  }

  inline void SetLastCapturedIndex(const unsigned& last_captured_index) {
    last_captured_index_ = last_captured_index;
  }

  // Public utils
  bool ValueMatchesLooseCard(const unsigned& card_value) const;
  bool ValueMatchesBuild(const unsigned& card_value,
      const unsigned& player_number) const;

  bool PlayerOwnsAnyBuilds(const unsigned& player_number) const;
  bool CardsOnTable(const std::vector<std::string>& cards,
      std::vector<unsigned>& card_indices) const;

  bool CardsInBuild(const std::vector<std::string>& cards,
      unsigned& build_index) const;

  void AddDealtCards(const std::vector<std::shared_ptr<Card>>& dealt);
  inline void AddLooseCard(const std::shared_ptr<Card>& card) {
    loose_cards_.push_back(card);
  }

  inline void AddBuild(const std::shared_ptr<Build>& build) {
    current_builds_.push_back(build);
  }

  std::vector<std::shared_ptr<Card>> DeconstructBuild(const unsigned& index);
  inline unsigned GetTableSize() const {
    return loose_cards_.size() + current_builds_.size();
  }

  inline bool TableEmpty() const {
    return loose_cards_.empty() && current_builds_.empty();
  }

  inline std::shared_ptr<Card> GetLooseCard(const unsigned& index) const {
    return loose_cards_[index];
  }

  void RemoveLooseCards(const std::vector<unsigned>& indices);
  inline void RemoveBuild(const unsigned& index) {
    current_builds_.erase(current_builds_.begin() + index);
  }

  void RemoveMultipleBuilds(const std::vector<unsigned>& indices);
  bool HasAces() const;

  std::vector<std::shared_ptr<Card>> ClearTable();
  std::string ToString() const;

 private:
  std::vector<std::shared_ptr<Build>> current_builds_;
  std::vector<std::shared_ptr<Card>> loose_cards_;
  unsigned last_captured_index_;
};

#endif