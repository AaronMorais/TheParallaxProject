#ifndef _LEGOIZER_H__
#define _LEGOIZER_H__

#include "lego_data.h"
#include "voxelizer.h"
#include "preprocessor.h"

namespace plx {

class Legoizer {
public:
    Legoizer(std::shared_ptr<tinyobj::ObjData> data);

    void voxelize();
    void preprocess();

    void printAlloyAsVoxels(std::ostream& os);
    void printAlloyAsBricks(std::ostream& os);
    void printObj(std::ostream& os);

    std::shared_ptr<plx::LegoData> data();

    static size_t subdivisions;
    static bool shouldFillShell;

    std::shared_ptr<plx::Voxelizer> m_voxelizer;
    std::shared_ptr<plx::Preprocessor> m_preprocessor;

private:
    std::shared_ptr<plx::LegoData> m_data;
};

}

#endif
