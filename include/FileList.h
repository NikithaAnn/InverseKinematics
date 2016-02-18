#ifndef FILELIST_H__
#define FILELIST_H__

#include<string>
#include <ngl/Vec3.h>
#include "Node.h"

/// \author NIKITHA ANN GEORGE
/// \version 3.0
/// \date 29th January 2015 \n


/// \class FileList
/// \brief A class that reads the data off a BVH file

class FileList
{
public:
  inline FileList():m_numberofjoints(0)//,m_node(NULL)//,m_start(NULL), m_end(NULL)
{
}
 ~FileList();

  void PrintPosition();

  void PrintDrawAngles();

  void addChild(ngl::Vec3 _childposition);

  void load(const std::string &_fname);  //Loading the file

  void loadStructure(std::ifstream &_fname); //Loading the complete structure into a class object

  void loadNode(std::ifstream &_fname);

  void loadAngles();

  void buildVAO();

  void drawFile(int _spinXFace, int _spinYFace, ngl::Mat4 _mouseGlobalTX, ngl ::Vec3 _modelPos, ngl::Camera _cam);

  ngl::Vec3 return_End();
  std::vector <Node>  m_jointData;

private:
  Node m_node;
  int m_numberofjoints;
  std::unique_ptr<ngl::VertexArrayObject> m_vao;


};//end  class

#endif
