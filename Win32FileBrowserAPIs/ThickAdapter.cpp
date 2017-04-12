#include "ThickAdapter.hpp"
#undef min

std::unique_ptr<ThickAdapter> ThickAdapter::instance = nullptr;

//Constructor
ThickAdapter::ThickAdapter() {
	thin = std::unique_ptr<ThinAdapter>(new ThinAdapter());
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

void ThickAdapter::DrawInputSection(bool const doRecursiveSearch, std::string const regex, std::string const rootFolder, std::string const applyFilterLabel, std::string const filterLabel, std::string const searchLabel, std::string const rootFolderLabel, std::string const searchRecursivelyLabel, unsigned short const width, unsigned short const sectionSize, unsigned long const textBoxLength) {
	//Fill background
	unsigned long charsWritten = 0;
	unsigned long length = consoleScreenBufferInfo.dwSize.X * sectionSize;
	COORD location = COORD{ 0, 0 };
	thin->fillConsoleOutputCharacter(' ', length, location, charsWritten);
	thin->fillConsoleOutputAttribute(BACKGROUND_GREEN, length, location, charsWritten);

	//Starting coordinates
	short column = 1;
	short row = 0;

	//Root folder Label and textbox
	WriteTextToConsole(column, row, rootFolderLabel);
	column += rootFolderLabel.length();
	WriteTextToConsole(column, row, rootFolder);
	location = COORD{ column, row };
	thin->fillConsoleOutputAttribute(BACKGROUND_GREEN | BACKGROUND_INTENSITY, textBoxLength, location, charsWritten);
	column += textBoxLength;
	
	//Search Recursively label and checkbox
	++column;
	WriteTextToConsole(column, row, searchRecursivelyLabel);
	column += searchRecursivelyLabel.length();
	length = 1;
	location = COORD{ column, row };
	const std::string checked = doRecursiveSearch ? "x" : " ";
	WriteTextToConsole(column, row, checked);
	thin->fillConsoleOutputAttribute(BACKGROUND_INTENSITY, length, location, charsWritten);

	//Search Button
	column += 3;
	length = searchLabel.length() + 2; // +2 for space before and after button
	WriteTextToConsole(column, row, searchLabel);
	location = COORD{ (column - 1), row };
	thin->fillConsoleOutputAttribute(BACKGROUND_BLUE | BACKGROUND_INTENSITY, length, location, charsWritten);
	
	//New row
	column = 1;
	row = 2;

	//Filter Label and textbox
	WriteTextToConsole(column, row, filterLabel);
	column += filterLabel.length();
	WriteTextToConsole(column, row, regex);
	location = COORD{ column, row };
	thin->fillConsoleOutputAttribute(BACKGROUND_GREEN | BACKGROUND_INTENSITY, textBoxLength, location, charsWritten);
	column += textBoxLength;
	
	//Apply Filter Button
	column += 3;
	length = applyFilterLabel.length() + 2;
	WriteTextToConsole(column, row, applyFilterLabel);
	location = COORD{ (column - 1), row };
	thin->fillConsoleOutputAttribute(BACKGROUND_BLUE | BACKGROUND_INTENSITY, length, location, charsWritten);
}

void ThickAdapter::DrawFileSection(unsigned short const width, unsigned short const verticalScrollBarSize, short const rowOffset, unsigned short const sectionSize, unsigned short const linePosition, const std::vector<std::string> lines) {
	//Fill Background
	unsigned long charsWritten = 0;
	unsigned long const length = consoleScreenBufferInfo.dwSize.X * sectionSize;
	COORD location = COORD{ 0, rowOffset };
	thin->fillConsoleOutputCharacter(' ', length, location, charsWritten);
	thin->fillConsoleOutputAttribute(BACKGROUND_INTENSITY, length, location, charsWritten);

	//Scroll bar
	const unsigned long vertialScrollBarLocation = width - 1;
	for (int i = 1; i < (sectionSize - 1); ++i) {
		WriteTextToConsole(vertialScrollBarLocation, i + rowOffset, "|");
	}
	WriteTextToConsole(vertialScrollBarLocation, rowOffset, "^"); //Up arrow of scroll bar
	WriteTextToConsole(vertialScrollBarLocation, (sectionSize + rowOffset) - 1, "v"); //down arrow of scroll bar

	//Figure out where the scroll bar element should be
	double percentage = (double)linePosition / (lines.size() - verticalScrollBarSize);
	unsigned short const position = verticalScrollBarSize * percentage;
	location = COORD{ (width - 1), (rowOffset + position + 1) };
	thin->fillConsoleOutputAttribute(BACKGROUND_BLUE | BACKGROUND_INTENSITY, 1, location, charsWritten);

	//Draw text to console
	for (int i = 0; i + linePosition < lines.size() && i < sectionSize; ++i)
		WriteTextToConsole(0, i + rowOffset, lines[i + linePosition]);
}

void ThickAdapter::DrawStatsSection(unsigned int const sizeOfFilesMatched, unsigned int const filesMatched, unsigned int const totalFilesFoldersSearched, std::string const filesMatchedSizeLabel, std::string const filesMatchedLabel, std::string const totalFilesFolderLabel, short const rowOffset, unsigned short const width, unsigned short const sectionSize, unsigned long const textBoxLength) {
	//Fill Background
	unsigned long charsWritten = 0;
	unsigned long length = consoleScreenBufferInfo.dwSize.X * sectionSize;
	COORD location = COORD{ 0, rowOffset };
	thin->fillConsoleOutputCharacter(' ', length, location, charsWritten);
	thin->fillConsoleOutputAttribute(BACKGROUND_GREEN, length, location, charsWritten);

	//Starting coordinates
	short column = 1;
	short row = rowOffset;

	//Total Files/Folders Label and textbox
	WriteTextToConsole(column, row, totalFilesFolderLabel);
	column += totalFilesFolderLabel.length() + 1;
	WriteTextToConsole(column, row, std::to_string(totalFilesFoldersSearched));

	++row; //New row
	column = 1; //Reset column

	//Files Matched Label and texetbox
	WriteTextToConsole(column, row, filesMatchedLabel);
	column += filesMatchedLabel.length() + 1;
	WriteTextToConsole(column, row, std::to_string(filesMatched));
	column += textBoxLength;

	//Files Matched Size Label and texetbox
	++column;
	WriteTextToConsole(column, row, filesMatchedSizeLabel);
	column += filesMatchedSizeLabel.length() + 1;
	WriteTextToConsole(column, row, std::to_string(sizeOfFilesMatched));
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

void ThickAdapter::setConsoleCtrlHandler(bool(*handler)(unsigned long), bool add) {
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

std::unique_ptr<ThickAdapter> ThickAdapter::GetInstance() {
	if (instance == nullptr)
		instance = std::unique_ptr<ThickAdapter>(new ThickAdapter());
	return std::move(instance);
}

void ThickAdapter::WriteTextToConsole(unsigned long x, unsigned long y, std::string text) {
	unsigned long charsWritten = 0;
	COORD location = COORD{ (SHORT)x, (SHORT)y };
	thin->writeConsoleOutputCharacterA(text.c_str(), text.length(), location, charsWritten);
}

void ThickAdapter::WriteTextToConsoleWithCoordinates(COORD const location, std::string text) {
	unsigned long charsWritten = 0;
	thin->writeConsoleOutputCharacterA(text.c_str(), text.length(), location, charsWritten);
}

//void ThickAdapter::WriteTextToConsoleWithAttributes(unsigned long x, unsigned long y, std::string text, std::vector<unsigned short> attributes) {
//	unsigned long charsWritten = 0;
//	COORD location = COORD{ (SHORT)x, (SHORT)y };
//	thin->writeConsoleOutputCharacterA(text.c_str(), text.length(), location, charsWritten);
//	thin->writeConsoleOutputAttribute(attributes.data(), text.length, location, charsWritten);
//}