#include "lego_data.h"

namespace plx {

LegoData::LegoData(std::shared_ptr<tinyobj::ObjData> data)
{
    m_data = data;
}

std::vector<glm::vec3>&
LegoData::faces()
{
    return m_faces;
}

std::vector<glm::vec3>&
LegoData::vertices()
{
    return m_vertices;
}

std::shared_ptr<tinyobj::ObjData>
LegoData::data()
{
    return m_data;
}

void
LegoData::print(
    std::ostream& os
    )
{
    os << "g" << std::endl;

    for (glm::vec3& vertex : this->vertices()) {
        os << "v " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
    }

    for (glm::vec3& face : this->faces()) {
        os << "f " << face.x << " " << face.y << " " << face.z << std::endl;
    }
}

}
