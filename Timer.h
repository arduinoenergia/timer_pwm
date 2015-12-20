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
	void begin(unsigned long int);
	void stop();
	void attach(void (*)());
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

};

#endif /* TIMER_H_ */
