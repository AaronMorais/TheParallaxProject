#include "bricks.h"

namespace plx {

const std::vector<std::shared_ptr<Voxel>>&
Brick::location() const
{
    return m_location;
}

const size_t&
Brick::id() const
{
    return m_id;
}

Brick::Brick(const std::vector<std::shared_ptr<Voxel>>& location, const size_t& id)
    : m_id(id)
{
    m_location = location;
}

OneOnePlate::OneOnePlate(const std::vector<std::shared_ptr<Voxel>>& location, const size_t id)
    : Brick(location, id)
{
}

const std::vector<std::vector<glm::vec3>>
OneOnePlate::orientations()
{
    std::vector<std::vector<glm::vec3>> orientations;
    std::vector<glm::vec3> orientation_1;
    orientation_1.push_back(glm::vec3(0, 0, 0));
    orientations.push_back(orientation_1);
    return orientations;
}

const std::string
OneOnePlate::name() const
{
    return "OneOnePlate";
}

OneTwoPlate::OneTwoPlate(const std::vector<std::shared_ptr<Voxel>>& location, const size_t id)
    : Brick(location, id)
{
}

const std::vector<std::vector<glm::vec3>>
OneTwoPlate::orientations()
{
    std::vector<std::vector<glm::vec3>> orientations;
    std::vector<glm::vec3> orientation_1;
    orientation_1.push_back(glm::vec3(0, 0, 0));
    orientation_1.push_back(glm::vec3(0, 0, 1));
    orientations.push_back(orientation_1);
    std::vector<glm::vec3> orientation_2;
    orientation_2.push_back(glm::vec3(0, 0, 0));
    orientation_2.push_back(glm::vec3(1, 0, 0));
    orientations.push_back(orientation_2);
    return orientations;
}

const std::string
OneTwoPlate::name() const
{
    return "OneTwoPlate";
}

OneFourPlate::OneFourPlate(const std::vector<std::shared_ptr<Voxel>>& location, const size_t id)
    : Brick(location, id)
{
}

const std::vector<std::vector<glm::vec3>>
OneFourPlate::orientations()
{
    std::vector<std::vector<glm::vec3>> orientations;
    std::vector<glm::vec3> orientation_1;
    orientation_1.push_back(glm::vec3(0, 0, 0));
    orientation_1.push_back(glm::vec3(0, 0, 1));
    orientation_1.push_back(glm::vec3(0, 0, 2));
    orientation_1.push_back(glm::vec3(0, 0, 3));
    orientations.push_back(orientation_1);
    std::vector<glm::vec3> orientation_2;
    orientation_2.push_back(glm::vec3(0, 0, 0));
    orientation_2.push_back(glm::vec3(1, 0, 0));
    orientation_2.push_back(glm::vec3(2, 0, 0));
    orientation_2.push_back(glm::vec3(3, 0, 0));
    orientations.push_back(orientation_2);
    return orientations;
}

const std::string
OneFourPlate::name() const
{
    return "OneFourPlate";
}

}
