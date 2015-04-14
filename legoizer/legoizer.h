#pragma once

#include <string>
#include <vector>
class Legoizer
{
public:
  typedef std::vector<std::vector<std::vector<int>>> THREE_D_VECTOR;
  struct Vertex {
    double x;
    double y;
    double z;
  };
  struct Face {
    std::vector<Vertex> vertices;
  };
  Legoizer(std::vector<Face> face_list);
  ~Legoizer();
  THREE_D_VECTOR convert();
  THREE_D_VECTOR convertShell();
  void setMaxHeight(int x);
  void setMaxWidth(int y);
  void setMaxDepth(int z);
  int totalBlocks();
  Vertex getDimensions();
private:
  const int RESOLUTION = 10;// magnitudes of 10
  const double LEGOSCALE = 1.65625; // (Width/depth: 15.9 / height: 9.6)
  enum dimension { X, Y, Z };
  int height;
  int width;
  int depth;
  double maxX;
  double minX;
  double maxY;
  double minY;
  double maxZ;
  double minZ;
  THREE_D_VECTOR legoGrid;
  THREE_D_VECTOR fillShell();
  std::vector<Face> faces;
  Vertex midPoint(Vertex v1, Vertex v2);
  int dist(Vertex v1, Vertex v2);
  void initializeLegoGrid();
  void setMaxValues();
  int scale(double value, dimension dim);
  void process(Face face);
  bool addToGrid(Vertex vtx);
  bool minimumFaceSize(Face face);
};