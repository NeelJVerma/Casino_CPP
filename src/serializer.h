/*******************************
 * Name: Neel Verma            *
 * Project: Casino CPP         *
 * Class: CMPS 366-01          *
 * Due Date: 10/2/18           *
 *******************************/

#ifndef _SERIALIZER_H_
#define _SERIALIZER_H_

#include <string>
#include <fstream>
#include "round.h"

class Round;

class Serializer {
 public:
  // Public utils
  static inline unsigned GetIntFromString(const std::string& int_as_string) {
    return std::stoi(int_as_string);
  }

  static void SaveRoundState(const std::string& file_name, Round* round);
  static std::vector<std::string> GetDeckFromFile(const std::string& file_name);
  static std::ifstream OpenLoadFile(const std::string& file_name);
  static unsigned GetRoundNumFromFile(std::ifstream& in_file);
  static unsigned GetPlayerScoreFromFile(std::ifstream& file);
  static std::vector<std::shared_ptr<Card>> GetCardsFromFile(
      std::ifstream& in_file);

  static std::vector<std::shared_ptr<Card>> GetLooseCardsFromFile(
      std::ifstream& in_file);

  static std::vector<std::shared_ptr<Build>> GetBuildsFromFile(
      std::ifstream& in_file);
      
  static std::shared_ptr<Deck> GetDeckFromFile(std::ifstream& in_file);
  static unsigned GetNextPlayerFromFile(std::ifstream& in_file);
};

#endif