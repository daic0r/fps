//
//  packedVertex.h
//  IceCraft
//
//  Created by Matthias Gruen on 13.10.20.
//  Copyright © 2020 Matthias Gruen. All rights reserved.
//

#ifndef packedVertex_h
#define packedVertex_h

#include <math/vector.h>

namespace fps::importers {

struct packedVertex {
   math::vec2f texCoord;
   math::vec3f vertex, normal;

   bool operator<(const packedVertex& other) const {
      return vertex < other.vertex or normal < other.normal or texCoord < other.texCoord;
   }
   float length() const noexcept {
      return vertex.length() + normal.length() + texCoord.length();
   }
};

}

#endif /* packedVertex_h */
