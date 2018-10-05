#include <algorithm>
#include <unordered_set>
#include "human.h"
#include "inputhandler.h"
#include "sanitizer.h"
#include "gui.h"
#include "serializer.h"

/**
 * Description: Executes the move selected by the player.
 * Parameters: const std::shared_ptr<Table>& table: The current table state.
 * Returns: Whether the move is valid or not.
 */

bool Human::MakeMove(std::shared_ptr<Table>& table) {
  std::string card = InputHandler::GetCardInput();
  unsigned card_index = 0;

  if (!CardInHand(card, card_index)) {
    GUI::DisplayCardNotInHandMessage();
    return false;
  }

  auto card_in_hand = hand_[card_index];
  unsigned action = InputHandler::GetActionInput();

  if (((table->ValueMatchesBuild(card_in_hand->GetValue(), number_)) ||
      (table->ValueMatchesLooseCard(card_in_hand->GetValue()))) &&
      (action != kCapture)) {
    GUI::DisplayMustCaptureMessage();
    return false;
  }

  if (table->TableEmpty() && action != kTrail) {
    GUI::DisplayMustTrailMessage();
    return false;
  }

  if (action == kBuild) {
    return EvaluateBuildChoice(
      card_index, InputHandler::GetBuildInput(), table);
  }

  if (action == kCapture) {
    return Capture(card_index, table);
  }

  return Trail(card_index, table);
}

/**
 * Description: Checks whether the card is in the user's hand.
 * Parameters: const std::string& card: The serialized card to look for.
 * unsigned& card_index: An input parameter that will be the index of the card
 *     found.
 * Returns: Whether or not the card was found.
 */

bool Human::CardInHand(const std::string& card, unsigned& card_index) const {
  for (unsigned i = 0; i < hand_.size(); i++) {
    if (card == hand_[i]->ToString()) {
      card_index = i;
      return true;
    }
  }

  return false;
}

/**
 * Description: Trails for the human.
 * Parameters: const unsigned& card_index: The index of the played card.
 * std::shared_ptr<Table>& table: The current table state.
 * Returns: Whether or not the trail was successful.
 */

bool Human::Trail(const unsigned& card_index, std::shared_ptr<Table>& table) {
  if (table->PlayerOwnsAnyBuilds(number_)) {
    GUI::DisplayOwnsBuildMessage();
    return false;
  }

  TrailAction(card_index, table);

  return true;
}

/**
 * Description: Makes a build for the human.
 * Parameters: const unsigned& card_index: The index of the played card.
 * std::shared_ptr<Table>& table: The current table state.
 * Returns: Whether or not the build was successful.
 */

bool Human::MakeBuild(
    const unsigned& card_index, std::shared_ptr<Table>& table) {
  std::vector<std::shared_ptr<Card>> cards;
  std::vector<unsigned> card_indices;
  cards.push_back(hand_[card_index]);
  unsigned build_sum = hand_[card_index]->GetValue();
  std::vector<std::string> build_cards = InputHandler::GetBuildCardsInput();

  if (build_cards.empty()) {
    GUI::DisplayBuildCardsEmptyMessage();
    return false;
  }

  if (!table->CardsOnTable(build_cards, card_indices)) {
    GUI::DisplayCardsNotOnTableMessage();
    return false;
  }

  for (unsigned i = 0; i < card_indices.size(); i++) {
    auto card = table->GetLooseCard(card_indices[i]);
    build_sum += card->GetValue();
    cards.push_back(card);
  }

  if (!HasCardWithBuildSum(card_index, build_sum)) {
    GUI::DisplayCannotBuildMessage();
    return false;
  }

  MakeBuildAction(card_index, cards, card_indices, build_sum, table);

  return true;
}

/**
 * Description: Evaluates the build choice for the human.
 * Parameters: const unsigned& card_index: The index of the played card.
 * const unsigned& build_choice: The build directive chosen.
 * Returns: Whether or not the choice was successful.
 */

bool Human::EvaluateBuildChoice(
    const unsigned& card_index, const unsigned& build_choice,
    std::shared_ptr<Table>& table) {
  if (build_choice == kMake) {
    return MakeBuild(card_index, table);
  }

  if (build_choice == kAdd) {
    return AddToBuild(card_index, table);
  }

  return IncreaseBuild(card_index, table);
}

/**
 * Description: Increases a build for the human.
 * Parameters: const unsigned& card_index: The index of the played card.
 * std::shared_ptr<Table>& table: The current table state.
 * Returns: Whether or not the build was successful.
 */

bool Human::IncreaseBuild(
    const unsigned& card_index, std::shared_ptr<Table>& table) {
  if (table->GetCurrentBuilds().empty()) {
    GUI::DisplayNoBuildsMessage();
    return false;
  }

  unsigned build_index = 0;

  if (!table->CardsInBuild(InputHandler::GetBuildAddOnInput(), build_index)) {
    GUI::DisplayNonexistantBuildMessage();
    return false;
  }

  auto card_in_hand = hand_[card_index];
  auto build = table->GetCurrentBuilds()[build_index];
  unsigned build_sum = card_in_hand->GetValue() + build->GetBuildSum();

  if (build->IsMultipleBuild()) {
    GUI::DisplayCannotIncreaseMultiBuildMessage();
    return false;
  }

  if (build->GetOwnerIndex() == number_) {
    GUI::DisplayCannotIncreaseOwnBuildMessage();
    return false;
  }

  std::cout << build_sum << std::endl;
  if (!HasCardWithBuildSum(card_index, build_sum)) {
    GUI::DisplayCannotBuildMessage();
    return false;
  }

  IncreaseBuildAction(card_index, build, build_sum, table);

  return true;
}

/**
 * Description: Adds to a build for the human.
 * Parameters: const unsigned& card_index: The index of the played card.
 * std::shared_ptr<Table>& table: The current table state.
 * Returns: Whether or not the build was successful.
 */

bool Human::AddToBuild(
    const unsigned& card_index, std::shared_ptr<Table>& table) {
  if (table->GetCurrentBuilds().empty()) {
    GUI::DisplayNoBuildsMessage();
    return false;
  }

  std::vector<std::shared_ptr<Card>> cards;
  auto card_in_hand = hand_[card_index];
  cards.push_back(card_in_hand);
  unsigned build_sum = card_in_hand->GetValue();
  std::vector<unsigned> card_indices;
  std::vector<std::string> build_cards_input = InputHandler::GetBuildCardsInput();

  if (!build_cards_input.empty()) {
    if (!table->CardsOnTable(build_cards_input, card_indices)) {
      GUI::DisplayCardsNotOnTableMessage();
      return false;
    }
  }

  for (unsigned i = 0; i < card_indices.size(); i++) {
    auto card = table->GetLooseCard(card_indices[i]);
    build_sum += card->GetValue();
    cards.push_back(card);
  }

  unsigned build_index = 0;

  if (!table->CardsInBuild(InputHandler::GetBuildAddOnInput(), build_index)) {
    GUI::DisplayNonexistantBuildMessage();
    return false;
  }

  auto build = table->GetCurrentBuilds()[build_index];

  if (build_sum != build->GetBuildSum()) {
    GUI::DisplayUnequalBuildSumMessage();
    return false;
  }

  if (!HasCardWithBuildSum(card_index, build_sum)) {
    GUI::DisplayCannotBuildMessage();
    return false;
  }

  AddToBuildAction(card_index, build, cards, card_indices, table);

  return true;
}

/**
 * Description: Captures for the human.
 * Parameters: const unsigned& card_index: The index of the played card.
 * std::shared_ptr<Table>& table: The current table state.
 * Returns: Whether or not the capture was successful.
 */

bool Human::Capture(
    const unsigned& card_index, std::shared_ptr<Table>& table) {
  bool captured_something = false;
  auto card_in_hand = hand_[card_index];
  bool played_is_ace = (card_in_hand->IsAce());

  if (!played_is_ace) {
    if (CaptureAllCardsWithSameValue(card_index, table)) {
      captured_something = true;
      GUI::DisplayAutomaticCaptureMessage();
    }
  } else {
    if (table->HasAces()) {
      captured_something = true;
    }
  }

  if (!HasMultipleOfSameCard(card_index)) {
    if (CaptureAllBuildsWithSameValue(card_index, table)) {
      GUI::DisplayAutomaticCaptureBuildMessage();
      captured_something = true;
    }
  }

  while (true) {
    if (!captured_something) {
      if (InputHandler::GetDiffCardInput() == "1") {
        return false;
      }
    }

    unsigned capture_choice = InputHandler::GetCaptureInput();

    if (captured_something && capture_choice == kEndTurn) {
      break;
    }

    if (!captured_something && capture_choice == kEndTurn) {
      GUI::DisplayNoCapturesMessage();
      continue;
    }

    if (capture_choice == kCaptureSet) {
      if (!CaptureSet(card_index, table)) {
        continue;
      }

      captured_something = true;
    } else {
      if (!CaptureBuild(card_index, table)) {
        continue;
      }

      captured_something = true;
    }
  }

  if (played_is_ace) {
    if (CaptureAllCardsWithSameValue(card_index, table)) {
      captured_something = true;
      GUI::DisplayAutomaticCaptureMessage();
    }
  }

  pile_.push_back(card_in_hand);
  RemoveFromHand(card_index);
  table->SetLastCapturedIndex(number_);

  return true;
}

/**
 * Description: Captures a set for the human.
 * Parameters: const unsigned& card_index: The index of the played card.
 * std::shared_ptr<Table>& table: The current table state.
 * Returns: Whether or not the capture was successful.
 */

bool Human::CaptureSet(
    const unsigned& card_index, std::shared_ptr<Table>& table) {
  std::vector<unsigned> card_indices;
  std::vector<std::string> table_cards = InputHandler::GetCaptureSetInput();

  if (!table->CardsOnTable(table_cards, card_indices)) {
    GUI::DisplayCardsNotOnTableMessage();
    return false;
  }

  auto card_in_hand = hand_[card_index];
  unsigned capture_sum = 0;

  for (unsigned i = 0; i < card_indices.size(); i++) {
    capture_sum += table->GetLooseCard(card_indices[i])->GetValue();
  }

  bool capture_ace_set = false;

  if (capture_sum == Card::kAceTwo && card_in_hand->IsAce()) {
    capture_ace_set = true;
  }

  if (!capture_ace_set && capture_sum != card_in_hand->GetValue()) {
    std::cout << capture_sum << std::endl;
    GUI::DisplayUnequalCaptureSumMessage();
    return false;
  }

  CaptureSetAction(card_indices, table);

  return true;
}

/**
 * Description: Captures a build for the human.
 * Parameters: const unsigned& card_index: The index of the played card.
 * std::shared_ptr<Table>& table: The current table state.
 * Returns: Whether or not the capture was successful.
 */

bool Human::CaptureBuild(
    const unsigned& card_index, std::shared_ptr<Table>& table) {
  if (table->GetCurrentBuilds().empty()) {
    GUI::DisplayNoBuildsMessage();
    return false;
  }

  auto card_in_hand = hand_[card_index];
  unsigned build_index = 0;

  if (!table->CardsInBuild(
      InputHandler::GetCaptureBuildInput(), build_index)) {
    GUI::DisplayNonexistantBuildMessage();
    return false;
  }

  auto build = table->GetCurrentBuilds()[build_index];

  bool capture_ace_build = false;

  if (build->GetBuildSum() == Card::kAceTwo && card_in_hand->IsAce()) {
    capture_ace_build = true;
  }

  if (!capture_ace_build && build->GetBuildSum() != card_in_hand->GetValue()) {
    GUI::DisplayUnequalBuildSumMessage();
    return false;
  }

  CaptureBuildAction(build_index, table);

  return true;
}