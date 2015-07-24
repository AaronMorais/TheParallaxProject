#include "voxel_helpers.h"

namespace plx {

std::tuple<glm::vec3, glm::vec3>
voxel_helpers::calculateMinMaxDimensions(
    const std::vector<glm::vec3>& faces,
    const std::vector<glm::vec3>& vertices)
{
    glm::vec3 min(INT_MAX, INT_MAX, INT_MAX);
    glm::vec3 max(INT_MIN, INT_MIN, INT_MIN);

    for (const glm::vec3& face : faces)
    {
        std::vector<glm::vec3> face_vertices;
        face_vertices.push_back(vertices[face.x]);
        face_vertices.push_back(vertices[face.y]);
        face_vertices.push_back(vertices[face.z]);

        for (glm::vec3& vertex : face_vertices) {

            if (vertex.x > max.x) {
                max.x = vertex.x;
            }
            if (vertex.y > max.y) {
                max.y = vertex.y;
            }
            if (vertex.z > max.z) {
                max.z = vertex.z;
            }

            if (vertex.x < min.x) {
                min.x = vertex.x;
            }
            if (vertex.y < min.y) {
                min.y = vertex.y;
            }
            if (vertex.z < min.z) {
                min.z = vertex.z;
            }
        }
    }

    return std::make_tuple<glm::vec3&, glm::vec3&>(min, max);
}


float
voxel_helpers::calculateVoxelUnit(
    const glm::vec3& min,
    const glm::vec3& max,
    const size_t& subdivisions
    )
{
    float minimum_side = std::min(std::min((max.x - min.x), (max.y - min.y)), (max.z - min.z));
    float voxel_unit = minimum_side / subdivisions;
    std::cerr << "voxel_unit: " << voxel_unit << std::endl;
    return voxel_unit;
}

glm::vec3
voxel_helpers::calculateVoxelGridDimensions(
    const glm::vec3& min,
    const glm::vec3& max,
    const float& voxel_unit,
    const float& scale
    )
{
    float width = std::ceil((max.x - min.x) / voxel_unit) + 1;
    float height = std::ceil((max.y - min.y) / (voxel_unit * scale)) + 1;
    float depth = std::ceil((max.z - min.z) / voxel_unit) + 1;
    std::cerr << "size (whd): " << width << "," << height << "," << depth << std::endl;
    return glm::vec3(width, height, depth);
}

}
