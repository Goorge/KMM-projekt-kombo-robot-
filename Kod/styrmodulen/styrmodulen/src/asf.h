/**
 * \file
 *
 * \brief Autogenerated API include file for the Atmel Software Framework (ASF)
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#ifndef ASF_H
#define ASF_H

typedef unsigned char byte;

/*
 * This file includes all API header files for the selected drivers from ASF.
 * Note: There might be duplicate includes required by more than one driver.
 *
 * The file is automatically generated and will be re-written when
 * running the ASF driver selector tool. Any changes will be discarded.
 */

// From module: Common build items for user board support templates
#include <user_board.h>

// From module: Generic board support
#include <board.h>

// From module: Interrupt management - megaAVR and tinyAVR implementation
#include <interrupt.h>

// From module: MEGA compiler driver
#include <compiler.h>
#include <status_codes.h>

// From module: Part identification macros
#include <parts.h>

// Define , rename 
#define motor_left (OCR0A)
#define motor_right (OCR0B)
#define motor_dir_left (PB6)
#define motor_dir_right (PB5)
// Global variables
extern int regler_ready;
extern int counter_timer;
extern int manual_function;
extern int drive_mode;
extern int start;
extern float current_position;
extern int left;
extern int right;
extern bool waypoint;
extern bool oneway_turn;
extern bool twoway_turn_left;
extern bool twoway_turn_right;
extern int sensor_left;
extern int sensor_right;
//global init
void req_gyro_turn(void);
void initengine(void);
void timerinit(void);
void interrupt_init(void);
void reset(void);
//global functions
void manual_drive(void);
void regulator(void);
void labyreg(void);
void linje(void);
void i2c_handel_data(void);
bool i2c_send(byte prossesor,byte* data);


#endif // ASF_H
