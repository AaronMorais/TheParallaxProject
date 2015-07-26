#ifndef _BRICKS_H__
#define _BRICKS_H__

#include "lego_data.h"
#include <vector>

namespace plx {

struct Voxel {
    glm::vec3 position;
    size_t grid_index;

    Voxel(const glm::vec3& position, const size_t& index) {
        this->position = position;
        this->grid_index = index;
    }
};

class Brick {
public:
    Brick(const std::vector<std::shared_ptr<Voxel>>& location, const size_t& id);
    const std::vector<std::shared_ptr<Voxel>>& location() const;
    const size_t& id() const;

    virtual const std::string name() const = 0;

    const std::vector<std::vector<glm::vec3>>& orientations();
protected:
    std::vector<std::shared_ptr<Voxel>> m_location;
    size_t m_id;
};

class OneOnePlate : public Brick {
public:
    OneOnePlate(const std::vector<std::shared_ptr<Voxel>>& location, const size_t id);
    const std::string name() const;

    static const std::vector<std::vector<glm::vec3>> orientations();
    static const std::vector<std::vector<glm::vec3>> aboveConnectionsByOrientations();
    static const std::vector<std::vector<glm::vec3>> belowConnectionsByOrientations();
};

class OneTwoPlate : public Brick {
public:
    OneTwoPlate(const std::vector<std::shared_ptr<Voxel>>& location, const size_t id);
    const std::string name() const;

    static const std::vector<std::vector<glm::vec3>> orientations();
    static const std::vector<std::vector<glm::vec3>> aboveConnectionsByOrientations();
    static const std::vector<std::vector<glm::vec3>> belowConnectionsByOrientations();
};

class OneFourPlate : public Brick {
public:
    OneFourPlate(const std::vector<std::shared_ptr<Voxel>>& location, const size_t id);
    const std::string name() const;

    static const std::vector<std::vector<glm::vec3>> orientations();
    static const std::vector<std::vector<glm::vec3>> aboveConnectionsByOrientations();
    static const std::vector<std::vector<glm::vec3>> belowConnectionsByOrientations();
};

}

#endif
