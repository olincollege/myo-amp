#pragma once

#include "TB6612FNG.h"
#include "pico/stdlib.h"
#include <stdint.h>

class SpecialMotor : public Motor {
public:
  SpecialMotor() = delete;
  SpecialMotor(uint connection_1_pin, uint connection_2pin, uint PWM_pin,
               uint standby_pin, int offset);
  SpecialMotor(const MotorGPIO &motor_gpio, int offset);
  SpecialMotor(const Motor &) = delete;
  void drive(int speed) override;
  void drive(int speed, uint32_t duration) override;
  void brake() override;
  void standby() override;

private:
  void analog_write(uint pin, int speed) override;
  void fwd(int speed) override;
  void rev(int speed) override;
};