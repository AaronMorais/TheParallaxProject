#ifndef _VOXELIZER_H__
#define _VOXELIZER_H__


#include "obj_data.h"
#include <vector>
#include <memory>

class Voxelizer {
public:
    Voxelizer();
    std::shared_ptr<ObjData> Process(std::shared_ptr<ObjData> data);

private:
    void addFaces(tinyobj::face_t const & face, std::vector<glm::vec3>& v, std::vector<tinyobj::face_t>& f);
    void setMinMaxXYZ(std::vector<tinyobj::face_t>& faces, std::vector<glm::vec3>& vertices);
    void voxelizeFace(std::vector<std::vector<std::vector<int>>>& grid, glm::vec3& v1, glm::vec3& v2, glm::vec3& v3);
    void voxelToOBJ(std::vector<std::vector<std::vector<int>>>& grid, std::vector<tinyobj::face_t>& faces, std::vector<glm::vec3>& vertices);
    glm::vec3 midPoint(glm::vec3& v1, glm::vec3& v2);
    bool samePointAndOccupied(std::vector<std::vector<std::vector<int>>>& grid, glm::vec3& v1, glm::vec3& v2, glm::vec3& v3);
    void fillShell(std::vector<std::vector<std::vector<int>>>& grid);

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

#endif
