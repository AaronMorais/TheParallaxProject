#include "preprocessor.h"
#include "glm/ext.hpp"

#include <iostream>

namespace plx {

Preprocessor::Preprocessor(std::shared_ptr<plx::LegoData> data) :
    m_data(data)
{
}

void
Preprocessor::process()
{
    std::vector<glm::vec3>& voxels = m_data->voxels();

    glm::vec3 min(INT_MAX, INT_MAX, INT_MAX);
    glm::vec3 max(INT_MIN, INT_MIN, INT_MIN);

    for (const glm::vec3& voxel : voxels) {
        if (voxel.x > max.x) {
            max.x = voxel.x;
        }
        if (voxel.y > max.y) {
            max.y = voxel.y;
        }
        if (voxel.z > max.z) {
            max.z = voxel.z;
        }

        if (voxel.x < min.x) {
            min.x = voxel.x;
        }
        if (voxel.y < min.y) {
            min.y = voxel.y;
        }
        if (voxel.z < min.z) {
            min.z = voxel.z;
        }
    }

    glm::vec3 dimensions((max.x - min.x) + 1, (max.y - min.y) + 1, (max.z - min.z) + 1);

    std::cout << "min:" << glm::to_string(min) << std::endl;
    std::cout << "max:" << glm::to_string(max) << std::endl;
    std::cout << "dim:" << glm::to_string(dimensions) << std::endl;
    std::cout << "voxels: " << voxels.size() << std::endl;

    std::vector<std::vector<std::vector<size_t>>> grid =
        std::vector<std::vector<std::vector<size_t>>>(dimensions.x + 1,
        std::vector<std::vector<size_t>>(dimensions.y + 1,
        std::vector<size_t>(dimensions.z + 1, 0)));

    for (const glm::vec3& voxel : voxels) {
        grid[(size_t)(voxel.x)][(size_t)(voxel.y)][(size_t)(voxel.z)] = 1;
    }


    std::vector<std::vector<std::vector<glm::vec3>>> all_orientations;
    all_orientations.push_back(OneOnePlate::orientations());
    all_orientations.push_back(OneTwoPlate::orientations());

    std::vector<std::vector<glm::vec3>> brick_locations;
    for (const glm::vec3& voxel : voxels) {

        for (const std::vector<std::vector<glm::vec3>>& brick_orientations : all_orientations) {

            for (const std::vector<glm::vec3>& orientation : brick_orientations) {

                std::vector<glm::vec3> real_location;
                for (const glm::vec3& voxel_position : orientation) {
                    real_location.push_back(glm::vec3(voxel.x + voxel_position.x, voxel.y + voxel_position.y, voxel.z + voxel_position.z));
                }

                bool fits = true;
                for (const glm::vec3& voxel_position : real_location) {
                    if (grid[voxel_position.x][voxel_position.y][voxel_position.z] == 0) {
                        fits = false;
                    }
                }

                if (fits) {
                    brick_locations.push_back(real_location);
                }
            }
        }
    }

    for (const std::vector<glm::vec3>& pos : brick_locations) {
        for (const glm::vec3& p : pos) {
            std::cout << "(" << (size_t)p.x << "," << (size_t)p.y << "," << (size_t)p.z << ")";
        }

        std::cout << std::endl;
    }

    std::cout << "brick_locations: " << brick_locations.size() << std::endl;
}

std::shared_ptr<plx::LegoData>
Preprocessor::data()
{
    return m_data;
}


const std::vector<glm::vec3>&
Preprocessor::Brick::location() const
{
    return m_location;
}

Preprocessor::OneOnePlate::OneOnePlate(const std::vector<glm::vec3>& location)
{
    m_location = location;
}

const std::vector<std::vector<glm::vec3>>&
Preprocessor::OneOnePlate::orientations()
{
    if (OneOnePlate::m_orientations.empty()) {
        std::vector<glm::vec3> orientation1;
        orientation1.push_back(glm::vec3(0, 0, 0));
        OneOnePlate::m_orientations.push_back(orientation1);
    }
    return OneOnePlate::m_orientations;
}

std::vector<std::vector<glm::vec3>> Preprocessor::OneOnePlate::m_orientations;


Preprocessor::OneTwoPlate::OneTwoPlate(const std::vector<glm::vec3>& location)
{
    m_location = location;
}


const std::vector<std::vector<glm::vec3>>&
Preprocessor::OneTwoPlate::orientations()
{
    if (OneTwoPlate::m_orientations.empty()) {
        std::vector<glm::vec3> orientation1;
        orientation1.push_back(glm::vec3(0, 0, 0));
        orientation1.push_back(glm::vec3(0, 0, 1));
        OneTwoPlate::m_orientations.push_back(orientation1);
        std::vector<glm::vec3> orientation2;
        orientation2.push_back(glm::vec3(0, 0, 0));
        orientation2.push_back(glm::vec3(1, 0, 0));
        OneTwoPlate::m_orientations.push_back(orientation2);
    }
    return OneTwoPlate::m_orientations;
}

std::vector<std::vector<glm::vec3>> Preprocessor::OneTwoPlate::m_orientations;

}
