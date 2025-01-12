#include "main.h"
#include "pros/misc.h"
#include "subsystems.hpp"

void set_clamp(bool input) {
  clamp_pistons.set(input);
}

void clamp_opcontrol() {
  clamp_pistons.button_toggle(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1));
}