/*
 * Pwm.cpp
 *
 *  Created on: 19/dic/2015
 *      Author: massimo
 */

#include "Energia.h"
#include "Pwm.h"
#include "Timer.h"
#include <stdint.h>

PwmServo::PwmServo() {
	// TODO Auto-generated constructor stub

}

PwmServo::~PwmServo() {
	// TODO Auto-generated destructor stub
}

void PwmServo::setup(uint32_t freq, uint16_t fPwm){
	/// used pins are from P1.2 to P1.4
//	P1DIR |= BIT1 + BIT2 + BIT3 + BIT4;
//	P2SEL |= BIT1 + BIT2 + BIT3 + BIT4;;

#if defined(__MSP430_HAS_TA3__) || defined(__MSP430_HAS_T0A3__) || defined(__MSP430_HAS_T0A5__) || defined(__MSP430_HAS_TA5__)
	/// TIMER0 is present

	if(freq >= 3200000)
		TA0CTL 	= TASSEL_2 + MC_1 + TACLR + ID_3;	/// si usa il clock di sistema fdco >= 16MHZ
	else
		TA0CTL 	= TASSEL_2 + MC_1 + TACLR; 	/// si usa il clock di sistema fdco = 24.576MHz
	/// la frequenza è divisa per 8 dal coefficiente ID_3. Quindi F1 = fdco/8 = 3.072MHz
	/// F1 / T_PWM = 3.072000 / 8000 = 384; numero di conteggi da scrivere in TA2CCR0
	TA0CCR0 = (freq >> 3) / fPwm;

	/// PWM mode
	TA0CCTL1 = OUTMOD_7;
	TA0CCTL2 = OUTMOD_7;
	TA0CCTL3 = OUTMOD_7;
	TA0CCTL4 = OUTMOD_7;
	//TA0CCTL5 = OUTMOD_7;
	//TA0CCTL6 = OUTMOD_7;

	/// initial value on different PWM output
	TA0CCR1 = 0;
	TA0CCR2 = 0;
	TA0CCR3 = 0;
	TA0CCR4 = 0;
	//TA0CCR5 = 0;
	//TA0CCR6 = 0;
	// set up PWM period
	F_PWM = TA0CCR0;
#endif
}


void PwmServo::pinSetting(uint8_t pin){
    uint8_t bit = digitalPinToBitMask(pin); // get pin bit
    uint8_t port = digitalPinToPort(pin);   // get pin port
    volatile uint8_t *sel;

    if (port == NOT_A_PORT) return; // pin on timer?

    /// set pin in output mode BEFORE alternate pin function
    pinMode(pin, OUTPUT);

    sel = portSelRegister(port); // get the port function select register address
    *sel |= bit;                 // set bit in pin function select register

}

void PwmServo::pinValue(uint8_t pin, float value){

	unsigned int valore;

	valore = (unsigned int) value * 0.01 * F_PWM;

	switch(digitalPinToTimer(pin)) {                // which timer and CCR?
		//case: T0A0                            // CCR0 used as period register
		case T0A1:                              // TimerA0 / CCR1

		 TA0CCTL1 = OUTMOD_7;            		// reset/set
		 TA0CCR1 = valore;		 			// PWM duty cycle
		 break;

		case T0A2:                              // TimerA0 / CCR1

		 TA0CCTL2 = OUTMOD_7;            		// reset/set
		 TA0CCR2 = valore;		 			// PWM duty cycle
		 break;

		case T0A3:                              // TimerA0 / CCR1

		 TA0CCTL3 = OUTMOD_7;            		// reset/set
		 TA0CCR3 = valore; 					// PWM duty cycle
		 break;

		case T0A4:                              // TimerA0 / CCR1

		 TA0CCTL4 = OUTMOD_7;            		// reset/set
		 TA0CCR4 = valore; 					// PWM duty cycle
		 break;

	}
}


void PwmServo::pinValue(uint8_t pin, uint8_t val, uint16_t scale){

	float valTmp;
	unsigned int valIntero;

	valTmp = (float) val / scale * 0.01 * F_PWM;
	valIntero = (unsigned int) valTmp;

    switch(digitalPinToTimer(pin)) {                // which timer and CCR?
	//case: T0A0                            // CCR0 used as period register
	case T0A1:                              // TimerA0 / CCR1

	 TA0CCTL1 = OUTMOD_7;            		// reset/set
	 TA0CCR1 = valIntero; 			// PWM duty cycle
	 break;

	case T0A2:                              // TimerA0 / CCR1

	 TA0CCTL2 = OUTMOD_7;            		// reset/set
	 TA0CCR2 = valIntero; 			// PWM duty cycle
	 break;

	case T0A3:                              // TimerA0 / CCR1

	 TA0CCTL3 = OUTMOD_7;            		// reset/set
	 TA0CCR3 = valIntero; 			// PWM duty cycle
	 break;

	case T0A4:                              // TimerA0 / CCR1

	 TA0CCTL4 = OUTMOD_7;            		// reset/set
	 TA0CCR4 = valIntero; 			// PWM duty cycle
	 break;

    }
}

/// value used an integer angolar value
void PwmServo::pinValue(uint8_t pin, int angle){

	int16_t valore;
	volatile float fVal;
	/// conversion from angle to PWM value
	if (angle > 90)
		angle = 90;
	else if(angle < -90)
		angle = -90;

	/// granularity is 0.1%
	/// a PWM between 5 to 10% is split in 150 parts
	fVal = (float)angle / 90.0 * 2.5 + 7.5;
	fVal *= 30;
	valore = (int16_t) fVal;
	pinValue(pin, valore, 30);
	Serial.print("valore PWM: ");
	Serial.println(valore);
}


///
/// imposta i pin in funzione alternativa
void Pwm::pinSetting(uint8_t pin){
    uint8_t bit = digitalPinToBitMask(pin); // get pin bit
    uint8_t port = digitalPinToPort(pin);   // get pin port
    volatile uint8_t *sel;

    if (port == NOT_A_PORT) return; // pin on timer?

    /// set pin in output mode BEFORE alternate pin function
    pinMode(pin, OUTPUT);

    sel = portSelRegister(port); // get the port function select register address
    *sel |= bit;                 // set bit in pin function select register

}

void Pwm::begin(unsigned long int a){

	Timer::begin(a);
}
