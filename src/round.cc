#include "round.h"
#include "inputhandler.h"
#include "gui.h"
#include "serializer.h"

/**
 * Description: Constructor that initializes a fresh round.
 * Parameters: std::vector<std::shared_ptr<Player>>& players: The players
 *     received from the tournament.
 * const unsigned& round_num: The round number to start with.
 * Returns: Nothing.
 */

Round::Round(
    std::vector<std::shared_ptr<Player>>& players,
    const unsigned& round_num) :
    players_(players), round_num_(round_num) {
  InitRound();
}

/**
 * Description: Constructor that initializes a loaded round.
 * Parameters: std::vector<std::shared_ptr<Player>>& players: The players
 *     received from the tournament.
 * std::shared_ptr<Deck>& deck: The loaded deck state.
 * const unsigned& current_player_index: The loaded current player.
 * const unsigned& round_num: The loaded round number.
 * Returns: Nothing.
 */

Round::Round(
    std::vector<std::shared_ptr<Player>>& players,
    std::shared_ptr<Table>& table, std::shared_ptr<Deck>& deck,
    const unsigned& current_player_index, const unsigned& round_num) :
    players_(players), table_(table), deck_(deck),
    current_player_index_(current_player_index), round_num_(round_num) {
  if (current_player_index_) {
    players_[0]->SetIsTurn(false);
    players_[1]->SetIsTurn(true);
  } else {
    players_[1]->SetIsTurn(false);
    players_[0]->SetIsTurn(true);
  }
}

/**
 * Description: Handles menu input.
 * Parameters: const unsigned& choice: The menu directive chosen.
 * Returns: Whether to switch turns or not. Exits if the choice was save or
 *     quit
 */

bool Round::HandleMenuInput(const unsigned& choice) {
  if (choice == kSave) {
    Serializer::SaveRoundState(InputHandler::GetFileInput(), this);
    exit(0);
  }

  if (choice == kMove) {
    return players_[current_player_index_]->MakeMove(table_);
  }

  if (choice == kHelp) {
    if (!players_[current_player_index_]->IsHuman()) {
      GUI::DisplayInvalidHelp();
    } else {
      players_[current_player_index_]->ShowHint(table_);
    }

    return false;
  }

  exit(0);
}

/**
 * Description: Initializes a fresh round.
 * Parameters: None.
 * Returns: Nothing.
 */

void Round::InitRound() {
  if (InputHandler::GetDeckInput() == kNew) {
    deck_ = std::shared_ptr<Deck>(new Deck);
  } else {
    deck_ = std::shared_ptr<Deck>(
        new Deck(Serializer::GetDeckFromFile(InputHandler::GetFileInput())));
  }

  table_ = std::shared_ptr<Table>(new Table);
  current_player_index_ = (players_[0]->IsTurn() ? 0 : 1);

  for (unsigned i = 0; i < players_.size(); i++) {
    players_[i]->ClearPile();
    players_[i]->ClearHand();
  }

  DealCards();
  table_->AddDealtCards(deck_->DealNext());
}

/**
 * Description: Deals the cards to the players.
 * Parameters: None.
 * Returns: Nothing.
 */

void Round::DealCards() {
  if (players_[0]->IsHuman()) {
    players_[0]->ReplaceHand(deck_->DealNext());
    players_[1]->ReplaceHand(deck_->DealNext());
  } else {
    players_[1]->ReplaceHand(deck_->DealNext());
    players_[0]->ReplaceHand(deck_->DealNext());
  }
}

/**
 * Description: Switches the turns.
 * Parameters: None.
 * Returns: Nothing.
 */

void Round::SwitchTurn() {
  current_player_index_ = (players_[0]->IsTurn() ? 1 : 0);
  
  if (players_[1]->IsTurn()) {
    players_[0]->SetIsTurn(true);
    players_[1]->SetIsTurn(false);
  } else {
    players_[1]->SetIsTurn(true);
    players_[0]->SetIsTurn(false);
  }
}

/**
 * Description: Checks if all hands are empty.
 * Parameters: None.
 * Returns: Whether or not all hands are empty.
 */

bool Round::AllHandsEmpty() {
  for (unsigned i = 0; i < players_.size(); i++) {
    if (!players_[i]->GetHand().empty()) {
      return false;
    }
  }

  return true;
}

/**
 * Description: Plays a fresh round.
 * Parameters: None.
 * Returns: Nothing.
 */

void Round::PlayRound() {
  GUI::DisplayRoundStartMessage();

  while (true) {
    if (AllHandsEmpty()) {
      if (deck_->IsEmpty()) {
        break;
      }

      GUI::DisplayAllHandsEmptyMessage();
      DealCards();
    }

    GUI::DisplayPlayerTurnMessage(players_);
    GUI::DisplayAllPlayerInfo(players_);
    GUI::DisplayTable(table_);
    GUI::DisplayDeck(deck_);

    if (!HandleMenuInput(InputHandler::GetMenuInput())) {
      continue;
    }

    SwitchTurn();
    GUI::DisplayTurnSwitchMessage();
  }

  CalcScores();
  auto to_pile = table_->ClearTable();
  unsigned last_captured_index = table_->GetLastCapturedIndex();

  for (unsigned i = 0; i < to_pile.size(); i++) {
    players_[last_captured_index]->AddToPile(to_pile[i]);
  }

  for (unsigned i = 0; i < players_.size(); i++) {
    if (i == last_captured_index) {
      players_[i]->SetIsTurn(true);
      continue;
    }

    players_[i]->SetIsTurn(false);
  }

  GUI::DisplayPlayerPiles(players_);
  
  for (unsigned i = 0; i < players_.size(); i++) {
    GUI::DisplayNumPoints(players_[i]);
    GUI::DisplayNumCards(players_[i]);
  }
}

/**
 * Description: Gets the index of the player that got the most cards.
 * Parameters: None.
 * Returns: The index.
 */

unsigned Round::GetMaxCardsIndex() {
  unsigned max_cards = 0;
  unsigned max_index = 0;

  for (unsigned i = 0; i < players_.size(); i++) {
    unsigned pile_size = players_[i]->GetPile().size();

    if (pile_size > max_cards) {
      max_cards = pile_size;
      max_index = i;
    }
  }

  return max_index;
}

/**
 * Description: Gets the index of the player that got the most spades.
 * Parameters: None.
 * Returns: The index.
 */

unsigned Round::GetMaxSpadesIndex() {
  unsigned max_spades = 0;
  unsigned max_index = 0;

  for (unsigned i = 0; i < players_.size(); i++) {
    auto pile = players_[i]->GetPile();
    unsigned num_spades = 0;

    for (unsigned j = 0; j < pile.size(); j++) {
      if (pile[j]->GetValue() == Card::kSpades) {
        num_spades++;
      }
    }

    if (num_spades > max_spades) {
      max_spades = num_spades;
      max_index = i;
    }
  }

  return max_index;
}

/**
 * Description: Calcs player scores.
 * Parameters: None.
 * Returns: Nothing.
 */

void Round::CalcScores() {
  bool card_tie = false;

  if (players_[0]->GetPile().size() == Deck::kMaxDeckSize / players_.size()) {
    card_tie = true;
  }

  if (!card_tie) {
    players_[GetMaxCardsIndex()]->AddToScore(3);
  }

  players_[GetMaxSpadesIndex()]->AddToScore(1);

  for (unsigned i = 0; i < players_.size(); i++) {
    auto pile = players_[i]->GetPile();

    for (unsigned j = 0; j < pile.size(); j++) {
      if (pile[j]->ToString() == "DX") {
        players_[i]->AddToScore(2);
      } else if (pile[j]->ToString() == "S2" || pile[j]->IsAce()) {
        players_[i]->AddToScore(1);
      }
    }
  }
}

/**
 * Description: Gets the serialized round data.
 * Parameters: None.
 * Returns: The serialized data.
 */

std::string Round::GetRoundData() {
  std::string data = "Round: ";
  data += std::to_string(round_num_) + '\n';

  for (unsigned i = 0; i < players_.size(); i++) {
    data += players_[i]->ToString();
  }

  data += table_->ToString() + '\n';

  for (unsigned i = 0; i < table_->GetCurrentBuilds().size(); i++) {
    auto build = table_->GetCurrentBuilds()[i];
    data += "Build Owner: " + build->ToString()
         + (build->GetOwnerIndex() ? "Human\n" : "Computer\n");
  }

  data += deck_->ToString() + '\n';
  data += "Next Player: ";
  data +=
      (players_[current_player_index_]->IsHuman() ? "Human\n" : "Computer\n");

  return data;
}