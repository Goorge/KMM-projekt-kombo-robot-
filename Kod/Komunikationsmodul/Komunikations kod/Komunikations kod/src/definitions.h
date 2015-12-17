/*
 * definitions.h
 *
 * Created: 2015-11-16 12:49:21
 *  Author: Pontus
 */ 

#ifndef definitions
#define definitions

//typedef int bool;
#define true 1
#define false 0

typedef unsigned char byte;

// Räkna ut antalet element i en array
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

// BLUETOOTh pinnar
#define RXD PD0
#define TXD PD1
#define CTS PD2
#define RTS PD3

#define F_CPU 14745600UL
#include <util/delay.h>

#endif