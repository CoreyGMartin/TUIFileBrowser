#pragma once
#include <algorithm>
#include <sstream>
#include <vector>
#include <memory>

#include "ThinAdapter.hpp"

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

class ThickAdapter {

	static std::unique_ptr<ThickAdapter>		instance;
	std::unique_ptr<ThinAdapter>				thin;
	std::vector<char>							title;
	CONSOLE_SCREEN_BUFFER_INFO					consoleScreenBufferInfo;
	CONSOLE_CURSOR_INFO							consoleCursorInfo;
	std::vector<CHAR_INFO>						buffer;
	COORD										bufferCoord;
	unsigned long								consoleMode;
	unsigned short								consoleWidth;

	// Prevent construction
	ThickAdapter();

public:

	~ThickAdapter(); //Destructor
	ThickAdapter(const ThickAdapter&) = delete; // Prevent construction by copying
	ThickAdapter& operator=(const ThickAdapter&) = delete; // Prevent assignment construction

	//Methods
	static ThickAdapter*		GetInstance();

	void						saveConsoleState();
	void						restoreConsoleState();
	void						setTitle(char* const title);
	void						setSize(short const width, short const height);
	void						FillAndSetConsoleBackgroundColour(unsigned short const colour, char const& filler);
	void						setConsoleMode(unsigned long const mode);
	void						hideCursor();
	void						showAndSetCursorPosition(short const x, short const y);
	void						SetCursorPosition(short const x, short const y);

	void						readConsoleInput(std::vector<INPUT_RECORD> &buffer, unsigned long const length, unsigned long &numOfEvents);
	void						setConsoleCtrlHandler(bool(*handler)(unsigned long const), bool const add);
	void						setBlockColour(unsigned short const x, unsigned short const y, unsigned short const colour);

	void						WriteTextToConsole(unsigned long x, unsigned long y, std::string text);
	void						fillConsoleOutputCharacter(char const& filler, unsigned long const& length, short const& x, short const& y);
	void						fillConsoleOutputAttribute(unsigned short const& colour, unsigned long const& consoleSize, short const& x, short const& y);
};