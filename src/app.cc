#include "app.h"
#include "inputhandler.h"
#include "serializer.h"

/**
 * Description: Sets the player states from a file.
 * Parameters: std::ifstream& in_file: The file stream.
 * Returns: Nothing.
 */

void App::SetPlayerDataFromFile(std::ifstream& in_file) {
  auto players = tournament_->GetPlayers();

  for (unsigned i = 0; i < players.size(); i++) {
    players[i]->SetScore(Serializer::GetPlayerScoreFromFile(in_file));
    players[i]->SetHand(Serializer::GetCardsFromFile(in_file));
    players[i]->SetPile(Serializer::GetCardsFromFile(in_file));
  }
}

/**
 * Description: Starts the app.
 * Parameters: None.
 * Returns: Nothing.
 */

void App::Start() {
  tournament_ = std::shared_ptr<Tournament>(new Tournament);

  if (InputHandler::GetLoadChoiceInput() == kLoad) {
    std::ifstream in_file =
        Serializer::OpenLoadFile(InputHandler::GetFileInput());
    tournament_->SetRoundNum(Serializer::GetRoundNumFromFile(in_file));
    SetPlayerDataFromFile(in_file);
    tournament_->PlayLoaded(in_file);
    in_file.close();
  } else {
    tournament_->PlayNew();
  }
}