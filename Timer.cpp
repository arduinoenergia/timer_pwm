/*
 * Timer.cpp
 *
 *  Created on: 14/dic/2015
 *      Author: massimo
 */

#include "Energia.h"
#include "Timer.h"

/// inizializzazione delle variabili statiche: obbligatorio
unsigned int Timer::tBaseAdd[] = {0x0, 0x0, 0x0};
unsigned char Timer::m_Cont = 0;
void (* Timer::isr[3])() = {0x0, 0x0, 0x0};



Timer::Timer() {
	// TODO Auto-generated constructor stub

	/// tengo traccia del mio numero di timer
	m_whoAmI = m_Cont + 1;;

	switch(m_Cont){
	case 0:
		tBaseAdd[0] = __MSP430_BASEADDRESS_T1A3__; /// TA1
		m_Cont++;

	break;

	case 1:
		tBaseAdd[1] = __MSP430_BASEADDRESS_T2A3__; /// TA2
		m_Cont++;
	break;

	case 2:
		tBaseAdd[2] = __MSP430_BASEADDRESS_T0B7__; /// TB0
	break;
	}


}


Timer::~Timer() {
	// TODO Auto-generated destructor stub

	/// cancello il mio numero di orologio
//	m_whoAmI = 0;
//	switch(m_Cont){
//	case 0:
//		tBaseAdd[0] = 0; /// TA1
//	break;
//
//	case 1:
//		tBaseAdd[1] = 0; /// TA2
//		m_Cont--;
//	break;
//
//	case 2:
//		tBaseAdd[2] = 0; /// TB0
//		m_Cont--;
//	break;
//	}

}

///
/// imposta il periodo del timer
void Timer::begin(unsigned long int Periodo){

	unsigned long int divisore = F_CPU / 1000 * Periodo;
	/// il valore massimo di perioro e': 65535*64 / 25000000 = 0.1677 s
	switch(m_whoAmI - 1){
	case 0:
		if (divisore > 524280){
			TA1EX0 |= TAIDEX_7;
			TA1CTL |= ID_3;
			TA1CCR0 = divisore / 64;
		}
		else if (divisore > 65535){
			TA1CTL |= ID_3;
			TA1CCR0 = divisore / 8;
		}
			else
				TA1CCR0 = F_CPU / Periodo;

		TA1CTL |= TASSEL_2 | MC_1 | TACLR;   // Clock TIMERA from SMCLK, run in up mode counting from to 0-TA1CCR0

	break;

	case 1:
		if (divisore > 524280){
			TA2EX0 |= TAIDEX_7;
			TA2CTL |= ID_3;
			TA2CCR0 = divisore  / 64;
		}
		else if (divisore > 65535){
			TA2CTL |= ID_3;
			TA2CCR0 = divisore / 8;
		}
			else
				TA2CCR0 = F_CPU / Periodo;

		TA2CTL |= TASSEL_2 | MC_1 | TACLR;   // Clock TIMERA from SMCLK, run in up mode counting from to 0-TA1CCR0

	break;

	case 2:
		if (divisore > 524280){
			TB0EX0 |= TBIDEX_7;
			TB0CTL |= ID_3;
			TB0CCR0 = divisore / 64;
		}
		else if (divisore > 65535){
			TB0CTL |= ID_3;
			TB0CCR0 = divisore / 8;
		}
			else
				TB0CCR0 = F_CPU / Periodo;

		TB0CTL |= TBSSEL_2 | MC_1 | TBCLR;   // Clock TIMERA from SMCLK, run in up mode counting from to 0-TA1CCR0

	break;


	}
}

///
/// spegne il timer
void Timer::stop(){
	switch(m_whoAmI - 1){
	case 0:
		TA1CTL = TASSEL_2 | TACLR;
	break;

	case 1:
		TA2CTL = TASSEL_2 | TACLR;
	break;

	case 2:
		TB0CTL = TASSEL_2 | TACLR;
	break;
	}
}

///
/// collega la funzione di servizio dell'interruzione al relativo timer
/// indicato da m_whoAmI - 1
void Timer::attach(void(* fPtr)()){
	Timer::isr[m_whoAmI - 1] = fPtr;
}

/// abilita la relativa interruzione
void Timer::enableInt(void){

	switch(m_whoAmI - 1){
	case 0:
		/// enable interrupt
		TA1CCTL0 = CCIE;
	break;

	case 1:
		/// enable interrupt
		TA2CCTL0 = CCIE;
	break;

	case 2:
		/// enable interrupt
		TB0CCTL0 = CCIE;
	break;
	}
}

///
/// carica le funzioni di servizio negli apposti spazi della MCU

__attribute__((interrupt(TIMER1_A0_VECTOR))) void t1_isr(void){
	Timer::isr[0]();

}

__attribute__((interrupt(TIMER2_A0_VECTOR))) void t2_isr(void){
	Timer::isr[1]();
}


__attribute__((interrupt(TIMER0_B0_VECTOR))) void tb_isr(void){
	Timer::isr[2]();
}


