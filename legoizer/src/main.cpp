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
    std::cerr << "Expected input: " << argv[0] << " -File <file_name>"  << std::endl;
    std::cerr << "-File <filename>      input file in .obj" << std::endl;
    std::cerr << "-Output <filename>    output file (if none, to cout)" << std::endl;
    std::cerr << "-Materials <filepath> materials path for input .obj " << std::endl;
    std::cerr << "-voxelize             voxelize .obj input" << std::endl;
    std::cerr << "-preprocess           preprocess .obj input for alloy" << std::endl;
    std::cerr << "-alloy                write as alloy model (deprecated)" << std::endl;
    std::cerr << "-obj                  write as obj model" << std::endl;
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

    bool should_voxelize = false;
    bool should_preprocess = false;
    bool should_print_alloy = false;
    bool should_print_obj = false;

    std::string file_name;
    std::string model_path;
    std::ostream* os = &std::cout;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-File") == 0 && (i+1 < argc)) {
            file_name = argv[i+1]; i+=1;
        } else if (strcmp(argv[i], "-Output") == 0 && (i+1 < argc)) {
            std::string outfile = argv[i+1]; i+=1;
            std::ofstream* ofs = new std::ofstream(outfile.c_str());
            os = ofs;
        } else if (strcmp(argv[i], "-Materials") == 0 && (i+1 < argc)) {
            model_path = argv[i+1]; i+=1;
        } else if (strcmp(argv[i], "-voxelize") == 0) {
            should_voxelize = true;
        } else if (strcmp(argv[i], "-preprocess") == 0) {
            should_voxelize = true;
            should_preprocess = true;
        } else if (strcmp(argv[i], "-alloy") == 0) {
            should_print_alloy = true;
        } else if (strcmp(argv[i], "-alloy_obj") == 0) {
            should_print_obj = true;
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

    std::shared_ptr<plx::Legoizer> legoizer = std::make_shared<plx::Legoizer>(obj_data);

    if (should_voxelize) {
        legoizer->voxelize();

        if (should_preprocess) {
            legoizer->preprocess();
        }
    }

    if (should_voxelize && should_preprocess) {
        legoizer->printProcessed(*os);
    } else if (should_print_alloy) {
        legoizer->printAlloy(*os);
    } else if (should_print_obj) {
        legoizer->printObj(*os);
    }

    std::cerr << "Completed" << std::endl;

    if (os != &std::cout) {
        delete os;
    } else {
        os = nullptr;
    }

    return 0;
}
