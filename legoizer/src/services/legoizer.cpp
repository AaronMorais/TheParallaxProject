#include "legoizer.h"
#include "voxelizer.h"
#include "preprocessor.h"

namespace plx {

size_t Legoizer::subdivisions = 40;
bool Legoizer::shouldFillShell = false;

Legoizer::Legoizer(std::shared_ptr<tinyobj::ObjData> obj_data) :
    m_data(std::make_shared<plx::LegoData>(obj_data))
{
}

void Legoizer::voxelize()
{
    std::shared_ptr<plx::Voxelizer> voxelizer = std::make_shared<plx::Voxelizer>(m_data);

    voxelizer->voxelize();
}

void Legoizer::preprocess()
{
    std::shared_ptr<plx::Preprocessor> preprocessor = std::make_shared<plx::Preprocessor>(m_data);

    preprocessor->process();
}

void Legoizer::writeObj(
    std::ostream& os
    )
{
    m_data->printObj(os);
}

void Legoizer::writeAlloy(
    std::ostream& os
    )
{

}

void Legoizer::writeAlloyOld(
    std::ostream& os
    )
{
    m_data->printAlloy(os);
}

std::shared_ptr<plx::LegoData>
Legoizer::data()
{
    return m_data;
}

}
