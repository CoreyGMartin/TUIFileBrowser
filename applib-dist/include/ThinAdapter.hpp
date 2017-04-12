#pragma once
#include <Windows.h>
#include <vector>

class ThinAdapter {
	HANDLE const				consoleInput;
	HANDLE const				consoleOutput;

public:
	ThinAdapter();
	~ThinAdapter() {};

	void						fillConsoleOutputCharacter(char const& filler, unsigned long const& length, COORD const& location, unsigned long& numberWritten);
	void						fillConsoleOutputAttribute(unsigned short const& colour, unsigned long const& consoleSize, COORD const& location, unsigned long& charsWritten);

	void						getConsoleTitle(char* storage, unsigned long const& size);
	void						getConsoleScreenBufferInfo(CONSOLE_SCREEN_BUFFER_INFO& csbi);
	void						getConsoleCursorInfo(CONSOLE_CURSOR_INFO& cursorInfo);

	void						setConsoleTitle(char* const title);
	void						setConsoleCursorInfo(CONSOLE_CURSOR_INFO const& cursorInfo);
	void						setConsoleCursorPosition(COORD const& curosrPosition);
	void						setConsoleCtrlHandler(bool(*handler)(unsigned long), bool add);
	void						setConsoleWindowInfo(bool const& isAbsolute, SMALL_RECT const& consoleWindow);
	void						setConsoleScreenBufferSize(COORD const& dwSize);
	void						setConsoleMode(unsigned long const& mode);

	void						readConsoleInput(std::vector<INPUT_RECORD> &buffer, unsigned long const& length, unsigned long &numOfEvents);
	void						readConsoleOutput(CHAR_INFO* storage, COORD const& dwSize, COORD const& bufferCoord, SMALL_RECT& bufferRect);
	
	void						writeConsoleOutput(CHAR_INFO* const content, COORD const& dwSize, COORD const& bufferCoord, SMALL_RECT& bufferRect);
	void						writeConsoleOutputCharacterA(const char* text, unsigned long const& length, COORD const& location, unsigned long& charsWritten);
	void						writeConsoleOutputAttribute(unsigned short const *attributes, unsigned long const& length, COORD const& location, unsigned long& charsWritten);

};
