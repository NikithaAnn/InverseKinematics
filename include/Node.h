#ifndef NODE_H__
#define NODE_H__
#define MAX_CHCOUNT 10 //Create a maximum limit of 4 for this program
#include <string>
#include <stack>
#include <sstream>
#include <ngl/Camera.h>
#include <ngl/Vec3.h>
#include <ngl/Colour.h>
#include <ngl/Transformation.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <string>
#include <ngl/Mat3.h>
#include <memory>

/// \author NIKITHA ANN GEORGE
/// \version 1.0
/// \date 29th January 2015 \n


/// \class Node
/// \brief A class that has the basic structure of a node

class Node
{
public:
  inline Node() :m_length(1),m_drawAngles(0),m_localposition(ngl::Vec3 (0,0,0)) //m_child(NULL),m_parent(NULL)//,m_mat(ngl::Mat3(0,0,0,0,0,0,0,0,0))
  {

  }
  ~Node();

  ngl::Vec3 m_localposition;//coordinates in local Space
  ngl::Vec3 m_worldposition;// coordinates in world space

  float m_length; //Distance to the next Node
  float m_zAngle; // Default orientation when read from file
  float m_drawAngles; //Used mainly for plotting
  int m_jointnumber;  //Joint id when read from a file.


};// end class
#endif
