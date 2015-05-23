#ifndef _OBJ_DATA_H__
#define _OBJ_DATA_H__

#include "tiny_obj_loader.h"
#include <vector>
#include <string>
#include <memory>

struct ObjData {
  ObjData(std::shared_ptr<std::vector<tinyobj::shape_t>> shapes, std::shared_ptr<std::vector<tinyobj::material_t>> materials);
  static std::shared_ptr<ObjData> CreateDataFromFile(const char* filename, const char* basepath);

  std::shared_ptr<std::vector<tinyobj::shape_t>> m_shapes;
  std::shared_ptr<std::vector<tinyobj::material_t>> m_materials;
};

#endif
