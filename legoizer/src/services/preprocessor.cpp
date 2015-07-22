#include "preprocessor.h"

namespace plx {

Preprocessor::Preprocessor(std::shared_ptr<plx::LegoData> data) :
    m_data(data)
{

}

void
Preprocessor::process()
{

}

std::shared_ptr<plx::LegoData>
Preprocessor::data()
{
    return m_data;
}

}
