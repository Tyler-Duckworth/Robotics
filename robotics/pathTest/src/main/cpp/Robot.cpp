/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <iostream>
#include <frc/Joystick.h>
#include <frc/PWMVictorSPX.h>
#include <frc/TimedRobot.h>
#include <frc/WPILib.h>
#include "AHRS.h"
#include "ctre/Phoenix.h"
#include <frc/drive/DifferentialDrive.h>
#include <frc/SpeedControllerGroup.h>
#include <pathfinder-frc.h>
#include <pathfinder.h>
#include <stdio.h>

/**
 * This is a demo program showing the use of the DifferentialDrive class.
 * Runs the motors with arcade steering.
 */
class Robot : public frc::TimedRobot {
private:
  // TalonSRX m_lfront{1};
  // TalonSRX m_lrear{2};
  // TalonSRX m_rfront{3};
  // TalonSRX m_rrear{4};
  // frc::SpeedControllerGroup l_motor{m_lfront, m_lrear};
  // frc::SpeedControllerGroup r_motor{m_rfront, m_rrear};
  WPI_TalonSRX f_leftMotor{1};
	WPI_TalonSRX b_leftMotor{2};
	WPI_TalonSRX f_rightMotor{3};
	WPI_TalonSRX b_rightMotor{4};

	frc::SpeedControllerGroup l_motor{f_leftMotor, b_leftMotor};
	frc::SpeedControllerGroup r_motor{f_rightMotor, b_rightMotor};

  Segment l_trajectory[1024];
  Segment r_trajectory[1024];

  AHRS *navx = new AHRS(SPI::Port::kMXP);

  EncoderFollower l_follower;
  EncoderFollower r_follower;

  EncoderConfig l_config = {0, 1000, 0.67, 1.0, 0.0, 0.0, 0.0, 1.0 / 5.0};
  EncoderConfig r_config = {0, 1000, 0.67, 1.0, 0.0, 0.0, 0.0, 1.0 / 5.0};
  FILE *l_fp;
  FILE *r_fp;
  int l_length;
  int r_length;

  
  frc::DifferentialDrive m_robotDrive{l_motor, r_motor};
  frc::Joystick m_stick{0};
  

 public:
  void RobotInit() {
    // m_lfront.Set(ControlMode::Current, 0);
    // m_lrear.Set(ControlMode::Current, 0);
    // m_rfront.Set(ControlMode::Current, 0);
    // m_rrear.Set(ControlMode::Current, 0);
    f_leftMotor.Set(0);
    b_leftMotor.Set(0);
    f_rightMotor.Set(0);
    b_rightMotor.Set(0);

    l_follower.last_error = 0; 
    l_follower.segment = 0; 
    l_follower.finished = 0;
    
    r_follower.last_error = 0; 
    r_follower.segment = 0; 
    r_follower.finished = 0;

  }
  void TeleopPeriodic() {
    // Drive with arcade style
    m_robotDrive.TankDrive(m_stick.GetY(), m_stick.GetX());
  }
  void AutonomousInit() {
    l_fp = fopen("/home/lvuser/deploy/paths/test.right.pf1.csv", "r");
    l_length = pathfinder_deserialize_csv(l_fp, l_trajectory);
    fclose(l_fp);

    r_fp = fopen("/home/lvuser/deploy/paths/test.left.pf1.csv", "r");
    r_length = pathfinder_deserialize_csv(l_fp, r_trajectory);
    fclose(r_fp);

  }
  void AutonomousPeriodic() {
    double l = pathfinder_follow_encoder(l_config, &l_follower, 
                                         l_trajectory, l_length, 
                                         f_leftMotor.GetSelectedSensorPosition());
                                         //m_lfront.GetSelectedSensorPosition());
    double r = pathfinder_follow_encoder(r_config, &r_follower, 
                                         r_trajectory, r_length, 
                                         f_rightMotor.GetSelectedSensorPosition());
                                         //m_rfront.GetSelectedSensorPosition());
    double current_heading = navx->GetAngle();
    double desired_heading = r2d(l_follower.heading);

    double angle_diff = current_heading - desired_heading;

    angle_diff = std::fmod(angle_diff, 360);

    if(std::abs(angle_diff)) {
      angle_diff = (angle_diff > 0) ? (angle_diff - 360) : (angle_diff + 360);
    }

    double turn = 0.8 * (-1.0/80.0) * angle_diff;

    m_robotDrive.TankDrive((l + turn), (r - turn), true);
  }
};

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
