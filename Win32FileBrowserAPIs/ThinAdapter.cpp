#include "ThinAdapter.hpp"
#include "Win32ConsoleError.hpp"

ThinAdapter::ThinAdapter() : consoleInput(GetStdHandle(STD_INPUT_HANDLE)), consoleOutput(GetStdHandle(STD_OUTPUT_HANDLE)) {}

void ThinAdapter::getConsoleTitle(char* storage, unsigned long const& size) {
	THROW_IF_CONSOLE_ERROR(GetConsoleTitleA(storage, size));
}

void ThinAdapter::getConsoleScreenBufferInfo(CONSOLE_SCREEN_BUFFER_INFO& csbi) {
	THROW_IF_CONSOLE_ERROR(GetConsoleScreenBufferInfo(consoleOutput, &csbi));
}

void ThinAdapter::readConsoleOutput(CHAR_INFO* storage, COORD const& dwSize, COORD const& bufferCoord, SMALL_RECT& bufferRect) {
	THROW_IF_CONSOLE_ERROR(ReadConsoleOutputA(consoleOutput, storage, dwSize, bufferCoord, &bufferRect));
}

void ThinAdapter::getConsoleCursorInfo(CONSOLE_CURSOR_INFO &cursorInfo) {
	THROW_IF_CONSOLE_ERROR(GetConsoleCursorInfo(consoleOutput, &cursorInfo));
}

void ThinAdapter::setConsoleWindowInfo(bool const& isAbsolute, SMALL_RECT const& consoleWindow) {
	THROW_IF_CONSOLE_ERROR(SetConsoleWindowInfo(consoleOutput, isAbsolute, &consoleWindow));
}

void ThinAdapter::setConsoleScreenBufferSize(COORD const& dwSize) {
	THROW_IF_CONSOLE_ERROR(SetConsoleScreenBufferSize(consoleOutput, dwSize));
}

void ThinAdapter::writeConsoleOutput(CHAR_INFO* const content, COORD const& dwSize, COORD const& bufferCoord, SMALL_RECT& bufferRect) {
	THROW_IF_CONSOLE_ERROR(WriteConsoleOutputA(consoleOutput, content, dwSize, bufferCoord, &bufferRect));
}

void ThinAdapter::setConsoleTitle(char* const title) {
	THROW_IF_CONSOLE_ERROR(SetConsoleTitleA(title));
}

void ThinAdapter::setConsoleCursorInfo(CONSOLE_CURSOR_INFO const& cursorInfo) {
	THROW_IF_CONSOLE_ERROR(SetConsoleCursorInfo(consoleOutput, &cursorInfo));
}

void ThinAdapter::setConsoleCursorPosition(COORD const& curosrPosition) {
	THROW_IF_CONSOLE_ERROR(SetConsoleCursorPosition(consoleOutput, curosrPosition));
}

void ThinAdapter::setConsoleCtrlHandler(bool(*handler)(unsigned long const), bool const add) {
	THROW_IF_CONSOLE_ERROR(SetConsoleCtrlHandler((PHANDLER_ROUTINE)handler, add));
}

void ThinAdapter::fillConsoleOutputCharacter(char const& filler, unsigned long const& length, COORD const& location, unsigned long& numberWritten) {
	THROW_IF_CONSOLE_ERROR(FillConsoleOutputCharacterA(consoleOutput, filler, length, location, &numberWritten));
}

void ThinAdapter::fillConsoleOutputAttribute(unsigned short const& colour, unsigned long const& consoleSize, COORD const& location, unsigned long& charsWritten) {
	THROW_IF_CONSOLE_ERROR(FillConsoleOutputAttribute(consoleOutput, colour, consoleSize, location, &charsWritten));
}

void ThinAdapter::readConsoleInput(std::vector<INPUT_RECORD> &buffer, unsigned long const& length, unsigned long &numOfEvents) {
	THROW_IF_CONSOLE_ERROR(ReadConsoleInput(consoleInput, buffer.data(), length, &numOfEvents));
}

void ThinAdapter::setConsoleMode(unsigned long const& mode) {
	THROW_IF_CONSOLE_ERROR(SetConsoleMode(consoleInput, mode));
}

void ThinAdapter::writeConsoleOutputCharacterA(const char* text, unsigned long const& length, COORD const& location, unsigned long& charsWritten) {
	THROW_IF_CONSOLE_ERROR(WriteConsoleOutputCharacterA(consoleOutput, text, length, location, &charsWritten));
}

void ThinAdapter::writeConsoleOutputAttribute(unsigned short const *attributes, unsigned long const& length, COORD const& location, unsigned long& charsWritten) {
	THROW_IF_CONSOLE_ERROR(WriteConsoleOutputAttribute(consoleOutput, attributes, length, location, &charsWritten));
}
