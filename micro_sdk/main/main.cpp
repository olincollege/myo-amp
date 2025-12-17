#include "TB6612FNG.h"
#include "context.h"
#include "includes.h"

#include "hardware/gpio.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"

#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"

#include <array>
#include <stdint.h>
#include <stdio.h>

void core1_entry(void) {
  // Pick up pointer from pushing to the Multicore queue and recast it.
  PicoContext *instance =
      reinterpret_cast<PicoContext *>(multicore_fifo_pop_blocking());
  // Clear the multicore queue.
  multicore_fifo_drain();
  sleep_ms(75);
  while (true) {
    if (multicore_fifo_rvalid()) {
      // set speed and duration to values inside instance (the ml read code
      // values) set linear actuators directly
      int model_ret = multicore_fifo_pop_blocking();
      PicoContext::convert_to_in_range(model_ret);

      sleep_ms(1500);
      printf("Linear Actuator 0 Drive out\n");
      instance->get_linear_actuator(0).drive(255);
      sleep_ms(1500);
      printf("Linear Actuator 0 Drive in\n");
      instance->get_linear_actuator(0).drive(-255);
      sleep_ms(1500);
      printf("Linear Actuator 1 Drive out\n");
      instance->get_linear_actuator(1).drive(255);
      sleep_ms(1500);
      printf("Linear Actuator 1 Drive in\n");
      instance->get_linear_actuator(1).drive(-255);
      sleep_ms(1500);
      printf("Linear Actuator 2 Drive out\n");
      instance->get_linear_actuator(2).drive(255);
      sleep_ms(1500);
      printf("Linear Actuator 2 Drive in\n");
      instance->get_linear_actuator(2).drive(-255);
      sleep_ms(1500);

      // instance->get_linear_actuator(0).drive(instance->get_model_value(0));
      // instance->get_linear_actuator(1).drive(instance->get_model_value(1));
      // instance->get_linear_actuator(2).drive(instance->get_model_value(2));
      // sleep_ms(1500);

      multicore_fifo_drain();
      // Clear the IO stream after every interaction with core 1.
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
  printf("startup!\n");

  // Initialize MotorGPIO array for the PicoContext object.
  std::array<const MotorGPIO, linear_actuator_count> motor_gpio_connections{
      {{MOTOR_0_CONNECTION_1, MOTOR_0_CONNECTION_2, PWM_MOTOR_0,
        STANDBY_DRIVER_0},
       {MOTOR_1_CONNECTION_1, MOTOR_1_CONNECTION_2, PWM_MOTOR_1,
        STANDBY_DRIVER_0},
       {MOTOR_2_CONNECTION_1, MOTOR_2_CONNECTION_2, 0, 0}}};
  PicoContext::initialize_instance(motor_gpio_connections);

  // Launch core 1
  multicore_launch_core1(core1_entry);
  sleep_ms(1000);

  // Get PicoContext instance and push it's pointer value to the multicore
  // queue, to be recast back to the pointer.
  PicoContext *instance = PicoContext::get_instance();
  multicore_fifo_push_blocking((uint32_t)instance);
  sleep_ms(75);

  // Count of how many refreshes happen to know when to trigger motor changes.
  uint refresh_counter = 0;

  while (true) {
    // Returns 1, 2, or 3 if it is ready to refresh. -1 gets the actual sensor
    // value.
    int is_ready_to_refresh = instance->push_value();
    if (is_ready_to_refresh > 0) {
      PicoContext::refresh(is_ready_to_refresh - 1);
      if (refresh_counter < 3) {
        refresh_counter++;
        // printf("Refresh counter: %d\n", refresh_counter);
      } else {
        // insert ml read code
        // should follow - or 6 if there should be a delay with the function
        // push pointer to say hey - i have completed this
        refresh_counter = 0;
        // Send signal to start driving motors

        multicore_fifo_push_blocking(1);
      }
    }
  }
}
