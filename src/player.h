/*******************************
 * Name: Neel Verma            *
 * Project: Casino CPP         *
 * Class: CMPS 366-01          *
 * Due Date: 10/2/18           *
 *******************************/

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <vector>
#include "card.h"
#include "table.h"
#include "buildnode.h"
#include "capturenode.h"

using BuildAndLooseIndices =
    std::vector<std::pair<unsigned, std::vector<unsigned>>>;

class Player {
 public:
  // Constructors
  Player() : score_(0), is_turn_(false), is_human_(false), number_(0) {}

  // Accessors
  inline unsigned GetScore() const { return score_; }
  inline std::vector<std::shared_ptr<Card>> GetPile() const { return pile_; }
  inline std::vector<std::shared_ptr<Card>> GetHand() const { return hand_; }
  inline bool IsTurn() const { return is_turn_; }
  inline bool IsHuman() const { return is_human_; }
  inline unsigned GetNumber() const { return number_; }

  // Mutators
  inline void SetScore(const unsigned& score) { score_ = score; } 
  inline void SetIsTurn(const bool& is_turn) { is_turn_ = is_turn; }
  inline void SetIsHuman(const bool& is_human) { is_human_ = is_human; }
  inline void SetNumber(const unsigned& number) { number_ = number; }
  inline void SetPile(const std::vector<std::shared_ptr<Card>>& pile) {
    pile_ = pile;
  }

  inline void SetHand(const std::vector<std::shared_ptr<Card>>& hand) {
    hand_ = hand;
  }

  // Public utils
  inline void AddToScore(const unsigned& num_points) { score_ += num_points; }
  inline void AddToPile(const std::shared_ptr<Card>& card) {
    pile_.push_back(card);
  }

  inline void ReplaceHand(const std::vector<std::shared_ptr<Card>>& hand) {
    hand_ =  hand;
  }

  inline void ClearPile() { pile_.clear(); }
  inline void ClearHand() { hand_.clear(); }
  std::string ToString() const;
  std::shared_ptr<Card> RemoveFromHand(const unsigned& index);
  virtual bool MakeMove(std::shared_ptr<Table>& table) = 0;
  void ShowHint(const std::shared_ptr<Table>& table) const;

 protected:
  unsigned score_;
  std::vector<std::shared_ptr<Card>> pile_;
  std::vector<std::shared_ptr<Card>> hand_;
  bool is_turn_;
  bool is_human_;
  unsigned number_;

  // Protected utils
  void TrailAction(const unsigned& card_index, std::shared_ptr<Table>& table);
  void MakeBuildAction(const unsigned& card_index,
      const std::vector<std::shared_ptr<Card>>& cards,
      const std::vector<unsigned>& card_indices, const unsigned& build_sum,
      std::shared_ptr<Table>& table);
  
  void IncreaseBuildAction(const unsigned& card_index,
      std::shared_ptr<Build>& build, const unsigned& build_sum,
      std::shared_ptr<Table>& table);

  void AddToBuildAction(const unsigned& card_index,
      std::shared_ptr<Build>& build,
      const std::vector<std::shared_ptr<Card>>& cards,
      const std::vector<unsigned>& card_indices,
      std::shared_ptr<Table>& table);

  void CaptureSetAction(const std::vector<unsigned>& card_indices,
      std::shared_ptr<Table>& table);

  void CaptureBuildAction(const unsigned& build_index,
      std::shared_ptr<Table>& table);

  bool CaptureAllCardsWithSameValue(const unsigned& card_index,
      std::shared_ptr<Table>& table);

  bool CaptureAllBuildsWithSameValue(const unsigned& card_index,
      std::shared_ptr<Table>& table);

  bool HasCardWithBuildSum(const unsigned& card_index,
      const unsigned& build_sum) const;
  
  bool HasMultipleOfSameCard(const unsigned& card_index) const;
  unsigned GetCardScore(const std::shared_ptr<Card>& card) const;
  std::vector<std::vector<unsigned>> GetAllValidSets(
      const unsigned& value, const std::shared_ptr<Table>& table) const;

  bool MatchesCardOnTable(const unsigned& value,
      const std::shared_ptr<Table>& table) const;

  bool MatchesBuildOnTable(const unsigned& value,
      const std::shared_ptr<Table>& table) const;

  std::vector<std::vector<unsigned>> GetAllValidSetsOfSets(
      const std::vector<std::vector<unsigned>>& sets) const;

  std::shared_ptr<CaptureNode> FindBestCapture(
      const std::shared_ptr<Table>& table) const;

  std::shared_ptr<BuildNode> FindBestBuild(
      const std::shared_ptr<Table>& table) const;

  std::shared_ptr<BuildNode> FindBestSingleBuild(const unsigned& index,
      const std::shared_ptr<Table>& table) const;

  std::shared_ptr<BuildNode> FindBestMultiBuild(const unsigned& index,
      const std::shared_ptr<Table>& table) const;

  std::shared_ptr<BuildNode> FindBestIncreaseBuild(const unsigned& index,
      const std::shared_ptr<Table>& table) const;
};

#endif