#include "voxelizer.h"
#include "tiny_obj_loader.h"

#include <math.h>
#include <iostream>

Voxelizer::Voxelizer() :
    m_maxX(INT_MIN),
    m_maxY(INT_MIN),
    m_maxZ(INT_MIN),
    m_minX(INT_MAX),
    m_minY(INT_MAX),
    m_minZ(INT_MAX)
{
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

        for (std::vector<tinyobj::face_t>::iterator face = faces.begin(); face != faces.end(); ++face )
        {
            auto v1 = mesh.vertices[(*face).v1];
            auto v2 = mesh.vertices[(*face).v2];
            auto v3 = mesh.vertices[(*face).v3];
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

        // std::cout << "m: " << m_minX << ", " << m_maxX << std::endl;
        // std::cout << "m: " << m_minY << ", " << m_maxY << std::endl;
        // std::cout << "m: " << m_minZ << ", " << m_maxZ << std::endl;

        float s = std::min(std::min((m_maxX - m_minX), (m_maxY - m_minY)), (m_maxZ - m_minZ));
        m_subdivisions = 10;
        m_unit = s / m_subdivisions;

        // std::cout << "t: " << s << "," << m_unit << std::endl;

        m_width = std::ceil((m_maxX - m_minX) / m_unit) + 1;
        m_height = std::ceil((m_maxY - m_minY) / m_unit) + 1;
        m_depth = std::ceil((m_maxZ - m_minZ) / m_unit) + 1;

        // std::cout << "x:" << m_width << ", y:" << m_height << ", z:" << m_depth << std::endl;

        std::vector<std::vector<std::vector<int>>> grid =
            std::vector<std::vector<std::vector<int>>>(m_width,
            std::vector<std::vector<int>>(m_height,
            std::vector<int>(m_depth, 0)));

        for (std::vector<tinyobj::face_t>::iterator face = faces.begin(); face != faces.end(); ++face )
        {
            glm::vec3 v1 = mesh.vertices[(*face).v1];
            glm::vec3 v2 = mesh.vertices[(*face).v2];
            glm::vec3 v3 = mesh.vertices[(*face).v3];

            int x1 = std::floor((v1.x - m_minX) / m_unit);
            int y1 = std::floor((v1.y - m_minY) / m_unit);
            int z1 = std::floor((v1.z - m_minZ) / m_unit);

            int x2 = std::floor((v2.x - m_minX) / m_unit);
            int y2 = std::floor((v2.y - m_minY) / m_unit);
            int z2 = std::floor((v2.z - m_minZ) / m_unit);

            int x3 = std::floor((v3.x - m_minX) / m_unit);
            int y3 = std::floor((v3.y - m_minY) / m_unit);
            int z3 = std::floor((v3.z - m_minZ) / m_unit);

            // std::cout << "v1: " << x1 << "," << y1 << "," << z1 << std::endl;
            grid[x1][y1][z1] = 1;
            // std::cout << "v2: " << x2 << "," << y2 << "," << z2 << std::endl;
            grid[x2][y2][z2] = 1;
            // std::cout << "v3: " << x3 << "," << y3 << "," << z3 << std::endl;
            grid[x3][y3][z3] = 1;
        }

        vertices.clear();
        faces.clear();

        for (size_t i = 0; i < grid.size(); ++i)
        {
            for (size_t j = 0; j < grid[i].size(); ++j)
            {
                for (size_t k = 0; k < grid[i][j].size(); ++k)
                {
                    if (grid[i][j][k] != 0)
                    {
                        size_t last = mesh.vertices.size();
                        vertices.push_back(glm::vec3(i+0.0f+0.001f,j+0.0f+0.001f,k+0.0f+0.001f)); // 0
                        vertices.push_back(glm::vec3(i+1.0f+0.001f,j+0.0f+0.001f,k+0.0f+0.001f)); // 1
                        // std::cerr << "index = " << last << " " << vertices[last].x << " " << vertices[last].y << " " << vertices[last].z << "\n";
                        // std::cerr << "index = " << last+1 << " " << vertices[last+1].x << " " << vertices[last+1].y << " " << vertices[last+1].z << "\n";
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


            // std::cout << mesh.vertices[mesh.faces[0].v1].x << [mesh.faces[0].v1].y << ", " << mesh.vertices[mesh.faces[0].v1].z << std::endl;

                    }
                }
            }
        }
    }


    tinyobj::mesh_t& meshie = (*(data->m_shapes))[0].mesh;
    std::vector<tinyobj::face_t>& faces = meshie.faces;
    std::vector<glm::vec3>& vertices = meshie.vertices;

    // std::cout <<"poo: " << meshie.vertices[meshie.faces[0].v1].x << ", " << meshie.vertices[meshie.faces[0].v1].y << ", " << meshie.vertices[meshie.faces[0].v1].z << std::endl;

    return data;
}
