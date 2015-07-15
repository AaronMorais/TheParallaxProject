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

std::vector<glm::vec3>&
LegoData::voxels()
{
    return m_voxels;
}

std::shared_ptr<tinyobj::ObjData>
LegoData::data()
{
    return m_data;
}

void
LegoData::printObj(
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

void
LegoData::printAlloy(
    std::ostream& os
    )
{
    std::vector<glm::vec3>& voxels = this->voxels();

    os << "--" << voxels.size() << " " << "number of voxels" << std::endl;

    for (size_t i = 0; i < voxels.size(); i++) {
        os << "-- Block" << i << " -> " << (int)(voxels[i].x) << "," << (int)(voxels[i].y) << "," << (int)(voxels[i].z) << std::endl;
    }

    os << "abstract sig Voxel {}" << std::endl;

    os << "one sig" << " ";

    for (size_t i = 0; i < voxels.size(); i++) {
        if (i != 0) {
            os << ", ";
        }
        os << "Block" << i;
    }

    os << " " << "extends Voxel {}" << std::endl;

    os << "fun x[] : Voxel -> Int {" << std::endl;

    for (size_t i = 0; i < voxels.size(); i++) {
        if (i != 0) {
            os << "+";
        }
        os << "Block" << i << "->" << (int)(voxels[i].x);
    }

    os << "}" << std::endl;

    os << "fun y[] : Voxel -> Int {" << std::endl;

    for (size_t i = 0; i < voxels.size(); i++) {
        if (i != 0) {
            os << "+";
        }
        os << "Block" << i << "->" << (int)(voxels[i].y);
    }

    os << "}" << std::endl;

    os << "fun z[] : Voxel -> Int {" << std::endl;

    for (size_t i = 0; i < voxels.size(); i++) {
        if (i != 0) {
            os << "+";
        }
        os << "Block" << i << "->" << (int)(voxels[i].z);
    }

    os << "}" << std::endl;

    os << "fun colour[] : Voxel -> Colour {" << std::endl;

    for (size_t i = 0; i < voxels.size(); i++) {
        if (i != 0) {
            os << "+";
        }
        os << "Block" << i << "->" << "Red";
    }

    os << "}" << std::endl;
}

}
