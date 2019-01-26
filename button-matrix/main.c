/*
 * Copyright (C) 2017 Microchip Technology Inc. and its subsidiaries
 *
 * File:            main.c
 * Project:         systick-initialization
 * Date:            Nov 30, 2017
 * Compiler:        arm-gcc v6.3.1
 * 
 * Simple "GCC Executable" Code Example Project using the SysTick Timer.
 *
 * Configure SysTick to trigger an interrupt every millisecond and toggle LED0
 * every 1000mS.
 * 
 * Hardware Configuration: SAMD21 Xplained PRO (ATSAMD21J18A) 
 *
 */

/** INCLUDES ******************************************************************/


#include "sam.h"
#include "definitions.h"

/** VARIABLES *****************************************************************/

static uint32_t ul_tickcount=0 ;	// Global state variable for tick count

/** LOCAL PROTOTYPES **********************************************************/
uint8_t segementPins[10] = {0b00111111,0b000000011,0b1101101,0b1100111,0b1010011,0b1110110,0b1111110,0b0100011,0b11111111,0b1110011};
uint32_t columnList[NUM_OF_OUTPUTS] = {PORT_PA17, PORT_PA18};

void matrixConfig(void);
int calcNumber(int columnNum);
int isPressed(int columnNum);
int calcPort(int num);
void display(int number);
void AppInit(void);
void clockInit(void);
void eicInit(void);

int main(void){	
	AppInit();
	matrixConfig();
//	eicInit();
//	eicHandler();
//TODO: ADD COLOMN SUPPORT. ONLY CHECKS ROWS.
	//porA->OUTSET.reg = PORT_PA18;
    int currentNum = 0, prevNum = 0;
    while(1){
	/*	for(int i = 0; i < NUM_OF_OUTPUTS; i++){
			if(i != 0)
				porA->OUTCLR.reg = columnList[i-1];
			porA->OUTSET.reg = columnList[i];
			//currentNum = (porA->IN.reg & calcPort(currentNum) ? currentNum : calcNumber(i));
			currentNum = calcNumber(i);
			if(currentNum != -1)
				break;
		}
		porA->OUTCLR.reg = columnList[NUM_OF_OUTPUTS-1];
		if(currentNum == -1 || isPressed(prevNum)){
			display(prevNum);
		}
		else{
			prevNum = currentNum;
			display(currentNum);
		}*/
	if(isPressed(4))
		display(4);
	else display(-1);
	}
}


void eicInit(void){
	PM->APBAMASK.bit.EIC_ = 0x1u;                           // Enable EIC on APBA via PM
	uint32_t temp = 0x5u;                                  // EIC clock is 0x5
	temp |= 0x0u << 8;                                      // Selection of GCLK 0 for EIC
	GCLK->CLKCTRL.reg = temp;                              // clear clkctrl register?
	GCLK->CLKCTRL.reg |= 0x1u << 14;                       // Enable EIC clock
	
	eicptr->CTRL.reg = 0x0u;                               // Disable EIC
	eicptr->INTENSET.reg = 0x1u << 10;                     // Enable external interrupt EXTINT[10] for PA10
	eicptr->EVCTRL.bit.EXTINTEO10 = 1;                      // Event control detection enabled for pin that has EXTINT[10] -> PA10
	eicptr->CONFIG[1].bit.SENSE2 = 0x4;                    // EXTINT[n*8+x], Config[n] and Sensex and 0x4 is configured for high-level detection
	while(eicptr->STATUS.reg & EIC_STATUS_SYNCBUSY);       // Let the EIC sync with the system clock
	eicptr ->CTRL.reg = 0x2;                               // Enable EIC on APBA
	while(eicptr->STATUS.reg & EIC_STATUS_SYNCBUSY);
}

void eicHandler(void){
	/*if(eicptr->INTFLAG.reg & (0x1u << 15)){                // If INTFLAG is triggered from EXTINT[10] then the handler starts
		if(porA->IN.reg & PORT_PA10){                      //
			porA->OUTSET.reg = PA11;
			wait(2);
			porA->OUTCLR.reg = PA11;
			eicptr->INTFLAG.bit.EXTINT10 = 1;
		}
	}*/
	//PORT->Group[LED0_PORT].OUTTGL.reg = LED0_PIN;
}

void matrixConfig(void){
	//3x2 matrix input 1 enable on PA10
	porA->DIRCLR.reg = PORT_PA10;                                  // PA10 selected as input [BUTTON]
	porA->OUTCLR.reg = PORT_PA10;                                  // set input pull direction to pull-down
	porA->PINCFG[10].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
	//3x2 matrix input 2 enable on PA11
	porA->DIRCLR.reg = PORT_PA11;                                  // PA11 selected as input [BUTTON]
	porA->OUTCLR.reg = PORT_PA11;                                  // set input pull direction to pull-down
	porA->PINCFG[11].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
	//3x2 matrix input 3 enable on PA20
	porA->DIRCLR.reg = PORT_PA20;                                  // PA12 selected as input [BUTTON]
	porA->OUTCLR.reg = PORT_PA20;                                  // set input pull direction to pull-down
	porA->PINCFG[20].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
	//3x2 matrix output 1 enable on PA17
	porA->DIRSET.reg = PORT_PA17;
	porA->PINCFG[17].reg &= ~PORT_PINCFG_INEN; 
	//3x2 matrix output 2 enable on PA18
	porA->DIRSET.reg = PORT_PA18;
	porA->PINCFG[18].reg &= ~PORT_PINCFG_INEN;
}

void AppInit(void){
	//Assign PA10 as the input (writing a one to corresponding bit in the DIR w/ DIRCLR sets the IO pin direction to configure the IO as input.
	// Assign the LED0 pin as OUTPUT
	PORT->Group[LED0_PORT].DIRSET.reg = LED0_PIN;
	// Set the LED0 pin level, ie put to 3.3V -> this light of the LED
	PORT->Group[LED0_PORT].OUTSET.reg = LED0_PIN;
	PORT->Group[OUT0_PORT].DIRSET.reg =	OUT0_PIN;
	PORT->Group[OUT1_PORT].DIRSET.reg =	OUT1_PIN;
	PORT->Group[OUT2_PORT].DIRSET.reg =	OUT2_PIN;
	PORT->Group[OUT3_PORT].DIRSET.reg =	OUT3_PIN;
	PORT->Group[OUT4_PORT].DIRSET.reg =	OUT4_PIN;
	PORT->Group[OUT5_PORT].DIRSET.reg =	OUT5_PIN;
	PORT->Group[OUT6_PORT].DIRSET.reg =	OUT6_PIN;
	//Port A(which is 0) on pin 15 needs to have PULLEN and OUT Enabled
	//PORT->Group[0].PINCFG[15].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
	//PORT->Group[0].OUTSET.reg = SW0_PIN;	
	
	SysTick->CTRL = 0;					// Disable SysTick
	SysTick->LOAD = 500UL;				// Set reload register for 1mS interrupts
	NVIC_SetPriority(SysTick_IRQn, 3);	// Set interrupt priority to least urgency
	SysTick->VAL = 0;					// Reset the SysTick counter value
	SysTick->CTRL = 0x00000007;			// Enable SysTick, Enable SysTick Exceptions, Use CPU Clock
	NVIC_EnableIRQ(SysTick_IRQn);		// Enable SysTick Interrupt
	
}


void SysTick_Handler(void){
	static uint8_t holder = 0;
	//ul_tickcount++ ;
	if(ul_tickcount % 1000 == 0){
		//PORT->Group[OUT0_PORT].OUTCLR.reg = 0b11111111;
		if(holder == 10){
		//	PORT->Group[OUT0_PORT].OUTCLR.reg = holder/2;
			holder = 0;
		}
		//PORT->Group[LED0_PORT].OUTTGL.reg = LED0_PIN ;
	//	PORT->Group[OUT0_PORT].OUTSET.reg = segementPins[holder];
	//	PORT->Group[OUT0_PORT].OUTCLR.reg = holder/2;
		holder++;
		ul_tickcount=0;
	}
}

void clockInit(void){
	SYSCTRL->OSC8M.bit.PRESC = 0;                          // no prescaler (is 8 on reset)
	SYSCTRL->OSC8M.reg |= 0x1u << 1;                       // enable source

	GCLK->GENDIV.bit.ID = 0x0;                             // select GCLK_GEN[0]
	GCLK->GENDIV.bit.DIV = 0;                              // no prescaler

	GCLK->GENCTRL.bit.ID = 0x0;                            // select GCLK_GEN[0]
	GCLK->GENCTRL.reg |= GCLK_GENCTRL_SRC_OSC8M;           // OSC8M source
	GCLK->GENCTRL.bit.GENEN = 1;                           // enable generator

	PM->APBASEL.bit.APBADIV = 0;                           // no prescaler
	PM->APBAMASK.reg = 0x1u << 3;                          // enable APBA clock for GCLK
}

void display(int number){
	PORT->Group[OUT0_PORT].OUTCLR.reg = 0xFFFFFFFF;
	PORT->Group[OUT0_PORT].OUTSET.reg = ((number == (-1)) ? 0b01000000 : segementPins[number]);
}

int calcPort(int num){
	if(num == -1)
		return num;
	switch (num % 3){
		case 0:
			return PORT_PA10;
		case 1:
			return PORT_PA11;
		case 2:
			return PORT_PA11;
	}
}

int calcNumber(int columnNum){
	if(porA->IN.reg & PORT_PA10)
		return 0 + 3*columnNum;
	else if(porA->IN.reg & PORT_PA11)
		return 1 + 3*columnNum;
	else if(porA->IN.reg & PORT_PA20)
		return 2 + 3*columnNum;
	else
		return -1;
}

int isPressed(int num){
	int column = num/3;
	porA->OUTCLR.reg = 0xFFFFFFFF;
	porA->OUTSET.reg = columnList[column];
	int temp = (porA->IN.reg & calcPort(num));
	porA->OUTCLR.reg = 0xFFFFFFFF;
	return temp;
}