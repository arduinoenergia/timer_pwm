/*
 * Timer.h
 *
 *  Created on: 14/dic/2015
 *      Author: massimo
 */

#ifndef TIMER_H_
#define TIMER_H_

class Timer {
public:
	Timer();
	/// richiede il periodo del timer
	void begin(unsigned long int);
	void stop();
	/// collega la routine di servizio gestita ad interruzione
	void attach(void (*)());
	/// abilita la generazione del segnale di interruzione
	void enableInt();

	virtual ~Timer();


	static unsigned char m_Cont;
	/// sintassi per un vettore di puntatori a funzione void che restituisce un void.
	/// il numero di elementi va messo subito dopo il nome del vettore di puntatori a funzione
	static void (*isr[3])();

protected:
	static unsigned int tBaseAdd[3];
	//static unsigned int tUsed[3];

	/// e non in questo modo:  void (*isr)()[3]
	unsigned char m_whoAmI;
	/// periodo del timer
	unsigned int T;

};

#endif /* TIMER_H_ */
