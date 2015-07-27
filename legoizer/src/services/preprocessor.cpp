#include "preprocessor.h"
#include "utilities/glm_helpers.h"

#include <iostream>
#include <iterator>
#include <map>

namespace plx {

Preprocessor::Preprocessor(std::shared_ptr<plx::LegoData> data) :
    m_data(data),
    m_min(INT_MAX, INT_MAX, INT_MAX),
    m_max(INT_MIN, INT_MIN, INT_MIN)
{
}

void
Preprocessor::process()
{
    std::vector<glm::vec3>& voxels = m_data->voxels();

    for (const glm::vec3& voxel : voxels) {
        if (voxel.x > m_max.x) {
            m_max.x = voxel.x;
        }
        if (voxel.y > m_max.y) {
            m_max.y = voxel.y;
        }
        if (voxel.z > m_max.z) {
            m_max.z = voxel.z;
        }

        if (voxel.x < m_min.x) {
            m_min.x = voxel.x;
        }
        if (voxel.y < m_min.y) {
            m_min.y = voxel.y;
        }
        if (voxel.z < m_min.z) {
            m_min.z = voxel.z;
        }
    }

    m_dimensions = std::move(glm::vec3((m_max.x - m_min.x) + 1, (m_max.y - m_min.y) + 1, (m_max.z - m_min.z) + 1));

    m_grid = std::move(
        std::vector<std::vector<std::vector<size_t>>>(
            m_dimensions.x,
            std::vector<std::vector<size_t>>(
                m_dimensions.y,
                std::vector<size_t>(m_dimensions.z, 0)
            )
        )
    );

    processVoxels(voxels);
    processBricks(voxels);

    std::cerr << "min:" << glm::to_string(m_min) << std::endl;
    std::cerr << "max:" << glm::to_string(m_max) << std::endl;
    std::cerr << "dim:" << glm::to_string(m_dimensions) << std::endl;
    std::cerr << "#bricks: " << m_bricks.size() << std::endl;
}

void
Preprocessor::processVoxels(
    const std::vector<glm::vec3>& voxels
    )
{
    // 1-indexed because grid is unsigned and 0 is reserved for empty voxels
    for (size_t i = 0; i < voxels.size(); i++) {
        const glm::vec3& voxel = voxels[i];
        size_t idx = i+1;

        m_grid[(size_t)voxel.x][(size_t)voxel.y][(size_t)voxel.z] = idx;
        m_voxels[idx] = std::make_shared<Voxel>(voxel, idx);
    }
}

void
Preprocessor::processBricks(
    const std::vector<glm::vec3>& voxels
    )
{
    std::vector<std::pair<std::vector<std::vector<glm::vec3>>, std::string>> all_orientations;
    all_orientations.push_back(std::make_pair(OneOnePlate::orientations(), std::string(OneOnePlate::name)));
    all_orientations.push_back(std::make_pair(OneTwoPlate::orientations(), std::string(OneTwoPlate::name)));
    all_orientations.push_back(std::make_pair(OneFourPlate::orientations(), std::string(OneFourPlate::name)));

    size_t idx = 1;
    for (const glm::vec3& voxel : voxels) {

        for (const std::pair<std::vector<std::vector<glm::vec3>>, std::string>& brick_type : all_orientations) {

            for (const std::vector<glm::vec3>& unit_orientation : brick_type.first) {

                std::vector<glm::vec3> translated_orientation = translateOrientation(voxel, unit_orientation);

                if (orientationFits(translated_orientation)) {

                    std::shared_ptr<Brick> brick = Brick::Factory::create(brick_type.second.c_str(), std::move(voxelsFromOrientation(translated_orientation)), idx);

                    m_bricks.push_back(brick);

                    for (std::shared_ptr<Voxel> orientation_voxel : brick->location()) {
                        m_overlappingBricks[orientation_voxel->index].push_back(brick);
                    }

                    idx++;
                }

            }

        }

    }

    storeData();

}

std::vector<glm::vec3>
Preprocessor::translateOrientation(
    const glm::vec3& voxel,
    const std::vector<glm::vec3>& orientation
    )
{
    std::vector<glm::vec3> real_orientation;

    for (const glm::vec3& voxel_position : orientation) {
        real_orientation.push_back(glm::vec3(voxel.x + voxel_position.x, voxel.y + voxel_position.y, voxel.z + voxel_position.z));
    }

    return real_orientation;
}

bool
Preprocessor::orientationFits(
    const std::vector<glm::vec3>& real_orientation
    )
{
    bool fits = true;
    for (const glm::vec3& voxel : real_orientation) {
        if (voxel.x >= m_dimensions.x ||
            voxel.y >= m_dimensions.y ||
            voxel.z >= m_dimensions.z ||
            index(voxel) == 0
        ) {
            fits = false;
        }
    }

    return fits;
}

std::vector<std::shared_ptr<Voxel>>
Preprocessor::voxelsFromOrientation(
    const std::vector<glm::vec3>& real_orientation
    )
{
    std::vector<std::shared_ptr<Voxel>> voxel_orientation;

    for (const glm::vec3& real_voxel : real_orientation) {
        voxel_orientation.push_back(m_voxels[index(real_voxel)]);
    }

    return voxel_orientation;
}

size_t
Preprocessor::index(
    const glm::vec3& voxel
    )
{
    return m_grid[(size_t)voxel.x][(size_t)voxel.y][(size_t)voxel.z];
}

void
Preprocessor::storeData()
{

    std::vector<std::vector<size_t>>& brick_locations = m_data->brick_locations();
    for (size_t i = 0; i < m_bricks.size(); i++) {
        std::shared_ptr<Brick> brick = m_bricks[i];
        std::vector<size_t> associated_voxels;
        for (std::shared_ptr<Voxel> voxel : brick->location()) {
            associated_voxels.push_back(voxel->index - 1);
        }
        brick_locations.push_back(associated_voxels);
    }

    std::vector<std::vector<size_t>>& brick_conflicts = m_data->brick_conflicts();
    for (std::shared_ptr<Brick> brick : m_bricks) {
        std::vector<size_t> conflicting_bricks;
        for (std::shared_ptr<Voxel> voxel : brick->location()) {
            for (std::shared_ptr<Brick> conflicted_brick : m_overlappingBricks[voxel->index]) {
                if (brick->id() != conflicted_brick->id()) {
                    conflicting_bricks.push_back(conflicted_brick->id() - 1);
                }
            }
        }
        brick_conflicts.push_back(conflicting_bricks);
    }

    std::vector<std::vector<size_t>>& brick_connections = m_data->brick_connections();
    for (size_t i = 0; i < m_bricks.size(); ++i) {
        std::shared_ptr<Brick> current_brick = m_bricks[i];
        std::vector<size_t> associated_bricks;
        for (std::shared_ptr<Voxel> voxel : current_brick->location()) {
            // all bricks above this voxel can be connected to the current brick
            if ((size_t)voxel->position.y + 1 < m_dimensions.y) {
                size_t voxel_above = m_grid[(size_t)voxel->position.x][(size_t)voxel->position.y + 1][(size_t)voxel->position.z];
                if (voxel_above != 0) {
                    for (std::shared_ptr<Brick> above_brick : m_overlappingBricks[voxel_above]) {
                        if (current_brick->id() != above_brick->id()) {
                            associated_bricks.push_back(above_brick->id() - 1);
                        }
                    }
                }
            }
            // all bricks below this voxel can be connected to the current brick
            if ((size_t)voxel->position.y > 0) {
                size_t voxel_below = m_grid[(size_t)voxel->position.x][(size_t)voxel->position.y - 1][(size_t)voxel->position.z];
                if (voxel_below != 0) {
                    for (std::shared_ptr<Brick> below_brick : m_overlappingBricks[voxel_below]) {
                        if (current_brick->id() != below_brick->id()) {
                            associated_bricks.push_back(below_brick->id() - 1);
                        }
                    }
                }
            }
        }
        brick_connections.push_back(associated_bricks);
    }

}

void
Preprocessor::print(std::ostream& os)
{
    for (size_t i = 0; i < m_bricks.size(); i++) {
        std::shared_ptr<Brick> brick = m_bricks[i];

        std::cerr << "b" << brick->id() << ": ";
        for (std::shared_ptr<Voxel> voxel : brick->location()) {
            std::cerr << glm_helpers::to_int_string(voxel->position) << ", ";
        }
        std::cerr << std::endl;

        std::cerr << "b" << brick->id() << " -> (";
        for (std::shared_ptr<Voxel> voxel : brick->location()) {
            std::cerr << " v" << voxel->index << ", ";
        }
        std::cerr << ")" << std::endl;
    }

    std::cerr << "conflicts: " << std::endl;

    size_t num_conflicts = 0;

    for (std::shared_ptr<Brick> brick : m_bricks) {

        std::cerr << "b" << brick->id() << " -> ";

        for (std::shared_ptr<Voxel> voxel : brick->location()) {

            for (std::shared_ptr<Brick> conflicted_brick : m_overlappingBricks[voxel->index]) {

                if (brick->id() != conflicted_brick->id()) {

                    std::cerr << " + b" << conflicted_brick->id();

                    num_conflicts += 1;
                }

            }

        }

        std::cerr << std::endl;
    }

    std::cerr << num_conflicts << " conflicts for " << m_bricks.size() << " bricks" << std::endl;

    std::cerr << "connected: " << std::endl;

    for (size_t i = 0; i < m_bricks.size(); ++i) {

        std::shared_ptr<Brick> current_brick = m_bricks[i];

        std::cerr << "b" << current_brick->id() << " -> ";

        for (std::shared_ptr<Voxel> voxel : current_brick->location()) {

            // all bricks above this voxel can be connected to the current brick
            if ((size_t)voxel->position.y + 1 < m_dimensions.y) {

                size_t voxel_above = m_grid[(size_t)voxel->position.x][(size_t)voxel->position.y + 1][(size_t)voxel->position.z];

                if (voxel_above != 0) {

                    for (std::shared_ptr<Brick> above_brick : m_overlappingBricks[voxel_above]) {

                        if (current_brick->id() != above_brick->id()) {
                            std::cerr << " + b" << above_brick->id();
                        }

                    }

                }

            }

            // all bricks below this voxel can be connected to the current brick
            if ((size_t)voxel->position.y > 0) {

                size_t voxel_below = m_grid[(size_t)voxel->position.x][(size_t)voxel->position.y - 1][(size_t)voxel->position.z];

                if (voxel_below != 0) {

                    for (std::shared_ptr<Brick> below_brick : m_overlappingBricks[voxel_below]) {

                        if (current_brick->id() != below_brick->id()) {
                            std::cerr << " + b" << below_brick->id();
                        }

                    }

                }

            }
        }
        std::cerr << std::endl;
    }

}

std::shared_ptr<plx::LegoData>
Preprocessor::data()
{
    return m_data;
}

}
