#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cstring>
#include <math.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/Transformation.h>
#include <ngl/Mat3.h>
#include <ngl/Material.h>
#include <ngl/Camera.h>
#include <ngl/Vec3.h>
#include "FileList.h"
#include "Node.h"
#include "Def.h"
//----------------------------------------------------------------------------------------------------------------------
FileList::~FileList()
{
  std::cout << "Filelist destructor" << std::endl;
}
//----------------------------------------------------------------------------------------------------------------------
ngl::Vec3 FileList::return_End()
{
  return m_jointData[m_jointData.size()-1].m_worldposition;
}
//----------------------------------------------------------------------------------------------------------------------
void FileList::addChild(ngl::Vec3 _childlocalposition)
{

//  std::cout<<m_numberofjoints<<std::endl;
//  std::cout<<"ADDCHILD:"<<std::endl;

  m_node.m_localposition.m_x = _childlocalposition.m_x;
  m_node.m_localposition.m_y = _childlocalposition.m_y;
  m_node.m_localposition.m_z = 0;


  if(m_numberofjoints > MAX_CHCOUNT)
     exit(0);
  else
  {

    if (m_numberofjoints == 0) //Creating a null root
    {
        m_node.m_worldposition = m_node.m_localposition;
        m_node.m_zAngle =0;
        m_node.m_jointnumber= 1;

//        std::cout<<"INSIDE 0 ROOT"<<std::endl;
    }
    else
    {
      auto id = m_jointData.rbegin();
      //id++;

      ngl::Vec3 localposition = (*id).m_localposition;
      ngl::Vec3 worldposition = (*id).m_worldposition;

      float length;
      float angle ;


      length = CalculateDistance(worldposition, worldposition+_childlocalposition);

      angle = CalculateAngle(worldposition, worldposition+_childlocalposition);

      (*id).m_length = length;
      (*id).m_zAngle = angle;

      m_node.m_worldposition.m_x = worldposition.m_x + length * cos((*id).m_zAngle);
      m_node.m_worldposition.m_y =worldposition.m_y + length * sin((*id).m_zAngle);
      m_node.m_jointnumber = (*id).m_jointnumber +1;
      m_node.m_zAngle = (*id).m_zAngle ;
    }


  }
  m_jointData.push_back( m_node);
  m_numberofjoints++;

}
void FileList::PrintPosition()
{
  std::cout<<"---Printing World Position--- "<<std::endl;
  for(int i=0; i<m_jointData.size();i++)
  {
    std::cout<<m_jointData[i].m_worldposition.m_x<<" "<<m_jointData[i].m_worldposition.m_y<<std::endl;

  }
}
//----------------------------------------------------------------------------------------------------------------------
void FileList::PrintDrawAngles()
{
  std::cout<<"---Printing Draw Angles--- "<<std::endl;
  for(int i=0; i<m_jointData.size();i++)
  {
    std::cout<<m_jointData[i].m_drawAngles<<std::endl;

  }
}

void FileList::loadAngles()
{

  std::cout<<"*******LOAD ANGLES"<<std::endl;

  for (int i = 0;i<m_jointData.size(); ++i)
  {

    ngl:: Vec3 _BC;

     _BC = m_jointData[i].m_worldposition- m_jointData[i+1].m_worldposition;


     std::cout<<std::endl<<"Printing x and y values : "<<_BC.m_x<<" "<<_BC.m_y<<std::endl;
     std::cout<<"Printing absolute value of x : "<<fabs(_BC.m_x)<<std::endl;

      if(fabs(_BC.m_x) > Epsilon)  //!= 0)
        {
          m_jointData[i].m_drawAngles = atan(_BC.m_y/_BC.m_x)*180/3.141-90;
          std::cout<<" ????Calculating Angles: "<<m_jointData[i].m_drawAngles<<std::endl;
         }


      if(i==(m_jointData.size()-1))
      {
        std::cout<<" Other value : "<<m_jointData[i].m_drawAngles<<std::endl;
        m_jointData[i].m_drawAngles = 0;
      }

  }

  PrintDrawAngles();


}
//----------------------------------------------------------------------------------------------------------------------
void FileList::load(const std::string & _fname)
{
  std::ifstream input;
  std::string line;
  input.open(_fname);
  //m_numberofjoints = 0;
  if(!input.is_open())
  {
    std::cerr<<"Problem Opening File"<<_fname<<"\n";
    exit(EXIT_FAILURE);
  }

  while(!input.eof())
  {
     input>>line;
     if(trim(line)=="HIERARCHY")
      loadStructure(input);
     break;
  }

     input.close();

     Node temp;

//  std::cout<<"\nDEBUG\n";
//  std::cout<<"Size :"<<m_jointData.size()<<std::endl;


}
//----------------------------------------------------------------------------------------------------------------------
void FileList::loadStructure(std::ifstream &_fname)
{
  std::string read;

      while(!_fname.eof() )
      {
          _fname>>read;

          if(trim(read) == "ROOT")
          {
              _fname>>read;
              loadNode(_fname);
          }

       }
}
//----------------------------------------------------------------------------------------------------------------------
void FileList::loadNode(std::ifstream &_fname)
{
   std::string tmp;
   ngl::Vec3 _temp;

  while( !_fname.eof() )
  {
    _fname >> tmp;
    tmp = trim(tmp);

    if( tmp == "OFFSET" )
    {
      // reading  m_position values
      _fname>>_temp.m_x>> _temp.m_y>>_temp.m_z;

//      std::cout<<"VALUE TO BE INSERTED: "<<_temp.m_x<<" "<<_temp.m_y<<" "<<_temp.m_z<<std::endl<<std::endl;
      addChild(_temp);
    }
    else if( tmp == "JOINT" )
    {
      loadNode(_fname);
    }
    else if( tmp == "End" )
    {
      // to finish off the word End Site
      _fname>> tmp >> tmp; // temp currently holds {
      _fname >> tmp;

      if( tmp == "OFFSET" )
        _fname >>_temp.m_x //setting the x value of the temp new node
               >> _temp.m_y
               >> _temp.m_z;

//      std::cout<<"ENDSITE :"<<std::endl;
      addChild(_temp);

//      std::cout<<"inside LoadnOde last : "<<_temp.m_x<<" "<<_temp.m_y<<" "<<_temp.m_z<<std::endl;

      //_fname >> tmp;
     }
  }

}
//----------------------------------------------------------------------------------------------------------------------
void FileList::buildVAO()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  shader->use("nglColourShader");
  shader->setRegisteredUniform4f("Colour",1 ,0.972549 ,0.862745,1); //Joint Color


  ngl::Vec3 verts[]=
  {
    ngl::Vec3(0,    0.5,  0.5),
    ngl::Vec3(0,    0,    0),
    ngl::Vec3(-0.5, 0,    0.5),
    ngl::Vec3(0,    0.5,  0.5),
    ngl::Vec3(0,    0,    0),
    ngl::Vec3(0.5,  0,    0.5),
    ngl::Vec3(0,    0.5,  0.5),
    ngl::Vec3(0,    0,    2.5),
    ngl::Vec3(-0.5, 0,    0.5),
    ngl::Vec3(0,    0.5,  0.5),
    ngl::Vec3(0,    0,    2.5),
    ngl::Vec3(0.5,  0,    0.5)
  };
//  std::cout<<"sizeof(verts) "<<sizeof(verts)<<" sizeof(ngl::Vec3) "<<sizeof(ngl::Vec3)<<"\n";
  // create a vao as a series of GL_TRIANGLES
  m_vao.reset( ngl::VertexArrayObject::createVOA(GL_TRIANGLES));
  m_vao->bind();
 // in this case we are going to set our data as the vertices above
  m_vao->setData(sizeof(verts),verts[0].m_x);
  // now we set the attribute pointer to be 0 (as this matches vertIn in our shader)
  m_vao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
  m_vao->setNumIndices(sizeof(verts)/sizeof(ngl::Vec3));
 // now unbind
  m_vao->unbind();




}
//----------------------------------------------------------------------------------------------------------------------
void FileList::drawFile(int _spinXFace,int _spinYFace, ngl::Mat4 _mouseGlobalTX,ngl ::Vec3 _modelPos, ngl::Camera _cam)
{

  loadAngles();


  ngl::Transformation trans;
  // Rotation based on the mouse position for our global
  // transform
  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  // create the rotation matrices
  rotX.rotateX(_spinXFace);
  rotY.rotateY(_spinYFace);
  // multiply the rotations
  _mouseGlobalTX=rotY*rotX;
  // add the translations
  _mouseGlobalTX.m_m[3][0] = _modelPos.m_x;
  _mouseGlobalTX.m_m[3][1] = _modelPos.m_y;
  _mouseGlobalTX.m_m[3][2] =_modelPos.m_z;
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["nglColourShader"]->use();

  ngl::Mat4 MVP;



  for(auto it = m_jointData.begin(); it!= m_jointData.end(); ++it)
  {
    trans.reset();

    trans.setPosition((*it).m_worldposition);
    std::cout << "Angle: " << (*it).m_drawAngles << std::endl;
    trans.setRotation(-90,0,(*it).m_drawAngles);

    //Distance between joints
    float scale = (*it).m_length / 2.5;
    trans.setScale(scale,scale,scale);

    MVP=trans.getMatrix()*_mouseGlobalTX*_cam.getVPMatrix();

   shader->setShaderParamFromMat4("MVP",MVP);


    m_vao->bind();
    m_vao->draw();
    m_vao->unbind();
  }
}
//----------------------------------------------------------------------------------------------------------------------




