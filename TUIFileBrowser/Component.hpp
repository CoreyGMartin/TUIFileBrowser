#pragma once
#include <string>

class Component {


public:

	Component(short const& x, short const& y, std::string const& text, unsigned long const& length, unsigned short const& BackgroundColour, unsigned short const ForegroundColour);
	~Component() = default;

	short const					x;
	short const					y;
	std::string const			text;
	unsigned long const			length;
	unsigned short const		BackgroundColour;
	unsigned short const		ForegroundColour;

};