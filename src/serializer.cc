/*******************************
 * Name: Neel Verma            *
 * Project: Casino CPP         *
 * Class: CMPS 366-01          *
 * Due Date: 10/2/18           *
 *******************************/

#include <stack>
#include <algorithm>
#include "serializer.h"
#include "sanitizer.h"
#include "gui.h"

/**
 * Description: Saves the current state of the round.
 * Parameters: const std::string& file_name: The file name to save to.
 * Round* round: The round state (regular pointer passed because it is *this).
 * Returns: Nothing.
 */

void Serializer::SaveRoundState(const std::string& file_name, Round* round) {
  std::ofstream out_file(file_name); 
  out_file << round->GetRoundData();
  out_file.close();
}

/**
 * Description: Reads the deck from the file (Only a deck file).
 * Parameters: const std::string& file_name: The file name to read from.
 * Returns: A vector of serialized cards.
 */

std::vector<std::string> Serializer::GetDeckFromFile(
    const std::string& file_name) {
  std::ifstream in_file(file_name);

  if (!in_file.good()) {
    GUI::DisplayInvalidFileMessage();
    exit(0);
  }

  std::string card;
  std::vector<std::string> cards;

  while (std::getline(in_file, card)) {
    cards.insert(cards.begin(), card);
  }

  in_file.close();

  return cards;
}

/**
 * Description: Opens the file to read from.
 * Parameters: const std::string& file_name: The file name to read from.
 * Returns: A stream object.
 */

std::ifstream Serializer::OpenLoadFile(const std::string& file_name) {
  std::ifstream in_file(file_name);

  if (!in_file.good()) {
    GUI::DisplayInvalidFileMessage();
    exit(0);
  }

  return in_file;
}

/**
 * Description: Reads the round number from the file.
 * Parameters: std::ifstream& in_file: The stream object.
 * Returns: The round number.
 */

unsigned Serializer::GetRoundNumFromFile(std::ifstream& in_file) {
  std::string line;
  std::getline(in_file, line);
  unsigned split_index = line.find(':');

  return GetIntFromString(line.substr(split_index + 2));
}

/**
 * Description: Reads the score of a player from the file.
 * Parameters: std::ifstream& in_file: The stream object.
 * Returns: THe score.
 */

unsigned Serializer::GetPlayerScoreFromFile(std::ifstream& in_file) {
  unsigned max_line_count = 1;
  unsigned line_count = 0;
  std::string line;

  while (std::getline(in_file, line)) {
    if (line_count == max_line_count) {
      break;
    }

    line_count++;
  }

  unsigned split_index = line.find(':');
  
  return GetIntFromString(line.substr(split_index + 2));
}

/**
 * Description: Reads a set of cards from the file.
 * Parameters: std::ifstream& in_file: The stream object.
 * Returns: A vector of cards.
 */

std::vector<std::shared_ptr<Card>> Serializer::GetCardsFromFile(
    std::ifstream& in_file) {
  std::string line;
  std::getline(in_file, line);
  unsigned split_index = line.find(':');
  std::vector<std::string> str_cards =
      Sanitizer::TokenizeInput(line.substr(split_index + 2));
  std::vector<std::shared_ptr<Card>> cards;

  for (unsigned i = 0; i < str_cards.size(); i++) {
    cards.push_back(std::shared_ptr<Card>(new Card(str_cards[i])));
  }

  return cards;
}

/**
 * Description: Reads builds from the file.
 * Parameters: std::ifstream& in_file: The stream object.
 * Returns: A vector of builds.
 */

std::vector<std::shared_ptr<Build>> Serializer::GetBuildsFromFile(
    std::ifstream& in_file) {
  std::vector<std::shared_ptr<Build>> builds;
  std::string line;

  while (true) {
    if ((char) in_file.peek() == 'D') {
      break;
    }
    
    std::getline(in_file, line);
    std::stack<unsigned> bracket_indices;
    unsigned split_index = line.find(':');
    unsigned end_index = line.find_last_of(']');
    std::string actual_build =
        line.substr(split_index + 2, end_index - split_index - 1);
    std::shared_ptr<Build> build(new Build);
    build->SetOwnerIndex(line.substr(end_index + 2) == "Human" ? 1 : 0);

    for (unsigned i = 1; i < actual_build.length() - 1; i++) {
      if (actual_build[i] == '[') {
        bracket_indices.push(i);
      } else if (actual_build[i] == ']') {
        unsigned top_index = bracket_indices.top();
        std::vector<std::string> build_str =
            Sanitizer::TokenizeInput(
                actual_build.substr(top_index + 1, i - top_index - 1));
        std::vector<std::shared_ptr<Card>> cards;
        unsigned build_sum = 0;

        for (unsigned i = 0; i < build_str.size(); i++) {
          cards.push_back(std::shared_ptr<Card>(new Card(build_str[i])));
          build_sum += cards[i]->GetValue();
        }

        build->AddToBuild(cards);
        build->SetBuildSum(build_sum);
        bracket_indices.pop();
      }
    }

    builds.push_back(build);
  }

  return builds;
}

/**
 * Description: Reads a loose cards from the file.
 * Parameters: std::ifstream& in_file: The stream object.
 * Returns: A vector of cards.
 */

std::vector<std::shared_ptr<Card>> Serializer::GetLooseCardsFromFile(
    std::ifstream& in_file) {
  std::vector<std::shared_ptr<Card>> loose_cards;
  std::string line;
  std::getline(in_file, line);
  std::size_t end_build_index = line.find_last_of(']');
  std::vector<std::string> str_loose_cards =
      Sanitizer::TokenizeInput(line.substr(end_build_index + 1));
  unsigned start_iter = (end_build_index == std::string::npos ? 1 : 0);

  for (unsigned i = start_iter; i < str_loose_cards.size(); i++) {
    loose_cards.push_back(std::shared_ptr<Card>(new Card(str_loose_cards[i])));
  }

  return loose_cards;
}

/**
 * Description: Reads a deck of cards from the file (from full save state).
 * Parameters: std::ifstream& in_file: The stream object.
 * Returns: A deck object.
 */

std::shared_ptr<Deck> Serializer::GetDeckFromFile(std::ifstream& in_file) {
  std::vector<std::shared_ptr<Card>> cards = GetCardsFromFile(in_file);
  std::reverse(cards.begin(), cards.end());

  return std::shared_ptr<Deck>(new Deck(cards));
}

/**
 * Description: Reads the next player from the file.
 * Parameters: std::ifstream& in_file: The stream object.
 * Returns: The index of the next player.
 */

unsigned Serializer::GetNextPlayerFromFile(std::ifstream& in_file) {
  std::string line;
  std::getline(in_file, line);
  unsigned split_index = line.find(':');

  return (line.substr(split_index + 2) == "Human" ? 1 : 0);
}