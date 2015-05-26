#include "obj_viewer.h"
#include "obj_printer.h"
#include "obj_data.h"
#include "voxelizer.h"

#include <iostream>

int
main(
  int argc,
  char** argv
)
{
  std::shared_ptr<ObjData> data;
  if (argc > 1) {
    const char* basepath = NULL;
    if (argc > 2) {
      basepath = argv[2];
    }

    data = ObjData::CreateDataFromFile(argv[1], basepath);
  } else {
    return 1;
  }

  std::shared_ptr<Voxelizer> voxelizer = std::make_shared<Voxelizer>();

  auto vData = voxelizer->Process(data);

  ObjViewer::GetInstance().SetData(vData);

  ObjViewer::GetInstance().Run();


  std::cout << "Test Completed" << std::endl;

  return 0;
}
