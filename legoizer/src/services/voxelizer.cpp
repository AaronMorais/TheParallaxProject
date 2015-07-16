#include "voxelizer.h"
#include "legoizer.h"
#include "utilities/glm_helpers.h"

#include <cstdlib>
#include <iostream>
#include <algorithm>

namespace plx {

Voxelizer::Voxelizer(std::shared_ptr<plx::LegoData> data) :
    m_data(data)
{
}

void
Voxelizer::voxelize()
{
    for (const tinyobj::shape_t& shape : m_data->data()->shapes())
    {
        std::shared_ptr<ShapeVoxelizer> shapeVoxelizer = std::make_shared<ShapeVoxelizer>(m_data, shape);
        shapeVoxelizer->voxelize();
    }
}

Voxelizer::ShapeVoxelizer::ShapeVoxelizer(
    std::shared_ptr<plx::LegoData> data,
    const tinyobj::shape_t& shape
) :
    m_data(data),
    m_shape(shape)
{
    std::tuple<glm::vec3, glm::vec3> minMax = calculateMinMaxDimensions(shape.mesh.faces, shape.mesh.vertices);
    m_min = std::get<0>(minMax);
    m_max = std::get<1>(minMax);

    m_unit = calculateVoxelUnit(m_min, m_max);

    m_dimensions = calculateVoxelGridDimensions(m_min, m_max, m_unit);

    m_grid =
        std::vector<std::vector<std::vector<int>>>(m_dimensions.x,
        std::vector<std::vector<int>>(m_dimensions.y,
        std::vector<int>(m_dimensions.z, Legoizer::shouldFillShell ? -1 : 0)));
}

void
Voxelizer::ShapeVoxelizer::voxelize()
{
    std::cerr << "Begin voxelization" << std::endl;

    std::cerr << "Matching faces to grid" << std::endl;

    for (const glm::vec3& face : m_shape.mesh.faces) {
        const glm::vec3& face_vertex_x = m_shape.mesh.vertices[face.x];
        const glm::vec3& face_vertex_y = m_shape.mesh.vertices[face.y];
        const glm::vec3& face_vertex_z = m_shape.mesh.vertices[face.z];

        glm::vec3 x(face_vertex_x.x, face_vertex_x.y, face_vertex_x.z);
        glm::vec3 y(face_vertex_y.x, face_vertex_y.y, face_vertex_y.z);
        glm::vec3 z(face_vertex_z.x, face_vertex_z.y, face_vertex_z.z);
        voxelize(x, y, z);
    }

    if (Legoizer::shouldFillShell) {
        fillShell();
    }

    std::cerr << "Adding to grid" << std::endl;

    save();

    std::cerr << "Finished voxelization" << std::endl;
}

std::tuple<glm::vec3, glm::vec3>
Voxelizer::ShapeVoxelizer::calculateMinMaxDimensions(
    const std::vector<glm::vec3>& faces,
    const std::vector<glm::vec3>& vertices)
{
    glm::vec3 min(INT_MAX, INT_MAX, INT_MAX);
    glm::vec3 max(INT_MIN, INT_MIN, INT_MIN);

    for (const glm::vec3& face : faces)
    {
        std::vector<glm::vec3> face_vertices;
        face_vertices.push_back(vertices[face.x]);
        face_vertices.push_back(vertices[face.y]);
        face_vertices.push_back(vertices[face.z]);

        for (glm::vec3& vertex : face_vertices) {

            if (vertex.x > max.x) {
                max.x = vertex.x;
            }
            if (vertex.y > max.y) {
                max.y = vertex.y;
            }
            if (vertex.z > max.z) {
                max.z = vertex.z;
            }

            if (vertex.x < min.x) {
                min.x = vertex.x;
            }
            if (vertex.y < min.y) {
                min.y = vertex.y;
            }
            if (vertex.z < min.z) {
                min.z = vertex.z;
            }
        }
    }

    return std::make_tuple<glm::vec3&, glm::vec3&>(min, max);
}

float
Voxelizer::ShapeVoxelizer::calculateVoxelUnit(
    const glm::vec3& min,
    const glm::vec3& max
    )
{
    float minimum_side = std::min(std::min((max.x - min.x), (max.y - min.y)), (max.z - min.z));
    float voxel_unit = minimum_side / Legoizer::subdivisions;
    std::cerr << "voxel_unit: " << voxel_unit << std::endl;
    return voxel_unit;
}

glm::vec3
Voxelizer::ShapeVoxelizer::calculateVoxelGridDimensions(
    const glm::vec3& min,
    const glm::vec3& max,
    const float& voxel_unit
    )
{
    float width = std::ceil((max.x - min.x) / voxel_unit) + 1;
    float height = std::ceil((max.y - min.y) / (voxel_unit * Voxelizer::scale())) + 1;
    float depth = std::ceil((max.z - min.z) / voxel_unit) + 1;
    std::cerr << "size (whd): " << width << "," << height << "," << depth << std::endl;
    return glm::vec3(width, height, depth);
    }

bool Voxelizer::ShapeVoxelizer::arePointsClose(
    glm::vec3& v1,
    glm::vec3& v2,
    glm::vec3& v3
    )
{
    return  std::abs((int)((size_t)v1.x - (size_t)v2.x)) < 2 &&
            std::abs((int)((size_t)v2.x - (size_t)v3.x)) < 2 &&
            std::abs((int)((size_t)v1.y - (size_t)v2.y)) < 2 &&
            std::abs((int)((size_t)v2.y - (size_t)v3.y)) < 2 &&
            std::abs((int)((size_t)v1.z - (size_t)v2.z)) < 2 &&
            std::abs((int)((size_t)v2.z - (size_t)v3.z)) < 2;
}

bool Voxelizer::ShapeVoxelizer::arePointsOnOccupiedVoxel(
    glm::vec3& v1,
    glm::vec3& v2,
    glm::vec3& v3
    )
{
    return  m_grid[(size_t)v1.x][(size_t)v1.y][(size_t)v1.z] == 1 &&
            m_grid[(size_t)v2.x][(size_t)v2.y][(size_t)v2.z] == 1 &&
            m_grid[(size_t)v3.x][(size_t)v3.y][(size_t)v3.z] == 1;
}

void Voxelizer::ShapeVoxelizer::voxelize(
    glm::vec3& v1,
    glm::vec3& v2,
    glm::vec3& v3
    )
{
    // scale model to NxNxN grid
    glm::vec3 p1 = glm::vec3(
        (v1.x - m_min.x) / m_unit,
        (v1.y - m_min.y) / (m_unit * Voxelizer::scale()),
        (v1.z - m_min.z) / m_unit
        );
    glm::vec3 p2 = glm::vec3(
        (v2.x - m_min.x) / m_unit,
        (v2.y - m_min.y) / (m_unit * Voxelizer::scale()),
        (v2.z - m_min.z) / m_unit
        );
    glm::vec3 p3 = glm::vec3(
        (v3.x - m_min.x) / m_unit,
        (v3.y - m_min.y) / (m_unit * Voxelizer::scale()),
        (v3.z - m_min.z) / m_unit
        );

    if (arePointsClose(p1, p2, p3) || arePointsOnOccupiedVoxel(p1, p2, p3)){
        return;
    }

    m_grid[(size_t)p1.x][(size_t)p1.y][(size_t)p1.z] = 1;
    m_grid[(size_t)p2.x][(size_t)p2.y][(size_t)p2.z] = 1;
    m_grid[(size_t)p3.x][(size_t)p3.y][(size_t)p3.z] = 1;

    glm::vec3 mid1 = glm_helpers::midpoint(v1, v2);
    glm::vec3 mid2 = glm_helpers::midpoint(v2, v3);
    glm::vec3 mid3 = glm_helpers::midpoint(v1, v3);

    voxelize(v1, mid1, mid3);
    voxelize(mid1, v2, mid2);
    voxelize(mid3, mid2, v3);
    voxelize(mid1, mid2, mid3);
}

void
Voxelizer::ShapeVoxelizer::fillShell()
{
    std::cerr << "Filling Shell" << std::endl;

    std::cerr << "Chipping z axis" << std::endl;
    for (size_t i = 0; i < m_grid.size(); ++i)
    {
        for (size_t j = 0; j < m_grid[i].size(); ++j)
        {
            size_t edge = 0;

            for (size_t k = 0; k < m_grid[i][j].size(); k++, edge++) {
                if (m_grid[i][j][k] == 1) {
                    break;
                }

                m_grid[i][j][k] = 0;
            }

            for (size_t k = m_grid[i][j].size()-1; k > edge; k--) {
                if (m_grid[i][j][k] == 1) {
                    break;
                }

                m_grid[i][j][k] = 0;
            }

        }
    }

    std::cerr << "Chipping y axis" << std::endl;
    for (size_t i = 0; i < m_grid.size(); ++i)
    {
        for (size_t k = 0; k < m_grid[i][0].size(); ++k)
        {
            size_t edge = 0;

            for (size_t j = 0; j < m_grid[i].size(); j++, edge++) {
                if (m_grid[i][j][k] == 1) {
                    break;
                }

                m_grid[i][j][k] = 0;
            }

            for (size_t j = m_grid[i].size()-1; j > edge; j--) {
                if (m_grid[i][j][k] == 1) {
                    break;
                }

                m_grid[i][j][k] = 0;
            }
        }
    }

    std::cerr << "Chipping x axis" << std::endl;
    for (size_t j = 0; j < m_grid[0].size(); ++j)
    {
        for (size_t k = 0; k < m_grid[0][j].size(); ++k)
        {
            size_t edge = 0;

            for (size_t i = 0; i < m_grid.size(); i++, edge++) {
                if (m_grid[i][j][k] == 1) {
                    break;
                }

                m_grid[i][j][k] = 0;
            }

            for (size_t i = m_grid.size()-1; i > edge; i--) {
                if (m_grid[i][j][k] == 1) {
                    break;
                }

                m_grid[i][j][k] = 0;
            }
        }
    }

    std::cerr << "Shell Filled" << std::endl;
}

void
Voxelizer::ShapeVoxelizer::save()
{
    std::vector<glm::vec3>& vertices = m_data->vertices();
    std::vector<glm::vec3>& faces = m_data->faces();
    std::vector<glm::vec3>& voxels = m_data->voxels();

    for (size_t i = 0; i < m_grid.size(); ++i) {

        for (size_t j = 0; j < m_grid[i].size(); ++j) {

            for (size_t k = 0; k < m_grid[i][j].size(); ++k) {

                if (Legoizer::shouldFillShell && m_grid[i][j][k] != 0)  {

                    voxels.push_back(glm::vec3(i,j,k));

                    float scale = Voxelizer::scale();
                    size_t last = vertices.size();

                    vertices.push_back(glm::vec3(i+0.0f+0.001f, (j*scale)+0.0f+0.001f,  k+0.0f+0.001f)); // 0
                    vertices.push_back(glm::vec3(i+1.0f+0.001f, (j*scale)+0.0f+0.001f,  k+0.0f+0.001f)); // 1
                    vertices.push_back(glm::vec3(i+0.0f+0.001f, (j*scale)+scale+0.001f, k+0.0f+0.001f)); // 2
                    vertices.push_back(glm::vec3(i+0.0f+0.001f, (j*scale)+0.0f+0.001f,  k+1.0f+0.001f)); // 3
                    vertices.push_back(glm::vec3(i+1.0f+0.001f, (j*scale)+scale+0.001f, k+0.0f+0.001f)); // 4
                    vertices.push_back(glm::vec3(i+0.0f+0.001f, (j*scale)+scale+0.001f, k+1.0f+0.001f)); // 5
                    vertices.push_back(glm::vec3(i+1.0f+0.001f, (j*scale)+0.0f+0.001f,  k+1.0f+0.001f)); // 6
                    vertices.push_back(glm::vec3(i+1.0f+0.001f, (j*scale)+scale+0.001f, k+1.0f+0.001f)); // 7

                    faces.push_back(glm::vec3(last+1, last+2, last+0));
                    faces.push_back(glm::vec3(last+2, last+4, last+1));

                    faces.push_back(glm::vec3(last+0, last+3, last+2));
                    faces.push_back(glm::vec3(last+2, last+3, last+5));

                    faces.push_back(glm::vec3(last+4, last+6, last+1));
                    faces.push_back(glm::vec3(last+4, last+7, last+6));

                    faces.push_back(glm::vec3(last+2, last+5, last+4));
                    faces.push_back(glm::vec3(last+4, last+5, last+7));

                    faces.push_back(glm::vec3(last+3, last+0, last+1));
                    faces.push_back(glm::vec3(last+3, last+1, last+6));

                    faces.push_back(glm::vec3(last+5, last+6, last+7));
                    faces.push_back(glm::vec3(last+5, last+3, last+6));

                }
            }
        }
    }
}


}
