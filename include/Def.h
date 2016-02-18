#ifndef DEF_H__
#define DEF_H__
#define Epsilon 0.001

#include <iostream>
#include <string>
#include <ngl/Vec3.h>
#include <math.h>

/// \author NIKITHA ANN GEORGE
/// \version 3.0
/// \date 29th January 2015 \n



/// \brief A header file that does some Math functions


extern inline std::string trim(std::string & str)
{
   size_t first = str.find_first_not_of(' ');
   size_t last = str.find_last_not_of(' ');
   return str.substr(first, (last-first+1));
}
extern inline float CalculateAngle(ngl::Vec3 _A, ngl::Vec3 _B)
{
  ngl::Vec3 _result;
  float result;

  _result = _B.operator -(_A);

  if(_result.m_x!=0)
    result = atan(_result.m_y/abs(_result.m_x));
  else
    result = 1.5708;


  return result;

}
extern inline float Simplyfy(float _x)
{
  if(abs(_x)< Epsilon)
    return 0;
}

extern inline float CalculateAnglebetweenLines(ngl::Vec3 _a, ngl::Vec3 _b)
{
  float temp;
  _a.normalize();
  _b.normalize();

  float cosAngle = _a.dot(_b);

  std::cout<<"COS VALUE TO CHECK NEGATIVE"<<cosAngle<<std::endl;
  temp = acos(cosAngle);

  return temp;


}
extern inline float CalculateDistance(ngl::Vec3 _A, ngl::Vec3 _B)
{

 ngl::Vec3 _result;

  _result = _B.operator -(_A);
  float _resultfloat =_result.length();

  return _resultfloat;

}
/// @brief a code to check if the vector is rotated clockwise or counter clockwise
/// Modified from :-
/// Ryan Juckett (February 11, 2011). Cyclic Coordinate Descent [online].
/// [Accessed 2015]. Available from: <http://gamedev.stackexchange.com/questions/22133/how-to-detect-if-object-is-moving-in-clockwise-or-counterclockwise-direction>
extern inline bool isLeft(ngl::Vec3 a, ngl::Vec3 b, ngl::Vec3  c)
{
     return ((b.m_x - a.m_x)*(c.m_y - a.m_y) - (b.m_y - a.m_y)*(c.m_x - a.m_x)) > 0;
}

#endif
