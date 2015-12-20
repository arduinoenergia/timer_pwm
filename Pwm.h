/*
 * Pwm.h
 *
 *  Created on: 19/dic/2015
 *      Author: massimo
 */

#ifndef PWM_H_
#define PWM_H_

#include "Energia.h"
#include "Timer.h"

class PwmServo {
public:
	PwmServo();
	void pinSetting(uint8_t pin);
	void setup(uint32_t freq, uint16_t fPwm);
	void pinValue(uint8_t pin, float value);
	void pinValue(uint8_t pin, uint8_t val, uint16_t scale);
	void pinValue(uint8_t pin, int angle);
	virtual ~PwmServo();

private:
	/// proprties
	uint16_t F_PWM;
};

class Pwm : public Timer {
public:
	Pwm() : Timer() {}
	virtual ~Pwm(){ this->~Timer();}
	void pinSetting(uint8_t pin);
	void begin(unsigned long int);
};

#endif /* PWM_H_ */


