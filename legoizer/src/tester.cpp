#include "visualizers/obj_viewer.h"
#include "obj_data.h"
#include "services/voxelizer.h"
#include "services/legoizer.h"

#include <iostream>
#include <cstring>

void printError(
    int argc,
    char** argv
    )
{
    std::cerr << "Expected input: " << argv[0] << " -File <file_name>"  << std::endl;
    std::cerr << "-File <filename>      input file in .obj" << std::endl;
    std::cerr << "-Materials <filepath> materials path for input .obj " << std::endl;
    std::cerr << "-voxelize             voxelize .obj input" << std::endl;
    std::cerr << "-subdivisions <int>   number of subdivisions" << std::endl;
    std::cerr << "-fill                 fill the voxel model" << std::endl;
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

    std::string file_name;
    std::string model_path;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-File") == 0 && (i+1 < argc)) {
            file_name = argv[i+1]; i+=1;
        } else if (strcmp(argv[i], "-Materials") == 0 && (i+1 < argc)) {
            model_path = argv[i+1]; i+=1;
        } else if (strcmp(argv[i], "-voxelize") == 0) {
            should_legoize = true;
        } else if (strcmp(argv[i], "-fill") == 0) {
            plx::Legoizer::shouldFillShell = true;
        } else if (strcmp(argv[i], "-subdivisions") == 0 && (i+1 < argc)) {
            plx::Legoizer::subdivisions = atoi(argv[i+1]);
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

    if (should_legoize) {
        legoizer->voxelize();
    }

    ObjViewer::GetInstance().SetData(lego_data);

    ObjViewer::GetInstance().Run();

    std::cerr << "Completed" << std::endl;

    return 0;
}
