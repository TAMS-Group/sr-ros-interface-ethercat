/**
 * @file   sr_controller.cpp
 * @author Ugo Cupcic <ugo@shadowrobot.com>
 * @date   Thu Aug 25 12:29:38 2011
 *
* Copyright 2011 Shadow Robot Company Ltd.
*
* This program is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the Free
* Software Foundation, either version 2 of the License, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*
 * @brief   A generic controller for the Shadow Robot EtherCAT hand's joints.
 *
 */

#include "sr_edc_mechanism_controllers/sr_controller.hpp"
#include "angles/angles.h"
#include "pluginlib/class_list_macros.h"
#include <sstream>
#include <math.h>
#include "sr_utilities/sr_math_utils.hpp"

#include <std_msgs/Float64.h>

using namespace std;

namespace controller {

  SrController::SrController()
    : joint_state_(NULL), command_(0),
      loop_count_(0),  initialized_(false), robot_(NULL), last_time_(0),
      n_tilde_("~"),
      max_force_demand(1023.), friction_deadband(5)
  {
  }

  SrController::~SrController()
  {
    sub_command_.shutdown();
  }

  void SrController::after_init()
  {
    sub_command_ = node_.subscribe<std_msgs::Float64>("command", 1, &SrController::setCommandCB, this);
  }


  std::string SrController::getJointName()
  {
    return joint_state_->joint_->name;
  }

// Set the joint position command
  void SrController::setCommand(double cmd)
  {
    command_ = cmd;
  }

// Return the current position command
  void SrController::getCommand(double & cmd)
  {
    cmd = command_;
  }

  void SrController::setCommandCB(const std_msgs::Float64ConstPtr& msg)
  {
    command_ = msg->data;
  }

  bool SrController::init(pr2_mechanism_model::RobotState *robot, ros::NodeHandle &n)
  {
    return true;
  }

  void SrController::update()
  {
  }

  void SrController::starting()
  {}

  void SrController::getGains(double &p, double &i, double &d, double &i_max, double &i_min)
  {}
}

/* For the emacs weenies in the crowd.
Local Variables:
   c-basic-offset: 2
End:
*/

