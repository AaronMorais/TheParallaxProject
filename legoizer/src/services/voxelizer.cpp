#include "voxelizer.h"

#include <math.h>
#include <iostream>
#include <algorithm>

const bool FILL_SHELL = false;

Voxelizer::Voxelizer() :
    m_maxX(INT_MIN),
    m_maxY(INT_MIN),
    m_maxZ(INT_MIN),
    m_minX(INT_MAX),
    m_minY(INT_MAX),
    m_minZ(INT_MAX)
{
}

void Voxelizer::setMinMaxXYZ(std::vector<glm::vec3>& faces, std::vector<glm::vec3>& vertices)
{
    for (std::vector<glm::vec3>::iterator face = faces.begin(); face != faces.end(); ++face )
    {
        auto v1 = vertices[(*face).x];
        auto v2 = vertices[(*face).y];
        auto v3 = vertices[(*face).z];
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

Voxelizer::Vertex Voxelizer::midPoint(Vertex& v1, Vertex& v2)
{
    return Vertex((v1.x + v2.x) / 2, (v1.y + v2.y) / 2, (v1.z + v2.z) / 2);
}

bool Voxelizer::samePointAndOccupied(std::vector<std::vector<std::vector<int>>>& grid, Vertex& v1, Vertex& v2, Vertex& v3)
{
    return  std::abs((int)v1.x - (int)v2.x) < 2 &&
            std::abs((int)v2.x - (int)v3.x) < 2 &&
            std::abs((int)v1.y - (int)v2.y) < 2 &&
            std::abs((int)v2.y - (int)v3.y) < 2 &&
            std::abs((int)v1.z - (int)v2.z) < 2 &&
            std::abs((int)v2.z - (int)v3.z) < 2 &&
            grid[(int)v1.x][(int)v1.y][(int)v1.z] == 1 &&
            grid[(int)v2.x][(int)v2.y][(int)v2.z] == 1 &&
            grid[(int)v3.x][(int)v3.y][(int)v3.z] == 1;
}


void Voxelizer::voxelizeFace(
    std::vector<std::vector<std::vector<int>>>& grid,
    Vertex& v1, Vertex& v2, Vertex& v3)
{
    // scale model to NxNxN grid
    Vertex p1 = Vertex((v1.x - m_minX) / m_unit, (v1.y - m_minY) / (m_unit * LEGO_SCALE), (v1.z - m_minZ) / m_unit);
    Vertex p2 = Vertex((v2.x - m_minX) / m_unit, (v2.y - m_minY) / (m_unit * LEGO_SCALE), (v2.z - m_minZ) / m_unit);
    Vertex p3 = Vertex((v3.x - m_minX) / m_unit, (v3.y - m_minY) / (m_unit * LEGO_SCALE), (v3.z - m_minZ) / m_unit);

    if (samePointAndOccupied(grid, p1, p2, p3)){
        return;
    }

    grid[(int)p1.x][(int)p1.y][(int)p1.z] = 1;
    grid[(int)p2.x][(int)p2.y][(int)p2.z] = 1;
    grid[(int)p3.x][(int)p3.y][(int)p3.z] = 1;

    Vertex mid1 = midPoint(v1, v2);
    Vertex mid2 = midPoint(v2, v3);
    Vertex mid3 = midPoint(v1, v3);

    voxelizeFace(grid, v1, mid1, mid3);
    voxelizeFace(grid, mid1, v2, mid2);
    voxelizeFace(grid, mid3, mid2, v3);
    voxelizeFace(grid, mid1, mid2, mid3);
}

std::shared_ptr<tinyobj::ObjData>
Voxelizer::Process(
    std::shared_ptr<tinyobj::ObjData> data
)
{
    std::shared_ptr<std::vector<tinyobj::shape_t>> shapes = data->m_shapes;

    for (unsigned int pp = 0; pp < shapes->size(); ++pp)
    {
        tinyobj::mesh_t& mesh = (*shapes)[pp].mesh;
        std::vector<glm::vec3>& faces = mesh.faces;
        std::vector<glm::vec3>& vertices = mesh.vertices;

        setMinMaxXYZ(faces, vertices);

        float s = std::min(std::min((m_maxX - m_minX), (m_maxY - m_minY)), (m_maxZ - m_minZ));
        m_subdivisions = 50;
        m_unit = s / m_subdivisions;

        m_width = std::ceil((m_maxX - m_minX) / m_unit) + 1;
        m_height = std::ceil((m_maxY - m_minY) / (m_unit * LEGO_SCALE)) + 1;
        m_depth = std::ceil((m_maxZ - m_minZ) / m_unit) + 1;

        std::cout << "unit: " << m_unit << std::endl;
        std::cout << "size: " << m_width << "," << m_height << "," << m_depth << std::endl;

        std::vector<std::vector<std::vector<int>>> grid =
            std::vector<std::vector<std::vector<int>>>(m_width,
            std::vector<std::vector<int>>(m_height,
            std::vector<int>(m_depth, FILL_SHELL ? -1 : 0)));

        std::cout << "Voxelizing shell" << std::endl;
        for (std::vector<glm::vec3>::const_iterator face = faces.begin(); face != faces.end(); ++face )
        {
            Vertex x = Vertex(vertices[face->x].x, vertices[face->x].y, vertices[face->x].z);
            Vertex y = Vertex(vertices[face->y].x, vertices[face->y].y, vertices[face->y].z);
            Vertex z = Vertex(vertices[face->z].x, vertices[face->z].y, vertices[face->z].z);
            voxelizeFace(grid, x, y, z);
        }

        if (FILL_SHELL) {
            fillShell(grid);
        }

        vertices.clear();
        faces.clear();

        std::cout << "Adding to grid" << std::endl;

        voxelToOBJ(grid, faces, vertices);

        std::cout << "Finished adding to grid" << std::endl;
    }

    return data;
}

void Voxelizer::voxelToOBJ(std::vector<std::vector<std::vector<int>>>& grid, std::vector<glm::vec3>& faces, std::vector<glm::vec3>& vertices)
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
                    vertices.push_back(glm::vec3(i+0.0f+0.001f,(j*LEGO_SCALE)+0.0f+0.001f,k+0.0f+0.001f)); // 0
                    vertices.push_back(glm::vec3(i+1.0f+0.001f,(j*LEGO_SCALE)+0.0f+0.001f,k+0.0f+0.001f)); // 1
                    vertices.push_back(glm::vec3(i+0.0f+0.001f,(j*LEGO_SCALE)+LEGO_SCALE+0.001f,k+0.0f+0.001f)); // 2
                    vertices.push_back(glm::vec3(i+0.0f+0.001f,(j*LEGO_SCALE)+0.0f+0.001f,k+1.0f+0.001f)); // 3
                    vertices.push_back(glm::vec3(i+1.0f+0.001f,(j*LEGO_SCALE)+LEGO_SCALE+0.001f,k+0.0f+0.001f)); // 4
                    vertices.push_back(glm::vec3(i+0.0f+0.001f,(j*LEGO_SCALE)+LEGO_SCALE+0.001f,k+1.0f+0.001f)); // 5
                    vertices.push_back(glm::vec3(i+1.0f+0.001f,(j*LEGO_SCALE)+0.0f+0.001f,k+1.0f+0.001f)); // 6
                    vertices.push_back(glm::vec3(i+1.0f+0.001f,(j*LEGO_SCALE)+LEGO_SCALE+0.001f,k+1.0f+0.001f)); // 7

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

void Voxelizer::fillShell(std::vector<std::vector<std::vector<int>>>& grid)
{
    std::cout<<"Filling Shell"<<std::endl;
    //x direction
    std::cout<<"Chipping x axis"<<std::endl;
    for (size_t i = 0; i < grid.size(); ++i)
    {
        for (size_t j = 0; j < grid[i].size(); ++j)
        {
            size_t min = 0;
            bool minFound = false;
            for (size_t k = 0; k < grid[i][j].size() && !minFound; ++k)
            {
                if (grid[i][j][k] == 1) {
                    if (!minFound)
                        min = k;
                        minFound = true;
                } else {
                    grid[i][j][k] = 0;
                }
            }
            if (minFound) {
                minFound = false;
                for (size_t k = grid[i][j].size() - 1; k > min && !minFound; --k)
                {
                    if (grid[i][j][k] == 1) {
                        if (!minFound)
                            minFound = true;
                    } else {
                        grid[i][j][k] = 0;
                    }
                }
            }
        }
    }

    //y direction
    std::cout<<"Chipping y axis"<<std::endl;
    for (size_t i = 0; i < grid.size(); ++i)
    {
        for (size_t k = 0; k < grid[i][0].size(); ++k)
        {
            size_t min = 0;
            bool minFound = false;
            for (size_t j = 0; j < grid[i].size() && !minFound; ++j)
            {
                if (grid[i][j][k] == 1) {
                    if (!minFound)
                        min = j;
                        minFound = true;
                } else {
                    grid[i][j][k] = 0;
                }
            }
            if (minFound) {
                minFound = false;
                for (size_t j = grid[i].size() - 1; j > min && !minFound; --j)
                {
                    if (grid[i][j][k] == 1) {
                        if (!minFound)
                            minFound = true;
                    } else {
                        grid[i][j][k] = 0;
                    }
                }
            }
        }
    }

    //z direction
    std::cout<<"Chipping z axis"<<std::endl;
    for (size_t j = 0; j < grid[0].size(); ++j)
    {
        for (size_t i = 0; i < grid.size(); ++i)
        {
            size_t min = 0;
            bool minFound = false;
            for (size_t k = 0; k < grid[i][j].size() && !minFound; ++k)
            {
                if (grid[i][j][k] == 1) {
                    if (!minFound)
                        min = k;
                        minFound = true;
                } else {
                    grid[i][j][k] = 0;
                }
            }
            if (minFound) {
                minFound = false;
                for (size_t k = grid[i][j].size() - 1; k > min && !minFound; --k)
                {
                    if (grid[i][j][k] == 1) {
                        if (!minFound)
                            minFound = true;
                    } else {
                        grid[i][j][k] = 0;
                    }
                }
            }
        }
    }

    std::cout<<"Shell Filled"<<std::endl;
}
