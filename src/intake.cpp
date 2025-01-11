#include "EZ-Template/util.hpp"
#include "main.h"
#include "autons.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"
#include "subsystems.hpp"

// intake
void set_intake(int input) {
  intake.move(input);
}

void intake_opcontrol() {
  if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
    set_intake(127);
  } 
  else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
    set_intake(-127);
  } 
  else {
    set_intake(0);
  }
}

void set_ipiston(bool input) {
  i_piston.set(input);
}

void intake_piston_opcontrol() {
  i_piston.button_toggle(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT));
}

