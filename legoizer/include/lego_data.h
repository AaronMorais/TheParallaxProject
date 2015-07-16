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

    void printObj(std::ostream& os);
    void printAlloy(std::ostream& os);

private:
    std::vector<glm::vec3> m_faces;
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec3> m_voxels;
    std::shared_ptr<tinyobj::ObjData> m_data;
};

}

#endif
