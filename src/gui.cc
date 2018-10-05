/*******************************
 * Name: Neel Verma            *
 * Project: Casino CPP         *
 * Class: CMPS 366-01          *
 * Due Date: 10/2/18           *
 *******************************/

#include "gui.h"

/**
 * Description: Displays the main menu to the player.
 * Parameters: None.
 * Returns: Nothing.
 */

void GUI::DisplayMenu() {
  std::cout << std::endl;
  std::cout << "1. Save the game" << std::endl;
  std::cout << "2. Make a move" << std::endl;
  std::cout << "3. Ask for help (only before human player plays)" << std::endl;
  std::cout << "4. Quit the game" << std::endl;
}

/**
 * Description: Displays the main action menu to the player.
 * Parameters: None.
 * Returns: Nothing.
 */

void GUI::DisplayActionMenu() {
  std::cout << "1. Build" << std::endl;
  std::cout << "2. Capture" << std::endl;
  std::cout << "3. Trail" << std::endl;
}

void GUI::DisplayDeckMenu() {
  std::cout << "1. Load random deck" << std::endl;
  std::cout << "2. Load preset deck" << std::endl;
}

/**
 * Description: Displays the build owners as part of the table.
 * Parameters: const std::shared_ptr<Table>& table: The current state of the
 *             table.
 * Returns: Nothing.
 */

void GUI::DisplayBuildOwners(const std::shared_ptr<Table>& table) {
  for (unsigned i = 0; i < table->GetCurrentBuilds().size(); i++) {
    auto build = table->GetCurrentBuilds()[i];
    std::cout << "Build Owner: " << build->ToString()
              << (build->GetOwnerIndex() ? "Human" : "Computer")
              << std::endl;
  }

  if (!table->GetCurrentBuilds().empty()) {
    std::cout << std::endl;
  }
}

/**
 * Description: Displays the current state of the table.
 * Parameters: const std::shared_ptr<Table>& table: The current state of the
 *             table.
 * Returns: Nothing.
 */

void GUI::DisplayTable(const std::shared_ptr<Table>& table) {
  std::cout << table->ToString() << std::endl << std::endl;
  DisplayBuildOwners(table);
}

/**
 * Description: Displays all info about the players.
 * Parameters: const std::vector<std::shared_ptr<Player>>& players: The current
 *             state of all players.
 * Returns: Nothing.
 */

void GUI::DisplayAllPlayerInfo(
    const std::vector<std::shared_ptr<Player>>& players) {
  for (unsigned i = 0; i < players.size(); i++) {
    std::cout << players[i]->ToString() << std::endl;
  }
}

/**
 * Description: Displays the build menu to the player.
 * Parameters: None.
 * Returns: Nothing.
 */

void GUI::DisplayBuildMenu() {
  std::cout << "1. Make" << std::endl;
  std::cout << "2. Add to exisiting build" << std::endl;
  std::cout << "3. Increase" << std::endl;
}

/**
 * Description: Displays the capture menu to the player.
 * Parameters: None.
 * Returns: Nothing.
 */

void GUI::DisplayCaptureMenu() {
  std::cout << "1. Capture a set" << std::endl;
  std::cout << "2. Capture a build" << std::endl;
  std::cout << "3. End turn" << std::endl;
}

void GUI::DisplayPlayerPiles(
    const std::vector<std::shared_ptr<Player>>& players) {
  for (unsigned i = 0; i < players.size(); i++) {
    auto pile = players[i]->GetPile();
    std::cout << (players[i]->IsHuman() ? "Human pile: " : "Computer pile: ");

    for (unsigned j = 0; j < pile.size(); j++) {
      std::cout << pile[j]->ToString() << ' ';
    }

    std::cout << std::endl;
  }

  std::cout << std::endl;
}

/**
 * Description: Display who won the game.
 * Parameters: const std::vector<std::shared_ptr<Player>>& players: The current
 *     players.
 * Returns: Nothing.
 */

void GUI::DisplayWinningState(
    const std::vector<std::shared_ptr<Player>>& players) {
  bool tie = true;

  for (unsigned i = 0; i < players.size() - 1; i++) {
    if (players[i]->GetScore() != players[i + 1]->GetScore()) {
      tie = false;
    }
  }

  if (tie) {
    std::cout << "*** It is a tie ***" << std::endl;
  } else {
    unsigned max_index = 0;
    unsigned max = 0;

    for (unsigned i = 0; i < players.size(); i++) {
      if (players[i]->GetScore() > max) {
        max = players[i]->GetScore();
        max_index = i;
      }
    }

    std::cout << "*** Player " << max_index + 1 << " wins with a score of "
        << players[max_index]->GetScore() << "***" << std::endl;
  }
}

/**
 * Description: Displays the load menu to the player.
 * Parameters: None.
 * Returns: Nothing.
 */

void GUI::DisplayLoadMenu() {
  std::cout << "1. Load game from file" << std::endl;
  std::cout << "2. Start new game" << std::endl;
}

/**
 * Description: Displays the AI's best capture choice.
 * Parameters: const std::shared_ptr<CaptureNode>& capture_node: The best
 *     capture move found.
 * const std::shared_ptr<Table>& table: The current table state.
 * const std::shared_ptr<Card>& played_card: The card that was played.
 * Returns: Nothing.
 */

void GUI::DisplayAiCaptureOption(
    const std::shared_ptr<CaptureNode>& capture_node,
    const std::shared_ptr<Table>& table,
    const std::shared_ptr<Card>& played_card) {
  std::cout <<
      "The best move is to capture with " <<
      played_card->ToString() << std::endl;
  auto loose_cards = table->GetLooseCards();
  auto builds = table->GetCurrentBuilds();
  auto loose_indices = capture_node->GetLooseCardIndices();
  auto build_indices = capture_node->GetBuildIndices();
  auto set_indices = capture_node->GetSetIndices();

  if (!loose_indices.empty()) {
    std::cout << "Loose cards to capture: ";
    
    for (unsigned i = 0; i < loose_indices.size(); i++) {
      std::cout << loose_cards[loose_indices[i]]->ToString() << ' ';
    }

    std::cout << std::endl;
  }

  if (!build_indices.empty()) {
    std::cout << "Builds to capture: ";
    
    for (unsigned i = 0; i < build_indices.size(); i++) {
      std::cout << builds[build_indices[i]]->ToString() << ' ';
    }

    std::cout << std::endl;
  }

  if (!set_indices.empty()) {
    std::cout << "Sets to capture: ";
    
    for (unsigned i = 0; i < set_indices.size(); i++) {
      for (unsigned j = 0; j < set_indices[i].size(); j++) {
        std::cout << loose_cards[set_indices[i][j]]->ToString() << ' ';
      }

      std::cout << std::endl;
    }
  }

  std::cout <<
      "The score of this move is " << capture_node->GetScore() << std::endl;
}

/**
 * Description: Displays the AI's best build choice.
 * Parameters: const std::shared_ptr<BuildNode>& build_node: The best
 *     build move found.
 * const std::shared_ptr<Table>& table: The current table state.
 * const std::shared_ptr<Card>& played_card: The card that was played.
 * Returns: Nothing.
 */

void GUI::DisplayAiBuildOption(
    const std::shared_ptr<BuildNode>& build_node,
    const std::shared_ptr<Table>& table,
    const std::shared_ptr<Card>& played_card) {
  std::cout <<
      "The best move is to build with " <<
      played_card->ToString() << std::endl;
  unsigned type = build_node->GetType();
  auto loose_cards = table->GetLooseCards();
  auto builds = table->GetCurrentBuilds();
  auto loose_indices = build_node->GetLooseCardIndices();
  unsigned build_index = build_node->GetBuildIndex();

  if (type == BuildNode::kMake) {
    std::cout << "Make a build with ";

    for (unsigned i = 0; i < loose_indices.size(); i++) {
      std::cout << loose_cards[loose_indices[i]]->ToString() << ' ';
    }

    std::cout << std::endl;
  } else if (type == BuildNode::kAdd) {
    std::cout << "Add to " << builds[build_index]->ToString() << std::endl;
    std::cout << "Combine played card with ";
    
    for (unsigned i = 0; i < loose_indices.size(); i++) {
      std::cout << loose_cards[loose_indices[i]]->ToString() << ' ';
    }

    std::cout << std::endl;
  } else {
    std::cout << "Increase " << builds[build_index]->ToString() << std::endl;
  }

  std::cout <<
      "The score of this move is " << build_node->GetScore() << std::endl;
}