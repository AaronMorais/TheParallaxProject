#include "obj_viewer.h"
#include "obj_printer.h"
#include "obj_data.h"

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

  // ObjPrint::PrintFromData(data);

  ObjViewer::GetInstance().SetData(data);

  ObjViewer::GetInstance().Run();

  std::cout << "Completed" << std::endl;

  return 0;
}
