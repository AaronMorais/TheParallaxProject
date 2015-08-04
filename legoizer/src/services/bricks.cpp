#include "bricks.h"

namespace plx {

Voxel::Voxel(const glm::vec3& position, const size_t& index) {
    this->position = position;
    this->index = index;
}

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

std::shared_ptr<Brick>
OneOnePlate::Factory::create(
    const std::vector<std::shared_ptr<Voxel>>& location,
    const size_t& id
    )
{
    return std::make_shared<OneOnePlate>(std::move(location), id);
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

std::shared_ptr<Brick>
OneTwoPlate::Factory::create(
    const std::vector<std::shared_ptr<Voxel>>& location,
    const size_t& id
    )
{
    return std::make_shared<OneTwoPlate>(std::move(location), id);
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

std::shared_ptr<Brick>
OneFourPlate::Factory::create(
    const std::vector<std::shared_ptr<Voxel>>& location,
    const size_t& id
    )
{
    return std::make_shared<OneFourPlate>(std::move(location), id);
}

}
