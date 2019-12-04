#include <pluginlib/class_list_macros.h>
#include "stroke_converter.h"


void seed_converter::TypeF::makeTables()
{
  if (makeTable(shoulder_p.table, "shoulder_p.csv"))
    makeInvTable(shoulder_p.inv_table, shoulder_p.table);
  if (makeTable(shoulder_r.table, "shoulder_r.csv"))
    makeInvTable(shoulder_r.inv_table, shoulder_r.table);
  if (makeTable(elbow_p.table, "elbow_p.csv")) makeInvTable(elbow_p.inv_table, elbow_p.table);
  if (makeTable(wrist_p.table, "wrist_p.csv")) makeInvTable(wrist_p.inv_table, wrist_p.table);
  if (makeTable(wrist_r.table, "wrist_r.csv")) makeInvTable(wrist_r.inv_table, wrist_r.table);
  if (makeTable(neck_p.table, "neck_p.csv")) makeInvTable(neck_p.inv_table, neck_p.table);
  if (makeTable(neck_r.table, "neck_r.csv")) makeInvTable(neck_r.inv_table, neck_r.table);
  if (makeTable(waist_p.table, "waist_p.csv")) makeInvTable(waist_p.inv_table, waist_p.table);
  if (makeTable(waist_r.table, "waist_r.csv")) makeInvTable(waist_r.inv_table, waist_r.table);
  if (makeTable(leg.table, "leg.csv")) makeInvTable(leg.inv_table, leg.table);
}

void seed_converter::TypeF::Angle2Stroke
(std::vector<int16_t>& _strokes, const std::vector<double>& _angles)
{
  static const float rad2Deg = 180.0 / M_PI;
  static const float scale = 100.0;

  seed_converter::DiffJoint r_wrist
    = setDualAngleToStroke(rad2Deg * _angles[23], -rad2Deg * _angles[22],
                           wrist_r.table, wrist_p.table, true);
  seed_converter::DiffJoint l_wrist
    = setDualAngleToStroke(-rad2Deg * _angles[9], rad2Deg * _angles[8],
                           wrist_r.table, wrist_p.table, true);
  seed_converter::DiffJoint waist
    = setDualAngleToStroke(-rad2Deg * _angles[2], rad2Deg * _angles[1],
                           waist_r.table, waist_p.table, false);
  seed_converter::DiffJoint neck
    = setDualAngleToStroke(rad2Deg * _angles[16], rad2Deg * _angles[15],
                           neck_r.table, neck_p.table, false);

  _strokes[0] = static_cast<int16_t>(scale * rad2Deg * _angles[0]);
  _strokes[1] = static_cast<int16_t>(scale * waist.one);
  _strokes[2] = static_cast<int16_t>(scale * waist.two);

  _strokes[3]
    = static_cast<int16_t>(scale * setAngleToStroke(-rad2Deg * _angles[3], shoulder_p.table));
  _strokes[4]
    = static_cast<int16_t>(scale * setAngleToStroke(rad2Deg * _angles[4], shoulder_r.table));
  _strokes[5] = static_cast<int16_t>(-scale * rad2Deg * _angles[5]);
  _strokes[6]
    = static_cast<int16_t>(scale * setAngleToStroke(180 + rad2Deg * _angles[6], elbow_p.table));
  _strokes[7] = static_cast<int16_t>(-scale * rad2Deg * _angles[7]);
  _strokes[8] = static_cast<int16_t>(scale * l_wrist.one);
  _strokes[9] = static_cast<int16_t>(scale * l_wrist.two);
  _strokes[13] = static_cast<int16_t>(scale * (rad2Deg * _angles[13] + 50.0) * 0.18);

  _strokes[14] = static_cast<int16_t>(scale * rad2Deg * _angles[14]);
  _strokes[15] = static_cast<int16_t>(scale * neck.two);
  _strokes[16] = static_cast<int16_t>(scale * neck.one);

  _strokes[17]
    = static_cast<int16_t>(scale * setAngleToStroke(-rad2Deg * _angles[17], shoulder_p.table));
  _strokes[18]
    = static_cast<int16_t>(scale * setAngleToStroke(-rad2Deg * _angles[18], shoulder_r.table));
  _strokes[19] = static_cast<int16_t>(-scale * rad2Deg * _angles[19]);
  _strokes[20]
    = static_cast<int16_t>(scale * setAngleToStroke(180 + rad2Deg * _angles[20], elbow_p.table));
  _strokes[21] = static_cast<int16_t>(-scale * rad2Deg * _angles[21]);
  _strokes[22] = static_cast<int16_t>(scale * r_wrist.two);
  _strokes[23] = static_cast<int16_t>(scale * r_wrist.one);
  _strokes[27] = static_cast<int16_t>(-scale * (rad2Deg * _angles[27] - 50.0) * 0.18);

  _strokes[28]
    = static_cast<int16_t>(scale * setAngleToStroke(- rad2Deg * _angles[28], leg.table));  //knee
  _strokes[29]
    = static_cast<int16_t>(scale * setAngleToStroke(  rad2Deg * _angles[29], leg.table));  //ankle
}

void seed_converter::TypeF::Stroke2Angle
(std::vector<double>& _angles, const std::vector<int16_t>& _strokes)
{
  static const float deg2Rad = M_PI / 180.0;
  static const float scale_inv = 0.01;

  _angles[0] = deg2Rad * scale_inv * _strokes[0];
  _angles[1]
    = deg2Rad * setStrokeToAngle(scale_inv * (_strokes[2] + _strokes[1]) * 0.5, waist_p.inv_table);
  _angles[2]
    = deg2Rad * setStrokeToAngle(scale_inv * (_strokes[2] - _strokes[1]) * 0.5, waist_r.inv_table);
  _angles[3] = -deg2Rad * setStrokeToAngle(scale_inv * _strokes[3], shoulder_p.inv_table);
  _angles[4] = deg2Rad * setStrokeToAngle(scale_inv * _strokes[4], shoulder_r.inv_table);
  _angles[5] = -deg2Rad * scale_inv * _strokes[5];
  _angles[6] = - M_PI + deg2Rad * setStrokeToAngle(scale_inv * _strokes[6], elbow_p.inv_table);
  _angles[7] = -deg2Rad * scale_inv * _strokes[7];
  _angles[8]
    = -deg2Rad * setStrokeToAngle(scale_inv*(_strokes[9] - _strokes[8])*0.5, wrist_p.inv_table);
  _angles[9]
    = -deg2Rad * setStrokeToAngle(scale_inv*(_strokes[9] + _strokes[8])*0.5, wrist_r.inv_table);
  _angles[10] = -deg2Rad * (scale_inv * _strokes[13] * 5.556 - 50.0);
  _angles[11] = 0;
  _angles[12] = 0;
  _angles[13] = deg2Rad * (scale_inv * _strokes[13] * 5.556 - 50.0);

  _angles[14] = deg2Rad * scale_inv * _strokes[14];
  _angles[15]
    = deg2Rad * setStrokeToAngle(scale_inv*(_strokes[16] + _strokes[15])*0.5, neck_p.inv_table);
  _angles[16]
    = -deg2Rad * setStrokeToAngle(scale_inv*(_strokes[16] - _strokes[15])*0.5, neck_r.inv_table);

  _angles[17] = -deg2Rad * setStrokeToAngle(scale_inv * _strokes[17], shoulder_p.inv_table);
  _angles[18] = -deg2Rad * setStrokeToAngle(scale_inv * _strokes[18], shoulder_r.inv_table);
  _angles[19] = -deg2Rad * scale_inv * _strokes[19];
  _angles[20] = - M_PI + deg2Rad * setStrokeToAngle(scale_inv * _strokes[20], elbow_p.inv_table);
  _angles[21] = -deg2Rad * scale_inv * _strokes[21];
  _angles[22]
    = -deg2Rad * setStrokeToAngle(scale_inv*(_strokes[23] - _strokes[22])*0.5, wrist_p.inv_table);
  _angles[23]
    = deg2Rad * setStrokeToAngle(scale_inv*(_strokes[23] + _strokes[22])*0.5, wrist_r.inv_table);
  _angles[24] = deg2Rad * (scale_inv * _strokes[27] * 5.556 - 50.0);
  _angles[25] = 0;
  _angles[26] = 0;
  _angles[27] = -deg2Rad * (scale_inv * _strokes[27] * 5.556 - 50.0);

  _angles[28] = -deg2Rad * setStrokeToAngle(scale_inv * _strokes[28], leg.inv_table);  // knee
  _angles[29] = deg2Rad * setStrokeToAngle(scale_inv * _strokes[29], leg.inv_table);  // ankle
}

PLUGINLIB_EXPORT_CLASS(seed_converter::TypeF, seed_converter::StrokeConverter)
