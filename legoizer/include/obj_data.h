#ifndef _OBJ_DATA_H__
#define _OBJ_DATA_H__

#include "external/tiny_obj_loader/tiny_obj_loader.h"
#include <vector>
#include <string>
#include <memory>

namespace tinyobj {

class ObjData {
public:
    ObjData(
        std::vector<tinyobj::shape_t>& shapes,
        std::vector<tinyobj::material_t>& materials
        );

    const std::vector<tinyobj::shape_t>& shapes() const;
    const std::vector<tinyobj::material_t>& materials() const;

    void PrintShapes() const;
    void PrintMaterials() const;

    void PrintObj(std::ostream& os) const;

    class Factory {
    public:
        static std::shared_ptr<tinyobj::ObjData> Create(
            const char* filename,
            const char* basepath = nullptr
            );
    };

private:
    std::vector<tinyobj::shape_t> m_shapes;
    std::vector<tinyobj::material_t> m_materials;
};

}

#endif
