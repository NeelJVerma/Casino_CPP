#ifndef _COMPUTER_H_
#define _COMPUTER_H_

#include <utility>
#include "player.h"

class Computer : public Player {
 public:
  // Constructors
  using Player::Player;

  // Public utils
  bool MakeMove(std::shared_ptr<Table>& table);

 private:
  // Private utils
  void Capture(const std::shared_ptr<CaptureNode>& capture_node,
      std::shared_ptr<Table>& table);

  void Build(const std::shared_ptr<BuildNode>& build_node,
      std::shared_ptr<Table>& table);
};

#endif