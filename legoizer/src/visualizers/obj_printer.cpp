#include "obj_printer.h"

void
ObjPrint::PrintObjInfo(
  const std::vector<tinyobj::shape_t>& shapes,
  const std::vector<tinyobj::material_t>& materials
)
{
  std::cout << "# of shapes    : " << shapes.size() << std::endl;
  std::cout << "# of materials : " << materials.size() << std::endl;

  for (size_t i = 0; i < shapes.size(); i++) {
    printf("shape[%u].name = %s\n", i, shapes[i].name.c_str());
    printf("Size of shape[%u].indices: %u\n", i, shapes[i].mesh.indices.size());
    printf("Size of shape[%u].material_ids: %u\n", i, shapes[i].mesh.material_ids.size());
    assert((shapes[i].mesh.indices.size() % 3) == 0);
    for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++) {
      printf("  idx[%u] = %d, %d, %d. mat_id = %d\n", f, shapes[i].mesh.indices[3*f+0], shapes[i].mesh.indices[3*f+1], shapes[i].mesh.indices[3*f+2], shapes[i].mesh.material_ids[f]);
    }

    printf("shape[%u].vertices: %u\n", i, shapes[i].mesh.positions.size());
    assert((shapes[i].mesh.positions.size() % 3) == 0);
    for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
      printf("  v[%u] = (%f, %f, %f)\n", v,
        shapes[i].mesh.positions[3*v+0],
        shapes[i].mesh.positions[3*v+1],
        shapes[i].mesh.positions[3*v+2]);
    }
  }

  for (size_t i = 0; i < materials.size(); i++) {
    printf("material[%u].name = %s\n", i, materials[i].name.c_str());
    printf("  material.Ka = (%f, %f ,%f)\n", materials[i].ambient[0], materials[i].ambient[1], materials[i].ambient[2]);
    printf("  material.Kd = (%f, %f ,%f)\n", materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2]);
    printf("  material.Ks = (%f, %f ,%f)\n", materials[i].specular[0], materials[i].specular[1], materials[i].specular[2]);
    printf("  material.Tr = (%f, %f ,%f)\n", materials[i].transmittance[0], materials[i].transmittance[1], materials[i].transmittance[2]);
    printf("  material.Ke = (%f, %f ,%f)\n", materials[i].emission[0], materials[i].emission[1], materials[i].emission[2]);
    printf("  material.Ns = %f\n", materials[i].shininess);
    printf("  material.Ni = %f\n", materials[i].ior);
    printf("  material.dissolve = %f\n", materials[i].dissolve);
    printf("  material.illum = %d\n", materials[i].illum);
    printf("  material.map_Ka = %s\n", materials[i].ambient_texname.c_str());
    printf("  material.map_Kd = %s\n", materials[i].diffuse_texname.c_str());
    printf("  material.map_Ks = %s\n", materials[i].specular_texname.c_str());
    printf("  material.map_Ns = %s\n", materials[i].normal_texname.c_str());
    std::map<std::string, std::string>::const_iterator it(materials[i].unknown_parameter.begin());
    std::map<std::string, std::string>::const_iterator itEnd(materials[i].unknown_parameter.end());
    for (; it != itEnd; it++) {
      printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
    }
    printf("\n");
  }
}

bool
ObjPrint::PrintFromData(
  std::shared_ptr<ObjData> data
)
{
  PrintObjInfo(*(data->m_shapes), *(data->m_materials));
  return true;
}

bool
ObjPrint::PrintFromFile(
  const char* filename,
  const char* basepath
)
{
  std::cout << "Loading " << filename << std::endl;

  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string err = tinyobj::LoadObj(shapes, materials, filename, basepath);

  if (!err.empty()) {
    std::cerr << err << std::endl;
    return false;
  }

  ObjPrint::PrintObjInfo(shapes, materials);

  return true;
}


bool
ObjPrint::PrintFromStream(std::stringstream& objStream, std::string matString)
{
    std::cout << "Stream Loading " << std::endl;

    using namespace tinyobj;
    class MaterialStringStreamReader:
        public MaterialReader
    {
        public:
            MaterialStringStreamReader(const std::string& matSStream): m_matSStream(matSStream) {}
            virtual ~MaterialStringStreamReader() {}
            virtual std::string operator() (
              const std::string& matId,
              std::vector<material_t>& materials,
              std::map<std::string, int>& matMap)
            {
                return LoadMtl(matMap, materials, m_matSStream);
            }

        private:
            std::stringstream m_matSStream;
    };

    MaterialStringStreamReader matSSReader(matString);
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err = tinyobj::LoadObj(shapes, materials, objStream, matSSReader);

    if (!err.empty()) {
      std::cerr << err << std::endl;
      return false;
    }

    ObjPrint::PrintObjInfo(shapes, materials);

    return true;
}

bool
ObjPrint::PrintFromDataToFile(std::shared_ptr<ObjData> data, const char* filename)
{
    std::ofstream ofs(filename);
    if (!ofs) {
        std::cerr << "Cannot open file [" << filename << "]" << std::endl;
        return false;
    }

    std::shared_ptr<std::vector<tinyobj::shape_t>> shapes = data->m_shapes;

    for (unsigned int pp = 0; pp < shapes->size(); ++pp)
    {
        ofs << std::endl << "g" << std::endl;
        tinyobj::mesh_t& mesh = (*shapes)[pp].mesh;
        std::vector<glm::vec3>& vertices = mesh.vertices;
        std::vector<tinyobj::face_t>& faces = mesh.faces;

        for (auto it = vertices.begin(); it != vertices.end(); ++it) {
            glm::vec3 vertex = *it;
            ofs << "v " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
        }
        ofs << std::endl;
        for (auto it = faces.begin() ; it != faces.end(); ++it) {
            tinyobj::face_t face = *it;
            ofs << "f " << face.v1 << " " << face.v2 << " " << face.v3 << std::endl;
        }
    }

    return true;
}
