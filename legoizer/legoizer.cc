#include "legoizer.h"
#include <fstream>
#include <string>
#include <math.h>
#include <cassert>
#include <iostream>
#include <climits>

Legoizer::Legoizer(std::vector<Face> face_list) {
  faces = face_list;
  setMaxValues();
  initializeLegoGrid();
}

Legoizer::~Legoizer() {}

Legoizer::Vertex Legoizer::getDimensions() {
  Vertex dim;
  dim.x = width;
  dim.y = height;
  dim.z = depth;
  return dim;
}

int Legoizer::totalBlocks() {
  int count = 0;
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      for (int k = 0; k < depth; k++) {
        count += legoGrid[i][j][k];
      }
    }
  }
  return count;
}
Legoizer::THREE_D_VECTOR Legoizer::convert() {
  convertShell();
  return fillShell();
}

Legoizer::THREE_D_VECTOR Legoizer::convertShell() {
  for(std::vector<Face>::iterator face = faces.begin() ; face != faces.end(); ++face) {
    process(*face);
  }
  return legoGrid;
}

Legoizer::THREE_D_VECTOR Legoizer::fillShell() {
  //TODO
  return legoGrid;
}

Legoizer::Vertex Legoizer::midPoint(Vertex v1, Vertex v2) {
  double x = (v1.x + v2.x) / 2;
  double y = (v1.y + v2.y) / 2;
  double z = (v1.z + v2.z) / 2;
  return Vertex{x,y,z};
}

int Legoizer::dist(Legoizer::Vertex v1, Legoizer::Vertex v2) {
  return round(RESOLUTION * sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2) + pow(v2.z - v1.z, 2)));
}

bool Legoizer::minimumFaceSize(Face face) {
  return dist(face.vertices[0], face.vertices[1]) == 0 &&
          dist(face.vertices[1], face.vertices[2]) == 0 &&
          dist(face.vertices[0], face.vertices[2]) == 0;
}

void Legoizer::process(Face face) {
  assert(face.vertices.size() == 3);
  if (minimumFaceSize(face)) {
    addToGrid(face.vertices[0]);
    return;
  }
  for(std::vector<Vertex>::iterator vtx = face.vertices.begin() ; vtx != face.vertices.end(); ++vtx) {
    addToGrid(*vtx);
  }
  Vertex mid1 = midPoint(face.vertices[0], face.vertices[1]);
  Vertex mid2 = midPoint(face.vertices[1], face.vertices[2]);
  Vertex mid3 = midPoint(face.vertices[0], face.vertices[2]);
  std::vector<Vertex> face1;
  std::vector<Vertex> face2;
  std::vector<Vertex> face3;
  std::vector<Vertex> face4;
  face1.push_back(face.vertices[0]);
  face1.push_back(mid1);
  face1.push_back(mid3);

  face2.push_back(mid1);
  face2.push_back(face.vertices[1]);
  face2.push_back(mid2);

  face3.push_back(mid3);
  face3.push_back(mid2);
  face3.push_back(face.vertices[2]);

  face4.push_back(mid1);
  face4.push_back(mid2);
  face4.push_back(mid3);
  process(Face{face1});
  process(Face{face2});
  process(Face{face3});
  process(Face{face4});
}

void Legoizer::initializeLegoGrid() {
  legoGrid =
      std::vector<std::vector<std::vector<int>>>(width,
      std::vector<std::vector<int>>(height,
      std::vector<int>(depth, 0)));
}

int Legoizer::scale(double val, dimension dim) {
  int ret;
  switch(dim) {
    case X:
      ret = (width-1) * (val / (maxX - minX));
      break;
    case Y:
      ret = (height-1) * (val / (maxY - minY));
      break;
    case Z:
      ret = (depth-1) * (val / (maxZ - minZ));
      break;
    default:
      ret = 0;
      break;
  }
  return ret;
}

bool Legoizer::addToGrid(Vertex vtx) {
  int x = scale(vtx.x, X);
  int y = scale(vtx.y, Y);
  int z = scale(vtx.z, Z);
  if (x >= width || y >= height || z >= depth) {
    return false;
  }
  if (!legoGrid[x][y][z]) {
    // std::cout << "Adding to grid (" << x << ", " << y << ", " << z << ")" << std::endl;
    legoGrid[x][y][z] = 1;
  }
  return true;
}

void Legoizer::setMaxWidth(int x) {
  width = x;
}
void Legoizer::setMaxHeight(int y) {
  height = y;
}
void Legoizer::setMaxDepth(int z) {
  depth = z;
}

void Legoizer::setMaxValues() {
  maxX = INT_MIN;
  maxY = INT_MIN;
  maxZ = INT_MIN;
  minX = INT_MAX;
  minY = INT_MAX;
  minZ = INT_MAX;
  for(std::vector<Face>::iterator face = faces.begin() ; face != faces.end(); ++face) {
    for(std::vector<Vertex>::iterator it = face->vertices.begin() ; it != face->vertices.end(); ++it) {
      Vertex vertex = *it;
      if (vertex.x > maxX) {
        maxX = vertex.x;
      }
      if (vertex.x < minX) {
        minX = vertex.x;
      }
      if (vertex.y > maxY) {
        maxY = vertex.y;
      }
      if (vertex.y < minY) {
        minY = vertex.y;
      }
      if (vertex.z > maxZ) {
        maxZ = vertex.z;
      }
      if (vertex.z < minZ) {
        minZ = vertex.z;
      }
    }
  }
  setMaxWidth(floor((maxX - minX) * RESOLUTION) + 1);
  setMaxHeight(floor((maxY - minY) * RESOLUTION) + 1);
  setMaxDepth(floor((maxZ - minZ) * RESOLUTION) + 1);
}