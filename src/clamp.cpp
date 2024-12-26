#include "main.h"
#include "subsystems.hpp"

void set_clamp(bool input) {
  clamp_pistons.set(input);
}

void clamp_opcontrol() {
  clamp_pistons.button_toggle(master.get_digital_new_press(DIGITAL_L1));
  pros::delay(ez::util::DELAY_TIME);
}