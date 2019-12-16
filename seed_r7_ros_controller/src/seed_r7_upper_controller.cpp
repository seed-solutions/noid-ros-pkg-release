#include "seed_r7_ros_controller/seed_r7_upper_controller.h"


robot_hardware::UpperController::UpperController(const std::string& _port)
{
  ros::param::get("/joint_settings/upper/joints", name_);
  ros::param::get("/joint_settings/upper/aero_index", aero_index_);
  int raw_data_size, body_data_size, base_data_size;
  ros::param::get("/joint_settings/raw_data_size", raw_data_size);
  ros::param::get("/joint_settings/body_data_size", body_data_size);

  upper_ = new aero::controller::AeroCommand();
  if(upper_->openPort(_port,BAUDRATE)){
    ROS_INFO("%s is connected", _port.c_str());
    upper_->flushPort();
    is_open_ = true;
  }
  else{
    ROS_ERROR("%s is not connected", _port.c_str());
    is_open_ = false;
  }

  raw_data_.resize(raw_data_size);
  fill(raw_data_.begin(),raw_data_.end(),0);

  //make table for remap aero <-> ros
  aero_table_.resize(body_data_size);
  for(size_t i = 0; i < aero_table_.size() ; ++i){
    size_t index = std::distance(aero_index_.begin(), std::find(aero_index_.begin(),aero_index_.end(),i));
    if(index != aero_index_.size()) aero_table_.at(i) = std::make_pair(index,name_.at(index));
  }
  
}

robot_hardware::UpperController::~UpperController()
{
  if(is_open_) upper_->closePort();
}

void robot_hardware::UpperController::getPosition()
{
  if(is_open_) raw_data_ = upper_->getPosition(0);
}

void robot_hardware::UpperController::sendPosition(uint16_t _time, std::vector<int16_t>& _data)
{
  if(is_open_) raw_data_ = upper_->actuateByPosition(_time, _data.data());
  else raw_data_.assign(_data.begin(), _data.end());
}

void robot_hardware::UpperController::remapAeroToRos
(std::vector<int16_t>& _ros, std::vector<int16_t>& _aero)
{
  _ros.resize(name_.size());
  for(size_t i = 0; i < _ros.size(); ++i){
    if(aero_index_.at(i) != -1) _ros.at(i) = _aero.at(aero_index_.at(i));
  }
}


void robot_hardware::UpperController::remapRosToAero
(std::vector<int16_t>& _aero, std::vector<int16_t>& _ros)
{
  _aero.resize(aero_table_.size());
  for(size_t i = 0; i < _ros.size(); ++i){
    _aero.at(i) = _ros.at(aero_table_.at(i).first);
  }
}

void robot_hardware::UpperController::setCurrent(uint8_t _number, uint8_t _max, uint8_t _down) 
{
    if(is_open_)upper_->setCurrent(_number, _max, _down);
}

void robot_hardware::UpperController::runScript(uint8_t _number, uint16_t _script) 
{
    if(is_open_)upper_->runScript(_number, _script);
}

std::string robot_hardware::UpperController::getFirmwareVersion()
{
  if(is_open_) return upper_->getVersion(0);
  else return "";
}
