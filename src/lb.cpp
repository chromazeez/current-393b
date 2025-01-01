#include "main.h"
#include "pros/misc.h"
#include "subsystems.hpp"

const int numStates = 3;
int states[numStates]= {0,30,250};
int currState = 0;
int target = 0;

void nextState(){
  currState += 1;
  if (currState == numStates){
    currState = 0;
  }
  target = states[currState];
}

void liftControl(){
  double kp = 0.5;
  double error = target - lb_angle.get_position();
  double velocity = error * kp;
  lb.move(velocity);
}

void liftInitialize(){
  pros::Task liftControlTask([]{
    while(true){
      liftControl();
      pros::delay(20);
    }
  });
}

void liftOpControl(){
  if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)){
    nextState();

  pros::delay(20);

  }
}

/*
void lb_init() {
  lb_angle.reset_position();
  lb.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  lbPID.exit_condition_set(80, 50, 300, 150, 500, 500);
}

void lift_task() {
  pros::delay(2000);  // Set EZ-Template calibrate before this function starts running
  while (true){
    set_lb(lbPID.compute(lb_angle.get_position()/100),100);
    pros::delay(ez::util::DELAY_TIME);
  }

}
pros::Task Lift_Task(lift_task);  // Create the task, this will cause the function to start running

/*void autonomous() {
    lbPID.target_set(500);
  // You can do stuff here and it'll happen while the lift moves
    lb_wait();  // Wait for the lift to reach its target

    pros::delay(1000);

    lbPID.target_set(0);
  // You can do stuff here and it'll happen while the lift moves
    lb_wait();  // Wait for the lift to reach its target
}

void lb_opcontrol() {
  // load
  if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
      lbPID.target_set(50); 
  }
  // score
  else if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
      lbPID.target_set(2500);
  }
  /*else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
      lbPID.target_set(0);
  }
  pros::delay(ez::util::DELAY_TIME);
}    
*/