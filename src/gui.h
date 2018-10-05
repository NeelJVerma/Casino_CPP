/*******************************
 * Name: Neel Verma            *
 * Project: Casino CPP         *
 * Class: CMPS 366-01          *
 * Due Date: 10/2/18           *
 *******************************/

#ifndef _GUI_H_
#define _GUI_H_

#include <memory>
#include <iostream>
#include "table.h"
#include "player.h"
#include "deck.h"
#include "capturenode.h"
#include "buildnode.h"

class GUI {
 public:
  // Public utils
  static void DisplayMenu();
  static void DisplayActionMenu();
  static void DisplayBuildMenu();
  static void DisplayMultiBuildMenu();
  static void DisplayExistingBuildOption();
  static void DisplayCaptureMenu();
  static void DisplayDeckMenu();
  static void DisplayTable(const std::shared_ptr<Table>& table);
  static void DisplayAllPlayerInfo(
      const std::vector<std::shared_ptr<Player>>& players);
  
  static void DisplayAiCaptureOption(
      const std::shared_ptr<CaptureNode>& capture_node,
      const std::shared_ptr<Table>& table,
      const std::shared_ptr<Card>& played_card);

  static void DisplayAiBuildOption(
      const std::shared_ptr<BuildNode>& build_node,
      const std::shared_ptr<Table>& table,
      const std::shared_ptr<Card>& played_card);

  static void DisplayPlayerPiles(
      const std::vector<std::shared_ptr<Player>>& players);

  static void DisplayWinningState(
      const std::vector<std::shared_ptr<Player>>& players);

  static void DisplayLoadMenu();
  static inline void DisplayCardPlayMessage() {
    std::cout << "Enter the card you want to play" << std::endl;
  }

  static void DisplayAutomaticCaptureMessage() {
    std::cout <<
        "*** Automatically captured cards with the same value as played card ***"
        << std::endl;
  }

  static inline void DisplayBuildCardsMessage() {
    std::cout <<
        "Enter the cards you want to build with. Enter nothing if only card from hand (only for adding)"
        << std::endl;
  }

  static inline void DisplayBuildAddOnMessage() {
    std::cout << "Enter the cards in the build you want to add to"
        << std::endl;
  }

  static inline void DisplayInvalidChoiceMessage() {
    std::cout << "*** Not a valid choice ***" << std::endl;
  }

  static inline void DisplayCoinMessage() {
    std::cout << "Toss a coin to see who goes first (0 or 1)" << std::endl;
  }

  static inline void DisplayTurnSwitchMessage() {
    std::cout << "*** Switched turns ***" << std::endl;
  }

  static inline void DisplayMustCaptureMessage() {
    std::cout << "*** Must capture with this card ***" << std::endl;
  }

  static inline void DisplayAllHandsEmptyMessage() {
    std::cout << "*** Dealing because all hands empty ***" << std::endl;
  }

  static inline void DisplayPlayerTurnMessage(
      const std::vector<std::shared_ptr<Player>>& players) {
    std::cout <<
        (players[0]->IsTurn() ? "Player 1's turn" : "Player 2's turn")
        << std::endl;
  }

  static inline void DisplayMustTrailMessage() {
    std::cout << "*** Must trail because table empty ***" << std::endl;
  }

  static inline void DisplayOwnsBuildMessage() {
    std::cout << "*** Can't trail if you own build ***" << std::endl;
  }

  static inline void DisplayAceChoiceMessage( const std::string& card) {
    std::cout << "Should " << card << " be a (1) or (14)?" << std::endl;
  }

  static inline void DisplayCardNotInHandMessage() {
    std::cout << "*** Card not in hand ***" << std::endl;
  }

  static inline void DisplayBuildCardsEmptyMessage() {
    std::cout << "*** Can't make build with no cards ***" << std::endl;
  }

  static inline void DisplayCardsNotOnTableMessage() {
    std::cout << "*** Cards not on table *** " << std::endl;
  }

  static inline void DisplayInvalidCaptureCardMessage() {
    std::cout << "*** Invalid capture card ***" << std::endl;
  }

  static inline void DisplayNoBuildsMessage() {
    std::cout << "*** No builds on table ***" << std::endl;
  }

  static inline void DisplayNonexistantBuildMessage() {
    std::cout << "*** Build with those cards doesn't exist ***" << std::endl;
  }

  static inline void DisplayCannotIncreaseMultiBuildMessage() {
    std::cout << "*** Cannot increase a multiple build ***" << std::endl;
  }

  static inline void DisplayCannotIncreaseOwnBuildMessage() {
    std::cout << "*** Cannot increase your own build ***" << std::endl;
  }

  static inline void DisplayUnequalBuildSumMessage() {
    std::cout << "*** Not the correct build sum ***" << std::endl;
  }

  static inline void DisplayUnequalCaptureSumMessage() {
    std::cout << "*** Cards don't add up to the played card ***" << std::endl;
  }

  static inline void DisplayNoCapturesMessage() {
    std::cout << "*** You haven't captured anything ***" << std::endl;
  }

  static inline void DisplayCaptureSetMessage() {
    std::cout << "Enter the cards in the set" << std::endl;
  }

  static inline void DisplayMustCaptureBuildMessage() {
    std::cout << "*** Must capture the build tied to the played card ***"
        << std::endl;
  }

  static inline void DisplayPickDifferentCardMessage() {
    std::cout << "Press 1 to pick a different card, anything else to continue"
        << std::endl;
  }

  static inline void DisplayCaptureBuildMessage() {
    std::cout << "Enter the cards in the build you want to capture"
        << std::endl;
  }

  static inline void DisplayRoundStartMessage() {
    std::cout << "*** ROUND STARTED ***" << std::endl;
  }

  static inline void DisplayFileMessage() {
    std::cout << "Enter the name of a file" << std::endl;
  }

  static inline void DisplayInvalidFileMessage() {
    std::cout << "*** File doesn't exist, exiting program ***" << std::endl;
  }

  static inline void DisplayCannotBuildMessage() {
    std::cout << "*** Can't make that build ***" << std::endl;
  }

  static inline void DisplayAutomaticCaptureBuildMessage() {
    std::cout <<
        "*** Automatically captured all of your builds matching played card ***"
        << std::endl;
  }

  static inline void DisplayDeck(const std::shared_ptr<Deck>& deck) {
    std::cout << deck->ToString() << std::endl;
  }

  static inline void DisplayAiTrailOption(const std::shared_ptr<Card>& card) {
    std::cout << "The best move is to trail " << card->ToString() << std::endl;
  }

  static void DisplayInvalidHelp() {
    std::cout << "*** Only human can ask for help ***" << std::endl;
  }

  static inline void DisplayNumCards(const std::shared_ptr<Player>& player) {
    std::cout <<
        "Player " << player->GetNumber() + 1 << " got " <<
        player->GetPile().size() << " cards" << std::endl;
  }

  static inline void DisplayNumPoints(const std::shared_ptr<Player>& player) {
    std::cout <<
        "Player " << player->GetNumber() + 1 << " has " <<
        player->GetScore() << " points" << std::endl;
  }
  
 private:
  // Private utils
  static void DisplayBuildOwners(const std::shared_ptr<Table>& table);
};

#endif