#include "computer.h"
#include "gui.h"

bool Computer::MakeMove(std::shared_ptr<Table>& table) {
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
    TrailAction(0, table);
    return true;
  }

  auto capture_card = hand_[best_capture->GetPlayedCardIndex()];
  auto build_card = hand_[best_build->GetPlayedCardIndex()];

  if (!can_build) {
    GUI::DisplayAiCaptureOption(best_capture, table, capture_card);
    Capture(best_capture, table);
    table->SetLastCapturedIndex(number_);
    return true;
  }

  if (!can_capture) {
    GUI::DisplayAiBuildOption(best_build, table, build_card);
    Build(best_build, table);
    return true;
  }

  if (best_capture->GetScore() > best_build->GetScore()) {
    GUI::DisplayAiCaptureOption(best_capture, table, capture_card);
    Capture(best_capture, table);
    table->SetLastCapturedIndex(number_);
    return true;
  }

  GUI::DisplayAiBuildOption(best_build, table, build_card);
  Build(best_build, table); 
  
  return true;
}

void Computer::Capture(
    const std::shared_ptr<CaptureNode>& capture_node,
    std::shared_ptr<Table>& table) {
  unsigned card_index = capture_node->GetPlayedCardIndex();
  auto set_indices = capture_node->GetSetIndices();
  auto loose_cards = table->GetLooseCards();

  for (unsigned i = 0; i < set_indices.size(); i++) {
    CaptureSetAction(set_indices[i], table);
  }

  CaptureAllCardsWithSameValue(card_index, table);
  CaptureAllBuildsWithSameValue(card_index, table);

  pile_.push_back(hand_[card_index]);
  RemoveFromHand(card_index);
}

void Computer::Build(
    const std::shared_ptr<BuildNode>& build_node,
    std::shared_ptr<Table>& table) {
  unsigned type = build_node->GetType();
  auto loose_indices = build_node->GetLooseCardIndices();
  auto builds = table->GetCurrentBuilds();
  unsigned build_sum = hand_[build_node->GetPlayedCardIndex()]->GetValue();
  std::vector<std::shared_ptr<Card>> cards;
  cards.push_back(hand_[build_node->GetPlayedCardIndex()]);

  for (unsigned index : loose_indices) {
    cards.push_back(table->GetLooseCard(index));
    build_sum += table->GetLooseCard(index)->GetValue();
  }

  if (type == BuildNode::kMake) {
    MakeBuildAction(build_node->GetPlayedCardIndex(), cards, loose_indices,
        build_sum, table);
  } else if (type == BuildNode::kAdd) {
    AddToBuildAction(build_node->GetPlayedCardIndex(),
        builds[build_node->GetBuildIndex()], cards, loose_indices, table);
  } else {
    IncreaseBuildAction(build_node->GetPlayedCardIndex(),
        builds[build_node->GetBuildIndex()],
        builds[build_node->GetBuildIndex()]->GetBuildSum(), table);
  }
}