#include "Button.h"

touch lcd_touch;

Button::Button(size_t x_location, size_t y_location, size_t width, size_t height, size_t colorFill, size_t fontColor, char* button_message) { _x_location = x_location; _y_location = y_location; _width = width; _height = height, _colorFill = colorFill; _fontColor = fontColor; _message = button_message;}


Button::~Button()
{
}



void Button::drawButton(size_t font)
{
	_pressed = false; 
	lcd_touch.begin();
	size_t start_y = _y_location;
	for (size_t i = start_y; i < _y_location + _height; i++)
	{
		lcd.drawLine(_x_location, i, _x_location + _width, i, _colorFill);
	}
	uploadToLCD(font);
}


void Button::updateButtonState()
{
	lcd_touch.readxy();
	size_t y = lcd_touch.ready();
	size_t x = lcd_touch.readx();
	(x > _x_location&& x<(_x_location + _width) && y>_y_location&& y < (_y_location + _height)) ? _pressed = true : _pressed = false;
}


void Button::setPressed(bool state)
{
    _pressed = state;
}



bool Button::getButtonState()
{
	return _pressed;
}






void Button::uploadToLCD(size_t font)
{
  switch (font)
  {
    case(2):
    {
        lcd.gotoxy(_x_location+20, _y_location+10);
        lcd.setColor(_fontColor, _colorFill);
        lcd.setFont(2);
        lcd.print(_message);
        break;
    }
    case(3):
    {
        lcd.gotoxy(_x_location+40, _y_location+20);
        lcd.setColor(_fontColor, _colorFill);
        lcd.setFont(3);
        lcd.print(_message);
        break;
    }
  }
}
