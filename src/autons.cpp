#include "autons.hpp"
#include "main.h"
#include "pros/rtos.hpp"
#include "subsystems.hpp"

/////
// For installation, upgrading, documentations, and tutorials, check out our website!
// https://ez-robotics.github.io/EZ-Template/
/////

// These are out of 127
const int DRIVE_SPEED = 110;
const int TURN_SPEED = 90;
const int SWING_SPEED = 90;

///
// Constants
///
void default_constants() {
  // regular pid 
  chassis.pid_heading_constants_set(11, 0, 20);
  chassis.pid_drive_constants_set(40, 0, 150);
  chassis.pid_turn_constants_set(3, 0.05, 20, 15);
  chassis.pid_swing_constants_set(6, 0, 65);
  chassis.pid_odom_angular_constants_set(6.5, 0.0, 52.5);    // Angular control for odom motions
  chassis.pid_odom_boomerang_constants_set(5.8, 0.0, 32.5);  // Angular control for boomerang motions

  


  // exit conditions
  chassis.pid_turn_exit_condition_set(90_ms, 3_deg, 250_ms, 7_deg, 500_ms, 500_ms);
  chassis.pid_swing_exit_condition_set(90_ms, 3_deg, 250_ms, 7_deg, 500_ms, 500_ms);
  chassis.pid_drive_exit_condition_set(90_ms, 1_in, 250_ms, 3_in, 500_ms, 500_ms);
  chassis.pid_odom_turn_exit_condition_set(90_ms, 3_deg, 250_ms, 7_deg, 500_ms, 750_ms);
  chassis.pid_odom_drive_exit_condition_set(90_ms, 1_in, 250_ms, 3_in, 500_ms, 750_ms);
  chassis.pid_turn_chain_constant_set(3_deg);
  chassis.pid_swing_chain_constant_set(5_deg);
  chassis.pid_drive_chain_constant_set(3_in);
  chassis.pid_turn_chain_constant_set(3_deg);
  chassis.pid_swing_chain_constant_set(5_deg);
  chassis.pid_drive_chain_constant_set(3_in);

  // slew
  chassis.slew_turn_constants_set(3_deg, 70);
  chassis.slew_drive_constants_set(3_in, 70);
  chassis.slew_swing_constants_set(3_in, 80);
  //chassis.slew_drive_constants_set(7_in, 60);

  // The amount that turns are prioritized over driving in odom motions
  // - if you have tracking wheels, you can run this higher.  1.0 is the max
  chassis.odom_turn_bias_set(0.9);

  chassis.odom_look_ahead_set(7_in);           // This is how far ahead in the path the robot looks at
  chassis.odom_boomerang_distance_set(16_in);  // This sets the maximum distance away from target that the carrot point can be
  chassis.odom_boomerang_dlead_set(0.625);     // This handles how aggressive the end of boomerang motions are

  chassis.pid_angle_behavior_set(ez::shortest);  // Changes the default behavior for turning, this defaults it to the shortest path there
}

void drive_and_turn() {
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
  chassis.pid_wait();
}

void wait_until_change_speed() {
  // pid_wait_until will wait until the robot gets to a desired position

  // When the robot gets to 6 inches slowly, the robot will travel the remaining distance at full speed
  chassis.pid_drive_set(24_in, 30, true);
  chassis.pid_wait_until(6_in);
  chassis.pid_speed_max_set(DRIVE_SPEED);  // After driving 6 inches at 30 speed, the robot will go the remaining distance at DRIVE_SPEED
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  // When the robot gets to -6 inches slowly, the robot will travel the remaining distance at full speed
  chassis.pid_drive_set(-24_in, 30, true);
  chassis.pid_wait_until(-6_in);
  chassis.pid_speed_max_set(DRIVE_SPEED);  // After driving 6 inches at 30 speed, the robot will go the remaining distance at DRIVE_SPEED
  chassis.pid_wait();
}

void swing_example() {
  // The first parameter is ez::LEFT_SWING or ez::RIGHT_SWING
  // The second parameter is the target in degrees
  // The third parameter is the speed of the moving side of the drive
  // The fourth parameter is the speed of the still side of the drive, this allows for wider arcs

  chassis.pid_swing_set(ez::LEFT_SWING, 45_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::RIGHT_SWING, 0_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::RIGHT_SWING, 45_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::LEFT_SWING, 0_deg, SWING_SPEED, 45);
  chassis.pid_wait();
}

void motion_chaining() {
  // Motion chaining is where motions all try to blend together instead of individual movements.
  // This works by exiting while the robot is still moving a little bit.
  // To use this, replace pid_wait with pid_wait_quick_chain.
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait_quick_chain();

  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait_quick_chain();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  // Your final motion should still be a normal pid_wait
  chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
  chassis.pid_wait();
}

// Auto that tests everything
void combining_movements() {
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::RIGHT_SWING, -45_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
  chassis.pid_wait();
}

// Interference example
void tug(int attempts) {
  for (int i = 0; i < attempts - 1; i++) {
    // Attempt to drive backward
    printf("i - %i", i);
    chassis.pid_drive_set(-12_in, 127);
    chassis.pid_wait();

    // If failsafed...
    if (chassis.interfered) {
      chassis.drive_sensor_reset();
      chassis.pid_drive_set(-2_in, 20);
      pros::delay(1000);
    }
    // If the robot successfully drove back, return
    else {
      return;
    }
  }
}

// If there is no interference, the robot will drive forward and turn 90 degrees.
// If interfered, the robot will drive forward and then attempt to drive backward.
void interfered_example() {
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  if (chassis.interfered) {
    tug(3);
    return;
  }

  chassis.pid_turn_set(90_deg, TURN_SPEED);
  chassis.pid_wait();
}

void blue_solowp(){
  set_ipiston(true);
  pros::delay(75);
  chassis.pid_drive_set(-17_in, 80, true); 
  chassis.pid_wait();
  pros::delay(250);
  chassis.pid_turn_set(-90,127);
  chassis.pid_wait();
  pros::delay(200);
  chassis.pid_drive_set(-2_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  set_intake(127);
  pros::delay(1000);
  set_intake(0);
  pros::delay(200);

  chassis.pid_drive_set(2_in,80, true);
  chassis.pid_wait();

  chassis.pid_turn_set(130,127);
  chassis.pid_wait();

  chassis.pid_drive_set(-32.75_in,75,true);
  chassis.pid_wait();

  pros::delay(100);
  chassis.pid_drive_set(-1.75_in,50,true);
  chassis.pid_wait();

  pros::delay(250);
  clamp_pistons.set(true);

  pros::delay(100);
  chassis.pid_turn_set(0,127,true);
  chassis.pid_wait();

  pros::delay(100);

  set_intake(127);

  chassis.pid_drive_set(23_in,127,true);
  chassis.pid_wait();
  pros::delay(100);
//  chassis.pid_drive_set(2.5_in,80,true);
//  chassis.pid_wait();

//  pros::delay(500);

  chassis.pid_turn_set(-90,127);
  chassis.pid_wait();
  
  pros::delay(100);

  chassis.pid_drive_set(14.5_in,100,true);
  chassis.pid_wait();
  pros::delay(100);
//  chassis.pid_drive_set(1_in,80,true);
//  chassis.pid_wait();
//  pros::delay(500);

  chassis.pid_turn_set(180,127,true);
  chassis.pid_wait();
  pros::delay(500);

//  chassis.pid_drive_set(2_in,80,true);
//  chassis.pid_wait();
//  pros::delay(500);

  chassis.pid_drive_set(22_in,127,true);
  chassis.pid_wait();
  pros::delay(2500);
}

void red_solowp(){
  set_ipiston(true);
  pros::delay(75);
  // this makes chasiss drive a set amount of inches (+/- for direction) and with a speed, the speed is out of of 127
  chassis.pid_drive_set(-17_in, 80, true); 
  // YOU NEED TO ADD THIS AFTER EVER CHASSIS MOVEMENT OR TURN  after anything that is chassis.pid.etc...
  chassis.pid_wait();
  // this is the amount of delay between each movement | 1000 milliseconds = 1 second
  pros::delay(250);
  // turning -90 degrees KEEP IN MIND this turning is from the starting point so it is not relative to where u are but where u started
  chassis.pid_turn_set(90,127);
  // YOU NEED TO ADD THIS AFTER EVER CHASSIS MOVEMENT OR TURN also dont put anything after ur chassis movement UNTIL u have this wait statement
  chassis.pid_wait();
  pros::delay(200);
  chassis.pid_drive_set(-2_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  // this command spins the intake
  set_intake(127);
  // wait a second
  pros::delay(1000);
  // stop spinning the intake --> these three lines have spun the intake for 1 second
  set_intake(0);
  pros::delay(200);

  chassis.pid_drive_set(2_in,80, true);
  chassis.pid_wait();

  chassis.pid_turn_set(-130,127);
  chassis.pid_wait();

  chassis.pid_drive_set(-32.75_in,75,true);
  chassis.pid_wait();

  pros::delay(100);
  chassis.pid_drive_set(-1.75_in,50,true);
  chassis.pid_wait();

  pros::delay(250);
  clamp_pistons.set(true);

  pros::delay(100);
  chassis.pid_turn_set(0,127,true);
  chassis.pid_wait();

  pros::delay(100);

  set_intake(127);

  chassis.pid_drive_set(23_in,127,true);
  chassis.pid_wait();
  pros::delay(100);
//  chassis.pid_drive_set(2.5_in,80,true);
//  chassis.pid_wait();

//  pros::delay(500);

  chassis.pid_turn_set(90,127);
  chassis.pid_wait();
  
  pros::delay(100);

  chassis.pid_drive_set(14.5_in,100,true);
  chassis.pid_wait();
  pros::delay(100);
//  chassis.pid_drive_set(1_in,80,true);
//  chassis.pid_wait();
//  pros::delay(500);

  chassis.pid_turn_set(180,127,true);
  chassis.pid_wait();
  pros::delay(500);

//  chassis.pid_drive_set(2_in,80,true);
//  chassis.pid_wait();
//  pros::delay(500);

  chassis.pid_drive_set(22_in,127,true);
  chassis.pid_wait();
  pros::delay(2500);
}

void goalside_red(){
  set_ipiston(true);
  pros::delay(75);
  chassis.pid_drive_set(-21.00_in,127,true);
  chassis.pid_wait();
  pros::delay(250);
  chassis.pid_drive_set(-2_in,90,true);
  chassis.pid_wait();
  pros::delay(250);
  set_clamp(true);
  pros::delay(100);
  set_intake(127);
  pros::delay(500);
  set_intake(0);
  pros::delay(100);
  chassis.pid_turn_set(-95,127);
  chassis.pid_wait();
  pros::delay(100);
  set_intake(127);
  chassis.pid_drive_set(26_in,90,true);
  chassis.pid_wait();
  pros::delay(500);
  chassis.pid_turn_set(-80,127);
  chassis.pid_wait();
  pros::delay(600);
  chassis.pid_drive_set(-29_in,127,true);
  chassis.pid_wait();
  pros::delay(2000);
}