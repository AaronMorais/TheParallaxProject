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

    static int subdivisions;
    static bool shouldFillShell;

private:
    std::shared_ptr<plx::LegoData> m_data;
};

}

#endif
