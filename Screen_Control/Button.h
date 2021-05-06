#pragma once
#include "TFT9341.h"
#include "Defines.h"
#include "touch.h"

class Button
{
	public:
		Button(size_t x_location, size_t y_location, size_t width, size_t height, size_t colorFill, size_t fontColor, char* button_message);
		~Button();
		void drawButton(size_t font);
		void updateButtonState();
		bool getButtonState();
		void uploadToLCD(size_t font);

    void setPressed(bool state);


	private:
		char* _message;
		size_t _x_location;
		size_t _y_location;
		size_t _width;
		size_t _height;
		size_t _colorFill;
		size_t _fontColor;
		bool _pressed;

};

extern touch lcd_touch;
