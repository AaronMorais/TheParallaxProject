#ifndef _GLM_HELPERS_H__
#define _GLM_HELPERS_H__

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <iostream>
#include <vector>

namespace plx {
namespace glm_helpers {

const glm::vec3 midpoint(const glm::vec3& v1, const glm::vec3& v2);

std::ostream& operator<< (std::ostream& out, const std::vector<glm::vec3>& list);

std::ostream& operator<< (std::ostream& out, const std::vector<std::vector<std::vector<size_t>>>& grid);

}
}

#endif
