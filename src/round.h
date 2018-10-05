#ifndef _ROUND_H_
#define _ROUND_H_

#include "player.h"
#include "table.h"
#include "deck.h"
#include "round.h"

class Round {
 public:
  // Delete copy constructor and assignment operator
  Round(const Round& round) = delete;
  Round& operator=(const Round& round) = delete;

  // Constructors
  Round(std::vector<std::shared_ptr<Player>>& players, const unsigned& round_num);
  Round(std::vector<std::shared_ptr<Player>>& players, std::shared_ptr<Table>& table,
      std::shared_ptr<Deck>& deck, const unsigned& current_player_index,
      const unsigned& round_num);

  // Public utils
  std::string GetRoundData();
  void PlayRound();

 private:
  // Private enums
  enum MenuChoice {
    kSave = 1,
    kMove,
    kHelp,
    kQuit
  };

  enum DeckChoice {
    kNew = 1,
    kRandom
  };

  std::vector<std::shared_ptr<Player>>& players_;
  std::shared_ptr<Table> table_;
  std::shared_ptr<Deck> deck_;
  unsigned current_player_index_;
  unsigned last_captured_index_;
  unsigned round_num_;

  // Private utils
  bool HandleMenuInput(const unsigned& choice);
  void InitRound();
  void SwitchTurn();
  void DealCards();
  bool AllHandsEmpty();
  unsigned GetMaxCardsIndex();
  unsigned GetMaxSpadesIndex();
  void CalcScores();
};

#endif