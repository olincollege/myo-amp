#include "DRV8871.h"
#include "TB6612FNG.h"

#include "TB6612FNG.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "includes.h"

#include "pico/stdlib.h"
#include <memory>

// name change for readability
#define HIGH true
#define LOW false

SpecialMotor::SpecialMotor(uint connection_1_pin, uint connection_2_pin,
                           uint PWM_pin, uint standby_pin, int offset) {
  connection_1 = connection_1_pin;
  connection_2 = connection_2_pin;
  // PWM and standby pins no longer necessary
  PWM = 0;
  stand_by = 0;
  this->offset = offset;

  // Predefine both as PWM pins, and set them to 0 when on standby and on 255
  // when needing to brake.
  gpio_init(connection_1_pin);
  gpio_set_dir(connection_1_pin, GPIO_OUT);
  gpio_set_function(connection_1_pin, GPIO_FUNC_PWM);
  uint slice_1 = pwm_gpio_to_slice_num(connection_1_pin);
  pwm_set_wrap(slice_1, DEFAULTSPEED);
  pwm_set_enabled(slice_1, true);

  gpio_init(connection_2_pin);
  gpio_set_dir(connection_2_pin, GPIO_OUT);
  gpio_set_function(connection_2_pin, GPIO_FUNC_PWM);
  uint slice_2 = pwm_gpio_to_slice_num(connection_2_pin);
  pwm_set_wrap(slice_2, DEFAULTSPEED);
  pwm_set_enabled(slice_2, true);
}

SpecialMotor::SpecialMotor(const MotorGPIO &motor_gpio, int offset) {
  connection_1 = motor_gpio.connection_1_pin;
  connection_2 = motor_gpio.connection_2_pin;
  PWM = 0;
  stand_by = 0;
  this->offset = offset;

  gpio_init(motor_gpio.connection_1_pin);
  gpio_set_dir(motor_gpio.connection_1_pin, GPIO_OUT);
  gpio_set_function(motor_gpio.connection_1_pin, GPIO_FUNC_PWM);
  uint slice_1 = pwm_gpio_to_slice_num(motor_gpio.connection_1_pin);
  pwm_set_wrap(slice_1, DEFAULTSPEED);
  pwm_set_enabled(slice_1, true);

  gpio_init(motor_gpio.connection_2_pin);
  gpio_set_dir(motor_gpio.connection_2_pin, GPIO_OUT);
  gpio_set_function(motor_gpio.connection_2_pin, GPIO_FUNC_PWM);
  uint slice_2 = pwm_gpio_to_slice_num(motor_gpio.connection_2_pin);
  pwm_set_wrap(slice_2, DEFAULTSPEED);
  pwm_set_enabled(slice_2, true);
}

void SpecialMotor::analog_write(uint pin, int speed) {
  uint slice = pwm_gpio_to_slice_num(pin);
  uint channel = pwm_gpio_to_channel(pin);

  pwm_set_chan_level(slice, channel, speed);
}

void SpecialMotor::drive(int speed) {
  speed *= offset;
  if (speed >= 0)
    fwd(speed);
  else
    rev(-speed);
}

void SpecialMotor::drive(int speed, uint32_t duration) {
  drive(speed);
  sleep_ms(duration);
}

void SpecialMotor::fwd(int speed) {
  analog_write(connection_1, speed);
  analog_write(connection_2, 0);
}

void SpecialMotor::rev(int speed) {
  analog_write(connection_1, 0);
  analog_write(connection_2, speed);
}

void SpecialMotor::brake() {
  analog_write(connection_1, DEFAULTSPEED);
  analog_write(connection_2, DEFAULTSPEED);
}

void SpecialMotor::standby() {
  analog_write(connection_1, 0);
  analog_write(connection_2, 0);
}
