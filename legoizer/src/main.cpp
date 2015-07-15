#include "obj_data.h"
#include "services/legoizer.h"

#include <cstring>
#include <iostream>
#include <fstream>

void printError(
    int argc,
    char** argv
    )
{
    std::cerr << "Expected input: " << argv[0] << "-f <file_name> [-m <materials path>] [-v] [-o <outfile>]"  << std::endl;
    std::cerr << "-f    .obj file" << std::endl;
    std::cerr << "-o    output file (if none, to cout)" << std::endl;
    std::cerr << "-m    materials path for .obj " << std::endl;
    std::cerr << "-l    should voxelize .obj input" << std::endl;
    std::cerr << "-i    print .obj information" << std::endl;
}

int
main(
    int argc,
    char** argv
    )
{
    if (argc == 1) {
        std::cerr << "No arguments specified" << std::endl;
        printError(argc, argv);
        return 1;
    }

    bool should_legoize = false;
    bool should_print_obj_info = false;

    std::string file_name;
    std::string model_path;
    std::ostream* os = &std::cout;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 && (i+1 < argc)) {
            file_name = argv[i+1]; i+=1;
        } else if (strcmp(argv[i], "-o") == 0 && (i+1 < argc)) {
            std::string outfile = argv[i+1]; i+=1;
            std::ofstream ofs(outfile.c_str());
            os = &ofs;
        } else if (strcmp(argv[i], "-m") == 0 && (i+1 < argc)) {
            model_path = argv[i+1]; i+=1;
        } else if (strcmp(argv[i], "-l") == 0) {
            should_legoize = true;
        } else if (strcmp(argv[i], "-i") == 0) {
            should_print_obj_info = true;
        }
    }

    if (file_name.empty()) {
        std::cerr << "File name must be specified" << std::endl;
        printError(argc, argv);
        return 1;
    }

    std::shared_ptr<tinyobj::ObjData> obj_data;
    if (model_path.empty()) {
        obj_data = tinyobj::ObjData::Factory::Create(file_name.c_str());
    } else {
        obj_data = tinyobj::ObjData::Factory::Create(file_name.c_str(), model_path.c_str());
    }

    std::shared_ptr<plx::LegoData> lego_data = std::make_shared<plx::LegoData>(obj_data);
    std::shared_ptr<plx::Legoizer> legoizer = std::make_shared<plx::Legoizer>(lego_data);

    if (should_print_obj_info) {
        legoizer->data()->data()->print();
    }

    if (should_legoize) {
        legoizer->voxelize();
    }

    legoizer->data()->print(*os);

    std::cout << "Completed" << std::endl;

    if (os == &std::cout) {
        os = nullptr;
    }

    return 0;
}
