#pragma once

#include "TB6612FNG.h"
#include "hardware/adc.h"
#include "includes.h"
#include "pico/stdlib.h"
#include <array>
#include <atomic>
#include <memory>
#include <stdint.h>

#define linear_actuator_count (uint)3
#define sensor_count (uint)3
#define max_data (uint)255

class PicoContext {
public:
  static int push_value();
  static void refresh(uint sensor);
  static PicoContext *get_instance();
  static void initialize_instance(
      std::array<const MotorGPIO, linear_actuator_count> &motor_gpios);
  static uint get_current_sensor();
  static Motor &get_linear_actuator(uint linear_actuator);
  static void set_standby();

private:
  static std::atomic<PicoContext *> instance;
  static bool instance_initialized;
  PicoContext(std::array<const MotorGPIO, linear_actuator_count> &motor_gpios);
  uint sensor_state = 0;
  std::array<std::array<uint16_t, max_data>, sensor_count> sensors;
  std::array<Motor, linear_actuator_count> linear_actuators;
  constexpr static const std::array<const int, linear_actuator_count> offsets{
      1, 1, 1};
  static std::array<uint8_t, sensor_count> counters;
};

static inline void sensor_init() {
  adc_init();
  adc_gpio_init(ANALOG0);
  adc_gpio_init(ANALOG1);
  adc_gpio_init(ANALOG2);
  adc_set_round_robin(0b00111);
}