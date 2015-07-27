#ifndef _LEGO_DATA_H__
#define _LEGO_DATA_H__

#include "obj_data.h"
#include <ostream>

namespace plx {

class LegoData {
public:
    LegoData(std::shared_ptr<tinyobj::ObjData> data);

    std::vector<glm::vec3>& faces();
    std::vector<glm::vec3>& vertices();
    std::vector<glm::vec3>& voxels();
    std::shared_ptr<tinyobj::ObjData> data();
    std::vector<std::vector<size_t>>& brick_locations();
    std::vector<std::vector<size_t>>& brick_conflicts();
    std::vector<std::vector<size_t>>& brick_connections();

    void printObj(std::ostream& os);
    void printAlloyAsVoxels(std::ostream& os);
    void printAlloyAsBricks(std::ostream& os);

private:
    std::vector<glm::vec3> m_faces;
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec3> m_voxels;
    std::shared_ptr<tinyobj::ObjData> m_data;
    std::vector<std::vector<size_t>> m_brick_locations;
    std::vector<std::vector<size_t>> m_brick_conflicts;
    std::vector<std::vector<size_t>> m_brick_connections;
    void printAlloyVoxels(std::ostream& os);
    void printAlloyColours(std::ostream& os);
    void printAlloyLocations(std::ostream& os);
    void printAlloyConflicts(std::ostream& os);
    void printAlloyConnections(std::ostream& os);
};

}

#endif
