#ifndef CCDIK_H__
#define CCDIK_H__
#define MAXCOUNT 25

#include <string>
#include "Node.h"
#include <ngl/Vec3.h>
#include "FileList.h"

/// \author NIKITHA ANN GEORGE
/// \version 3.0
/// \date 29th January 2015 \n


/// \class CCDIK
/// \brief A class that applies the CCDIK to its components.

class CCDIK
{	
public:
  CCDIK();
  ~CCDIK();
  void Reset();
  void AlgorithmCCDIK(int _spinXFace,int _spinYFace, ngl::Mat4 _mouseGlobalTX,ngl ::Vec3 _modelPos, ngl::Camera _cam);
  void CCDIKBuildVAO();
  void CCDIK_drawFile(int _spinXFace,int _spinYFace, ngl::Mat4 _mouseGlobalTX,ngl ::Vec3 _modelPos, ngl::Camera _cam);

  ngl::Vec3 m_target;
  FileList m_mainstructure;
  int m_numberofiterations;




};// end class
#endif
