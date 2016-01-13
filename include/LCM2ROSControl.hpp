#ifndef LCM2ROSCONTROL_HPP
#define LCM2ROSCONTROL_HPP

#include <hardware_interface/joint_command_interface.h>
#include <hardware_interface/imu_sensor_interface.h>
#include <hardware_interface/force_torque_sensor_interface.h>
#include <controller_interface/controller.h>
#include <pluginlib/class_list_macros.h>
#include <ros/node_handle.h>

#include <lcm/lcm-cpp.hpp>
#include "lcmtypes/pronto/joint_state_t.hpp"
#include "lcmtypes/pronto/joint_angles_t.hpp"
#include "lcmtypes/pronto/six_axis_force_torque_array_t.hpp"
#include "lcmtypes/pronto/force_torque_t.hpp"

namespace valkyrie_translator
{
   class LCM2ROSControl : public controller_interface::Controller<hardware_interface::EffortJointInterface>
   {
   public:
        LCM2ROSControl();
        virtual ~LCM2ROSControl();

        void starting(const ros::Time& time);
        void update(const ros::Time& time, const ros::Duration& period);
        void stopping(const ros::Time& time);

        void jointCommandHandler(const lcm::ReceiveBuffer* rbuf, const std::string &channel,
                           const pronto::joint_angles_t* msg);

   protected:        
        virtual bool initRequest(hardware_interface::RobotHW* robot_hw, 
                         ros::NodeHandle& root_nh, ros::NodeHandle& controller_nh, 
                         std::set<std::string>& claimed_resources) override;

   private:
        boost::shared_ptr<lcm::LCM> lcm_;

        std::map<std::string, hardware_interface::JointHandle> effortJointHandles;
        std::map<std::string, hardware_interface::ImuSensorHandle> imuSensorHandles;
        std::map<std::string, hardware_interface::ForceTorqueSensorHandle> forceTorqueHandles;
        std::map<std::string, double> buffer_command_efforts;
        std::map<std::string, double> buffer_current_positions;
        std::map<std::string, double> buffer_current_velocities;
        std::map<std::string, double> buffer_current_efforts;
   };
}
#endif