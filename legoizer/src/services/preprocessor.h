#ifndef _PREPROCESSOR_H__
#define _PREPROCESSOR_H__

#include "lego_data.h"

namespace plx {

class Preprocessor {
public:
    Preprocessor(std::shared_ptr<plx::LegoData> data);

    void process();

    std::shared_ptr<plx::LegoData> data();

private:
    std::shared_ptr<plx::LegoData> m_data;
};

}

#endif
