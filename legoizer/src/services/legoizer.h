#ifndef _LEGOIZER_H__
#define _LEGOIZER_H__

#include "lego_data.h"
#include <memory>

namespace plx {

class Legoizer {
public:
    Legoizer(std::shared_ptr<plx::LegoData> data);

    void voxelize();

    std::shared_ptr<plx::LegoData> data();

private:
    std::shared_ptr<plx::LegoData> m_data;
};

}

#endif
