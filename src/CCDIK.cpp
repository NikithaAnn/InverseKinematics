#include <iostream>
#include <string>
#include "Def.h"
#include "FileList.h"
#include "CCDIK.h"
//----------------------------------------------------------------------------------------------------------------------
CCDIK::CCDIK()
{
  m_numberofiterations= 0;
  m_target.m_z = 0;
}
//----------------------------------------------------------------------------------------------------------------------
CCDIK:: ~CCDIK()
{
  if (m_numberofiterations!=0)
    Reset();

}
//----------------------------------------------------------------------------------------------------------------------
void CCDIK::Reset()
{
  m_numberofiterations = 0;
}
//----------------------------------------------------------------------------------------------------------------------
/// @brief a similar code
/// Modified from :-
/// Ryan Juckett (February 11, 2011). Cyclic Coordinate Descent [online].
/// [Accessed 2015]. Available from: <http://www.ryanjuckett.com/programming/cyclic-coordinate-descent-in-2d/>
///
/// /// @brief a similar code 2
/// Modified from :-
/// Auralius (December 2015). CCD Algorithm for Solving Inverse Kinematics Problem [online].
/// [Accessed 2015]. Available from: <https://sites.google.com/site/auraliusproject/ccd-algorithm>
///
/// @brief a similar code 3
/// Modified from :-
/// Auralius (December 2015). CCD Algorithm for Solving Inverse Kinematics Problem [online].
/// [Accessed 2015]. Available from: <https://sites.google.com/site/auraliusproject/ccd-algorithm>
/// /// @brief This is the algorithm to calculate the CCD
/// @parm [in] _spinXFace spin X cooordinates; related to view in NGLScene
/// @parm [in] _spinYFace spin X cooordinates; related to view in NGLScene
/// @parm [in] _mouseGlobalTX mouse changing cooordinates; related to view in NGLScene
/// @parm [in] _modelPos matrix for model in view in NGLScene
/// @parm [in] _cam Camera
void CCDIK::AlgorithmCCDIK(int _spinXFace,int _spinYFace, ngl::Mat4 _mouseGlobalTX,ngl ::Vec3 _modelPos, ngl::Camera _cam)
{
  m_target = ngl::Vec3 (1,2,0);
  int last,current;
  float _rotAngle,end_distance, end_to_current_distance, _x, _y;
  ngl::Vec3 end_distance_vector,end_effector, curr_node, curr_to_target, curr_to_end ;
  bool counter;

  ngl::Vec3  _nextvector, _distancevector, old;

  //end_vector_position has the vector position of end_effector
  end_effector=m_mainstructure.return_End();

  //end_distance_vector has vector value of end_vector_position - target
  end_distance_vector=m_target -(end_effector);

  //end_distance is the distance value
  end_distance = end_distance_vector.length();


  std::cout<<"---------------------------MAIN ALGORITHM------------------------"<<std::endl<<std::endl;

  //The last variable holds the end of the whole m_jointDAta type std::vector;
  last= m_mainstructure.m_jointData.size()-1;

  while (m_numberofiterations<MAXCOUNT && end_distance > Epsilon)
  {
    m_numberofiterations++;
    std::cout<<"--------------------------ITERATIONS------------------------"<<std::endl<<std::endl;
    std::cout<<m_numberofiterations<<std::endl;

    std::cout<<"*****End distance: "<<end_distance<<std::endl;


    for(current = last-1 ; current >= 0; current--)
    {


      //end_vector_position has the vector position of end_effector
      end_effector=m_mainstructure.return_End();

      //end_distance_vector has vector value of end_vector_position - target
      end_distance_vector=m_target -(end_effector);

      //end_distance is the distance value
      end_distance = end_distance_vector.length();

      std::cout<<"End position for checking if condition \n";
      std::cout<<"End Position:"<<end_effector.m_x<<" "<<end_effector.m_y<<" "<<end_effector.m_z<<std::endl;




      std::cout<<"-------------------------MOVE ONLY CURRENT JOINT------------------------"<<std::endl<<std::endl;

      curr_node =m_mainstructure.m_jointData[current].m_worldposition;

      std::cout<<"End Position:"<<end_effector.m_x<<" "<<end_effector.m_y<<" "<<end_effector.m_z<<std::endl;
      std::cout<<"Current Node:"<<curr_node.m_x<<" "<<curr_node.m_y<<" "<<curr_node.m_z<<std::endl;


      if(end_distance > Epsilon)
      {
        curr_to_target= m_target -(curr_node);
        curr_to_end = end_effector - (curr_node);
        curr_to_end.normalize();
        curr_to_target.normalize();

        end_to_current_distance = curr_to_end.length();

        counter = isLeft(curr_node, end_effector, m_target);

        _rotAngle = CalculateAnglebetweenLines(curr_to_end, curr_to_target);

        std::cout<<"v1: "<<curr_to_end.m_x<<" "<<curr_to_end.m_y<<" "<<curr_to_end.m_z<<std::endl;
        std::cout<<"v2: "<<curr_to_target.m_x<<" "<<curr_to_target.m_y<<" "<<curr_to_target.m_z<<std::endl;

        //std::cout<<"Distance between current and End :"<<end_to_current_distance<<std::endl;

        std::cout<<"Angle: "<<_rotAngle *180 /3.14<<std::endl;

        //---------------------------------------------------------------------------------------------------------------

        old = m_mainstructure.m_jointData[current].m_worldposition;

        for(int i=current;i<last; i++)
        {
          std::cout<<"---------------------------INNER LOOP-----------------------"<<std::endl<<std::endl;


          curr_node = old;
          std::cout<<"CURRENTNODE IN I LOOP: "<<curr_node.m_x<<" "<<curr_node.m_y<<std::endl;
          _nextvector = m_mainstructure.m_jointData[i+1].m_worldposition;
          std::cout<<"NEXTLOOP IN I LOOP: "<<_nextvector.m_x<<" "<<_nextvector.m_y<<std::endl;
          _distancevector = _nextvector - curr_node;

          if(counter)
          {
              _x=cos(_rotAngle)*_distancevector.m_x - sin(_rotAngle)*_distancevector.m_y ;
              _y=sin(_rotAngle)*_distancevector.m_x +cos(_rotAngle)*_distancevector.m_y ;
           }
           else
           {
              _x=cos(_rotAngle)*_distancevector.m_x + sin(_rotAngle)*_distancevector.m_y ;
              _y=-sin(_rotAngle)*_distancevector.m_x +cos(_rotAngle)*_distancevector.m_y ;
           }

           old = m_mainstructure.m_jointData[i+1].m_worldposition;

           std::cout<<"curr:"<<m_mainstructure.m_jointData[i].m_worldposition.m_x<<" "<<m_mainstructure.m_jointData[i].m_worldposition.m_y<<std::endl;


           m_mainstructure.m_jointData[i+1].m_worldposition.m_x =m_mainstructure.m_jointData[i].m_worldposition.m_x +  _x;
           m_mainstructure.m_jointData[i+1].m_worldposition.m_y =m_mainstructure.m_jointData[i].m_worldposition.m_y + _y;


           std::cout<<"resultvec:"<<_x<<" "<<_y<<std::endl;
           std::cout<<"resultpos:"<<m_mainstructure.m_jointData[i+1].m_worldposition.m_x<<" "<<m_mainstructure.m_jointData[i+1].m_worldposition.m_y<<std::endl;



           std::cout<<"List of Values :"<<std::endl;


          }
        }
    }



//        //end_vector_position has the vector position of end_effector
//        end_effector= m_mainstructure.m_jointData[last].m_worldposition;

//        //end_distance_vector has vector value of end_vector_position - target
//        end_distance_vector=m_target -(end_effector);

//        //end_distance is the distance value
//        end_distance = end_distance_vector.length();

//        std::cout<<"\nEnd Position:"<<end_effector.m_x<<" "<<end_effector.m_y<<" "<<end_effector.m_z<<std::endl;

      m_mainstructure.PrintPosition();


      }


  if(end_distance <= Epsilon)
  {
      std::cout<<"--------------------TARGET REACHED SUCCESSFULLY-----------------"<<std::endl;
      return ;

  }
  if(m_numberofiterations > MAXCOUNT)
  { std::cout<<"--------------------!!!TARGET NOT REACHED!!!-----------------"<<std::endl;
    return ;
  }
}







///end of citation
//----------------------------------------------------------------------------------------------------------------------
//Preparing Vertex Array Object for links
void CCDIK::CCDIKBuildVAO()
{
  m_mainstructure.buildVAO();
}
//----------------------------------------------------------------------------------------------------------------------
//Drawing the links
void CCDIK::CCDIK_drawFile(int _spinXFace,int _spinYFace, ngl::Mat4 _mouseGlobalTX,ngl ::Vec3 _modelPos, ngl::Camera _cam)
{

  m_mainstructure.drawFile( _spinXFace, _spinYFace,  _mouseGlobalTX,_modelPos,  _cam);
}
//----------------------------------------------------------------------------------------------------------------------
