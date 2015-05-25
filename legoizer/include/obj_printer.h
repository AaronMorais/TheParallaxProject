#ifndef _OBJ_PRINT_H
#define _OBJ_PRINT_H

#include "obj_data.h"
#include "tiny_obj_loader.h"

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>

class ObjPrint {
public:
    static bool PrintFromData(std::shared_ptr<ObjData> data);
    static bool PrintFromFile(const char* filename, const char* basepath = NULL);
    static bool PrintFromStream(std::stringstream& objStream, std::string materials);

    static bool PrintFromDataToFile(std::shared_ptr<ObjData> data, const char* filename);
private:
    static void PrintObjInfo(const std::vector<tinyobj::shape_t>& shapes, const std::vector<tinyobj::material_t>& materials);
};

#endif //
