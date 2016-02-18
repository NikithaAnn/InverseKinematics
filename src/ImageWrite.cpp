#include "ImageWrite.h"
//#include "Def.h"
#include "ngl/Vec3.h"
#include <cassert>
#include <fstream>
#include <iostream>

ImageWrite::ImageWrite(int _w,int _h)
{
  m_width=_w;
  m_height=_h;
  m_data= new unsigned char [m_width*m_height*3];
}

ImageWrite::~ImageWrite()
{
  if(m_data !=0)
 {
    delete [] m_data;
  }
}

void ImageWrite::save(const std::string &_fname)												//Saving to file named _fname
{
      std::ofstream output;
      output.open(_fname);
      if(!output.is_open())
      {
        std::cerr<<"Problem Opening File"<<_fname<<"\n";
        exit(EXIT_FAILURE);
      }
        output<<"P3\n";
        output<<m_width<<" "<<m_height<<"\n";
        output<<"255\n";

        for(int y=0;y<m_height;y++)
        {
          for(int x=0;x<m_width;++x)
           {
            output<<(int)m_data[(y*m_width*3)+x*3]<<" ";
            output<<(int)m_data[(y*m_width*3)+x*3+1]<<" ";
            output<<(int)m_data[(y*m_width*3)+x*3+2]<<"   ";
            }

          output<<"\n";
        }

        output.close();
}

void ImageWrite::setPixel(int _x, int _y, unsigned char _r, unsigned char _g, unsigned char _b) //Illuminating pixel at (x,y)
{
      assert(_x<m_width && _y<m_height);
      unsigned int index=(_y*m_width*3)+(_x*3);
      m_data[index]=_r;
      m_data[index+1]=_g;
      m_data[index+2]=_b;
}
void ImageWrite::clear(unsigned char _r,unsigned char _g,unsigned char _b)						//Setting up a background color
{
      for(int y=0;y<m_height;++y)
      {
        for(int x=0;x<m_width;++x)
        {
          setPixel(x,y,_r,_g,_b);
        }
      }
}
void ImageWrite::swapvertex(ngl::Vec3 &_a,ngl::Vec3 &_b)												//End is Start
{
      ngl::Vec3 temp;
      temp.m_x=_a.m_x;
      _a.m_x=_b.m_x;
      _b.m_x=temp.m_x;

      temp.m_y=_a.m_y;
      _a.m_y=_b.m_y;
      _b.m_y=temp.m_y;

}
void ImageWrite::swapvalues(ngl::Vec3 &_a)
{
  int temp;
  temp=_a.m_x;
  _a.m_x=_a.m_y;
  _a.m_y=temp;
}
void ImageWrite::plotLine(ngl::Vec3 start,ngl::Vec3 end)
{
      int _dx,_dy,y,steep=0,yinc=-1,x=0,D;

      _dx=end.m_x-start.m_x;
      _dy=end.m_y-start.m_y;

      if(abs(_dy)>abs(_dx))  																	//The line has greater height than width:steep
      {
        swapvalues(start);
        swapvalues(end);
        steep=1;
      }
      if(start.m_x>end.m_x)																			//_dx will be negative value
       swapvertex(start,end);
        
     if(start.m_y<end.m_y)																			//We have set yinc to be an incrementing flag that increments if y2>y1 and decrements elsewise
        yinc=1;

      _dx=end.m_x-start.m_x;
      _dy=abs(end.m_y-start.m_y);

      D=2*_dy-_dx;																				//D is function for plotting next point

      y=start.m_y;
      for(x=start.m_x;x<end.m_x;x++)
      {
        D=D+2*_dy;
        if(steep)																				//If the line is steep,we setPixel(y,m_x) as we go in opposite direction
          setPixel(y,x,255,0,127);
        else
          setPixel(x,y,255,255,0);

        if(D>0)
         {
            y+=yinc;
            D=D-(2*_dx);
        }

      }

}




	
	


