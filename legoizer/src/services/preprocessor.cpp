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
    std::vector<glm::vec3>& model = m_data->voxels();

    glm::vec3 min(INT_MAX, INT_MAX, INT_MAX);
    glm::vec3 max(INT_MIN, INT_MIN, INT_MIN);

    for (const glm::vec3& voxel : model) {
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
    std::cout << "voxels: " << model.size() << std::endl;

    std::vector<Position> brick_locations;
    processLocations(brick_locations, model, dimensions);

    unsigned int index = 0;
    for (const Position& pos : brick_locations) {
        std::cout << index << ": ";
        for (const glm::vec3& p : pos.location) {
            std::cout << "(" << (size_t)p.x << "," << (size_t)p.y << "," << (size_t)p.z << ")";
        }

        std::cout << std::endl;
        index++;
    }

    std::cout << "brick_locations: " << brick_locations.size() << std::endl;

    std::cout << "conflicts: " << std::endl;
    unsigned int num_conflicts = 0;
    std::vector<std::vector<unsigned int>> brick_conflicts;
    processConflicts(brick_conflicts, brick_locations);
    for (unsigned int location_index = 0; location_index < brick_conflicts.size(); ++location_index) {
        num_conflicts += brick_conflicts[location_index].size();
        std::cout << location_index << " -> " << brick_conflicts[location_index][0];
        for (unsigned int conflict_index = 1; conflict_index < brick_conflicts[location_index].size(); ++conflict_index) {
            std::cout << " + " << brick_conflicts[location_index][conflict_index];
        }
        std::cout << std::endl;
    }
    std::cout << num_conflicts << " conflicts for " << brick_conflicts.size() << " bricks" << std::endl;
}

void
Preprocessor::processLocations(std::vector<Preprocessor::Position>& brick_locations, const std::vector<glm::vec3>& model, glm::vec3 dimensions)
{
    std::vector<std::vector<std::vector<size_t>>> grid =
        std::vector<std::vector<std::vector<size_t>>>(dimensions.x,
        std::vector<std::vector<size_t>>(dimensions.y,
        std::vector<size_t>(dimensions.z, 0)));

    for (const glm::vec3& voxel : model) {
        grid[(size_t)(voxel.x)][(size_t)(voxel.y)][(size_t)(voxel.z)] = 1;
    }

    std::vector<std::pair<std::vector<std::vector<glm::vec3>>, std::string>> all_orientations;
    all_orientations.push_back(std::make_pair(OneOnePlate::orientations(), OneOnePlate::name()));
    all_orientations.push_back(std::make_pair(OneTwoPlate::orientations(), OneTwoPlate::name()));
    all_orientations.push_back(std::make_pair(OneFourPlate::orientations(), OneFourPlate::name()));

    for (const glm::vec3& voxel : model) {

        for (const std::pair<std::vector<std::vector<glm::vec3>>, std::string>& brick_orientation : all_orientations) {

            for (const std::vector<glm::vec3>& orientation : brick_orientation.first) {

                std::vector<glm::vec3> real_location;
                for (const glm::vec3& voxel_position : orientation) {
                    real_location.push_back(glm::vec3(voxel.x + voxel_position.x, voxel.y + voxel_position.y, voxel.z + voxel_position.z));
                }

                bool fits = true;
                for (const glm::vec3& voxel_position : real_location) {
                    if (voxel_position.x >= dimensions.x ||
                        voxel_position.y >= dimensions.y ||
                        voxel_position.z >= dimensions.z ||
                        grid[voxel_position.x][voxel_position.y][voxel_position.z] == 0) {
                        fits = false;
                    }
                }

                if (fits) {
                    Position position;
                    position.location = real_location;
                    position.brick_type = brick_orientation.second;
                    brick_locations.push_back(position);
                }
            }
        }
    }
}

void
Preprocessor::processConflicts(std::vector<std::vector<unsigned int>>& brick_conflicts, const std::vector<Preprocessor::Position>& brick_locations)
{
    for (const Position& location_i : brick_locations) {
        unsigned int index = 0;
        std::vector<unsigned int> conflicts;
        for (const Position& location_j : brick_locations) {
            if (&location_i != &location_j) {
                bool conflict = false;
                for (const glm::vec3& voxel_i : location_i.location) {
                    if (conflict) break;
                    for (const glm::vec3& voxel_j : location_j.location) {
                        if (conflict) break;
                        if (voxel_i == voxel_j) {
                            conflict = true;
                            conflicts.push_back(index);
                        }
                    }
                }
            }
            index++;
        }
        brick_conflicts.push_back(conflicts);
    }
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

const std::string
Preprocessor::OneOnePlate::name()
{
    return "OneOnePlate";
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

const std::string
Preprocessor::OneTwoPlate::name()
{
    return "OneTwoPlate";
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


Preprocessor::OneFourPlate::OneFourPlate(const std::vector<glm::vec3>& location)
{
    m_location = location;
}

const std::string
Preprocessor::OneFourPlate::name()
{
    return "OneFourPlate";
}

const std::vector<std::vector<glm::vec3>>&
Preprocessor::OneFourPlate::orientations()
{
    if (OneFourPlate::m_orientations.empty()) {
        std::vector<glm::vec3> orientation1;
        orientation1.push_back(glm::vec3(0, 0, 0));
        orientation1.push_back(glm::vec3(0, 0, 1));
        orientation1.push_back(glm::vec3(0, 0, 2));
        orientation1.push_back(glm::vec3(0, 0, 3));
        OneFourPlate::m_orientations.push_back(orientation1);
        std::vector<glm::vec3> orientation2;
        orientation2.push_back(glm::vec3(0, 0, 0));
        orientation2.push_back(glm::vec3(1, 0, 0));
        orientation2.push_back(glm::vec3(2, 0, 0));
        orientation2.push_back(glm::vec3(3, 0, 0));
        OneFourPlate::m_orientations.push_back(orientation2);
    }
    return OneFourPlate::m_orientations;
}

std::vector<std::vector<glm::vec3>> Preprocessor::OneFourPlate::m_orientations;

}
