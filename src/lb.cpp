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

void lb_opcontrol() {
  static int count = 0;  // Use static to retain the value across function calls

  // Increment count on A button press
  if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
    if (count < 3) {
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
  switch (count) {
    // start
    case 0:
      lbPID.target_set(0);
      break;
    // load
    case 1:
      lbPID.target_set(45);
      break;
    // high
    case 2:
      lbPID.target_set(120);
      break;
    // score
    case 3:
      lbPID.target_set(165);
      break;
  }

  // Additional control for X button press
  if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
    set_intake(127);
    pros::delay(250);
    set_intake(0);
    pros::delay(125);
    set_intake(127);
    pros::delay(250);
    set_intake(0);
    pros::delay(125);
    set_intake(127);
    pros::delay(250);
    set_intake(0);
    pros::delay(250);
  }
}
