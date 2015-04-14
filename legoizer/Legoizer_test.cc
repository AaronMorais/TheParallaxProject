#include "legoizer.h"
#include <iostream>
int TotalTests = 0;

template <class type>
bool expectEQ(type a, type b) {
  if (a != b) {
    std::cout << "FAIL: expected " << b << " but got " << a << std::endl;
    return false;
  }
  return true;
}

Legoizer basicLego() {
  Legoizer::Vertex vtx1;
  vtx1.x = 0;
  vtx1.y = 0;
  vtx1.z = 0;
  Legoizer::Vertex vtx2;
  vtx2.x = 0.01;
  vtx2.y = 0.01;
  vtx2.z = 0.01;
  Legoizer::Vertex vtx3;
  vtx3.x = 0.01;
  vtx3.y = 0.01;
  vtx3.z = 0;
  Legoizer::Face face;
  face.vertices.push_back(vtx1);
  face.vertices.push_back(vtx2);
  face.vertices.push_back(vtx3);
  std::vector<Legoizer::Face> faces;
  faces.push_back(face);
  return Legoizer(faces);
}

std::vector<Legoizer::Face> cube() {
  Legoizer::Vertex FDL;
  FDL.x = 0;
  FDL.y = 0;
  FDL.z = 0;
  Legoizer::Vertex BDL;
  BDL.x = 0;
  BDL.y = 0;
  BDL.z = 1;
  Legoizer::Vertex FUL;
  FUL.x = 0;
  FUL.y = 1;
  FUL.z = 0;
  Legoizer::Vertex BUL;
  BUL.x = 0;
  BUL.y = 1;
  BUL.z = 1;
  Legoizer::Vertex FDR;
  FDR.x = 1;
  FDR.y = 0;
  FDR.z = 0;
  Legoizer::Vertex BDR;
  BDR.x = 1;
  BDR.y = 0;
  BDR.z = 1;
  Legoizer::Vertex FUR;
  FUR.x = 1;
  FUR.y = 1;
  FUR.z = 0;
  Legoizer::Vertex BUR;
  BUR.x = 1;
  BUR.y = 1;
  BUR.z = 1;
  Legoizer::Face Fa;
  Legoizer::Face Fb;
  Legoizer::Face La;
  Legoizer::Face Lb;
  Legoizer::Face Ra;
  Legoizer::Face Rb;
  Legoizer::Face Ua;
  Legoizer::Face Ub;
  Legoizer::Face Ba;
  Legoizer::Face Bb;
  Legoizer::Face Da;
  Legoizer::Face Db;

  Fa.vertices.push_back(FUL);
  Fa.vertices.push_back(FUR);
  Fa.vertices.push_back(FDL);
  Fb.vertices.push_back(FDR);
  Fb.vertices.push_back(FUR);
  Fb.vertices.push_back(FDL);

  Ra.vertices.push_back(BUR);
  Ra.vertices.push_back(FUR);
  Ra.vertices.push_back(FDR);
  Rb.vertices.push_back(BUR);
  Rb.vertices.push_back(BDR);
  Rb.vertices.push_back(FDR);

  Ba.vertices.push_back(BUR);
  Ba.vertices.push_back(BUL);
  Ba.vertices.push_back(BDR);
  Bb.vertices.push_back(BDL);
  Bb.vertices.push_back(BUL);
  Bb.vertices.push_back(BDR);

  La.vertices.push_back(BDL);
  La.vertices.push_back(BUL);
  La.vertices.push_back(FUL);
  Lb.vertices.push_back(BDL);
  Lb.vertices.push_back(FDL);
  Lb.vertices.push_back(FUL);

  Ua.vertices.push_back(BUR);
  Ua.vertices.push_back(BUL);
  Ua.vertices.push_back(FUL);
  Ub.vertices.push_back(BUR);
  Ub.vertices.push_back(FUR);
  Ub.vertices.push_back(FUL);

  Da.vertices.push_back(FDL);
  Da.vertices.push_back(BDL);
  Da.vertices.push_back(BDR);
  Db.vertices.push_back(FDL);
  Db.vertices.push_back(FDR);
  Db.vertices.push_back(BDR);

  std::vector<Legoizer::Face> faces;
  faces.push_back(Fa);
  faces.push_back(Fb);
  faces.push_back(Ra);
  faces.push_back(Rb);
  faces.push_back(Ba);
  faces.push_back(Bb);
  faces.push_back(La);
  faces.push_back(Lb);
  faces.push_back(Ua);
  faces.push_back(Ub);
  faces.push_back(Da);
  faces.push_back(Db);

  return faces;
}

void addTest(std::string testName) {
  TotalTests++;
  std::cout << "RUNNING " << testName << " TEST" << std::endl;
}

bool initial() {
  addTest("Initialization");

  Legoizer legoizer = basicLego();
  Legoizer::Vertex dims = legoizer.getDimensions();

  bool test1 = expectEQ(legoizer.totalBlocks(), 0);
  bool test2 = expectEQ(dims.x, 1.0);
  bool test3 = expectEQ(dims.y, 1.0);
  bool test4 = expectEQ(dims.z, 1.0);

  return test1 && test2 && test3 && test4;
}

bool maxWidth() {
  addTest("Max Width");
  Legoizer legoizer = basicLego();
  legoizer.setMaxWidth(10);
  Legoizer::Vertex dims = legoizer.getDimensions();
  return expectEQ(dims.x, 10.0);
}

bool maxHeight() {
  addTest("Max Height");
  Legoizer legoizer = basicLego();
  legoizer.setMaxHeight(10);
  Legoizer::Vertex dims = legoizer.getDimensions();
  return expectEQ(dims.y, 10.0);
}

bool maxDepth() {
  addTest("Max Depth");
  Legoizer legoizer = basicLego();
  legoizer.setMaxDepth(10);
  Legoizer::Vertex dims = legoizer.getDimensions();
  return expectEQ(dims.z, 10.0);
}

bool blocksBasic() {
  addTest("Basic Block Count");
  Legoizer legoizer = basicLego();
  legoizer.convert();
  return expectEQ(legoizer.totalBlocks(), 1);
}

bool convertCube() {
  addTest("10x10 Cube");
  Legoizer legoizer = Legoizer(cube());
  Legoizer::THREE_D_VECTOR grid = legoizer.convertShell();
  return expectEQ(legoizer.totalBlocks(), 11 * 11 * 11 - 9 * 9 * 9);
}

int main() {
  std::cout << "RUNNING TESTS" << std::endl;
  int passingTests = 0;
  passingTests += initial();
  passingTests += maxHeight();
  passingTests += maxWidth();
  passingTests += maxDepth();
  passingTests += blocksBasic();
  passingTests += convertCube();
  std::cout << passingTests << "/" << TotalTests << " " << 100*(double)passingTests/(double)TotalTests << "\%" << std::endl;
}