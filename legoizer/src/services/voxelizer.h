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
    const float LEGO_SCALE = 1.230769; // height to width ratio (height: 9.6mm / width,depth: 7.8mm)

    struct Vertex {
        float x, y, z;
        Vertex(float x, float y, float z) : x(x), y(y), z(z) {}
    };

    void setMinMaxXYZ(std::vector<tinyobj::face_t>& faces, std::vector<glm::vec3>& vertices);
    void voxelizeFace(std::vector<std::vector<std::vector<int>>>& grid, Vertex& v1, Vertex& v2, Vertex& v3);
    void voxelToOBJ(std::vector<std::vector<std::vector<int>>>& grid, std::vector<tinyobj::face_t>& faces, std::vector<glm::vec3>& vertices);
    Vertex midPoint(Vertex& v1, Vertex& v2);
    bool samePointAndOccupied(std::vector<std::vector<std::vector<int>>>& grid, Vertex& v1, Vertex& v2, Vertex& v3);
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
