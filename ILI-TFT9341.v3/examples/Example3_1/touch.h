/*
 *  touch.h
 *  Author: Daniel Buza
 */ 


#ifndef _TOUCH_H_
#define _TOUCH_H_

#define T_SS POD7
#define T_IRQ PID2

class touch{
  private:
  	int16_t _x, _y;
  	uint16_t getX();
  	uint16_t getY();
	public:
  	void begin();
  	void readxy();
  	uint16_t readx() {return (_x);};
  	uint16_t ready() {return (_y);};
};

extern touch lcdtouch;

#endif /* TOUCH_H_ */
