#ifndef _BUILD_NODE_H_
#define _BUILD_NODE_H_

#include <vector>

class BuildNode {
 public:
  // Public enums
  enum Type {
    kMake = 1,
    kAdd,
    kIncrease
  };

  // Delete copy constructor and assignment operator
  BuildNode(const BuildNode& build_node) = delete;
  BuildNode& operator=(const BuildNode& build_node) = delete;

  // Constructors
  BuildNode() : score_(0) {}

  // Accessors
  inline unsigned GetScore() const { return score_; }
  inline unsigned GetPlayedCardIndex() const { return played_card_index_; }
  inline unsigned GetType() const { return type_; }
  inline std::vector<unsigned> GetLooseCardIndices() const {
    return loose_card_indices_;
  }

  // Mutators
  inline unsigned GetBuildIndex() const { return build_index_; }
  inline void SetPlayedCardIndex(const unsigned& played_card_index) {
    played_card_index_ = played_card_index;
  }

  inline void SetType(const unsigned& type) { type_ = type; }
  inline void SetLooseCardIndices(
      const std::vector<unsigned>& loose_card_indices) {
    loose_card_indices_ = loose_card_indices;
  }

  inline void SetBuildIndex(const unsigned& build_index) {
    build_index_ = build_index;
  }

  inline void AddToScore(const unsigned& to_add) { score_ += to_add; }

 private:
  unsigned type_;
  unsigned score_;
  unsigned played_card_index_;
  std::vector<unsigned> loose_card_indices_;
  unsigned build_index_;
};

#endif