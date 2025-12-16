/******************************************************************************
SparkFun_TB6612.h
TB6612FNG H-Bridge Motor Driver Example code
Michelle @ SparkFun Electronics
8/20/16
https://github.com/sparkfun/SparkFun_TB6612FNG_Arduino_Library

Uses 2 motors to show examples of the functions in the library.  This causes
a robot to do a little 'jig'.  Each movement has an equal and opposite movement
so assuming your motors are balanced the bot should end up at the same place it
started.

Resources:
TB6612 SparkFun Library

Development environment specifics:
Developed on Arduino 1.6.4
Developed with ROB-9457
******************************************************************************/
#pragma once
#include "pico/stdlib.h"
#include <memory>
#include <stdint.h>

using std::unique_ptr;

// used in some functions so you don't have to send a speed
#define DEFAULTSPEED 255

#define HIGH true
#define LOW false

struct MotorGPIO {
  uint connection_1_pin;
  uint connection_2_pin;
  uint PWM_pin;
  uint standby_pin;
};

class Motor {
public:
  // Constructor. Mainly sets up pins.
  Motor(uint connection_1_pin, uint connection_2pin, uint PWM_pin,
        uint standby_pin, int offset);
  Motor(const MotorGPIO &motor_gpio, int offset);
  Motor() : connection_1(0), connection_2(0), PWM(0), offset(0), stand_by(0) {}
  ~Motor() = default;

  Motor(const Motor &) = delete;
  Motor &operator=(const Motor &) = delete;

  Motor(Motor &&other) = default;
  Motor &operator=(Motor &&other) = default;

  // Drive in direction given by sign, at speed given by magnitude of the
  // parameter.
  void drive(int speed);

  // drive(), but with a delay(duration)
  void drive(int speed, uint32_t duration);

  // currently not implemented
  // void stop();           // Stop motors, but allow them to coast to a halt.
  // void coast();          // Stop motors, but allow them to coast to a halt.

  // Stops motor by setting both input pins high
  void brake();

  // set the chip to standby mode.  The drive function takes it out of standby
  //(forward, back, left, and right all call drive)
  void standby();

private:
  // variables for the 2 inputs, PWM input, offset value, and the Standby pin
  uint connection_1, connection_2, PWM, offset, stand_by;

  // private functions that spin the motor CC and CCW
  void fwd(int speed);
  void rev(int speed);

  // to write as PWM to motors.
  void analog_write(int speed);
};

// This function takes 2 motors and and brakes them
void brake(Motor motor1, Motor motor2);
