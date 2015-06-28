#include "voxelizer.h"
#include "tiny_obj_loader.h"

#include <math.h>
#include <iostream>
#include <algorithm>

Voxelizer::Voxelizer() :
    m_maxX(INT_MIN),
    m_maxY(INT_MIN),
    m_maxZ(INT_MIN),
    m_minX(INT_MAX),
    m_minY(INT_MAX),
    m_minZ(INT_MAX)
{
}

void Voxelizer::setMinMaxXYZ(std::vector<tinyobj::face_t>& faces, std::vector<glm::vec3>& vertices)
{
    for (std::vector<tinyobj::face_t>::iterator face = faces.begin(); face != faces.end(); ++face )
    {
        auto v1 = vertices[(*face).v1];
        auto v2 = vertices[(*face).v2];
        auto v3 = vertices[(*face).v3];
        auto faceVertices = {v1, v2, v3};

        for (auto it = faceVertices.begin() ; it != faceVertices.end(); ++it)
        {
            glm::vec3 vertex = *it;
            if (vertex.x > m_maxX) {
                m_maxX = vertex.x;
            }
            if (vertex.x < m_minX) {
                m_minX = vertex.x;
            }
            if (vertex.y > m_maxY) {
                m_maxY = vertex.y;
            }
            if (vertex.y < m_minY) {
                m_minY = vertex.y;
            }
            if (vertex.z > m_maxZ) {
                m_maxZ = vertex.z;
            }
            if (vertex.z < m_minZ) {
                m_minZ = vertex.z;
            }
        }
    }
}

void Voxelizer::voxelizeFace(
    std::vector<std::vector<std::vector<int>>>& grid,
    glm::vec3& v1, glm::vec3& v2, glm::vec3& v3)
{
    // scale model to NxNxN grid
    glm::vec3 p1 = glm::vec3((v1.x - m_minX) / m_unit, (v1.y - m_minY) / m_unit, (v1.z - m_minZ) / m_unit);
    glm::vec3 p2 = glm::vec3((v2.x - m_minX) / m_unit, (v2.y - m_minY) / m_unit, (v2.z - m_minZ) / m_unit);
    glm::vec3 p3 = glm::vec3((v3.x - m_minX) / m_unit, (v3.y - m_minY) / m_unit, (v3.z - m_minZ) / m_unit);

    grid[(int)p2.x][(int)p2.y][(int)p2.z] = 1;
    grid[(int)p3.x][(int)p3.y][(int)p3.z] = 1;
    grid[(int)p1.x][(int)p1.y][(int)p1.z] = 1;
}

std::shared_ptr<ObjData>
Voxelizer::Process(
    std::shared_ptr<ObjData> data
)
{
    std::shared_ptr<std::vector<tinyobj::shape_t>> shapes = data->m_shapes;

    for (unsigned int pp = 0; pp < shapes->size(); ++pp)
    {
        tinyobj::mesh_t& mesh = (*shapes)[pp].mesh;
        std::vector<tinyobj::face_t>& faces = mesh.faces;
        std::vector<glm::vec3>& vertices = mesh.vertices;

        setMinMaxXYZ(faces,vertices);

        float s = std::min(std::min((m_maxX - m_minX), (m_maxY - m_minY)), (m_maxZ - m_minZ));
        m_subdivisions = 50;
        m_unit = s / m_subdivisions;

        m_width = std::ceil((m_maxX - m_minX) / m_unit) + 1;
        m_height = std::ceil((m_maxY - m_minY) / m_unit) + 1;
        m_depth = std::ceil((m_maxZ - m_minZ) / m_unit) + 1;

        std::cout << "unit: " << m_unit << std::endl;
        std::cout << "size: " << m_width << "," << m_height << "," << m_depth << std::endl;

        std::vector<std::vector<std::vector<int>>> grid =
            std::vector<std::vector<std::vector<int>>>(m_width,
            std::vector<std::vector<int>>(m_height,
            std::vector<int>(m_depth, 0)));


        size_t end = faces.size();
        std::cout << "fsize: " << end << std::endl;
        for (size_t i = 0; i < end; ++i)
        {
            addFaces(faces[i], vertices, faces);
        }

        for (std::vector<tinyobj::face_t>::const_iterator face = faces.begin(); face != faces.end(); ++face )
        {
            glm::vec3& v1 = vertices[face->v1];
            glm::vec3& v2 = vertices[face->v2];
            glm::vec3& v3 = vertices[face->v3];
            voxelizeFace(grid, v1, v2, v3);
        }

        vertices.clear();
        faces.clear();

        std::cout << "Adding to grid" << std::endl;

        voxelToOBJ(grid, faces, vertices);
    }

    return data;
}

void Voxelizer::voxelToOBJ(std::vector<std::vector<std::vector<int>>>& grid, std::vector<tinyobj::face_t>& faces, std::vector<glm::vec3>& vertices)
{
    for (size_t i = 0; i < grid.size(); ++i)
    {
        for (size_t j = 0; j < grid[i].size(); ++j)
        {
            for (size_t k = 0; k < grid[i][j].size(); ++k)
            {
                if (grid[i][j][k] != 0)
                {
                    size_t last = vertices.size();
                    vertices.push_back(glm::vec3(i+0.0f+0.001f,j+0.0f+0.001f,k+0.0f+0.001f)); // 0
                    vertices.push_back(glm::vec3(i+1.0f+0.001f,j+0.0f+0.001f,k+0.0f+0.001f)); // 1
                    vertices.push_back(glm::vec3(i+0.0f+0.001f,j+1.0f+0.001f,k+0.0f+0.001f)); // 2
                    vertices.push_back(glm::vec3(i+0.0f+0.001f,j+0.0f+0.001f,k+1.0f+0.001f)); // 3
                    vertices.push_back(glm::vec3(i+1.0f+0.001f,j+1.0f+0.001f,k+0.0f+0.001f)); // 4
                    vertices.push_back(glm::vec3(i+0.0f+0.001f,j+1.0f+0.001f,k+1.0f+0.001f)); // 5
                    vertices.push_back(glm::vec3(i+1.0f+0.001f,j+0.0f+0.001f,k+1.0f+0.001f)); // 6
                    vertices.push_back(glm::vec3(i+1.0f+0.001f,j+1.0f+0.001f,k+1.0f+0.001f)); // 7

                    faces.push_back(tinyobj::face_t(last+1, last+2, last+0));
                    faces.push_back(tinyobj::face_t(last+2, last+4, last+1));

                    faces.push_back(tinyobj::face_t(last+0, last+3, last+2));
                    faces.push_back(tinyobj::face_t(last+2, last+3, last+5));

                    faces.push_back(tinyobj::face_t(last+4, last+6, last+1));
                    faces.push_back(tinyobj::face_t(last+4, last+7, last+6));

                    faces.push_back(tinyobj::face_t(last+2, last+5, last+4));
                    faces.push_back(tinyobj::face_t(last+4, last+5, last+7));

                    faces.push_back(tinyobj::face_t(last+3, last+0, last+1));
                    faces.push_back(tinyobj::face_t(last+3, last+1, last+6));

                    faces.push_back(tinyobj::face_t(last+5, last+6, last+7));
                    faces.push_back(tinyobj::face_t(last+5, last+3, last+6));
                }
            }
        }
    }
}

void Voxelizer::addFaces(tinyobj::face_t const & face, std::vector<glm::vec3>& v, std::vector<tinyobj::face_t>& f) {

    glm::vec3& v1 = v[face.v1];
    glm::vec3& v2 = v[face.v2];
    glm::vec3& v3 = v[face.v3];

    if (glm::distance(v1, v2) < m_unit ||
        glm::distance(v2, v3) < m_unit ||
        glm::distance(v3, v1) < m_unit) {
        return;
    }

    size_t last = v.size();
    v.push_back(glm::vec3((v1.x + v2.x) / 2.0f, (v1.y + v2.y) / 2.0f, (v1.z + v2.z) / 2.0f));
    v.push_back(glm::vec3((v2.x + v3.x) / 2.0f, (v2.y + v3.y) / 2.0f, (v2.z + v3.z) / 2.0f));
    v.push_back(glm::vec3((v3.x + v1.x) / 2.0f, (v3.y + v1.y) / 2.0f, (v3.z + v1.z) / 2.0f));
    f.push_back(tinyobj::face_t(last, last+1, last+2));

    tinyobj::face_t f1(face.v1, last+0, last+2);
    tinyobj::face_t f2(face.v2, last+0, last+1);
    tinyobj::face_t f3(face.v3, last+2, last+1);
    tinyobj::face_t f4(last+0, last+1, last+2);
    addFaces(f1, v, f);
    addFaces(f2, v, f);
    addFaces(f3, v, f);
    addFaces(f4, v, f);
}
