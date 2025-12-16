#pragma once
#include "pico/stdlib.h"

#define MAX_COUNT 255

#define UART_ID uart0

#define BAUD_RATE 115200

// Analog pins

#define ANALOG0 (uint)26

#define ANALOG1 (uint)27

#define ANALOG2 (uint)28

#define STANDBY_DRIVER_0 (uint)22

#define STANDBY_DRIVER_1 (uint)21

// Motor 0 Connections

#define PWM_MOTOR_0 (uint)20

#define MOTOR_0_CONNECTION_1 (uint)10

#define MOTOR_0_CONNECTION_2 (uint)11

// Motor 1 Connections

#define PWM_MOTOR_1 (uint)19

#define MOTOR_1_CONNECTION_1 (uint)13

#define MOTOR_1_CONNECTION_2 (uint)12

// Motor 2 Connections

#define PWM_MOTOR_2 (uint)18

#define MOTOR_2_CONNECTION_1 (uint)8

#define MOTOR_2_CONNECTION_2 (uint)9

void core1_entry(void);
