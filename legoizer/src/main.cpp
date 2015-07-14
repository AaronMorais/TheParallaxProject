#include "obj_data.h"
#include "services/legoizer.h"

#include <cstring>
#include <iostream>
#include <fstream>

int
main(
    int argc,
    char** argv
    )
{
    if (argc == 1) {
        std::cerr << "No arguments specified" << std::endl;
        std::cerr << "Expected input: " << argv[0] << "-f <filename> [-m <materials path>] [-v] [-o <outfile>]"  << std::endl;
        std::cerr << "-f    .obj file" << std::endl;
        std::cerr << "-o    output file input" << std::endl;
        std::cerr << "-m    materials for .obj file" << std::endl;
        std::cerr << "-v    should voxelize .obj input" << std::endl;
        std::cerr << "-i    should print .obj information" << std::endl;
        return 1;
    }

    bool shouldLegoize = false;
    std::string filename;
    std::string modelpath;
    std::ostream* os = &std::cout;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 && (i+1 < argc)) {
            filename = argv[i+1]; i+=1;
        } else if (strcmp(argv[i], "-m") == 0 && (i+1 < argc)) {
            modelpath = argv[i+1]; i+=1;
        } else if (strcmp(argv[i], "-l") == 0) {
            shouldLegoize = true;
        } else if (strcmp(argv[i], "-o") == 0 && (i+1 < argc)) {
            std::string outfile = argv[i+1]; i+=1;
            std::ofstream ofs(outfile.c_str());
            os = &ofs;
        }
    }

    if (filename.empty()) {
        std::cerr << "Filename not specified" << std::endl;
        return 1;
    }

    std::shared_ptr<tinyobj::ObjData> data;
    if (modelpath.empty()) {
        data = tinyobj::ObjData::Factory::Create(filename.c_str());
    } else {
        data = tinyobj::ObjData::Factory::Create(filename.c_str(), modelpath.c_str());
    }

    std::shared_ptr<plx::LegoData> legoData = std::make_shared<plx::LegoData>(data);

    if (shouldLegoize) {
        std::shared_ptr<plx::Legoizer> legoizer = std::make_shared<plx::Legoizer>(data);

        std::shared_ptr<plx::LegoData> legoizer->process();
    }

    std::cout << "Completed" << std::endl;

    return 0;
}
