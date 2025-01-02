#pragma once

#include "EZ-Template/api.hpp"
#include "EZ-Template/piston.hpp"
#include "api.h"
#include "pros/motors.h"
#include "pros/rotation.hpp"

// Your motors, sensors, etc. should go here.  Below are examples


//clamp
inline ez::Piston clamp_pistons('A');
void set_clamp(bool input);
void clamp_opcontrol();

//intake
inline pros::Motor intake(19);  // Make this number negative if you want to reverse the motor
void set_intake(int input);
void intake_opcontrol();

//lb
inline pros::Motor lb(4);
inline pros::Rotation lb_angle(5);

void lb_opcontrol();
void lb_init();
inline void set_lb(int input) {
  lb.move(input);
}


inline ez::PID lbPID{1.8, 0, 17, 0, "LadyBrown"};

inline void lb_wait() {
  while (lbPID.exit_condition({lb}, true) == ez::RUNNING) {
    pros::delay(ez::util::DELAY_TIME);
  }
}

/*
void nextState(); 
void liftInitialize();
void liftControl();
void liftOpControl();
*/