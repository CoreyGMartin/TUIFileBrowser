#pragma once
#include <algorithm>
#include <sstream>
#include <vector>
#include <memory>

#include "ThinAdapter.hpp"

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

	//Utility Methods
	void										WriteTextToConsole(unsigned long x, unsigned long y, std::string text);
	void										WriteTextToConsoleWithCoordinates(COORD const location, std::string text);
	//void										WriteTextToConsoleWithAttributes(unsigned long x, unsigned long y, std::string text, std::vector<unsigned short> attributes);

public:

	~ThickAdapter() = default; //Destructor
	ThickAdapter(const ThickAdapter&) = delete; // Prevent construction by copying
	ThickAdapter& operator=(const ThickAdapter&) = delete; // Prevent assignment construction

	//Methods
	static std::unique_ptr<ThickAdapter>		GetInstance();

	void										saveConsoleState();
	void										restoreConsoleState();
	void										setTitle(char* const title);
	void										setSize(short const width, short const height);
	void										FillAndSetConsoleBackgroundColour(unsigned short const colour, char const& filler);
	void										setConsoleMode(unsigned long const mode);
	void										hideCursor();
	void										showAndSetCursorPosition(short const x, short const y);
	void										SetCursorPosition(short const x, short const y);

	void										readConsoleInput(std::vector<INPUT_RECORD> &buffer, unsigned long const length, unsigned long &numOfEvents);
	void										setConsoleCtrlHandler(bool(*handler)(unsigned long), bool add);
	void										setBlockColour(unsigned short const x, unsigned short const y, unsigned short const colour);

	//Added for FileBrowser
	void DrawInputSection(bool const doRecursiveSearch, std::string const regex, std::string const rootFolder, std::string const applyFilterLabel, std::string const filterLabel, std::string const searchLabel, std::string const rootFolderLabel, std::string const searchRecursivelyLabel, unsigned short const width, unsigned short const sectionSize, unsigned long const textBoxLength);
	void DrawStatsSection(unsigned int const sizeOfFilesMatched, unsigned int const filesMatched, unsigned int const totalFilesFoldersSearched, std::string const filesMatchedSizeLabel, std::string const filesMatchedLabel, std::string const totalFilesFolderLabel, short const rowOffset, unsigned short const width, unsigned short const sectionSize, unsigned long const textBoxLength);
	void DrawFileSection(unsigned short const width, unsigned short const verticalScrollBarSize, short const rowOffset, unsigned short const sectionSize, unsigned short const linePosition, const std::vector<std::string> lines);
};