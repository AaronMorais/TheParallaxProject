#ifndef _LEGO_DATA_H__
#define _LEGO_DATA_H__

#include "obj_data.h"

namespace plx {

class LegoData {
public:
    LegoData(std::shared_ptr<tinyobj::ObjData> data);

    void InfoPrint();
    void ObjPrint(std::ostream& os);

private:
    std::shared_ptr<tinyobj::ObjData> m_data;
};

}

#endif
