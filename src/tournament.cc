#include "tournament.h"
#include "gui.h"
#include "serializer.h"
#include "human.h"
#include "computer.h"
#include "inputhandler.h"

/**
 * Description: Constructs a fresh torunament.
 * Parameters: None.
 * Returns: Nothing.
 */

Tournament::Tournament() {
  players_.push_back(std::shared_ptr<Player>(new Computer));
  players_[0]->SetIsHuman(false);
  players_[0]->SetNumber(0);
  players_.push_back(std::shared_ptr<Player>(new Human));
  players_[1]->SetIsHuman(true);
  players_[1]->SetNumber(1);
  round_num_ = 1;
}

/**
 * Description: Checks if the tournament is over.
 * Parameters: None.
 * Returns: Whether or not the tournament is over.
 */

bool Tournament::TournamentOver() {
  for (unsigned i = 0; i < players_.size(); i++) {
    if (players_[i]->GetScore() >= kMaxScore) {
      return true;
    }
  }

  return false;
}

/**
 * Description: A generic play game function.
 * Parameters: None.
 * Returns: Nothing.
 */

void Tournament::PlayGame() {
  while (!TournamentOver()) {
    rounds_.push_back(
        std::shared_ptr<Round>(new Round(players_, round_num_++)));
    rounds_[rounds_.size() - 1]->PlayRound();
  }

  GUI::DisplayWinningState(players_);
}

/**
 * Description: Plays from a loaded state.
 * Parameters: std::ifstream& in_file: The file to load from.
 * Returns: Nothing.
 */

void Tournament::PlayLoaded(std::ifstream& in_file) {
  std::shared_ptr<Table> table(new Table);
  table->SetLooseCards(Serializer::GetLooseCardsFromFile(in_file));
  table->SetCurrentBuilds(Serializer::GetBuildsFromFile(in_file));
  std::shared_ptr<Deck> deck = Serializer::GetDeckFromFile(in_file);
  unsigned next_player = Serializer::GetNextPlayerFromFile(in_file);
  rounds_.push_back(std::shared_ptr<Round>(new Round(
      players_, table, deck, next_player, round_num_++)));
  rounds_[rounds_.size() - 1]->PlayRound();

  PlayGame();
}

/**
 * Description: Play a new tournament.
 * Parameters: None.
 * Returns: Nothing.
 */

void Tournament::PlayNew() {
  if (InputHandler::GetCoinInput() == FlipCoin()) {
    players_[0]->SetIsTurn(true);
    players_[1]->SetIsTurn(false);
  } else {
    players_[1]->SetIsTurn(true);
    players_[0]->SetIsTurn(false);
  }

  PlayGame();
}