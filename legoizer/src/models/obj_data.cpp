#include "obj_data.h"
#include "tiny_obj_loader.h"

#include <iostream>

ObjData::ObjData(
  std::shared_ptr<std::vector<tinyobj::shape_t>> shapes,
  std::shared_ptr<std::vector<tinyobj::material_t>> materials
)
{
  m_shapes = shapes;
  m_materials = materials;
}

std::shared_ptr<ObjData>
ObjData::CreateDataFromFile(
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

  std::shared_ptr<std::vector<tinyobj::shape_t>> dataShapes = std::make_shared<std::vector<tinyobj::shape_t>>(shapes);
  std::shared_ptr<std::vector<tinyobj::material_t>> dataMaterials = std::make_shared<std::vector<tinyobj::material_t>>(materials);
  std::shared_ptr<ObjData> data = std::make_shared<ObjData>(dataShapes, dataMaterials);
  return data;
}
