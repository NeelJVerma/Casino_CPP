#ifndef _TOURNAMENT_H_
#define _TOURNAMENT_H_

#include <fstream>
#include "player.h"
#include "round.h"

class Tournament {
 public:
  // Delete copy constructor and assignment operator
  Tournament(const Tournament& tournament) = delete;
  Tournament& operator=(const Tournament& tournament) = delete;

  // Constructors
  Tournament();

  // Accessors
  std::vector<std::shared_ptr<Player>> GetPlayers() const { return players_; }

  // Mutators
  inline void SetRoundNum(const unsigned& round_num) {
    round_num_ = round_num;
  }

  // Public utils
  void PlayNew();
  void PlayLoaded(std::ifstream& in_file);

 private:
  // Private constants
  const unsigned kMaxScore = 21;

  unsigned round_num_;
  std::vector<std::shared_ptr<Round>> rounds_;
  std::vector<std::shared_ptr<Player>> players_;
  
  // Private utils
  bool TournamentOver();
  inline unsigned FlipCoin() { return (rand() % 2); }
  void PlayGame();
};

#endif