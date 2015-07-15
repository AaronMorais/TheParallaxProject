#include "legoizer.h"
#include "voxelizer.h"

namespace plx {

Legoizer::Legoizer(std::shared_ptr<plx::LegoData> data)
{
    m_data = data;
}

void Legoizer::voxelize()
{
    std::shared_ptr<plx::Voxelizer> voxelizer = std::make_shared<plx::Voxelizer>(m_data);

    voxelizer->voxelize();
}

std::shared_ptr<plx::LegoData>
Legoizer::data()
{
    return m_data;
}

}
