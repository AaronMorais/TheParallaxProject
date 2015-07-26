#include "glm_helpers.h"

#include <sstream>

namespace plx {
namespace glm_helpers {

const glm::vec3
midpoint(
    const glm::vec3& v1,
    const glm::vec3& v2
    )
{
    return glm::vec3(
        (v1.x + v2.x)/ 2,
        (v1.y + v2.y)/ 2,
        (v1.z + v2.z)/ 2
    );
}

std::string
to_int_string(const glm::vec3& v)
{
    std::stringstream ss;
    ss << "(" << (int)v.x << "," << (int)v.y << "," << (int)v.z << ")";
    return ss.str();
}

std::ostream& operator<< (std::ostream& out, const std::vector<glm::vec3>& list)
{
    for (const glm::vec3& v : list) {
        out << '[' << glm::to_string(v) << "," << "]";
    }
    return out;
}

std::ostream& operator<< (std::ostream& out, const std::vector<std::vector<std::vector<size_t>>>& grid)
{
    for (size_t i = 0; i < grid.size(); ++i) {
        for (size_t j = 0; j < grid[i].size(); ++j) {
            for (size_t k = 0; k < grid[i][j].size(); ++k) {
                out << i << "," << j << "," << k << " = " << grid[i][j][k] << std::endl;
            }
        }
    }
    return out;
}

}
}
