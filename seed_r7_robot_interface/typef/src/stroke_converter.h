#ifndef _TYPE_F_STROKE_CONVERTER_H_
#define _TYPE_F_STROKE_CONVERTER_H_

#include <seed_r7_ros_controller/stroke_converter_base.h>


namespace seed_converter
{

class TypeF : public StrokeConverter
{
public:
  void makeTables();
  void Angle2Stroke(std::vector<int16_t>& _strokes, const std::vector<double>& _angles);
  void Stroke2Angle(std::vector<double>& _angles, const std::vector<int16_t>& _strokes);

private:
  ConvertTable shoulder_p,shoulder_r,elbow_p,wrist_p,wrist_r,neck_p,neck_r,waist_p,waist_r,leg;
};

}

#endif
