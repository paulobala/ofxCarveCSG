// Begin License:
// Copyright (C) 2006-2011 Tobias Sargeant (tobias.sargeant@gmail.com).
// All rights reserved.
//
// This file is part of the Carve CSG Library (http://carve-csg.com/)
//
// This file may be used under the terms of the GNU General Public
// License version 2.0 as published by the Free Software Foundation
// and appearing in the file LICENSE.GPL2 included in the packaging of
// this file.
//
// This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
// INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE.
// End:

#pragma once

#include <vector>

#include <carve.hpp>
#include <tag.hpp>
#include <geom.hpp>
#include <kd_node.hpp>
#include <geom3d.hpp>
#include <aabb.hpp>

namespace carve {
  namespace point {

    inline size_t PointSet::vertexToIndex_fast(const Vertex *v) const {
      return v - &vertices[0];
    }

  }
}
