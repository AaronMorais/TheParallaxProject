#ifndef _VOXELIZER_H__
#define _VOXELIZER_H__

#include "obj_data.h"
#include <vector>
#include <memory>

namespace plx {

class Voxelizer {
public:
    Voxelizer(std::shared_ptr<tinyobj::ObjData>);
    void process();

private:
    const float LEGO_SCALE = 1.230769; // height to width ratio (height: 9.6mm / width,depth: 7.8mm)

    void setMinMaxXYZ(std::vector<glm::vec3>& faces, std::vector<glm::vec3>& vertices);
    void voxelizeFace(std::vector<std::vector<std::vector<int>>>& grid, glm::vec3& v1, glm::vec3& v2, glm::vec3& v3);
    void voxelToOBJ(std::vector<std::vector<std::vector<int>>>& grid, std::vector<glm::vec3>& faces, std::vector<glm::vec3>& vertices);
    bool samePointAndOccupied(std::vector<std::vector<std::vector<int>>>& grid, glm::vec3& v1, glm::vec3& v2, glm::vec3& v3);
    void fillShell(std::vector<std::vector<std::vector<int>>>& grid);

    static glm::vec3 midpoint(glm::vec3& v1, glm::vec3& v2);

    float m_maxX;
    float m_maxY;
    float m_maxZ;
    float m_minX;
    float m_minY;
    float m_minZ;

    float m_width;
    float m_height;
    float m_depth;

    float m_subdivisions;
    float m_unit;
};

}

#endif
