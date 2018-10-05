/*******************************
 * Name: Neel Verma            *
 * Project: Casino CPP         *
 * Class: CMPS 366-01          *
 * Due Date: 10/2/18           *
 *******************************/

#include "build.h"

/**
 * Description: Serializes the build to a string.
 * Parameters: None.
 * Returns: The string format of the build.
 */

std::string Build::ToString() const {
  std::string build = "[ ";

  for (unsigned i = 0; i < build_.size(); i++) {
    build += '[';

    for (unsigned j = 0; j < build_[i].size(); j++) {
      build += build_[i][j]->ToString();

      if (j != build_[i].size() - 1) {
        build += ' ';
      }
    }

    build += "] ";
  }

  build += "] ";

  return build;
}