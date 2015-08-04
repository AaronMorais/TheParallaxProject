#ifndef _PREPROCESSOR_H__
#define _PREPROCESSOR_H__

#include "lego_data.h"
#include "bricks.h"

#include <iostream>

namespace plx {

class Preprocessor {
public:
    Preprocessor(std::shared_ptr<plx::LegoData> data);

    void process();

    std::shared_ptr<plx::LegoData> data();

private:
    std::shared_ptr<plx::LegoData> m_data;

    glm::vec3 m_min;
    glm::vec3 m_max;
    glm::vec3 m_dimensions;

    std::vector<std::shared_ptr<Brick>> m_bricks;
    std::vector<std::vector<std::shared_ptr<Brick>>> m_conflicts;
    std::vector<std::vector<std::vector<size_t>>> m_grid;
    std::map<size_t, std::shared_ptr<Voxel>> m_voxels;
    std::map<size_t, std::vector<std::shared_ptr<Brick>>> m_overlappingBricks;

    void processVoxels(const std::vector<glm::vec3>& voxels);
    void processBricks(const std::vector<glm::vec3>& voxels);

    void storeData();
    void storeDataLayered();

    std::vector<glm::vec3> translateOrientation(
        const glm::vec3& voxel,
        const std::vector<glm::vec3>& orientation
        );
    std::vector<std::shared_ptr<Voxel>> voxelsFromOrientation(
        const std::vector<glm::vec3>& real_orientation
        );
    bool orientationFits(const std::vector<glm::vec3>& real_orientation);
    size_t index(const glm::vec3& voxel);
};

}

#endif
