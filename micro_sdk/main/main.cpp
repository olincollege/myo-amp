#include "TB6612FNG.h"
#include "context.h"
#include "includes.h"

#include "hardware/gpio.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"

#include <array>
#include <stdint.h>
#include <stdio.h>

void core1_entry(void) {
  PicoContext *instance =
      reinterpret_cast<PicoContext *>(multicore_fifo_pop_blocking());
  multicore_fifo_drain();
  sleep_ms(75);
  while (true) {
    if (multicore_fifo_rvalid()) {
      // set speed and duration to values inside instance (the ml read code
      // values) set linear actuators directly
      instance->get_linear_actuator(0).drive(255, 1000);
      sleep_ms(75);
      instance->get_linear_actuator(0).drive(-255, 1000);
      sleep_ms(1500);
      instance->get_linear_actuator(1).drive(255, 1000);
      sleep_ms(75);
      instance->get_linear_actuator(1).drive(-255, 1000);
      sleep_ms(1500);
      multicore_fifo_drain();
      stdio_flush();
    } else {
      PicoContext::set_standby();
      // printf("Waiting for sensor changes..\n");
    }
    sleep_ms(75);
  }
}

int main() {

  stdio_init_all();
  sleep_ms(2000);

  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
  gpio_put(PICO_DEFAULT_LED_PIN, true);

  std::array<const MotorGPIO, linear_actuator_count> motor_gpio_connections{
      {{MOTOR_0_CONNECTION_1, MOTOR_0_CONNECTION_2, PWM_MOTOR_0,
        STANDBY_DRIVER_0},
       {MOTOR_1_CONNECTION_1, MOTOR_1_CONNECTION_2, PWM_MOTOR_1,
        STANDBY_DRIVER_0},
       {MOTOR_2_CONNECTION_1, MOTOR_2_CONNECTION_2, PWM_MOTOR_2,
        STANDBY_DRIVER_1}}};
  PicoContext::initialize_instance(motor_gpio_connections);

  multicore_launch_core1(core1_entry);
  sleep_ms(1000);
  PicoContext *instance = PicoContext::get_instance();
  multicore_fifo_push_blocking((uint32_t)instance);
  sleep_ms(75);

  uint refresh_counter = 0;

  while (true) {
    int is_ready_to_refresh = instance->push_value();
    if (is_ready_to_refresh > 0) {
      PicoContext::refresh(is_ready_to_refresh - 1);
      if (refresh_counter < 3) {
        refresh_counter++;
        printf("Refresh counter: %d\n", refresh_counter);
      } else {
        // insert ml read code
        // should function like - edit an array of 3 values that each motor
        // should follow - or 6 if there should be a delay with the function
        // push pointer to say hey - i have completed this
        refresh_counter = 0;
        multicore_fifo_push_blocking(1);
      }
      sleep_ms(75);
    }
  }
}
