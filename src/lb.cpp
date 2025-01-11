#include "EZ-Template/util.hpp"
#include "main.h"
#include "pros/misc.h"
#include "subsystems.hpp"

void lb_init() {
  //lb_angle.reset_position();
  lb.tare_position();
  lb.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  lbPID.exit_condition_set(80, 50, 300, 150, 500, 500);
}

void lift_task() {
  pros::delay(2000);  // Set EZ-Template calibrate before this function starts running
  while (true){
    set_lb(lbPID.compute(lb.get_position()));
    pros::delay(ez::util::DELAY_TIME);
  }
}
pros::Task Lift_Task(lift_task);  // Create the task, this will cause the function to start running

bool x_button_pressed = false;

void lb_opcontrol() {
  static int count = 0;  // Use static to retain the value across function calls

  // Increment count on A button press
  if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
    if (count < 4) {
      count += 1;
    }
  }
  // Decrement count on B button press
  else if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
    if (count > 0) {
      count -= 1;
    }
  }

  // Set lbPID target based on count
  if (!x_button_pressed) {
    switch (count) {
      // start
      case 0:
        lbPID.target_set(0);
        break;
      // low
      case 1:
        lbPID.target_set(600);
        break;
      // load
      case 2:
        lbPID.target_set(800);
        break;
      // high
      case 3:
        lbPID.target_set(1900);
        pros::delay(200);
        i_piston.set(true);
        break;
      // score
      case 4:
        lbPID.target_set(2400);
        break;
    }
  }

  // Additional control for X button press
if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
  x_button_pressed = true;
  lbPID.target_set(900); // Set lady brown to position 900
  set_intake(127);
  pros::delay(500); // Run intake for 0.5 seconds
  set_intake(0);
  lbPID.target_set(969); // Set lady brown to position 969

  // Quick switch to 1000 and back to 969 once
  //lbPID.target_set(1000);
  //pros::delay(100); // Adjust delay as necessary
  //lbPID.target_set(969);
  //pros::delay(100); // Adjust delay as necessary

  for (int i = 0; i < 5; ++i) {
    set_intake(127);
    pros::delay(125); // Run intake for 0.125 seconds
    set_intake(0);
    pros::delay(125); // Wait for 0.125 seconds
  }
  x_button_pressed = false;
}
}