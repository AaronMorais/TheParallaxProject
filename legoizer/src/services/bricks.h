#ifndef _BRICKS_H__
#define _BRICKS_H__

#include "lego_data.h"
#include <vector>

namespace plx {

struct Voxel {
    glm::vec3 position;
    size_t index;
    Voxel(const glm::vec3& position, const size_t& index);
};

class Brick {
public:
    Brick(const std::vector<std::shared_ptr<Voxel>>& location, const size_t& id);
    const std::vector<std::shared_ptr<Voxel>>& location() const;
    const size_t& id() const;

    class Factory {
    public:
        static std::shared_ptr<Brick> create(const std::string brick_type, const std::vector<std::shared_ptr<Voxel>>& location, const size_t& id);
    };

protected:
    std::vector<std::shared_ptr<Voxel>> m_location;
    size_t m_id;
};

class OneOnePlate : public Brick {
public:
    OneOnePlate(const std::vector<std::shared_ptr<Voxel>>& location, const size_t id);

    static constexpr const char* name = "OneOnePlate";

    static const std::vector<std::vector<glm::vec3>> orientations();
    static const std::vector<std::vector<glm::vec3>> aboveConnections();
    static const std::vector<std::vector<glm::vec3>> belowConnections();
};

class OneTwoPlate : public Brick {
public:
    OneTwoPlate(const std::vector<std::shared_ptr<Voxel>>& location, const size_t id);

    static constexpr const char* name = "OneTwoPlate";

    static const std::vector<std::vector<glm::vec3>> orientations();
    static const std::vector<std::vector<glm::vec3>> aboveConnections();
    static const std::vector<std::vector<glm::vec3>> belowConnections();
};

class OneFourPlate : public Brick {
public:
    OneFourPlate(const std::vector<std::shared_ptr<Voxel>>& location, const size_t id);

    static constexpr const char* name = "OneFourPlate";

    static const std::vector<std::vector<glm::vec3>> orientations();
    static const std::vector<std::vector<glm::vec3>> aboveConnections();
    static const std::vector<std::vector<glm::vec3>> belowConnections();
};

}

#endif
