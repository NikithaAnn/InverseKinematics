#include <QMouseEvent>
#include <QGuiApplication>
#include "NGLScene.h"
#include <ngl/Camera.h>
#include <ngl/Light.h>
#include <ngl/Transformation.h>
#include <ngl/Material.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <ngl/NGLStream.h>
#include <ngl/Util.h>
#include <QColorDialog>
#include "NGLScene.h"

//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for x/y translation with mouse movement
//----------------------------------------------------------------------------------------------------------------------
const static float INCREMENT=0.01;
//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for the wheel zoom
//----------------------------------------------------------------------------------------------------------------------
const static float ZOOM=0.1;

NGLScene::NGLScene( QWidget *_parent ) : QOpenGLWidget( _parent )
{
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  m_rotate=false;
  // mouse rotation values set to 0
  m_spinXFace=0;
  m_spinYFace=0;
  m_wireframe = false;

}
NGLScene::~NGLScene()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
}

void NGLScene::resizeGL(QResizeEvent *_event)
{
  m_width=_event->size().width()*devicePixelRatio();
  m_height=_event->size().height()*devicePixelRatio();

  m_projection=ngl::perspective(45,float(width()/height()),0.5f,50.0f);
}

void NGLScene::resizeGL(int _w , int _h)
{
  m_width=_w*devicePixelRatio();
  m_height=_h*devicePixelRatio();
  m_projection=ngl::perspective(45,float(width()/height()),0.5f,50.0f);
}


void NGLScene::initializeGL()
{
  // we need to initialise the NGL lib which will load all of the OpenGL functions, this must
  // be done once we have a valid GL context but before we call any GL commands. If we dont do
  // this everything will crash
  ngl::NGLInit::instance();

  // Blue Background
  glClearColor(0.184314f, 0.309804f, 0.309804f, 1.0f);

  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
   // now to load the shader and set the values

  ngl::Vec3 from(0,0,4);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);

  m_cam.set(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam.setShape(45,(float)720.0/576.0,0.001,150);



  m_text.reset(new  ngl::Text(QFont("Times",15)));
  m_text->setScreenSize(width(),height());

  m_view=ngl::lookAt(from,to,up);

  m_projection=ngl::perspective(45,float(width()/height()),0.5f,50.0f);

  m_Test.m_mainstructure.buildVAO();
}

//We load the Matrices to shader to get the required output for drawing on screen
void NGLScene::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  M=m_transform.getMatrix();
  MV=  M*m_view;
  MVP= MV*m_projection;
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setShaderParamFromMat4("MVP",MVP);
  shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
 }
//This is the function that actually draws on screen
void NGLScene::paintGL()
{

  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_width,m_height);

  // grab an instance of the shader manager
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)["Phong"]->use();


  //Text stating what to do
  m_text->setColour(ngl::Colour(0.780392, 0.0823529, 0.521569));
  QString text1=QString("Please left click for target position..");
  m_text->renderText(10,18,text1 );


  //Text stating whERE TARGET IS
  m_text->setColour(ngl::Colour(0.878431 ,1 ,1));
  QString text=QString("Target at position X: %1 Y:%2").arg(m_Test.m_target.m_x).arg(m_Test.m_target.m_y);
  m_text->renderText(10,40,text );

  loadMatricesToShader();
  m_Test.m_mainstructure.drawFile(m_spinXFace,m_spinYFace, m_mouseGlobalTX,m_modelPos, m_cam);

  setTarget();

  if(m_wireframe==true)
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  else
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

}
void NGLScene::setTarget()
{
  //This functions draws a cube at the target position
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["nglDiffuseShader"]->use();

  // Rotation based on the mouse position for our global transform
  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  // create the rotation matrices
  rotX.rotateX(m_spinXFace);
  rotY.rotateY(m_spinYFace);
  // multiply the rotations
  m_mouseGlobalTX=rotY*rotX;
  // add the translations
  m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
  m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
  m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;

  ngl::Mat4 MVP;

   // get the VBO instance and draw the sphere
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();

  m_transform.setPosition(m_Test.m_target);

  //m_mouseGlobalTX.m_m[3][1] = 3.5;
  MVP=m_transform.getMatrix()*m_mouseGlobalTX*m_cam.getVPMatrix();

  loadMatricesToShader();
  shader->setShaderParamFromMat4("MVP",MVP);

  prim->draw("cube");

}
//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseMoveEvent (QMouseEvent * _event)
{
  // note the method buttons() is the button state when event was called
  // this is different from button() which is used to check which button was
  // pressed when the mousePress/Release event is generated
  if(m_rotate && _event->buttons() == Qt::LeftButton)
  {
    int diffx=_event->x()-m_origX;
    int diffy=_event->y()-m_origY;
    m_spinXFace += (float) 0.5f * diffy;
    m_spinYFace += (float) 0.5f * diffx;
    m_origX = _event->x();
    m_origY = _event->y();
    update();

  }
        // right mouse translate code
  else if(m_translate && _event->buttons() == Qt::RightButton)
  {
    int diffX = (int)(_event->x() - m_origXPos);
    int diffY = (int)(_event->y() - m_origYPos);
    m_origXPos=_event->x();
    m_origYPos=_event->y();
    m_modelPos.m_x += INCREMENT * diffX;
    m_modelPos.m_y -= INCREMENT * diffY;
    update();

   }
}


//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mousePressEvent ( QMouseEvent * _event)
{
  // this method is called when the mouse button is pressed in this case we
  // store the value where the maouse was clicked (x,y) and set the Rotate flag to true

   if(_event->button() == Qt::RightButton)
  {
    m_origXPos = _event->x();
    m_origYPos = _event->y();
    m_translate=true;
  }


}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseReleaseEvent ( QMouseEvent * _event )
{
  // this event is called when the mouse button is released
  // we then set Rotate to false
  if (_event->button() == Qt::LeftButton)
  {
    m_rotate=false;
  }
        // right mouse translate mode
  if (_event->button() == Qt::RightButton)
  {
    m_translate=false;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::wheelEvent(QWheelEvent *_event)
{

  // check the diff of the wheel position (0 means no change)
  if(_event->delta() > 0)
  {
    m_modelPos.m_z+=ZOOM *10;
  }
  else if(_event->delta() <0 )
  {
    m_modelPos.m_z-=ZOOM * 10;
  }
  update();
}
//----------------------------------------------------------------------------------------------------------------------
//tHIS FUUNCTIONALITY IS DISABLED AS THE WIDGETS ARE USED TO DO NEEDED ACTIONS
void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
//  switch (_event->key())
//  {
//  // escape key to quite
//  case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
//  // show full screen
//  case Qt::Key_F : showFullScreen(); break;
//  // show windowed
//  case Qt::Key_N : showNormal(); break;
//    // show windowed
//  //case Qt::Key_Space : m_positions.clear(); break;
//  default : break;
//  }
    update();
}

ngl::Vec3 NGLScene::getWorldSpace(int _x, int _y)
{
  std::cout<<"Mouse pos "<<_x<<" "<<_y<<" ";
  ngl::Mat4 t=m_projection;
  ngl::Mat4 v=m_view;
  // as ngl:: and OpenGL use different formats need to transpose the matrix.
  t.transpose();
  v.transpose();
  ngl::Mat4 inverse=( t*v).inverse();

  ngl::Vec4 tmp(0,0,1.0f,1.0f);
  // convert into NDC
  tmp.m_x=(2.0f * _x) / width() - 1.0f;
  tmp.m_y=1.0f - (2.0f * _y) / height();
  // scale by inverse MV * Project transform
  ngl::Vec4 obj=inverse*tmp;
  // Scale by w
  obj/=obj.m_w;
  return obj.toVec3();
  /* ngl now has this built in as well
  return ngl::unProject(ngl::Vec3(_x,_y,1.0f),
                        m_view,
                        m_projection,
                        ngl::Vec4(0,0,width(),height())
                        );
*/
}
//----------------------------------------------------------------------------------------------------------------------
//Toggle between Wire and Solid frame
void NGLScene::toggleWireframe(bool _mode)
{
  m_wireframe = _mode;
  update();
}
//----------------------------------------------------------------------------------------------------------------------
//Loads the file
void NGLScene::loadFile()
{
  m_Test.m_mainstructure.load("Link.bvh");
    update();

}
void NGLScene::doCCDIK()
{
  m_Test.AlgorithmCCDIK(m_spinXFace,m_spinYFace, m_mouseGlobalTX,m_modelPos, m_cam);
    update();

}
//void NGLScene::setTargetX( float _x	)
//{
//  m_Test.m_target.m_x=_x;
//  update();
//}

//void NGLScene::setTargetY( float _y	)
//{
//  m_Test.m_target.m_y=_y;
//  update();
//}
