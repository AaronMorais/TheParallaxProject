#include "legoizer.h"

namespace plx {

size_t Legoizer::subdivisions = 40;
bool Legoizer::shouldFillShell = false;

Legoizer::Legoizer(std::shared_ptr<tinyobj::ObjData> obj_data) :
    m_data(std::make_shared<plx::LegoData>(obj_data))
{
}

void Legoizer::voxelize()
{
    m_voxelizer = std::make_shared<plx::Voxelizer>(m_data);
    m_voxelizer->voxelize();
}

void Legoizer::preprocess()
{
    m_preprocessor = std::make_shared<plx::Preprocessor>(m_data);
    m_preprocessor->process();
}

void Legoizer::printObj(
    std::ostream& os
    )
{
    m_data->printObj(os);
}

void Legoizer::printProcessed(
    std::ostream& os
    )
{
    m_preprocessor->print(os);
}

void Legoizer::printAlloy(
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
