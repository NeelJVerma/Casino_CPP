#ifndef _CAPTURE_NODE_H_
#define _CAPTURE_NODE_H_

#include <vector>

class CaptureNode {
 public:
  // Delete copy constructor and assignment operator
  CaptureNode(const CaptureNode& capture_node) = delete;
  CaptureNode& operator=(const CaptureNode& capture_node) = delete;

  // Constructors
  CaptureNode() : score_(0) {}

  // Accessors
  inline unsigned GetScore() const { return score_; }
  inline unsigned GetPlayedCardIndex() const { return played_card_index_; }
  inline std::vector<std::vector<unsigned>> GetSetIndices() const {
    return set_indices_;
  }

  inline std::vector<unsigned> GetLooseCardIndices() const {
    return loose_card_indices_;
  }

  inline std::vector<unsigned> GetBuildIndices() const {
    return build_indices_;
  }

  // Mutators
  inline void AddToScore(const unsigned& to_add) { score_ += to_add; }
  inline void SetPlayedCardIndex(const unsigned& played_card_index) {
    played_card_index_ = played_card_index;
  }

  inline void AddSetIndices(const std::vector<unsigned>& set_indices) {
    set_indices_.push_back(set_indices);
  }

  inline void SetLooseCardIndices(
      const std::vector<unsigned>& loose_card_indices) {
    loose_card_indices_ = loose_card_indices;
  }

  inline void SetBuildIndices(const std::vector<unsigned>& build_indices) {
    build_indices_ = build_indices;
  }

 private:
  unsigned score_;
  unsigned played_card_index_;
  std::vector<std::vector<unsigned>> set_indices_;
  std::vector<unsigned> loose_card_indices_;
  std::vector<unsigned> build_indices_;
};

#endif