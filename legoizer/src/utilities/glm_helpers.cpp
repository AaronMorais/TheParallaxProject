#include "glm_helpers.h"

namespace plx {
namespace glm_helpers {

glm::vec3
midpoint(
    glm::vec3& v1,
    glm::vec3& v2
    )
{
    return glm::vec3(
        (v1.x + v2.x)/ 2,
        (v1.y + v2.y)/ 2,
        (v1.z + v2.z)/ 2
    );
}

}
}
