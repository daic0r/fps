#ifndef MODEL_H
#define MODEL_H

#include "../math/vector.h"

namespace fps::rendering {
   using namespace fps::math;

   struct model {
      std::vector<vertex> vertices_;
      std::vector<std::size_t> indices_;
   };

}

#endif
