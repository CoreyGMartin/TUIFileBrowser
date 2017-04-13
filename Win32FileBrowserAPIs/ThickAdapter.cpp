#include "ThickAdapter.hpp"
#undef min

std::unique_ptr<ThickAdapter> ThickAdapter::instance = nullptr;

//Constructor
ThickAdapter::ThickAdapter() {
	thin = std::unique_ptr<ThinAdapter>(new ThinAdapter());
}

ThickAdapter::~ThickAdapter() {
	//delete instance;
}

void ThickAdapter::saveConsoleState() {
	//Save Title
	title = std::vector<char>(64 * 1024);
	thin->getConsoleTitle(title.data(), (unsigned long)title.size() + 1);
	title.shrink_to_fit();

	//Save Screen Buffer
	thin->getConsoleScreenBufferInfo(consoleScreenBufferInfo);

	//Save content
	buffer.resize(consoleScreenBufferInfo.dwSize.X * consoleScreenBufferInfo.dwSize.Y);
	bufferCoord = COORD{ 0 };
	SMALL_RECT bufferRect{ 0 };
	bufferRect.Right = consoleScreenBufferInfo.dwSize.X - 1;
	bufferRect.Bottom = consoleScreenBufferInfo.dwSize.Y - 1;
	thin->readConsoleOutput(buffer.data(), consoleScreenBufferInfo.dwSize, bufferCoord, bufferRect);

	//Save Cursor
	thin->getConsoleCursorInfo(consoleCursorInfo);
}

void ThickAdapter::restoreConsoleState() {
	//Restore settings
	SMALL_RECT sr{ 0 };
	thin->setConsoleWindowInfo(true, sr);
	thin->setConsoleScreenBufferSize(consoleScreenBufferInfo.dwSize);
	thin->setConsoleWindowInfo(true, consoleScreenBufferInfo.srWindow);

	//Restore desktop contents
	SMALL_RECT bufferRect{ 0 };
	bufferRect.Right = consoleScreenBufferInfo.dwSize.X - 1;
	bufferRect.Bottom = consoleScreenBufferInfo.dwSize.Y - 1;
	thin->writeConsoleOutput(buffer.data(), consoleScreenBufferInfo.dwSize, bufferCoord, bufferRect);
	thin->setConsoleTitle(title.data());

	//Restore Cursor
	thin->setConsoleCursorInfo(consoleCursorInfo);
	thin->setConsoleCursorPosition(consoleScreenBufferInfo.dwCursorPosition);
}

void ThickAdapter::setTitle(char* const title) {
	thin->setConsoleTitle(title);
}

void ThickAdapter::setSize(short width, short height) {
	SMALL_RECT sr{ 0 };
	thin->setConsoleWindowInfo(true, sr);

	COORD bufferSize;
	bufferSize.X = width;
	bufferSize.Y = height;
	thin->setConsoleScreenBufferSize(bufferSize);

	CONSOLE_SCREEN_BUFFER_INFO sbi(consoleScreenBufferInfo);
	thin->getConsoleScreenBufferInfo(sbi);

	sr.Top = sr.Left = 0;
	width = std::min(width, sbi.dwMaximumWindowSize.X);
	height = std::min(height, sbi.dwMaximumWindowSize.Y);
	sr.Right = width - 1;
	sr.Bottom = height - 1;

	thin->setConsoleWindowInfo(true, sr);
	consoleWidth = sr.Right - sr.Left + 1;
}

void ThickAdapter::FillAndSetConsoleBackgroundColour(unsigned short const colour, char const& filler = ' ') {
	unsigned long charsWritten = 0;
	unsigned long consoleSize = consoleScreenBufferInfo.dwSize.X * consoleScreenBufferInfo.dwSize.Y;
	COORD cursorHomeCoord{ 0, 0 };
	thin->fillConsoleOutputCharacter(filler, consoleSize, cursorHomeCoord, charsWritten);
	thin->fillConsoleOutputAttribute(colour, consoleSize, cursorHomeCoord, charsWritten);
}

void ThickAdapter::setConsoleMode(unsigned long const mode) {
	thin->setConsoleMode(mode);
}

void ThickAdapter::hideCursor() {
	CONSOLE_CURSOR_INFO cursor = consoleCursorInfo;
	cursor.bVisible = false;
	thin->setConsoleCursorInfo(cursor);
}

void ThickAdapter::showAndSetCursorPosition(short const x, short const y) {
	CONSOLE_CURSOR_INFO cursor = consoleCursorInfo;
	cursor.bVisible = true;
	thin->setConsoleCursorInfo(cursor);
	COORD cursorPosition = { x, y };
	thin->setConsoleCursorPosition(cursorPosition);
}

void ThickAdapter::SetCursorPosition(short const x, short const y) {
	COORD cursorPosition = { x, y };
	thin->setConsoleCursorPosition(cursorPosition);
}

void ThickAdapter::readConsoleInput(std::vector<INPUT_RECORD> &buffer, unsigned long const length, unsigned long &numOfEvents) {
	thin->readConsoleInput(buffer, length, numOfEvents);
}

void ThickAdapter::setConsoleCtrlHandler(bool(*handler)(unsigned long const), bool const add) {
	thin->setConsoleCtrlHandler(handler, add);
}

void ThickAdapter::setBlockColour(unsigned short const x, unsigned short const y, unsigned short const colour) {
	unsigned long charsWritten = 0;
	unsigned long consoleSize = 1;
	COORD block{ 0 };
	block.X = std::min((short)x, consoleScreenBufferInfo.dwMaximumWindowSize.X);
	block.Y = std::min((short)y, consoleScreenBufferInfo.dwMaximumWindowSize.Y);
	thin->fillConsoleOutputAttribute(colour, consoleSize, block, charsWritten);
}

ThickAdapter* ThickAdapter::GetInstance() {
	if (instance == nullptr) {
		instance = std::unique_ptr<ThickAdapter>(new ThickAdapter());
	}
	return instance.get();
}

void ThickAdapter::WriteTextToConsole(unsigned long x, unsigned long y, std::string text) {
	unsigned long charsWritten = 0;
	COORD location = COORD{ (SHORT)x, (SHORT)y };
	thin->writeConsoleOutputCharacterA(text.c_str(), text.length(), location, charsWritten);
}

void ThickAdapter::fillConsoleOutputAttribute(unsigned short const& colour, unsigned long const& consoleSize, short const& x, short const& y) {
	unsigned long charsWritten = 0;
	COORD const& location{ x, y };
	thin->fillConsoleOutputAttribute(colour, consoleSize, location, charsWritten);
}

void ThickAdapter::fillConsoleOutputCharacter(char const& filler, unsigned long const& length, short const& x, short const& y) {
	unsigned long charsWritten = 0;
	COORD const& location{ x, y };
	thin->fillConsoleOutputCharacter(filler, length, location, charsWritten);
}