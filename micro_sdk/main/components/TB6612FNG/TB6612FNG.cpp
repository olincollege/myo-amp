/******************************************************************************
TB6612.cpp
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

#include "TB6612FNG.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "includes.h"

#include "pico/stdlib.h"
#include <memory>

#define HIGH true
#define LOW false

static bool standby_set = false;

static void gpio_set_standby() {
  if (!standby_set) {
    gpio_init(STANDBY_DRIVER_0);
    gpio_set_dir(STANDBY_DRIVER_0, GPIO_OUT);
    gpio_init(STANDBY_DRIVER_1);
    gpio_set_dir(STANDBY_DRIVER_1, GPIO_OUT);
    standby_set = true;
  }
}

Motor::Motor(uint connection_1_pin, uint connection_2_pin, uint PWM_pin,
             uint standby_pin, int offset) {
  connection_1 = connection_1_pin;
  connection_2 = connection_2_pin;
  PWM = PWM_pin;
  stand_by = standby_pin;
  this->offset = offset;

  gpio_init(PWM_pin);
  gpio_set_dir(PWM_pin, GPIO_OUT);
  gpio_set_function(PWM_pin, GPIO_FUNC_PWM);

  gpio_init(connection_1_pin);
  gpio_set_dir(connection_1_pin, GPIO_OUT);

  gpio_init(connection_2_pin);
  gpio_set_dir(connection_2_pin, GPIO_OUT);

  gpio_set_standby();
}

Motor::Motor(const MotorGPIO &motor_gpio, int offset) {
  connection_1 = motor_gpio.connection_1_pin;
  connection_2 = motor_gpio.connection_2_pin;
  PWM = motor_gpio.PWM_pin;
  stand_by = motor_gpio.standby_pin;
  this->offset = offset;

  gpio_init(PWM);
  gpio_set_dir(PWM, GPIO_OUT);
  gpio_set_function(PWM, GPIO_FUNC_PWM);

  gpio_init(connection_1);
  gpio_set_dir(connection_1, GPIO_OUT);

  gpio_init(connection_2);
  gpio_set_dir(connection_2, GPIO_OUT);

  gpio_set_standby();
}

void Motor::analog_write(int speed) {
  uint slice = pwm_gpio_to_slice_num(PWM);
  uint channel = pwm_gpio_to_channel(PWM);

  pwm_set_wrap(slice, DEFAULTSPEED);
  pwm_set_chan_level(slice, channel, speed);
  pwm_set_enabled(slice, true);
}

void Motor::drive(int speed) {
  gpio_put(stand_by, HIGH);
  speed = speed * offset;
  if (speed >= 0)
    fwd(speed);
  else
    rev(-speed);
}
void Motor::drive(int speed, uint32_t duration) {
  drive(speed);
  sleep_ms(duration);
}

void Motor::fwd(int speed) {
  gpio_put(connection_1, HIGH);
  gpio_put(connection_2, LOW);
  analog_write(speed);
}

void Motor::rev(int speed) {
  gpio_put(connection_1, LOW);
  gpio_put(connection_2, HIGH);
  analog_write(speed);
}

void Motor::brake() {
  gpio_put(connection_1, HIGH);
  gpio_put(connection_2, HIGH);
  analog_write(0);
}

void Motor::standby() { gpio_put(stand_by, LOW); }

void brake(Motor motor1, Motor motor2) {
  motor1.brake();
  motor2.brake();
}