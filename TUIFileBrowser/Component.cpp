#include "Component.hpp"

Component::Component(short const& x, short const& y, std::string const& text, unsigned long const& length, unsigned short const& BackgroundColour, unsigned short const ForegroundColour)
	: x(x), y(y), text(text), length(length), BackgroundColour(BackgroundColour), ForegroundColour(ForegroundColour) {}

//TODO: emove if not needed
//Component::Component(short const& x, short const& y, std::string const& text, unsigned short const& BackgroundColour, unsigned short const ForegroundColour)
//	: x(x), y(y), text(text), BackgroundColour(BackgroundColour), ForegroundColour(ForegroundColour) {
//	length = text.length();
//}