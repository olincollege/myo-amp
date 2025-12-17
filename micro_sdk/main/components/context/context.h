#pragma once

#include "TB6612FNG.h"
#include "hardware/adc.h"
#include "includes.h"
#include "pico/stdlib.h"
#include <array>
#include <atomic>
#include <memory>
#include <stdint.h>

#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"

// amount of linear actuators
#define linear_actuator_count (uint)3

// amount of sensors
#define sensor_count (uint)3

// amount of data that can be held by a sensor area
#define max_data (uint)255

// type to keep writing this much easier
typedef std::array<const MotorGPIO, linear_actuator_count> motor_gpio_list;
typedef std::array<uint16_t, max_data> sensor_value_list;

/***
 * PicoContext:
 *  Class to hold all of the ongoing state in the program. Holds sensor values,
 *  motor driving values and it's own instance.
 */
class PicoContext {
public:
  // Reads value from the sensor and pushes it to the sensor's value array.
  static int push_value();

  // Refreshes the sensor's count. For debugging purposes, it can keep data
  // around or clear it.
  static void refresh(uint sensor);

  // Returns a pointer to the single PicoContext instance from it's atomic
  // container.
  static PicoContext *get_instance();

  // Initializes the single PicoContext instance, if not initialized already.
  static void initialize_instance(motor_gpio_list &motor_gpios);

  // Gets the current sensor. Since a Round robin implementation is used,
  // separate state for the current sensor must be recorded.
  static uint get_current_sensor();

  // Get a Linear Actuator from the Motor list.
  static Motor &get_linear_actuator(uint linear_actuator);

  // Set all of the motor drivers to be on standby. Calls all of the Motors to
  // be on standby.
  static void set_standby();

  static void set_model_value(int index, int value);

  static int get_model_value(int index);

  static void convert_to_in_range(int model_ret);

private:
  // The singular atomic PicoContext instance.
  static std::atomic<PicoContext *> instance;

  // Whether the instance has been initialized or not. This will determine the
  // behavior of initialize instance.
  static bool instance_initialized;

  // Constructor using the motor_gpio_list.
  PicoContext(motor_gpio_list &motor_gpios);

  // Current sensor being read. Changes rapidly due to round robin logic. Starts
  // with the first sensor.
  uint sensor_state = 0;

  // The array of the different sensors' value list.
  std::array<sensor_value_list, sensor_count> sensors;

  // The list of all of the Motor classes.
  std::array<Motor, linear_actuator_count> linear_actuators;

  std::array<int, sensor_count> model_values;

  std::array<uint8_t, sensor_count> motor_output;

  // All of the offsets, in order. Constexpr because you have to change this
  // before compile time anyways.
  constexpr static const std::array<const int, linear_actuator_count> offsets{
      1, 1, 1};

  // Holds all of the counters up to 255.
  static std::array<uint8_t, sensor_count> counters;
};

// Initializes the sensors and puts them in Round robin style.
static inline void sensor_init() {
  adc_init();
  adc_gpio_init(ANALOG0);
  adc_gpio_init(ANALOG1);
  adc_gpio_init(ANALOG2);
  adc_set_round_robin(0b00111);
}