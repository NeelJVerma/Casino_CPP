/*******************************
 * Name: Neel Verma            *
 * Project: Casino CPP         *
 * Class: CMPS 366-01          *
 * Due Date: 10/2/18           *
 *******************************/

#include <unordered_set>
#include <algorithm>
#include "player.h"
#include "inputhandler.h"
#include "gui.h"

/**
 * Description: Removes the card at index from the hand.
 * Parameters: const unsigned& index: The index of the card to remove.
 * Returns: An pointer to the card that was removed.
 */

std::shared_ptr<Card> Player::RemoveFromHand(const unsigned& index) {
  std::shared_ptr<Card> return_card = hand_[index];
  hand_.erase(hand_.begin() + index);

  return return_card;
}

/**
 * Description: Converts all player information to a string.
 * Parameters: None.
 * Returns: The string format of a player.
 */

std::string Player::ToString() const {
  std::string player = "";
  player += (is_human_ ? "Human:\n" : "Player:\n");
  player += "\tScore: ";
  player += std::to_string(score_);
  player += "\n\tHand: ";

  for (auto card : hand_) {
    player += card->ToString() + ' ';
  }

  player += "\n\tPile: ";

  for (auto card : pile_) {
    player += card->ToString() + ' ';
  }

  player += '\n';

  return player;
}

/**
 * Description: Trails for the given parameters.
 * Parameters: const unsigned& card_index: The index of the card to trail.
 * std::shared_ptr<Table>& table: The current table state.
 * Returns: Nothing.
 */

void Player::TrailAction(
    const unsigned& card_index, std::shared_ptr<Table>& table) {
  std::shared_ptr<Card> card = RemoveFromHand(card_index);
  table->AddLooseCard(card);
}

/**
 * Description: Makes a build for the given parameters.
 * Parameters: const unsigned& card_index: The index of the played card.
 * const std::vector<std::shared_ptr<Card>>& cards: The cards to build with.
 * const std::vector<unsigned>& card_indices: The indices of the chosen cards.
 * const unsigned& build_sum: The build sum.
 * std::shared_ptr<Table>& table: The current table state.
 * Returns: Nothing.
 */

void Player::MakeBuildAction(
    const unsigned& card_index,
    const std::vector<std::shared_ptr<Card>>& cards,
    const std::vector<unsigned>& card_indices, const unsigned& build_sum,
    std::shared_ptr<Table>& table) {
  std::shared_ptr<Build> build(new Build);
  RemoveFromHand(card_index);
  build->AddToBuild(cards);
  build->SetBuildSum(build_sum);
  table->RemoveLooseCards(card_indices);
  table->AddBuild(build);
  build->SetOwnerIndex(number_);
}

/**
 * Description: Increases a build for the given parameters.
 * Parameters: const unsigned& card_index: The index of the played card.
 * std::shared_ptr<Build>& build: The build to increase.
 * const unsigned& build_sum: The build sum.
 * std::shared_ptr<Table>& table: The current table state.
 * Returns: Nothing.
 */

void Player::IncreaseBuildAction(
      const unsigned& card_index, std::shared_ptr<Build>& build,
      const unsigned& build_sum, std::shared_ptr<Table>& table) {
  build->AddToSingleBuild(hand_[card_index]);
  RemoveFromHand(card_index);
  build->SetBuildSum(build_sum);
  build->SetOwnerIndex(number_);
}

/**
 * Description: Adds to a build for the given parameters.
 * Parameters: const unsigned& card_index: The index of the played card.
 * std::shared_ptr<Build>& build: The build to add to.
 * const std::vector<std::shared_ptr<Card>>& cards: The cards to build with.
 * const std::vector<unsigned>& card_indices: The indices of the chosen cards.
 * std::shared_ptr<Table>& table: The current table state.
 * Returns: Nothing.
 */

void Player::AddToBuildAction(
    const unsigned& card_index, std::shared_ptr<Build>& build,
    const std::vector<std::shared_ptr<Card>>& cards,
    const std::vector<unsigned>& card_indices, std::shared_ptr<Table>& table) {
  RemoveFromHand(card_index);
  build->AddToBuild(cards);
  table->RemoveLooseCards(card_indices);
  build->SetOwnerIndex(number_);
}

void Player::CaptureSetAction(
    const std::vector<unsigned>& card_indices, std::shared_ptr<Table>& table) {
  for (unsigned i = 0; i < card_indices.size(); i++) {
    pile_.push_back(table->GetLooseCard(card_indices[i]));
  }

  table->RemoveLooseCards(card_indices);
}

/**
 * Description: Captures a build for the given parameters.
 * Parameters: const unsigned& card_index: The index of the played card.
 * std::shared_ptr<Table>& table: The current table state.
 * Returns: Nothing.
 */

void Player::CaptureBuildAction(
    const unsigned& build_index, std::shared_ptr<Table>& table) {
  auto deconstructed_build = table->DeconstructBuild(build_index);

  for (unsigned i = 0; i < deconstructed_build.size(); i++) {
    pile_.push_back(deconstructed_build[i]);
  }

  table->RemoveBuild(build_index);
}

/**
 * Description: Captures all cards with the same values.
 * Parameters: const unsigned& card_index: The index of the played card.
 * std::shared_ptr<Table>& table: The current table state.
 * Returns: Nothing.
 */

bool Player::CaptureAllCardsWithSameValue(
    const unsigned& card_index, std::shared_ptr<Table>& table) {
  bool matches = false;
  unsigned value = hand_[card_index]->GetValue();
  auto card_in_hand = hand_[card_index];
  std::vector<unsigned> card_indices;

  for (unsigned i = 0; i < table->GetLooseCards().size(); i++) {
    auto card_on_table = table->GetLooseCards()[i];

    if ((card_on_table->IsAce() && card_in_hand->IsAce()) ||
        (card_on_table->GetValue() == value)) {
      pile_.push_back(card_on_table);
      card_indices.push_back(i);
      matches = true;
    }
  }

  table->RemoveLooseCards(card_indices);

  return matches;
}

/**
 * Description: Captures all builds with the same value.
 * Parameters: const unsigned& card_index: The index of the played card.
 * std::shared_ptr<Table>& table: The current table state.
 * Returns: Nothing.
 */

bool Player::CaptureAllBuildsWithSameValue(
    const unsigned& card_index, std::shared_ptr<Table>& table) {
  bool matches = false;
  unsigned value = hand_[card_index]->GetValue();
  auto card_in_hand = hand_[card_index];
  std::vector<unsigned> build_indices;

  for (unsigned i = 0; i < table->GetCurrentBuilds().size(); i++) {
    auto build = table->GetCurrentBuilds()[i];

    if (number_ && build->GetOwnerIndex() != number_) {
      continue;
    }

    if ((build->GetBuildSum() == value) ||
        (card_in_hand->IsAce() && build->GetBuildSum() == Card::kAceTwo)) {
      auto deconstructed_build = table->DeconstructBuild(i);

      for (unsigned j = 0; j < deconstructed_build.size(); j++) {
        pile_.push_back(deconstructed_build[j]);
      }

      build_indices.push_back(i);
      matches = true;
    }
  }

  table->RemoveMultipleBuilds(build_indices);

  return matches;
}

/**
 * Description: Checks if player has a card with the build sum.
 * Parameters: const unsigned& card_index: The index of the played card.
 * const unsigned& build_sum: The build sum.
 * Returns: Whether or not a card was found.
 */

bool Player::HasCardWithBuildSum(
    const unsigned& card_index, const unsigned& build_sum) const {
  for (unsigned i = 0; i < hand_.size(); i++) {
    auto card = hand_[i];

    if ((i != card_index && card->GetValue() == build_sum) ||
        (card->IsAce() && build_sum == Card::kAceTwo)) {
      return true;
    }
  }

  return false;
}

/**
 * Description: Checks if player has multiple cards with the same value.
 * Parameters: const unsigned& card_index: The index of the played card.
 * Returns: Whether or not multiple cards were found.
 */

bool Player::HasMultipleOfSameCard(const unsigned& card_index) const {
  unsigned value = hand_[card_index]->GetValue();

  for (unsigned i = 0; i < hand_.size(); i++) {
    if (i == card_index) {
      continue;
    }

    if (value == hand_[i]->GetValue()) {
      return true;
    }
  }

  return false;
}

/**
 * Description: Gets all valid sets that sum up to the given value.
 * Parameters: const unsigned& value: The value.
 * const std::shared_ptr<Table>& table: The current table state.
 * Returns: All valid sets.
 */

std::vector<std::vector<unsigned>> Player::GetAllValidSets(
    const unsigned& value, const std::shared_ptr<Table>& table) const {
  std::vector<std::vector<unsigned>> return_sets;
  auto loose_cards = table->GetLooseCards();

  for (unsigned i = 0; i < (1 << loose_cards.size()); i++) {
    std::vector<unsigned> subset;
    unsigned sum = 0;

    for (unsigned j = 0; j < loose_cards.size(); j++) {
      if (i & (1 << j)) {
        subset.push_back(j);
        sum += loose_cards[j]->GetValue();
      }
    }

    if ((subset.size() > 1 && (sum == value)) ||
        (value == Card::kAceOne && sum == Card::kAceTwo)) {
      return_sets.push_back(subset);
    }
  }

  return return_sets;
}

/**
 * Description: Gets all valid sets of sets that sum up to a given value
 *     (No duplicates).
 * Parameters: const std::vector<std::vector<unsigned>>& sets: The sets to check
 *     against.
 * Returns: All valid sets.
 */

std::vector<std::vector<unsigned>> Player::GetAllValidSetsOfSets(
    const std::vector<std::vector<unsigned>>& sets) const {
  std::vector<std::vector<unsigned>> return_sets;

  for (unsigned i = 0; i < (1 << sets.size()); i++) {
    std::vector<unsigned> subset;
    bool add = true;
    std::unordered_set<unsigned> seen;

    for (unsigned j = 0; j < sets.size(); j++) {
      if (i & (1 << j)) {
        for (unsigned k = 0; k < sets[j].size(); k++) {
          if (seen.find(sets[j][k]) != seen.end()) {
            add = false;
          }

          subset.push_back(sets[j][k]);
          seen.insert(sets[j][k]);
        }
      }
    }

    if (add) {
      return_sets.push_back(subset);
    }
  }

  return return_sets;
}

/**
 * Description: Finds the best capture move.
 * Parameters: const std::shared_ptr<Table>& table: The current table state.
 * Returns: The best capture.
 */

std::shared_ptr<CaptureNode> Player::FindBestCapture(
    const std::shared_ptr<Table>& table) const {
  unsigned max_score = 0;
  unsigned max_index = 0;
  std::vector<unsigned> max_set;
  auto loose_cards = table->GetLooseCards();
  auto builds = table->GetCurrentBuilds();
  std::vector<unsigned> max_loose;
  std::vector<unsigned> max_builds;

  for (unsigned i = 0; i < hand_.size(); i++) {
    auto valid_sets = 
        GetAllValidSetsOfSets(GetAllValidSets(hand_[i]->GetValue(), table));
    std::vector<unsigned> loose_indices;
    std::vector<unsigned> build_indices;
    unsigned score = 0;

    for (unsigned j = 0; j < loose_cards.size(); j++) {
      if ((hand_[i]->IsAce() && loose_cards[j]->IsAce()) ||
          (hand_[i]->GetValue() == loose_cards[j]->GetValue())) {
        loose_indices.push_back(j);
        score += GetCardScore(loose_cards[j]);
      }
    }

    for (unsigned j = 0; j < builds.size(); j++) {
      if ((hand_[i]->IsAce() && builds[j]->GetBuildSum() == Card::kAceTwo) ||
          (hand_[i]->GetValue() == builds[j]->GetBuildSum())) {
        build_indices.push_back(j);
        auto deconstructed_build = table->DeconstructBuild(j);

        for (unsigned k = 0; k < deconstructed_build.size(); k++) {
          score += GetCardScore(deconstructed_build[k]);
        }
      }
    }

    for (unsigned j = 0; j < valid_sets.size(); j++) {
      unsigned set_score = 0;
      
      for (unsigned k = 0; k < valid_sets[j].size(); k++) {
        set_score += GetCardScore(loose_cards[valid_sets[j][k]]);
      }

      score += set_score;

      if (score > max_score) {
        max_score = score;
        max_index = i;
        max_loose = loose_indices;
        max_builds = build_indices;
        max_set = valid_sets[j];
      } else {
        score -= set_score;
      }
    }
  }

  std::shared_ptr<CaptureNode> best_capture(new CaptureNode);
  best_capture->SetPlayedCardIndex(max_index);
  best_capture->AddToScore(max_score);
  best_capture->AddToScore(GetCardScore(hand_[max_index]));
  best_capture->SetBuildIndices(max_builds);
  best_capture->SetLooseCardIndices(max_loose);

  if (!max_set.empty()) {
    best_capture->AddSetIndices(max_set);
  }

  return best_capture;
}

/**
 * Description: Finds the best build move.
 * Parameters: const std::shared_ptr<Table>& table: The current table state.
 * Returns: The best build.
 */

std::shared_ptr<BuildNode> Player::FindBestBuild(
    const std::shared_ptr<Table>& table) const {
  unsigned best_single_score = 0;
  std::shared_ptr<BuildNode> best_single(new BuildNode);
  unsigned best_multi_score = 0;
  std::shared_ptr<BuildNode> best_multi(new BuildNode);
  unsigned best_increase_score = 0;
  std::shared_ptr<BuildNode> best_increase(new BuildNode);

  for (unsigned i = 0; i < hand_.size(); i++) {
    auto single_node = FindBestSingleBuild(i, table);
    auto multi_node = FindBestMultiBuild(i, table);
    auto increase_node = FindBestIncreaseBuild(i, table);

    if (single_node->GetScore() > best_single_score) {
      best_single_score = single_node->GetScore();
      best_single = single_node;
    }

    if (multi_node->GetScore() > best_multi_score) {
      best_multi_score = multi_node->GetScore();
      best_multi = multi_node;
    }

    if (increase_node->GetScore() > best_increase_score) {
      best_increase_score = increase_node->GetScore();
      best_increase = increase_node;
    }
  }

  std::vector<std::shared_ptr<BuildNode>> builds{
      best_single, best_multi, best_increase};

  std::sort(builds.begin(), builds.end(), [](
      const std::shared_ptr<BuildNode>& one,
      const std::shared_ptr<BuildNode>& two) {
    return one->GetScore() > two->GetScore();
  });

  return builds[0];
}

/**
 * Description: Checks if the value is found on the table.
 * Parameters: const unsgined& value: The value.
 * const std::shared_ptr<Table>& table: The current table state.
 * Returns: Whether or not the value was found.
 */

bool Player::MatchesCardOnTable(
    const unsigned& value, const std::shared_ptr<Table>& table) const {
  auto loose_cards = table->GetLooseCards();

  for (unsigned i = 0; i < loose_cards.size(); i++) {
    if (value == loose_cards[i]->GetValue()) {
      return true;
    }
  }

  return false;
}

/**
 * Description: Checks if the value is found as a build sum.
 * Parameters: const unsgined& value: The value.
 * const std::shared_ptr<Table>& table: The current table state.
 * Returns: Whether or not the value was found.
 */

bool Player::MatchesBuildOnTable(
    const unsigned& value, const std::shared_ptr<Table>& table) const {
  auto builds = table->GetCurrentBuilds();

  for (unsigned i = 0; i < builds.size(); i++) {
    if (value == builds[i]->GetBuildSum()) {
      return true;
    }
  }

  return false;
}

/**
 * Description: Finds the best single build.
 * Parameters: const unsgined& index: The index of the played card.
 * const std::shared_ptr<Table>& table: The current table state.
 * Returns: The best single build.
 */

std::shared_ptr<BuildNode> Player::FindBestSingleBuild(
    const unsigned& index, const std::shared_ptr<Table>& table) const {
  auto loose_cards = table->GetLooseCards();
  std::vector<std::vector<unsigned>> valid_sets;
  std::unordered_set<unsigned> values;
  unsigned value = hand_[index]->GetValue();

  for (unsigned i = 0; i < hand_.size(); i++) {
    if (index == i) {
      continue;
    }

    if (hand_[i]->GetValue() == Card::kAceOne) {
      values.insert(Card::kAceTwo);
    }

    values.insert(hand_[i]->GetValue());
  }

  if (!MatchesCardOnTable(hand_[index]->GetValue(), table)) {
    for (unsigned i = 0; i < (1 << loose_cards.size()); i++) {
      std::vector<unsigned> subset;
      unsigned sum = value;

      for (unsigned j = 0; j < loose_cards.size(); j++) {
        if (i & (1 << j)) {
          subset.push_back(j);
          sum += loose_cards[j]->GetValue();
        }
      }

      if (values.find(sum) != values.end()) {
        valid_sets.push_back(subset);
      }
    }
  }

  unsigned max_score = GetCardScore(hand_[index]);
  bool score_updated = false;
  std::vector<unsigned> max_set;
  std::shared_ptr<BuildNode> single_node(new BuildNode);
  single_node->SetPlayedCardIndex(index);

  for (unsigned i = 0; i < valid_sets.size(); i++) {
    unsigned score = GetCardScore(hand_[index]);

    for (unsigned j = 0; j < valid_sets[i].size(); j++) {
      score += GetCardScore(loose_cards[valid_sets[i][j]]);
    }

    if (score > max_score) {
      max_score = score;
      max_set = valid_sets[i];
      score_updated = true;
    }
  }

  if (score_updated) {
    single_node->AddToScore(max_score);
  }

  single_node->SetLooseCardIndices(max_set);
  single_node->SetType(BuildNode::kMake);

  return single_node;
}

std::shared_ptr<BuildNode> Player::FindBestMultiBuild(
    const unsigned& index, const std::shared_ptr<Table>& table) const {
  auto loose_cards = table->GetLooseCards();
  auto builds = table->GetCurrentBuilds();
  std::unordered_set<unsigned> values;
  unsigned value = hand_[index]->GetValue();
  BuildAndLooseIndices bali;

  for (unsigned i = 0; i < hand_.size(); i++) {
    if (index == i) {
      continue;
    }

    if (hand_[i]->GetValue() == Card::kAceOne) {
      values.insert(Card::kAceTwo);
    }

    values.insert(hand_[i]->GetValue());
  }

  if (!MatchesCardOnTable(hand_[index]->GetValue(), table)) {
    for (unsigned i = 0; i < (1 << loose_cards.size()); i++) {
      std::vector<unsigned> subset;
      unsigned sum = value;

      for (unsigned j = 0; j < loose_cards.size(); j++) {
        if (i & (1 << j)) {
          subset.push_back(j);
          sum += loose_cards[j]->GetValue();
        }
      }

      if (values.find(sum) != values.end()) {
        for (unsigned j = 0; j < builds.size(); j++) {
          if (sum == builds[j]->GetBuildSum()) {
            bali.push_back(std::make_pair(j, subset));
          }
        }
      }
    }
  }

  std::pair<unsigned, std::vector<unsigned>> max_build_and_loose;
  unsigned max_score = GetCardScore(hand_[index]);
  bool score_updated = false;
  std::shared_ptr<BuildNode> multi_node(new BuildNode);
  multi_node->SetPlayedCardIndex(index);

  for (auto p : bali) {
    unsigned score = GetCardScore(hand_[index]);
    auto deconstructed_build = table->DeconstructBuild(p.first);

    for (unsigned i = 0; i < deconstructed_build.size(); i++) {
      score += GetCardScore(deconstructed_build[i]);
    }

    for (unsigned i = 0; i < p.second.size(); i++) {
      score += GetCardScore(loose_cards[p.second[i]]);
    }

    if (score > max_score) {
      max_score = score;
      max_build_and_loose = p;
      score_updated = true;
    }
  }

  multi_node->SetLooseCardIndices(max_build_and_loose.second);
  multi_node->SetBuildIndex(max_build_and_loose.first);
  multi_node->SetType(BuildNode::kAdd);

  if (score_updated) {
    multi_node->AddToScore(max_score);
  }

  return multi_node;
}

std::shared_ptr<BuildNode> Player::FindBestIncreaseBuild(
    const unsigned& index, const std::shared_ptr<Table>& table) const {
  auto builds = table->GetCurrentBuilds();
  std::unordered_set<unsigned> values;
  std::vector<unsigned> build_indices;

  for (unsigned i = 0; i < hand_.size(); i++) {
    if (index == i) {
      continue;
    }

    if (hand_[i]->GetValue() == Card::kAceOne) {
      values.insert(Card::kAceTwo);
    }

    values.insert(hand_[i]->GetValue());
  }

  if (!MatchesCardOnTable(hand_[index]->GetValue(), table)) {
    for (unsigned i = 0; i < builds.size(); i++) {
      if ((builds[i]->GetOwnerIndex() == number_) ||
          (builds[i]->IsMultipleBuild())) {
        continue;
      }

      unsigned sum = hand_[index]->GetValue() + builds[i]->GetBuildSum();

      if (values.find(sum) != values.end()) {
        build_indices.push_back(i);
      }
    }
  }

  unsigned max_index = 0;
  unsigned max_score = GetCardScore(hand_[index]);
  std::shared_ptr<BuildNode> increase_node(new BuildNode);
  increase_node->SetPlayedCardIndex(index);
  bool score_updated = false;

  for (unsigned i = 0; i < build_indices.size(); i++) {
    unsigned score = GetCardScore(hand_[index]);
    auto deconstructed_build = table->DeconstructBuild(build_indices[i]);

    for (unsigned j = 0; j < deconstructed_build.size(); j++) {
      score += GetCardScore(deconstructed_build[j]);
    }

    if (score > max_score) {
      max_score = score;
      max_index = build_indices[i];
      score_updated = true;
    }
  }

  increase_node->SetBuildIndex(max_index);
  increase_node->SetType(BuildNode::kIncrease);
  
  if (score_updated) {
    increase_node->AddToScore(max_score);
  }

  return increase_node;
}

unsigned Player::GetCardScore(const std::shared_ptr<Card>& card) const {
  if (card->ToString() == "DX") {
    return 4;
  }

  if (card->IsAce()) {
    return 3;
  }

  if (card->ToString() == "S2" || card->GetSuit() == Card::kSpades) {
    return 2;
  }

  return 1;
}

void Player::ShowHint(const std::shared_ptr<Table>& table) const {
  auto best_capture = FindBestCapture(table);
  bool can_capture = true;

  if ((best_capture->GetLooseCardIndices().empty()) && 
      (best_capture->GetBuildIndices().empty()) && 
      (best_capture->GetSetIndices().empty())) {
    can_capture = false;
  }

  bool can_build = true;
  auto best_build = FindBestBuild(table);

  if (!best_build->GetScore()) {
    can_build = false;
  }

  if (!can_build && !can_capture) {
    GUI::DisplayAiTrailOption(hand_[0]);
    return;
  }

  auto capture_card = hand_[best_capture->GetPlayedCardIndex()];
  auto build_card = hand_[best_build->GetPlayedCardIndex()];

  if (!can_build) {
    GUI::DisplayAiCaptureOption(best_capture, table, capture_card);
    return;
  }

  if (!can_capture) {
    GUI::DisplayAiBuildOption(best_build, table, build_card);
    return;
  }

  if (best_capture->GetScore() > best_build->GetScore()) {
    GUI::DisplayAiCaptureOption(best_capture, table, capture_card);
    return;
  }

  GUI::DisplayAiBuildOption(best_build, table, build_card);
}