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

std::vector<std::vector<size_t>>&
LegoData::brick_locations()
{
    return m_brick_locations;
}

std::vector<std::vector<size_t>>&
LegoData::brick_conflicts()
{
    return m_brick_conflicts;
}

std::vector<std::vector<size_t>>&
LegoData::brick_connections()
{
    return m_brick_connections;
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
LegoData::printAlloyAsVoxels(
    std::ostream& os
    )
{
    printAlloyVoxels(os);
    printAlloyColours(os);
}

void
LegoData::printAlloyAsBricks(
    std::ostream& os
    )
{
    printAlloyVoxels(os);
    printAlloyLocations(os);
    printAlloyConflicts(os);
    printAlloyConnections(os);
}

void
LegoData::printAlloyColours(
    std::ostream& os
    )
{
    std::vector<glm::vec3>& voxels = this->voxels();

    os << "fun colour[] : Voxel -> Colour {" << std::endl;

    for (size_t i = 0; i < voxels.size(); i++) {
        if (i != 0) {
            os << "+";
        }
        os << "Voxel" << i << "->" << "Red";
    }

    os << "}" << std::endl;
}
void
LegoData::printAlloyVoxels(
    std::ostream& os
    )
{
    std::vector<glm::vec3>& voxels = this->voxels();

    os << "--" << "number of voxels: " << voxels.size() << std::endl;

    for (size_t i = 0; i < voxels.size(); i++) {
        os << "-- Voxel" << i << " -> " << (size_t)(voxels[i].x) << "," << (size_t)(voxels[i].y) << "," << (size_t)(voxels[i].z) << std::endl;
    }

    os << "abstract sig Voxel {}" << std::endl;

    os << "one sig" << " ";

    for (size_t i = 0; i < voxels.size(); i++) {
        if (i != 0) {
            os << ", ";
        }
        os << "Voxel" << i;
    }

    os << " " << "extends Voxel {}" << std::endl;

    os << "fun x[] : Voxel -> Int {" << std::endl;

    for (size_t i = 0; i < voxels.size(); i++) {
        if (i != 0) {
            os << "+";
        }
        os << "Voxel" << i << "->" << (size_t)(voxels[i].x);
    }

    os << "}" << std::endl;

    os << "fun y[] : Voxel -> Int {" << std::endl;

    for (size_t i = 0; i < voxels.size(); i++) {
        if (i != 0) {
            os << "+";
        }
        os << "Voxel" << i << "->" << (size_t)(voxels[i].y);
    }

    os << "}" << std::endl;

    os << "fun z[] : Voxel -> Int {" << std::endl;

    for (size_t i = 0; i < voxels.size(); i++) {
        if (i != 0) {
            os << "+";
        }
        os << "Voxel" << i << "->" << (size_t)(voxels[i].z);
    }

    os << "}" << std::endl;
}

void
LegoData::printAlloyLocations(
    std::ostream& os
    )
{
    std::vector<std::vector<size_t>>& locations = this->brick_locations();

    os << "--" << "number of locations: " << locations.size() << std::endl;

    os << "abstract sig BrickLocation {}" << std::endl;

    os << "one sig" << " ";

    for (size_t i = 0; i < locations.size(); i++) {
        if (i != 0) {
            os << ", ";
        }
        os << "BrickLocation" << i;
    }

    os << " " << "extends BrickLocation {}" << std::endl;


    os << "fun voxels[] : BrickLocation -> Voxel {" << std::endl;

    for (size_t i = 0; i < locations.size(); i++) {
        if (i != 0) {
            os << "+" << std::endl;
        }
        os << "BrickLocation" << i << "->(";
        for (size_t j = 0; j < locations[i].size(); j++) {
            if (j != 0) {
                os << "+";
            }
            os << "Voxel" << (size_t)(locations[i][j]);
        }
        os << ")";
    }

    os << "}" << std::endl;
}

void
LegoData::printAlloyConflicts(
    std::ostream& os
    )
{
    std::vector<std::vector<size_t>>& conflicts = this->brick_conflicts();

    os << "fun conflict[] : BrickLocation -> set BrickLocation {" << std::endl;

    for (size_t i = 0; i < conflicts.size(); i++) {
        if (conflicts[i].empty()) {
            continue;
        }
        if (i != 0) {
            os << "+" << std::endl;
        }
        os << "BrickLocation" << i << "->(";
        for (size_t j = 0; j < conflicts[i].size(); j++) {
            if (j != 0) {
                os << "+";
            }
            os << "BrickLocation" << (size_t)(conflicts[i][j]);
        }
        os << ")";
    }

    os << "}" << std::endl;
}


void
LegoData::printAlloyConnections(
    std::ostream& os
    )
{
    std::vector<std::vector<size_t>>& connections = this->brick_connections();

    os << "fun connectedLocation[] : BrickLocation -> set BrickLocation {" << std::endl;

    for (size_t i = 0; i < connections.size(); i++) {
        if (connections[i].empty()) {
            continue;
        }
        if (i != 0) {
            os << "+" << std::endl;
        }
        os << "BrickLocation" << i << "->(";
        for (size_t j = 0; j < connections[i].size(); j++) {
            if (j != 0) {
                os << "+";
            }
            os << "BrickLocation" << (size_t)(connections[i][j]);
        }
        os << ")";
    }

    os << "}" << std::endl;
}

}
