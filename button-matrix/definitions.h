/*
 * Copyright (C) 2017 Microchip Technology Inc. and its subsidiaries
 *
 * File:            definitions.h
 * Project:         
 * Date:            Nov 30, 2017
 * Compiler:        arm-gcc v6.3.1
 * 
 * Project-wide constants
 *
 * Hardware Configuration: SAMD21 Xplained PRO (ATSAMD21J18A) 
 *
 */


#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

// I/O Ports definitions
#define PORTA     (0ul)
#define PORTB     (1ul)

// LED0 definitions
#define LED0_PORT            PORTB
#define OUT0_PORT			 PORTB
#define OUT0_PIN_NUMBER		 (00ul)
#define LED0_PIN_NUMBER      (30ul)
#define LED0_PIN             PORT_PB30
#define OUT0_PIN			PORT_PB00

#define OUT1_PORT			 PORTB
#define OUT1_PIN_NUMBER		 (01ul)
#define OUT1_PIN			PORT_PB01

#define OUT2_PORT			 PORTB
#define OUT2_PIN_NUMBER		 (02ul)
#define OUT2_PIN			PORT_PB02

#define OUT3_PORT			 PORTB
#define OUT3_PIN_NUMBER		 (03ul)
#define OUT3_PIN			PORT_PB03

#define OUT4_PORT			 PORTB
#define OUT4_PIN_NUMBER		 (04ul)
#define OUT4_PIN			PORT_PB04

#define OUT5_PORT			 PORTB
#define OUT5_PIN_NUMBER		 (05ul)
#define OUT5_PIN			PORT_PB05

#define OUT6_PORT			 PORTB
#define OUT6_PIN_NUMBER		 (06ul)
#define OUT6_PIN			PORT_PB06

#define SW0_PIN				PORT_PA15			
PortGroup *porA = (PortGroup *)PORT;
PortGroup *porB = (PortGroup *)PORT;
// PortGroup *porB = (PortGroup *)PORT + 1;

Eic *eicptr = (Eic *)EIC;

#define NUM_OF_OUTPUTS 2

#endif /* DEFINITIONS_H_ */
