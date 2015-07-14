#include "lego_data.h"

namespace plx {

LegoData::LegoData(std::shared_ptr<tinyobj::ObjData> data) {
    m_data = data;
}

void LegoData::InfoPrint() {
    m_data->InfoPrint();
}

void LegoData::ObjPrint(std::ostream& os) {
    m_data->ObjPrint(os);
}

}
