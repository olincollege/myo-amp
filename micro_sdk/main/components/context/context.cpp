#include "context.h"

#include "TB6612FNG.h"
#include "includes.h"

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
    return_val = context->sensor_state + 1;
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
    printf("sensor_%u: %u\n", sensor, context->sensors[sensor][i]);
    context->sensors[sensor][i] = (uint16_t)0;
  }
  context->counters[sensor] = 0;
}

typedef std::array<const MotorGPIO, linear_actuator_count> motor_gpio_list;

void PicoContext::initialize_instance(motor_gpio_list &motor_gpios) {
  if (instance_initialized) {
    return;
  }
  auto *context = new PicoContext(motor_gpios);
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

PicoContext::PicoContext(
    std::array<const MotorGPIO, linear_actuator_count> &motor_gpios) {
  sensor_init();

  for (uint i = 0; i < linear_actuator_count; i++) {
    linear_actuators[i].~Motor();
    new (&linear_actuators[i]) Motor(motor_gpios[i], offsets[i]);
  }

  for (std::array<uint16_t, max_data> &sensor : sensors) {
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
