#include "TB6612FNG.h"
#include "context.h"
#include "includes.h"

#include "pico/multicore.h"
#include "pico/stdlib.h"

#include <array>
#include <stdint.h>
#include <stdio.h>

void core1_entry(void) {
  while (!multicore_fifo_rvalid()) {
  }
  PicoContext *instance =
      reinterpret_cast<PicoContext *>(multicore_fifo_pop_blocking());
  multicore_fifo_drain();
  while (true) {
    if (multicore_fifo_rvalid()) {
      instance->get_linear_actuator(0).drive(255, 1000);
      printf("Driving Linear Actuator 1 in\n");
      instance->get_linear_actuator(0).drive(-255, 1000);
      printf("Driving Linear Actuator 1 out\n");
      sleep_ms(1500);
      instance->get_linear_actuator(1).drive(255, 1000);
      printf("Driving Linear Actuator 2 in\n");
      instance->get_linear_actuator(1).drive(-255, 1000);
      printf("Driving Linear Actuator 2 out\n");
      sleep_ms(1500);
      multicore_fifo_drain();
    } else {
      PicoContext::set_standby();
    }
  }
}

int main() {
  stdio_init_all();

  std::array<const MotorGPIO, linear_actuator_count> motor_gpio_connections{
      {{MOTOR_0_CONNECTION_1, MOTOR_0_CONNECTION_2, PWM_MOTOR_0,
        STANDBY_DRIVER_0},
       {MOTOR_1_CONNECTION_1, MOTOR_1_CONNECTION_2, PWM_MOTOR_1,
        STANDBY_DRIVER_0},
       {MOTOR_2_CONNECTION_1, MOTOR_2_CONNECTION_2, PWM_MOTOR_2,
        STANDBY_DRIVER_1}}};
  PicoContext::initialize_instance(motor_gpio_connections);

  multicore_reset_core1();
  multicore_launch_core1(core1_entry);
  PicoContext *instance = PicoContext::get_instance();
  multicore_fifo_push_blocking((uint32_t)instance);

  while (true) {
    int is_ready_to_refresh = instance->push_value();
    if (is_ready_to_refresh > 0) {
      PicoContext::refresh(is_ready_to_refresh);
      sleep_ms(75);
    }

    if (multicore_fifo_wready()) {
      multicore_fifo_push_blocking(1);
    }
  }
}
