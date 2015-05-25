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
