#ifndef _VOXEL_HELPERS_H__
#define _VOXEL_HELPERS_H__

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <cstdlib>
#include <iostream>
#include <vector>
#include <tuple>

namespace plx {
namespace voxel_helpers {

std::tuple<glm::vec3, glm::vec3> calculateMinMaxDimensions(const std::vector<glm::vec3>& faces, const std::vector<glm::vec3>& vertices);

float calculateVoxelUnit(const glm::vec3& min, const glm::vec3& max, const size_t& subdivisions);

glm::vec3 calculateVoxelGridDimensions(const glm::vec3& min, const glm::vec3& max, const float& voxel_unit, const float& scale);

}
}

#endif
