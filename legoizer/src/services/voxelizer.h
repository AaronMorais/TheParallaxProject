#ifndef _VOXELIZER_H__
#define _VOXELIZER_H__

#include "obj_data.h"
#include "lego_data.h"
#include <vector>
#include <memory>

namespace plx {

class Voxelizer {

public:
    Voxelizer(std::shared_ptr<plx::LegoData> data);
    void voxelize();
    std::shared_ptr<plx::LegoData> data();

    static constexpr float scale() {
        return 1.230769;
    }

    static constexpr int subdivisions() {
        return 40;
    }

    static constexpr bool shouldFillShell() {
        return true;
    }

    class ShapeVoxelizer {
    public:
        ShapeVoxelizer(std::shared_ptr<plx::LegoData> data, const tinyobj::shape_t& shape);
        void voxelize();
        std::shared_ptr<plx::LegoData> data();

    private:
        std::shared_ptr<plx::LegoData> m_data;
        const tinyobj::shape_t& m_shape;

        static std::tuple<glm::vec3, glm::vec3> calculateMinMaxDimensions(const std::vector<glm::vec3>& faces, const std::vector<glm::vec3>& vertices);
        static float calculateVoxelUnit(glm::vec3& min, glm::vec3& max);
        static glm::vec3 calculateVoxelGridDimensions(glm::vec3& min, glm::vec3& max, float voxel_unit);

        bool arePointsClose(glm::vec3& v1, glm::vec3& v2, glm::vec3& v3);
        bool arePointsOnOccupiedVoxel(glm::vec3& v1, glm::vec3& v2, glm::vec3& v3);

        void voxelize(glm::vec3& v1, glm::vec3& v2, glm::vec3& v3);
        void fillShell();
        void save();

        glm::vec3 m_min;
        glm::vec3 m_max;
        float m_unit;
        glm::vec3 m_dimensions;

        std::vector<std::vector<std::vector<int>>> m_grid;
    };

private:
    std::shared_ptr<plx::LegoData> m_data;
};

}

#endif
