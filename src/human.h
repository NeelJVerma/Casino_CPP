/*******************************
 * Name: Neel Verma            *
 * Project: Casino CPP         *
 * Class: CMPS 366-01          *
 * Due Date: 10/2/18           *
 *******************************/

#ifndef _HUMAN_H_
#define _HUMAN_H_

#include "player.h"

class Human : public Player {
 public:
  // Constructors
  using Player::Player;

  // Public utils
  bool MakeMove(std::shared_ptr<Table>& table);

 private:
  // Private enums
  enum Action {
    kBuild = 1,
    kCapture,
    kTrail
  };

  enum BuildChoice {
    kMake = 1,
    kAdd,
    kIncrease
  };

  enum CaptureChoice {
    kCaptureSet = 1,
    kCaptureBuild,
    kEndTurn
  };

  // Private utils
  bool CardInHand(const std::string& card, unsigned& card_index) const;
  bool Trail(const unsigned& card_index, std::shared_ptr<Table>& table);
  bool MakeBuild(const unsigned& card_index, std::shared_ptr<Table>& table);
  bool IncreaseBuild(const unsigned& card_index,
      std::shared_ptr<Table>& table);

  bool AddToBuild(const unsigned& card_index, std::shared_ptr<Table>& table);
  bool EvaluateBuildChoice(const unsigned& card_index,
      const unsigned& build_choice, std::shared_ptr<Table>& table);

  bool Capture(const unsigned& card_index, std::shared_ptr<Table>& table);
  bool CaptureSet(const unsigned& card_index, std::shared_ptr<Table>& table);
  bool CaptureBuild(const unsigned& card_index, std::shared_ptr<Table>& table);
};

#endif