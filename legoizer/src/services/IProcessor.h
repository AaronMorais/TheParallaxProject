#ifndef _IPROCESSOR_H__
#define _IPROCESSOR_H__

#include "lego_data.h"

namespace plx {

class IProcessor {
    plx::LegoData process(plx::LegoData data) = 0;
};

}

#endif
