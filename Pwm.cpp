/*
 * Pwm.cpp
 *
 *  Created on: 19/dic/2015
 *      Author: massimo
 */

#include "Energia.h"
#include "Pwm.h"
#include "Timer.h"
#include "TimerSerial.h"
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
	T_PWM = TA0CCR0;
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

	valore = (unsigned int) value * 0.01 * T_PWM;

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

	valTmp = (float) val / scale * 0.01 * T_PWM;
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
	/// prima di stampare si chiede se la seriale e' stata definita
	if(TimerSerial::uso == 1){
		Serial.print("valore PWM: ");
		Serial.println(valore);
	}

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

///
/// il periodo della classe timer puo' arrivare al minimo al millisecondo
/// il periodo della classe Pwm puo' arrivare al minimo al microsecondo
void Pwm::begin(unsigned long int a){

	if (a > 1000){
		long int temp = a / 1000;
		/// chiama il costruttore della classe base
		Timer::begin(temp);
	}
	else{
		unsigned long int divisore = F_CPU / 1000000 * a;
		/// non servono divisori, in quanto al massimo il valore nel
		/// registro TAxCCR0 raggiunge 25000
		switch(m_whoAmI - 1){

			TA1CCR0 = divisore;
			TA1CTL |= TASSEL_2 | MC_1 | TACLR;   // Clock TIMERA from SMCLK, run in up mode counting from to 0-TA1CCR0
			T = TA1CCR0;
		break;

		case 1:
			TA2CCR0 = divisore;
			TA2CTL |= TASSEL_2 | MC_1 | TACLR;   // Clock TIMERA from SMCLK, run in up mode counting from to 0-TA1CCR0
			T = TA2CCR0;
		break;

		case 2:
			TB0CCR0 = divisore;
			TB0CTL |= TBSSEL_2 | MC_1 | TBCLR;   // Clock TIMERA from SMCLK, run in up mode counting from to 0-TA1CCR0
			T = TB0CCR0;
		break;

		}
	}
}

///
/// imposta il valore del delta rispetto al periodo che sta nella variabile della classe
/// base T
void Pwm::pinValue(uint8_t pin, float value){

	unsigned int valore;

	valore = (unsigned int) (value * 0.01 * T);

	/// PWM sul timer A1
	if((m_whoAmI - 1) == 0){
		/// seleziona il pin ed imposta il pwm
		switch(digitalPinToTimer(pin)) {
		case T1A1:
			TA1CCTL1 = OUTMOD_7;            	// reset/set
			TA1CCR1 = valore;		 			// PWM duty cycle
		break;
		}
		/// esce subito
		return;
	}

	/// PWM sul timer A2
	if((m_whoAmI - 1) == 1){
		/// seleziona il pin ed imposta il pwm
		switch(digitalPinToTimer(pin)) {
		case T2A1:
			TA2CCTL1 = OUTMOD_7;            	// reset/set
			TA2CCR1 = valore;		 			// PWM duty cycle
		break;

		case T2A2:
			TA2CCTL2 = OUTMOD_7;            	// reset/set
			TA2CCR2 = valore;		 			// PWM duty cycle
		break;
		}
		return;
	}

	/// PWM sul timer B0
	if((m_whoAmI - 1) == 2){
		switch(digitalPinToTimer(pin)) {
		case T0B1:
			TB0CCTL1 = OUTMOD_7;            	// reset/set
			TB0CCR1 = valore;		 			// PWM duty cycle
		break;

		case T0B2:
			TB0CCTL2 = OUTMOD_7;            	// reset/set
			TB0CCR2 = valore;		 			// PWM duty cycle
		break;

		case T0B3:
			TB0CCTL3 = OUTMOD_7;            	// reset/set
			TB0CCR3 = valore;		 			// PWM duty cycle
		break;

		case T0B4:
			TB0CCTL4 = OUTMOD_7;            	// reset/set
			TB0CCR4 = valore;		 			// PWM duty cycle
		break;

		case T0B5:
			TB0CCTL5 = OUTMOD_7;            	// reset/set
			TB0CCR5 = valore;		 			// PWM duty cycle
		break;

		case T0B6:
			TB0CCTL6 = OUTMOD_7;            	// reset/set
			TB0CCR6 = valore;		 			// PWM duty cycle
		break;

		}
		return;
	}

}

