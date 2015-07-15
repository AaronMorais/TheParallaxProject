#include "lego_data.h"

namespace plx {

LegoData::LegoData(std::shared_ptr<tinyobj::ObjData> data)
{
    m_data = data;
}

std::vector<glm::vec3>& LegoData::faces()
{
    return m_faces;
}

std::vector<glm::vec3>& LegoData::vertices()
{
    return m_vertices;
}

std::shared_ptr<tinyobj::ObjData> LegoData::data()
{
    return m_data;
}

}
