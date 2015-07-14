#ifndef _LEGOIZER_H__
#define _LEGOIZER_H__

#include "obj_data.h"

namespace plx {

class Legoizer {
public:
    Legoizer(std::shared_ptr<tinyobj::ObjData> data);

    void voxelize();
private:
    std::shared_ptr<tinyobj::ObjData> m_data;
};

}

#endif
