#ifndef _LEGOIZER_H__
#define _LEGOIZER_H__

#include "lego_data.h"

namespace plx {

class Legoizer {
public:
    Legoizer(std::shared_ptr<tinyobj::ObjData> data);

    void voxelize();
    void preprocessor();

    void writeAlloy(std::ostream& os);
    void writeAlloyOld(std::ostream& os);
    void writeObj(std::ostream& os);

    std::shared_ptr<plx::LegoData> data();

    static int subdivisions;
    static bool shouldFillShell;

private:
    std::shared_ptr<plx::LegoData> m_data;
};

}

#endif
