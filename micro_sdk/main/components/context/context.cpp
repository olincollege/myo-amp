#include "context.h"

#include "DRV8871.h"
#include "TB6612FNG.h"
#include "includes.h"

#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"

#include "pico/stdlib.h"
#include <array>
#include <atomic>
#include <memory>
#include <stdint.h>
#include <stdio.h>

// Static member definitions
std::atomic<PicoContext *> PicoContext::instance = nullptr;
bool PicoContext::instance_initialized = false;
std::array<uint8_t, sensor_count> PicoContext::counters{0, 0, 0};

int PicoContext::push_value() {
  PicoContext *context = get_instance();

  int return_val = 0;
  context->sensors[context->sensor_state]
                  [context->counters[context->sensor_state]] = adc_read();

  context->counters[context->sensor_state]++;

  if (context->counters[context->sensor_state] == 255) {
    return_val = context->sensor_state + 1; // Set to +1 to avoid reading 0.
  }
  context->sensor_state++;
  if (context->sensor_state >= 3) {
    context->sensor_state = 0;
  }

  return return_val;
}

void PicoContext::refresh(uint sensor) {
  PicoContext *context = get_instance();
  for (uint i = 0; i < max_data; i++) {
    // ! Print loop - set for removal after gathering training data is done.
    printf("sensor_%u: %u\n", sensor, context->sensors[sensor][i]);
    // TODO: Remove for live viewing
    context->sensors[sensor][i] = (uint16_t)0;
  }
  context->counters[sensor] = 0;
}

void PicoContext::initialize_instance(motor_gpio_list &motor_gpios) {
  if (instance_initialized) {
    return;
  }
  // Creates in heap
  PicoContext *context = new PicoContext(motor_gpios);

  // Stores in atomic container
  instance.store(context, std::memory_order_release);
  instance_initialized = true;
  return;
}

uint PicoContext::get_current_sensor() {
  PicoContext *context = get_instance();
  return context->sensor_state;
}

PicoContext *PicoContext::get_instance() {
  return instance.load(std::memory_order_acquire);
}

Motor &PicoContext::get_linear_actuator(uint linear_actuator) {
  PicoContext *context = get_instance();
  return context->linear_actuators[linear_actuator];
}

PicoContext::PicoContext(motor_gpio_list &motor_gpios) {
  sensor_init();

  linear_actuators[0].~Motor();
  linear_actuators[1].~Motor();
  linear_actuators[2].~Motor();
  new (&linear_actuators[0]) Motor(motor_gpios[0], offsets[0]);

  new (&linear_actuators[1]) Motor(motor_gpios[1], offsets[1]);

  new (&linear_actuators[2]) SpecialMotor(motor_gpios[2], offsets[2]);

  for (sensor_value_list &sensor : sensors) {
    sensor.fill(0);
  }

  sensor_state = 0;
}

void PicoContext::set_standby() {
  PicoContext *context = get_instance();
  for (int i = 0; i < 3; i++) {
    context->linear_actuators[i].standby();
  }
}
