#include "obj_data.h"

#include <iostream>

namespace tinyobj {

ObjData::ObjData(
    std::vector<tinyobj::shape_t>& shapes,
    std::vector<tinyobj::material_t>& materials
    ) :
    m_shapes(shapes),
    m_materials(materials)
{
}

const std::vector<tinyobj::shape_t>&
ObjData::shapes() const
{
    return m_shapes;
}

const std::vector<tinyobj::material_t>&
ObjData::materials() const
{
    return m_materials;
}

std::shared_ptr<ObjData>
ObjData::Factory::Create(
    const char* filename,
    const char* basepath
    )
{
    std::cout << "Loading from file:" << " " << filename << std::endl;

    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err = tinyobj::LoadObj(shapes, materials, filename, basepath);

    if (!err.empty()) {
        std::cerr << err << std::endl;
        return nullptr;
    }

    return std::make_shared<ObjData>(shapes, materials);
}

void
ObjData::PrintShapes() const
{
    const std::vector<tinyobj::shape_t>& shapes = this->shapes();

    std::cout << "# of shapes    : " << shapes.size() << std::endl;

    for (size_t i = 0; i < shapes.size(); i++) {
        printf("shape[%u].name = %s\n", i, shapes[i].name.c_str());
        printf("Size of shape[%u].indices: %u\n", i, shapes[i].mesh.indices.size());
        printf("Size of shape[%u].material_ids: %u\n", i, shapes[i].mesh.material_ids.size());
        assert((shapes[i].mesh.indices.size() % 3) == 0);
        for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++) {
            printf("  idx[%u] = %d, %d, %d. mat_id = %d\n", f, shapes[i].mesh.indices[3*f+0], shapes[i].mesh.indices[3*f+1], shapes[i].mesh.indices[3*f+2], shapes[i].mesh.material_ids[f]);
        }

        printf("shape[%u].vertices: %u\n", i, shapes[i].mesh.positions.size());
        assert((shapes[i].mesh.positions.size() % 3) == 0);
        for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
            printf("  v[%u] = (%f, %f, %f)\n", v,
                shapes[i].mesh.positions[3*v+0],
                shapes[i].mesh.positions[3*v+1],
                shapes[i].mesh.positions[3*v+2]);
        }
    }
}

void
ObjData::PrintMaterials() const
{
    const std::vector<tinyobj::material_t>& materials = this->materials();

    std::cout << "# of materials : " << materials.size() << std::endl;
    for (size_t i = 0; i < materials.size(); i++) {
        printf("material[%u].name = %s\n", i, materials[i].name.c_str());
        printf("  material.Ka = (%f, %f ,%f)\n", materials[i].ambient[0], materials[i].ambient[1], materials[i].ambient[2]);
        printf("  material.Kd = (%f, %f ,%f)\n", materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2]);
        printf("  material.Ks = (%f, %f ,%f)\n", materials[i].specular[0], materials[i].specular[1], materials[i].specular[2]);
        printf("  material.Tr = (%f, %f ,%f)\n", materials[i].transmittance[0], materials[i].transmittance[1], materials[i].transmittance[2]);
        printf("  material.Ke = (%f, %f ,%f)\n", materials[i].emission[0], materials[i].emission[1], materials[i].emission[2]);
        printf("  material.Ns = %f\n", materials[i].shininess);
        printf("  material.Ni = %f\n", materials[i].ior);
        printf("  material.dissolve = %f\n", materials[i].dissolve);
        printf("  material.illum = %d\n", materials[i].illum);
        printf("  material.map_Ka = %s\n", materials[i].ambient_texname.c_str());
        printf("  material.map_Kd = %s\n", materials[i].diffuse_texname.c_str());
        printf("  material.map_Ks = %s\n", materials[i].specular_texname.c_str());
        printf("  material.map_Ns = %s\n", materials[i].normal_texname.c_str());
        std::map<std::string, std::string>::const_iterator it(materials[i].unknown_parameter.begin());
        std::map<std::string, std::string>::const_iterator itEnd(materials[i].unknown_parameter.end());
        for (; it != itEnd; it++) {
            printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
        }
        printf("\n");
    }
}

void
ObjData::PrintObj(
    std::ostream& os
    ) const
{
    for (const tinyobj::shape_t& shape : this->shapes())
    {
        os << "g" << std::endl;

        for (const glm::vec3& vertex : shape.mesh.vertices) {
            os << "v " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
        }

        for (const glm::vec3& face : shape.mesh.faces) {
            os << "f " << face.x << " " << face.y << " " << face.z << std::endl;
        }
    }
}

}
