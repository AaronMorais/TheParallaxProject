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

  // ObjPrint::PrintFromData(data);

  std::shared_ptr<Voxelizer> voxelizer = std::make_shared<Voxelizer>();

  auto vData = voxelizer->Process(data);

    tinyobj::mesh_t& meshie = (*(vData->m_shapes))[0].mesh;
    std::vector<tinyobj::face_t>& faces = meshie.faces;
    std::vector<glm::vec3>& vertices = meshie.vertices;

    // std::cout <<"poo2: " << meshie.vertices[meshie.faces[0].v1].x << ", " << meshie.vertices[meshie.faces[0].v1].y << ", " << meshie.vertices[meshie.faces[0].v1].z << std::endl;


  ObjViewer::GetInstance().SetData(vData);

  ObjViewer::GetInstance().Run();


  std::cout << "Test Completed" << std::endl;

  return 0;
}
