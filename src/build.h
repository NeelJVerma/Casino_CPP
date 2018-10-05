/*******************************
 * Name: Neel Verma            *
 * Project: Casino CPP         *
 * Class: CMPS 366-01          *
 * Due Date: 10/2/18           *
 *******************************/

#ifndef _BUILD_H_
#define _BUILD_H_

#include <vector>
#include <memory>
#include "card.h"

class Build {
 public:
  // Delete copy constructor and assignment operator
  Build(const Build& build) = delete;
  Build& operator=(const Build& build) = delete;

  // Constructors
  Build() = default;

  // Accessors
  inline unsigned GetOwnerIndex() const { return owner_index_; }
  inline unsigned GetBuildSum() const { return build_sum_; }

  // Mutators
  inline void SetOwnerIndex(const unsigned& owner_index) {
    owner_index_ = owner_index;
  }

  inline void SetBuildSum(const unsigned& build_sum) {
    build_sum_ = build_sum;
  }

  // Public utils
  inline void AddToBuild(const std::vector<std::shared_ptr<Card>>& build) {
    build_.push_back(build);
  }

  inline unsigned GetBuildSize() const { return build_.size(); }
  inline bool IsMultipleBuild() const { return build_.size() > 1; }
  std::vector<std::shared_ptr<Card>> GetBuildAt(const unsigned& index) const {
    return build_[index];
  }

  void AddToSingleBuild(const std::shared_ptr<Card>& card) {
    build_[0].push_back(card);
  }

  std::string ToString() const;

 private:
  std::vector<std::vector<std::shared_ptr<Card>>> build_;
  unsigned owner_index_;
  unsigned build_sum_;
};

#endif