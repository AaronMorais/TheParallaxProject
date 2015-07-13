#ifndef _OBJ_DATA_H__
#define _OBJ_DATA_H__

#include "external/tiny_obj_loader/tiny_obj_loader.h"
#include <vector>
#include <string>
#include <memory>

namespace tinyobj {

class ObjData {
public:
    ObjData(std::shared_ptr<std::vector<tinyobj::shape_t>> shapes, std::shared_ptr<std::vector<tinyobj::material_t>> materials);

    std::shared_ptr<std::vector<tinyobj::shape_t>> m_shapes;
    std::shared_ptr<std::vector<tinyobj::material_t>> m_materials;

    class Factory {
    public:
        static std::shared_ptr<tinyobj::ObjData> Create(const char* filename, const char* basepath = nullptr);
    };

private:
    void InfoPrint();
    void ObjPrint(std::ostream& os);
};

}

#endif
